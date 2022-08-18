#include <io/common-io.h>
#include <io/log.h>
#include <game/game.h>
#include <game/user.h>
#include <game/statistics.h>
#include <game/history.h>
#include <types/array.h>
#include <types/vector.h>
#include <types/memory.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <limits.h>
#include <unistd.h>

#define GAME_SAVES_PATH "saves.bin"
#define DEFAULT_SAVES_SIZE 20
#define INVALID_TABLE_INDEX 40

game_data game;
vector *saves;

bool is_game_running = false;

#pragma region private_prototypes

void __game_init(userid user1, userid user2, userid user3, userid user4, size_t count);

void game_state_start();
void game_state_serve();
void game_state_cycle();
void game_state_end();

void check_user_validity(userid id);
void game_turn_advance();
player *current_player();
char *game_save_json(game_data *save, size_t id);

#pragma endregion private_prototypes

#pragma region game_init

void game_init_2players(userid user1, userid user2)
{
    check_user_validity(user1);
    check_user_validity(user2);
    __game_init(user1, user2, USERID_INVALID, USERID_INVALID, 2);
}

void game_init_3players(userid user1, userid user2, userid user3)
{
    check_user_validity(user1);
    check_user_validity(user2);
    check_user_validity(user3);
    __game_init(user1, user2, user3, USERID_INVALID, 3);
}

void game_init_4players(userid user1, userid user2, userid user3, userid user4)
{
    check_user_validity(user1);
    check_user_validity(user2);
    check_user_validity(user3);
    check_user_validity(user4);
    __game_init(user1, user2, user3, user4, 4);
}

void __game_init(userid user1, userid user2, userid user3, userid user4, size_t count)
{
    memset(&game, 0, sizeof(game));
    game.players_count = count;

    game.players[0].id = user1;
    strcpy(game.players[0].username, user_get_by_userid(user1)->username);
    game.players[1].id = user2;
    strcpy(game.players[1].username, user_get_by_userid(user2)->username);

    if (count >= 3)
    {
        game.players[2].id = user3;
        strcpy(game.players[2].username, user_get_by_userid(user3)->username);
    }

    if (count == 4)
    {
        game.players[3].id = user4;
        strcpy(game.players[3].username, user_get_by_userid(user4)->username);
    }

    game.timestamp = datetime_now();
    game.state = GAME_START;
    deck_init_full(&(game.dealer_deck));
    is_game_running = true;

    log_info("game initialized");
}

#pragma endregion game_init

#pragma region game_states

void game_state_start()
{
    game.turn = 0;
    deck_shuffle(&(game.dealer_deck));
    game.state = GAME_SERVE;

    log_info("game started");
}

void game_state_serve()
{
    if(game.dealer_deck.length / 3 <= 0)
    {
        game.state = GAME_END;
        return;
    }

    size_t cards_to_players = game.dealer_deck.length >= (3 * game.players_count) ? 3 : game.dealer_deck.length / game.players_count;

    for (size_t i = 0; i < game.players_count; i++)
    {
        for (size_t j = 0; j < cards_to_players; j++)
        {
            card c = deck_pop(&(game.dealer_deck));
            game.players[i].hand.cards[j] = c;
        }
        game.players[i].hand.length = 3;
    }

    size_t cards_to_table = game.dealer_deck.length >= 4 ? 4 : game.table.length;

    for (size_t i = 0; i < cards_to_table; i++)
    {
        card c = deck_pop(&(game.dealer_deck));
        game.table.cards[game.table.length++] = c;
    }

    game.turn = 0;
    game.state = GAME_CYCLE;

    log_info("game serving");
}

void game_state_cycle()
{
    if (hand_is_empty(current_player()) == false)
    {
        game.state = GAME_WAITING;
        log_info("game waiting");
    }
    else
    {
        if (game.dealer_deck.length < 1)
        {
            game.state = GAME_END;
            log_info("game end");
        }
        else
        {
            game.state = GAME_SERVE;
            log_info("game serve");
        }
    }

    log_info("game cycle");
}

