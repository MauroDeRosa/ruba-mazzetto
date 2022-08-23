/**
 * @file game.c
 * @author Mauro De Rosa (it.mauro.derosa@gmail.com)
 * @brief game module implementation @see game.h
 * @version 1.0.0 alpha
 * @date 2022-08-23
 * 
 * @copyright Copyright (c) 2022
 * @license licensed under MIT license
 * 
 */

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

#define GAME_SAVES_PATH "saves.bin" ///< saves file path

game_data game; ///< current game
vector *saves; ///< vector containing saved games

bool is_game_running = false; ///< contains if the game is currently running

#pragma region private_prototypes

/**
 * @brief initialize a new game
 * 
 * @param user1 first player user id
 * @param user2 second player user id
 * @param user3 third player user id or USER_INVALID if less than 3 players
 * @param user4 fourth player user id or USER_INVALID if less than 4 players
 * @param count how many players (2, 3, 4)
 */
void __game_init(userid user1, userid user2, userid user3, userid user4, size_t count);

void game_state_start(); ///< @brief game start state function @see game_state
void game_state_serve(); ///< @brief game serve state function @see game_state
void game_state_cycle(); ///< @brief game cycle state function @see game_state
void game_state_end();   ///< @brief game end state function   @see game_state

/**
 * @brief check if user is valid and logged in
 * 
 * @param id user id
 */
void check_user_validity(userid id);

/**
 * @brief change state to GAME_CYCLE and set player turn to the next player @see game_state
 */
void game_turn_advance();

/**
 * @brief get current player pointer
 * 
 * @return player* pointer to the currently playing player
 */
player *current_player();

void saves_free(); ///< @brief free saves vector

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
    // resets current game structure
    memset(&game, 0, sizeof(game));

    // sets player count based on how many users are playing
    game.players_count = count;

    // first player data
    game.players[0].id = user1;
    strcpy(game.players[0].username, get_user(user1)->username);

    // second player data
    game.players[1].id = user2;
    strcpy(game.players[1].username, get_user(user2)->username);

    if (count >= 3)
    {
        // third player data
        game.players[2].id = user3;
        strcpy(game.players[2].username, get_user(user3)->username);
    }

    if (count == 4)
    {
        // fourth player data
        game.players[3].id = user4;
        strcpy(game.players[3].username, get_user(user4)->username);
    }

    // game timestamp to current date and time
    game.timestamp = datetime_now();

    // initial game state set to GAME_START
    game.state = GAME_START;
    // initialize a full deck for the dealer
    deck_init_full(&(game.dealer_deck));
    // sets game running to true
    is_game_running = true;
    log_debug("game initialized");
}

#pragma endregion game_init

#pragma region game_states

void game_state_start()
{
    // sets current player to the first one
    game.turn = 0;
    // shuffle the dealer's deck
    deck_shuffle(&(game.dealer_deck));
    // move to serve state
    game.state = GAME_SERVE;

    log_debug("game started");
}

void game_state_serve()
{
    // if there are more then 4 cards put 4 cards on the table else put remaining cards in the dealer's deck
    size_t cards_to_table = (game.dealer_deck.length >= 4) ? 4 : game.dealer_deck.length;

    for (size_t i = 0; i < cards_to_table; i++)
    {
        // pop a card from the dealer's deck
        card c = deck_pop(&game.dealer_deck);
        // put the card on the table
        deck_append_card(&game.table, c);
    }

    // if there are at least 3 cards for each player give 3 cards to each one, else give the remaining cards in the deck divided by the count of players
    size_t cards_to_players = (game.dealer_deck.length / game.players_count  >= 3 ) ? 3 : game.dealer_deck.length / game.players_count;

    for (size_t i = 0; i < game.players_count; i++)
    {
        // for each player, for each card to give
        for (size_t j = 0; j < cards_to_players; j++)
        {
            // pop a card from the dealer's deck
            card c = deck_pop(&game.dealer_deck);
            // put the card in the player's hand
            game.players[i].hand.cards[j] = c;
        }
        // set hand length to how many cards you're serving for each player
        game.players[i].hand.length = cards_to_players;
    }

    // reset game turn to the first player
    game.turn = 0;
    // move to game cycle state
    game.state = GAME_CYCLE;
    log_debug("game serving");
}

void game_state_cycle()
{
    if (is_hand_empty(current_player()) == false)
    {
        // if current player's hand is not empty wait for any move
        game.state = GAME_WAITING;
        log_debug("game waiting");
    }
    else
    {
        //if current player's hand is empty
        if (is_deck_empty(&game.dealer_deck))
        {
            // and the dealer deck is empty move to the game end state
            game.state = GAME_END;
            log_debug("game end");
        }
        else
        {
            // and there are cards in the dealer's deck proceed to serve
            game.state = GAME_SERVE;
            log_debug("game serve");
        }
    }

    log_debug("game cycle");
}

