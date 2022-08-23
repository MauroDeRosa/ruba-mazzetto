/**
 * @file statistics.c
 * @author Mauro De Rosa (it.mauro.derosa@gmail.com)
 * @brief statistics module implementation @see statistics.h
 * @version 1.0.0 alpha
 * @date 2022-08-23
 * 
 * @copyright Copyright (c) 2022
 * @license licensed under MIT license
 * 
 */

#include <game/history.h>
#include <game/statistics.h>
#include <io/log.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <types/array.h>
#include <types/vector.h>
#include <types/memory.h>
#include <unistd.h>
#include <utils/mathutils.h>

#define LEADERBOARD_MAX_ENTRIES 10
#define STATISTICS_PATH "statistics.bin"

vector *statistics;

#pragma region private_prototypes

int compare_statistics_by_userid(const void *entry, const void *id);
void save_statistics();
void load_statistics();
void statistics_free();

#pragma endregion prototypes

#pragma region filter_and_compare_functions

int compare_leaderboard_entries(const void *a, const void *b)
{
    const leaderboard_entry *la = a, *lb = b;

    // win rate descending order
    if (la->data.win_rate > lb->data.win_rate)
        return -1;
    if (la->data.win_rate < lb->data.win_rate)
        return 1;

    // wins descending order
    if (la->data.wins > lb->data.wins)
        return -1;
    if (la->data.wins < lb->data.wins)
        return 1;

    // losses ascending order
    if (la->data.losses > lb->data.losses)
        return 1;
    if (la->data.losses < lb->data.losses)
        return -1;

    // points descending order
    if (la->data.avg_points > lb->data.avg_points)
        return -1;
    if (la->data.avg_points < lb->data.avg_points)
        return 1;

    // steals descending order
    if (la->data.avg_points > lb->data.avg_points)
        return -1;
    if (la->data.avg_points < lb->data.avg_points)
        return 1;

    return la->id - lb->id;
}

int compare_statistics_by_userid(const void *entry, const void *id)
{
    const statistics_entry *stats_entry = entry;
    const userid *user_id = id;

    return stats_entry->id - *user_id;
}

#pragma endregion filter_and_compare_functions

#pragma region core

void statistics_init()
{
    atexit(statistics_free);
    statistics = VEC_INIT(statistics_entry);

    if (access(STATISTICS_PATH, F_OK) == 0)
    {
        // file exists
        load_statistics();
    }
    else
    {
        // file doesn't exist
    }
}

#pragma endregion core

#pragma region persistence

void save_statistics()
{
    FILE *f = fopen(STATISTICS_PATH, "wb");
    check_null_pointer(f);
    fwrite(&statistics->count, sizeof(size_t), 1, f);
    fwrite(statistics->elements, statistics->element_size, statistics->count, f);
    fclose(f);
}

void load_statistics()
{
    FILE *f = fopen(STATISTICS_PATH, "rb");
    check_null_pointer(f);
    fread(&statistics->count, sizeof(size_t), 1, f);

    if (statistics->count > statistics->size)
    {
        VEC_RESIZE(statistics, statistics->count);
    }
    
    fread(statistics->elements, statistics->element_size, statistics->count, f);
    fclose(f);
}

#pragma endregion persistence

#pragma region getters_and_setters

statistics_entry *get_statistics_for(userid user)
{
    return binary_search(&user, statistics->elements, statistics->count, sizeof(statistics_entry), compare_statistics_by_userid);
}

leaderboard_entry *get_leaderboard(size_t players_count, size_t *leaderboard_size)
{
    if (players_count < 2 || players_count > 4)
    {
        log_warning("invalid leaderboard category");
        return NULL;
    }

    *leaderboard_size = statistics->count >= LEADERBOARD_MAX_ENTRIES ? LEADERBOARD_MAX_ENTRIES : statistics->count;
    leaderboard_entry *leadeboard = memory_allocate_typed(statistics->count, sizeof(leaderboard_entry));

    for (size_t i = 0; i < statistics->count; i++)
    {
        leadeboard[i].id = VEC_GET(statistics_entry, statistics, i).id;
        leadeboard[i].data = VEC_GET(statistics_entry, statistics, i).players[players_count - 2];
    }

    qsort(leadeboard, statistics->count, sizeof(leaderboard_entry), compare_leaderboard_entries);

    if (*leaderboard_size < statistics->count)
    {
        memory_resize_typed(leadeboard, *leaderboard_size, sizeof(leaderboard_entry));
    }

    return leadeboard;
}