void game_state_end()
{
    deck_append(
        &game.players[game.last_take].deck,
        &game.table);

    game.winner = 0;

    for (size_t i = 1; i < game.players_count; i++)
    {
        if (game.players[i].deck.length > game.players[game.winner].deck.length)
        {
            game.winner = i;
        }
    }

    add_history_entry(&game);

    for (size_t i = 0; i < game.players_count; i++)
    {
        update_statistics_for(game.players[i].id);
    }

    is_game_running = false;
}

#pragma endregion game_states

#pragma region game_core

void game_update()
{
    if (is_game_running == false)
    {
        log_warning("can't update, game's not running");
        return;
    }

    switch (game.state)
    {
    case GAME_START:
        game_state_start();
        break;
    case GAME_SERVE:
        game_state_serve();
        break;
    case GAME_CYCLE:
        game_state_cycle();
        break;
    case GAME_WAITING:
        break;
    case GAME_END:
        game_state_end();
        break;
    default:
        log_error("undefined game state");
        break;
    }
}

void game_throw_on_table(size_t hand_index)
{
    player *p = current_player();

    if (hand_index >= p->hand.length)
    {
        return;
    }

    game.table.cards[game.table.length++] = hand_throw(p, hand_index);

    game_turn_advance();
}

void game_pick_from_table(size_t hand_index, size_t table_index)
{
    player *p = current_player();

    if (hand_index >= p->hand.length ||
        table_index >= game.table.length)
    {
        return;
    }

    if (card_equal_by_value(hand_at(p, hand_index), deck_at(&game.table, table_index)))
    {
        card table_card = deck_pop_index(&game.table, table_index);
        card player_card = hand_throw(p, hand_index);
        deck_append_card(&p->deck, table_card);
        deck_append_card(&p->deck, player_card);
        game.last_take = game.turn;
        game_turn_advance();
    }
}

void game_pick_from_opponent(size_t hand_index, size_t opponent_index)
{
    player *p = current_player();

    if (hand_index >= p->hand.length ||
        opponent_index >= game.players_count)
    {
        return;
    }

    if (card_equal_by_value(hand_at(p, hand_index), deck_top(&game.players[opponent_index].deck)))
    {
        p->steals++;
        deck_append(&p->deck, &game.players[opponent_index].deck);
        deck_append_card(&p->deck, hand_throw(p, hand_index));
        game.last_take = game.turn;
        game_turn_advance();
    }
}

game_data *get_game_data()
{
    return &game;
}

#pragma endregion game_core


#pragma region utility_functions

player *current_player()
{
    return &(game.players[game.turn]);
}

void check_user_validity(userid id)
{
    if (user_is_valid(id) == false ||
        user_is_logged(id) == false)
    {
        log_error("invalid user");
    }
}

void game_turn_advance()
{
    game.state = GAME_CYCLE;
    game.turn = (game.turn + 1) % game.players_count;
}

#pragma endregion utility_functions

#pragma region game_persistence

void game_saves_init()
{
    saves = VEC_INIT(game_data);

    if (access(GAME_SAVES_PATH, F_OK) == 0)
    {
        FILE *f = fopen(GAME_SAVES_PATH, "rb");
        fread(&saves->count, sizeof(size_t), 1, f);
        VEC_RESIZE(saves, saves->count);
        fread(saves->elements, sizeof(game_data), saves->count, f);
        fclose(f);
    }
}

void game_save()
{
    if (is_game_running == false)
    {
        log_error("game is not running, nothing to save");
        return;
    }

    VEC_APPEND(game_data, saves, game);
    is_game_running = false;

    FILE *f = fopen(GAME_SAVES_PATH, "wb");
    fwrite(&saves->count, sizeof(size_t), 1, f);
    fwrite(saves->elements, sizeof(game_data), saves->count, f);
    fclose(f);
}

