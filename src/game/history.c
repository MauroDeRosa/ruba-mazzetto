#include <game/history.h>
#include <types/memory.h>
#include <types/array.h>
#include <types/vector.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#define DEFAULT_HISTORY_SIZE 20
#define HISTORY_PATH "history.bin"

vector *history;
matchid last_match_id;

#pragma region private_prototypes

bool filter_by_userid(void *entry, void *id);
bool filter_by_matchid(void *entry, void *id);
int compare_history_by_timestamp(const void *a, const void *b);
void save_history();
void load_history();
void history_free();

#pragma endregion prototypes

#pragma region filter_and_compare_functions

bool filter_by_userid(void *entry, void *id)
{
    history_entry *history_entry = entry;
    userid *user_id = id;

    return history_entry->user == *user_id;
}

bool filter_by_matchid(void *entry, void *id)
{
    history_entry *history_entry = entry;
    matchid *match_id = id;

    return history_entry->id == *match_id;
}

int compare_history_by_timestamp(const void *a, const void *b)
{
    const history_entry *ae = a;
    const history_entry *be = b;

    return compare_datetime_desc(&ae->timestamp, &be->timestamp);
}

#pragma endregion filter_and_compare_functions

#pragma region core

void history_init()
{
    atexit(history_free);
    history = VEC_INIT(history_entry);

    if (access(HISTORY_PATH, F_OK) == 0)
    {
        // file exists
        load_history();
    }
    else
    {
        // file doesn't exist
        last_match_id = 0;
    }
}

void add_history_entry(game_data *game)
{
    last_match_id++;
    history_entry entry;

    for (size_t i = 0; i < game->players_count; i++)
    {
        entry = (history_entry){
            .id = last_match_id,
            .user = game->players[i].id,
            .players = game->players_count,
            .timestamp = game->timestamp,
            .points = game->players[i].deck.length,
            .steals = game->players[i].steals,
            .won = i == game->winner};
        VEC_APPEND(history_entry, history, entry);
    };

    save_history();
}

#pragma endregion core

#pragma region persistence

void save_history()
{
    FILE *f = fopen(HISTORY_PATH, "wb");
    check_null_pointer(f);
    fwrite(&last_match_id, sizeof(matchid), 1, f);
    fwrite(&history->count, sizeof(size_t), 1, f);
    fwrite(history->elements, history->element_size, history->count, f);
    fclose(f);
}

void load_history()
{
    FILE *f = fopen(HISTORY_PATH, "rb");
    check_null_pointer(f);
    fread(&last_match_id, sizeof(matchid), 1, f);
    fread(&history->count, sizeof(size_t), 1, f);

    if (history->count > history->size)
    {
        VEC_RESIZE(history, history->count);
    }

    fread(history->elements, history->element_size, history->count, f);
    fclose(f);
}

#pragma endregion persistence

#pragma region getters

history_entry *get_history_for(userid user, size_t *history_size)
{
    check_null_pointer(history_size);

    if (history->count > 0 && is_userid_in_table(user))
    {
        history_entry *filtered = array_filter(history->elements, history->count, history->element_size, filter_by_userid, &user, history_size);
        qsort(filtered, *history_size, sizeof(history_entry), compare_history_by_timestamp);
        return filtered;
    }
    else
    {
        *history_size = 0;
        return NULL;
    }
}

#pragma endregion getters

char *history_json(history_entry *entry)
{
    char *json = memory_allocate_zero(4096, sizeof(char));
    char timestamp_str[DATETIME_STR_SIZE];

    datetime_string(entry->timestamp, timestamp_str);

    sprintf(json,
            "{"
            "\"matchid\":%zu,"
            "\"players\":%zu,"
            "\"points\":%zu,"
            "\"steals\":%zu,"
            "\"timestamp\":\"%s\","
            "\"userid\":%zu,"
            "\"won\":%s"
            "}",
            entry->id,
            entry->players,
            entry->points,
            entry->steals,
            timestamp_str,
            entry->user,
            entry->won ? "true" : "false");

    return json;
}

void history_free()
{
    VEC_FREE(history);
}