void game_state_end()
{
    // append all the cards remaining on the table to the last player that took from table or opponents
    deck_append(
        &game.players[game.last_take].deck,
        &game.table);

    // set the winner to the first player
    game.winner = 0;

    // for each player check if the player's deck is bigger than the current winner's deck
    for (size_t i = 1; i < game.players_count; i++)
    {
        if (game.players[i].deck.length > game.players[game.winner].deck.length)
        {
            // set winner to the index of the player with the bigger deck 🍆
            game.winner = i;
        }
    }

    // add history for the current game for each player
    add_history_entry(&game); 

    // update statistics for every player
    for (size_t i = 0; i < game.players_count; i++)
    {
        update_statistics_for(game.players[i].id);
    }

    // stops game from running
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

    // call the appropriate state function
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
        log_warning("wrong hand index");
        return;
    }

    // throw the card from the hand, and put it on the table
    card c = hand_throw(p, hand_index);
    deck_append_card(&game.table, c);
    // next player turn
    game_turn_advance();
}

void game_pick_from_table(size_t hand_index, size_t table_index)
{
    // gets current player pointer
    player *p = current_player();

    if (hand_index >= p->hand.length ||
        table_index >= game.table.length)
    {
        log_warning("invalid move");
        return;
    }

    // if the card in the hand of the current player has the same value of the selected card on the table
    if (is_card_equal(hand_at(p, hand_index), deck_at(&game.table, table_index)))
    {
        // get the card from the table
        card table_card = deck_pop_index(&game.table, table_index);
        // get the card from the hand
        card player_card = hand_throw(p, hand_index);
        // put them on top of the current player's deck
        deck_append_card(&p->deck, table_card);
        deck_append_card(&p->deck, player_card);
        // set the last taker to the current player index
        game.last_take = game.turn;
        // next player turn
        game_turn_advance();
    }
}

void game_pick_from_opponent(size_t hand_index, size_t opponent_index)
{
    player *p = current_player();

    if (hand_index >= p->hand.length ||
        opponent_index >= game.players_count)
    {
        log_warning("invalid move");
        return;
    }

    // if the selected hand card has the same value of the top card of the selected opponent
    if (is_card_equal(hand_at(p, hand_index), deck_top(&game.players[opponent_index].deck)))
    {
        // increase the steals count for statistics
        p->steals++;
        // append the opponent's deck on top of the current player's one
        deck_append(&p->deck, &game.players[opponent_index].deck);
        // append the hand selected card on top of the current player's deck
        deck_append_card(&p->deck, hand_throw(p, hand_index));
        // set last taker to the current player index
        game.last_take = game.turn;
        // next player turn
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
    if (is_user_valid(id) == false ||
        is_user_logged(id) == false)
    {
        log_error("invalid user");
    }
}

void game_turn_advance()
{
    game.state = GAME_CYCLE;
    game.turn = (game.turn + 1) % game.players_count;
}


void saves_free()
{
    VEC_FREE(saves);
}

#pragma endregion utility_functions

#pragma region game_persistence

void game_saves_init()
{
    // on application closure free saves vector
    atexit(saves_free);
    // initialize saves vector
    saves = VEC_INIT(game_data);

    if (access(GAME_SAVES_PATH, F_OK) == 0)
    {
        // if saves file exist open it in read mode
        FILE *f = fopen(GAME_SAVES_PATH, "rb");
        // read the saves count
        fread(&saves->count, sizeof(size_t), 1, f);

        if (saves->count > saves->size)
        {
            // if saves are more than current vector size resize the vector accordingly
            VEC_RESIZE(saves, saves->count);
        }
        
        // read saves array and close the file
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

    // append the current game to the saves vector and stop game from running
    VEC_APPEND(game_data, saves, game);
    is_game_running = false;

    // open saves file in writing mode
    FILE *f = fopen(GAME_SAVES_PATH, "wb");
    // write saves count and array and close file
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

    // get save at the given index
    game = VEC_GET(game_data, saves, save_index);
    
    check_user_validity(user1); // check if first user exist and is logged
    check_user_validity(user2); // check if second user exist and is logged

    if (game.players_count >= 3)
        check_user_validity(user3); // check if third user exist and is logged

    if (game.players_count == 4)
        check_user_validity(user4); // check if fourth user exist and is logged

    // set game to running and delete save from vector
    is_game_running = true;
    VEC_DELETE(saves, save_index);
    
    // open saves file in write mode and update it
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

    // if valid save index get its pointer
    return &VEC_GET(game_data, saves, id);
}

#pragma endregion game_persistence

#pragma region json

char *game_data_json(game_data *gd)
{
    char *json = memory_allocate_zero(4096, sizeof(char));
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
    char *json = memory_allocate_zero(4096, sizeof(char));
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