void game_load(size_t save_index, userid user1, userid user2, userid user3, userid user4)
{
    if (save_index >= saves->count)
    {
        log_error("invalid save index");
        return;
    }

    if (is_game_running == true)
    {
        log_error("game is already running, can't load");
        return;
    }

    game = VEC_GET(game_data, saves, save_index);

    check_user_validity(user1);
    check_user_validity(user2);

    if (game.players_count >= 3)
        check_user_validity(user3);

    if (game.players_count == 4)
        check_user_validity(user4);

    is_game_running = true;
    VEC_DELETE(saves, save_index);
    
    FILE *f = fopen(GAME_SAVES_PATH, "wb");
    fwrite(&saves->count, sizeof(size_t), 1, f);
    fwrite(saves->elements, sizeof(game_data), saves->count, f);
    fclose(f);
}

game_data *game_save_list(size_t *list_size)
{
    *list_size = saves->count;
    return saves->elements;
}

game_data *game_save_by_id(size_t id)
{
    if (id >= saves->count)
    {
        return NULL;
    }

    return &VEC_GET(game_data, saves, id);
}

#pragma endregion game_persistence

#pragma region json

char *game_data_json(game_data *gd)
{
    char *json = calloc(4096, sizeof(char));
    const char *state_string[5] = {
        "start",
        "serve",
        "cycle",
        "waiting",
        "end"};
    char *players_string[4];
    char *table_string = deck_json(&gd->table);

    for (size_t i = 0; i < 4; i++)
    {
        players_string[i] = player_json(&gd->players[i]);
    }

    sprintf(json, "{"
                  "\"players\": ["
                  "%s,%s,%s,%s"
                  "],"
                  "\"players_count\":%zu,"
                  "\"last_take\":%zu,"
                  "\"turn\":%zu,"
                  "\"dealer\":%zu,"
                  "\"table\":%s,"
                  "\"state\":\"%s\","
                  "\"winner\":%zu"
                  "}",
            players_string[0], players_string[1], players_string[2], players_string[3],
            gd->players_count, gd->last_take, gd->turn,
            gd->dealer_deck.length, table_string, state_string[gd->state], gd->winner);

    for (size_t i = 0; i < 4; i++)
    {
        free(players_string[i]);
    }
    free(table_string);

    return json;
}

char *game_save_json(game_data *save, size_t id)
{
    check_null_pointer(save);

    char *json = memory_allocate_typed(1024, sizeof(char));
    char timestamp[DATETIME_STR_SIZE] = {0};
    datetime_string(save->timestamp, timestamp);
    sprintf(json,
            "{"
            "\"id\":%zu,"
            "\"timestamp\":\"%s\","
            "\"users\":["
                "{\"id\":%zu,\"username\":\"%s\"},"
                "{\"id\":%zu,\"username\":\"%s\"},"
                "{\"id\":%zu,\"username\":\"%s\"},"
                "{\"id\":%zu,\"username\":\"%s\"}]"
            "}",
            id, timestamp,
            save->players[0].id, save->players[0].username,
            save->players[1].id, save->players[1].username,
            save->players[2].id, save->players[2].username,
            save->players[3].id, save->players[3].username);
    return json;
}

char *game_save_list_json()
{
    char *json = calloc(4096, sizeof(char));
    char players[3][1024] = {0};
    char *current;
    char *tmp;

    for (size_t i = 0; i < 3; i++)
    {
        strcat(players[i], "[");
    }

    for (size_t i = 0; i < saves->count; i++)
    {
        size_t index = saves->count - i - 1;
        size_t pc = VEC_GET(game_data, saves, index).players_count - 2;
        current = players[pc];
        tmp = game_save_json(&VEC_GET(game_data, saves, index), index);
        strcat(current, tmp);
        strcat(current, ",");
        free(tmp);
    }

    for (size_t i = 0; i < 3; i++)
    {
        if(players[i][strlen(players[i]) - 1] == ',')
        {
            players[i][strlen(players[i]) - 1] = ']';
        }
        else
        {
            strcat(players[i], "]");
        }
    }

    sprintf(json, "[%s, %s, %s]", players[0], players[1], players[2]);
    return json;
}

#pragma endregion json