void update_statistics_for(userid user)
{
    statistics_entry *entry = get_statistics_for(user);
    size_t history_size, win_streak = 0;
    history_entry *history;
    statistics_data *data;

    // if statistics for user doesn't exist create it
    if (entry == NULL)
    {
        VEC_APPEND(statistics_entry, statistics, (statistics_entry){0});
        entry = &VEC_GET(statistics_entry, statistics, statistics->count-1);
        entry->id = user;
    }

    history = get_history_for(user, &history_size);

    // setting default values for statistics calculation
    for (size_t i = 0; i < 3; i++)
    {
        memset(&entry->players[i], 0, sizeof(statistics_data));
        entry->players[i].avg_points = entry->players[i].avg_steals = 0.0f;
    }

    // if there are no history entries exit from function with default values
    if (history_size < 1)
    {
        save_statistics();
        return;
    }

    for (size_t i = 0; i < history_size; i++)
    {
        // set data pointer relative to current history entry player count (index 0: 2 players, index 1: 3 players, index 2: 4 players)
        data = &entry->players[history[i].players - 2];

        // increase match count
        data->matches++;
        // add points and steals for average calculation
        data->avg_points += history[i].points;
        data->avg_steals += history[i].steals;
        // finding max points in history
        data->max_points = MAX(history[i].points, data->max_points);
        // finding min points in history
        data->min_points = MIN(history[i].points, data->min_points);
        // finding max steals in history
        data->max_steals = MAX(history[i].steals, data->max_steals);
        // finding min steals in history
        data->min_steals = MIN(history[i].steals, data->min_steals);

        if (history->won)
        {
            // increasing wins and current win streak
            win_streak++;
            data->wins++;
        }
        else
        {
            // if current win streak is longer than the previously found one update max_win_streak
            if (win_streak > data->max_win_streak)
            {
                data->max_win_streak = win_streak;
            }

            // reset the win streak
            win_streak = 0;
            // increase losses counter
            data->losses++;
        }
    }

    for (size_t i = 0; i < 3; i++)
    {
        if (entry->players[i].matches > 0)
        {
            // calculate win rate
            entry->players[i].win_rate = entry->players[i].wins / (entry->players[i].wins + entry->players[i].losses) * 100;
            // calculate average points and steals
            entry->players[i].avg_points /= entry->players[i].matches;
            log_debug("%zup %f\n", i, entry->players[i].avg_points);
            entry->players[i].avg_steals /= entry->players[i].matches;
            log_debug("%zus %f\n", i, entry->players[i].avg_steals);
        }
        else
        {
            entry->players[i].win_rate = 0;
            entry->players[i].avg_points = 0;
            entry->players[i].avg_steals = 0;
        }
    }

    save_statistics();
}

#pragma endregion getters_and_setters

/**
 * @brief json representation of statistics_data structure
 *
 * @param data pointer to the statistics_data to represent
 * @return char* json string (must be freed)
 */
char *statistics_data_json(statistics_data *data)
{
    check_null_pointer(data);
    char *json = memory_allocate(2048);
    sprintf(json,
            "{"
            "\"matches\": %zu,"
            "\"wins\": %zu,"
            "\"losses\": %zu,"
            "\"win_rate\": %.2f,"
            "\"max_win_streak\": %zu,"
            "\"min_points\": %zu,"
            "\"max_points\": %zu,"
            "\"avg_points\": %.2f,"
            "\"min_steals\": %zu,"
            "\"max_steals\": %zu,"
            "\"avg_steals\": %.2f"
            "}",
            data->matches, data->wins, data->losses, data->win_rate, data->max_win_streak,
            data->min_points, data->max_points, data->avg_points,
            data->min_steals, data->max_steals, data->avg_steals);
    return json;
}

char *statistics_entry_json(statistics_entry *entry)
{
    check_null_pointer(entry);
    char *json = memory_allocate(4096);
    char username[USERNAME_SIZE] = "invalid user";
    char *json_2players = statistics_data_json(&entry->players[0]);
    char *json_3players = statistics_data_json(&entry->players[1]);
    char *json_4players = statistics_data_json(&entry->players[2]);

    if (user_exists(entry->id))
    {
        strcpy(username, get_user(entry->id)->username);
    }

    sprintf(json,
            "{"
            "\"user_id\":%zu,"
            "\"username\":\"%s\","
            "\"statistics\":[%s, %s, %s]"
            "}",
            entry->id, username, json_2players, json_3players, json_4players);
    free(json_2players);
    free(json_3players);
    free(json_4players);
    return json;
}

char *leaderboard_json(leaderboard_entry *leaderboard, size_t leaderboard_size)
{
    check_null_pointer(leaderboard);
    char *json = memory_allocate(4096), *data;
    char tmp[2048] = {0};
    char username[USERNAME_SIZE];
    sprintf(json, "[");

    for (size_t i = 0; i < leaderboard_size; i++)
    {
        if (user_exists(leaderboard[i].id))
        {
            strcpy(username, get_user(leaderboard[i].id)->username);
        }
        else
        {
            strcpy(username, "invalid user");
        }

        data = statistics_data_json(&leaderboard[i].data);
        sprintf(tmp, "{\"id\":%zu, \"username\":\"%s\", \"data\":%s}%s",
                leaderboard[i].id, username, data, i == leaderboard_size-1 ? "" : ",");
        free(data);
        strcat(json, tmp);
    }
    strcat(json, "]");
    return json;
}

void statistics_free()
{
    VEC_FREE(statistics);
}