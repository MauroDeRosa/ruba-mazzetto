#include <network/parser.h>
#include <network/server.h>
#include <network/responses.h>
#include <io/log.h>
#include <types/memory.h>
#include <string.h>
#include <stdbool.h>
#include <game/user.h>
#include <game/history.h>
#include <game/statistics.h>
#include <stdio.h>
#include <utils/timeutils.h>
#include <utils/stringutils.h>

char *tokens[1024];
size_t tokens_count;
size_t current_token;

void parse_request_user();
void parse_request_game();
void parse_request_history();
void parse_request_statistics();

void tokenize_request(char *message)
{
    check_null_pointer(message);
    current_token = 0;
    tokens_count = 0;
    tokens[tokens_count] = strtok(message, " ");

    while (tokens[tokens_count] != NULL)
    {
        tokens_count++;
        tokens[tokens_count] = strtok(NULL, " ");
    }
}

char *token_next()
{
    if (current_token >= tokens_count)
    {
        log_error("no more tokens");
    }

    return tokens[current_token++];
}

bool token_next_equal_to(const char *str)
{
    if (current_token >= tokens_count)
    {
        log_error("no more tokens");
    }

    if (is_string_equal(tokens[current_token], str))
    {
        current_token++;
        return true;
    }
    else
    {
        return false;
    }
}

void parse_request(char *message)
{
    tokenize_request(message);

    if (token_next_equal_to("user"))
    {
        parse_request_user();
    }
    else if (token_next_equal_to("game"))
    {
        parse_request_game();
    }
    else if (token_next_equal_to("history"))
    {
        parse_request_history();
    }
    else if (token_next_equal_to("statistics"))
    {
        parse_request_statistics();
    }
    else if (token_next_equal_to("quit"))
    {
        response_quit();
    }
    else
    {
        response_failed_message("unknown request");
    }
}

void parse_request_user()
{
    char *username, *password;
    if (token_next_equal_to("register"))
    {
        username = token_next();
        password = token_next();
        userid id = user_register(username, password);

        if (id == USERID_INVALID)
        {
            response_failed_message(last_log);
        }
        else
        {
            user *u = get_user(id);
            update_statistics_for(id);
            response_user(u);
        }
    }
    else if (token_next_equal_to("login"))
    {
        username = token_next();
        password = token_next();
        userid id = user_login(username, password);

        if (id == USERID_INVALID)
        {
            response_failed_message(last_log);
        }
        else
        {
            user *u = get_user(id);
            response_user(u);
        }
    }
    else if (token_next_equal_to("update"))
    {
        userid id;
        sscanf(token_next(), "%zu", &id);
        id = user_update(id, token_next());

        if (id == USERID_INVALID)
        {
            response_failed_message(last_log);
        }
        else
        {
            user *u = get_user(id);
            response_user(u);
        }
    }
    else if (token_next_equal_to("logout"))
    {
        userid id;
        sscanf(token_next(), "%zu", &id);
        user_logout(id);
        response_success_message("logged out successfully");
    }
    else if (token_next_equal_to("delete"))
    {
        userid id;
        sscanf(token_next(), "%zu", &id);
        user_delete(id);

        if (user_exists(id))
        {
            response_failed_message("user doesn't exist");
        }
        else
        {
            response_success_message("user deleted");
        }
    }
    else if (token_next_equal_to("get"))
    {
        userid id;
        sscanf(token_next(), "%zu", &id);

        if (user_exists(id) == false)
        {
            response_failed_message("user with the given id is not in table");
        }
        else
        {
            user *u = get_user(id);
            response_user(u);
        }
    }
    else
    {
        response_failed_message("unknown user request");
    }
}

