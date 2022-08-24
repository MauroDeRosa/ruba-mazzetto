/**
 * @file responses.c
 * @author Mauro De Rosa (it.mauro.derosa@gmail.com)
 * @brief responses module implementation @see responses.h
 * @version 1.0.0 alpha
 * @date 2022-08-24
 * 
 * @copyright Copyright (c) 2022
 * @license licensed under MIT license
 * 
 */

#include <io/log.h>
#include <network/responses.h>
#include <network/server.h>
#include <utils/timeutils.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <game/cards.h>
#include <game/history.h>
#include <game/statistics.h>

#pragma region basic_response_json

void response_json(const char *result, const char *json)
{
    char buffer[SRV_BUFF_SIZE] = {0};
    sprintf(buffer, "{\"result\":\"%s\",\"type\":\"json\",\"data\":%s}", result, json);
    server_send(buffer);
}

void response_failed_json(const char *json)
{
    response_json("failed", json);
}

void response_success_json(const char *json)
{
    response_json("success", json);
}

#pragma endregion basic_response_json

#pragma region basic_response_message

void response_message(const char *result, const char *message)
{
    char buffer[SRV_BUFF_SIZE] = {0};
    sprintf(buffer, "{\"result\":\"%s\",\"type\":\"message\",\"message\":\"%s\"}", result, message);
    server_send(buffer);
}

void response_failed_message(const char *message)
{
    response_message("failed", message);
}

void response_success_message(const char *message)
{
    response_message("success", message);
}

#pragma endregion basic_response_message


void response_quit()
{
    response_success_message("quit");
    server_quit = true;
}

void response_user(user *u)
{
    char *buffer = user_json(u);
    response_success_json(buffer);
    free(buffer);
}

void response_game_data(game_data *gd)
{
    char *buffer = game_data_json(gd);
    response_success_json(buffer);
    free(buffer);
}

void response_history(userid id, history_entry *entries, size_t entries_len, size_t page)
{
    char buffer[SRV_BUFF_SIZE] = {0};
    char username[USERNAME_SIZE] = "invalid user";
    char *history_str;
    size_t from = (page - 1) * 15;
    size_t to = from + 15 > entries_len ? entries_len : from + 15;
    user *u = get_user(id);

    if (u != NULL)
    {
        strcpy(username, u->username);
    }

    sprintf(buffer, "{\"pages\":%zu,\"username\":\"%s\",\"entries\":[", 
        entries_len % 15 != 0 ? entries_len / 15 + 1 : entries_len / 15,
        username);

    for (size_t index = from; index < to; index++)
    {
        history_str = history_json(&entries[index]);
        strcat(buffer, history_str);
        free(history_str);

        if (index != to - 1)
        {
            strcat(buffer, ",");
        }
    }

    strcat(buffer, "]}");
    response_success_json(buffer);
}

void response_statistics(statistics_entry *stats)
{
    char *buffer = statistics_entry_json(stats);
    response_success_json(buffer);
    free(buffer);
}

void response_leaderboard(leaderboard_entry *leaderboard, size_t leaderboard_size)
{
    char *buffer = leaderboard_json(leaderboard, leaderboard_size);
    response_success_json(buffer);
    free(buffer);
}

void response_game_saves_list(game_data *saves, size_t count)
{
    char *buffer = game_save_list_json(saves, count);
    response_success_json(buffer);
    free(buffer);
}

void response_game_save(game_data *save, size_t id)
{
    char *buffer = game_save_json(save, id);
    response_success_json(buffer);
    free(buffer);
}