void parse_request_game()
{
    size_t hand_index, objective_index;

    if (token_next_equal_to("init"))
    {
        size_t pl_cnt, id1, id2, id3, id4;
        sscanf(token_next(), "%zu", &pl_cnt);
        sscanf(token_next(), "%zu", &id1);
        sscanf(token_next(), "%zu", &id2);

        if (pl_cnt == 2)
        {
            game_init_2players(id1, id2);
        }
        else if (pl_cnt == 3)
        {
            sscanf(token_next(), "%zu", &id3);
            game_init_3players(id1, id2, id3);
        }
        else if (pl_cnt == 4)
        {
            sscanf(token_next(), "%zu", &id3);
            sscanf(token_next(), "%zu", &id4);
            game_init_4players(id1, id2, id3, id4);
        }
        else
        {
            response_failed_message("players count must be in range 2-4");
        }

        response_success_message("game started");
    }
    else if (token_next_equal_to("update"))
    {
        game_update();
        response_game_data(get_game_data());
    }
    else if (token_next_equal_to("pick"))
    {
        if (get_game_data()->state != GAME_WAITING)
        {
            response_failed_message("can't pick game is not in waiting state");
            return;
        }

        if (token_next_equal_to("table"))
        {
            sscanf(token_next(), "%zu", &hand_index);
            sscanf(token_next(), "%zu", &objective_index);
            game_pick_from_table(hand_index, objective_index);

            if (get_game_data()->state == GAME_WAITING)
            {
                response_failed_message("invalid pick table move");
            }
            else
            {
                response_game_data(get_game_data());
            }
        }
        else if (token_next_equal_to("opponent"))
        {
            sscanf(token_next(), "%zu", &hand_index);
            sscanf(token_next(), "%zu", &objective_index);
            game_pick_from_opponent(hand_index, objective_index);

            if (get_game_data()->state == GAME_WAITING)
            {
                response_failed_message("invalid pick opponent move");
            }
            else
            {
                response_game_data(get_game_data());
            }
        }
        else
        {
            response_failed_message("unknown pick move use 'opponent' or 'table'");
        }
    }
    else if (token_next_equal_to("throw"))
    {
        size_t hand_index;

        if (get_game_data()->state != GAME_WAITING)
        {
            response_failed_message("can't throw card game is not in waiting state");
            return;
        }

        sscanf(token_next(), "%zu", &hand_index);
        game_throw_on_table(hand_index);

        if (get_game_data()->state == GAME_WAITING)
        {
            response_failed_message("invalid throw move");
        }
        else
        {
            response_game_data(get_game_data());
        }
    }
    else if (token_next_equal_to("save"))
    {
        game_save();
        response_success_message("game saved");
    }
    else if (token_next_equal_to("savelist"))
    {
        size_t saves_count;
        game_data *saves = game_save_list(&saves_count);
        response_game_saves_list(saves, saves_count);
    }
    else if (token_next_equal_to("saveget"))
    {
        size_t id;
        sscanf(token_next(), "%zu", &id);
        game_data *save = game_save_by_id(id);
        response_game_save(save, id);
    }
    else if (token_next_equal_to("load"))
    {
        size_t id;
        userid user1, user2, user3, user4;
        sscanf(token_next(), "%zu", &id);
        sscanf(token_next(), "%zu", &user1);
        sscanf(token_next(), "%zu", &user2);
        sscanf(token_next(), "%zu", &user3);
        sscanf(token_next(), "%zu", &user4);
        game_load(id, user1, user2, user3, user4);
        response_success_message("game loaded");
    }
    else
    {
        response_failed_message("invalid game request");
    }
}

void parse_request_history()
{
    if (token_next_equal_to("get"))
    {
        userid id,
            page; // 1 indexed
        size_t entries_len = 0;
        sscanf(token_next(), "%zu", &id);
        sscanf(token_next(), "%zu", &page);
        history_entry *entries = get_history_for(id, &entries_len);
        response_history(id, entries, entries_len, page);
        free(entries);
    }
}

void parse_request_statistics()
{
    if (token_next_equal_to("get"))
    {
        userid id;
        sscanf(token_next(), "%zu", &id);
        statistics_entry *stats = get_statistics_for(id);

        if (stats != NULL)
        {
            response_statistics(stats);
        }
        else
        {
            response_failed_message("no statistics for the selected user");
        }
    }
    else if (token_next_equal_to("leaderboard"))
    {
        size_t category;
        size_t leaderboard_size;
        sscanf(token_next(), "%zu", &category);
        leaderboard_entry *leaderboard = get_leaderboard(category, &leaderboard_size);

        if (leaderboard != NULL)
        {
            response_leaderboard(leaderboard, leaderboard_size);
            free(leaderboard);
        }
        else
        {
            response_failed_message("unable to get leaderboard");
        }
    }
}