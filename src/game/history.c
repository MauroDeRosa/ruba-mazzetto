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

/**
 * @brief filtering function to filter history by user id
 * 
 * @param entry the currently checked entry
 * @param id the user id for which you want to filter history
 * @return true id equals the current entry user id
 * @return false id is not equal to the current entry user id
 */
bool filter_by_userid(void *entry, void *id);

/**
 * @brief filtering function to filter history by match id
 * 
 * @param entry the currently checked entry
 * @param id the match id for which you want to filter history
 * @return true id equals the current entry match id
 * @return false id is not equal to the current entry match id
 */
bool filter_by_matchid(void *entry, void *id);

/**
 * @brief compare history entries by timestamp in descending order
 * 
 * @param a first entry
 * @param b second entry
 * @return int [<0] a > b [=0] a == b [>0] b > a
 */
int compare_history_by_timestamp(const void *a, const void *b);

/**
 * @brief save history vector to file
 */
void save_history();

/**
 * @brief load history vector from file
 */
void load_history();

/**
 * @brief free history vector
 */
void history_free();

#pragma endregion prototypes

#pragma region filter_and_compare_functions

bool filter_by_userid(void *entry, void *id)
{
    history_entry *history_entry = entry;
    userid *user_id = id;
    
    // returns true if the current entry has the same user id of the one you're filtering for
    return history_entry->user == *user_id;
}

bool filter_by_matchid(void *entry, void *id)
{
    history_entry *history_entry = entry;
    matchid *match_id = id;
    
    // returns true if the current entry has the same match id of the one you're filtering for
    return history_entry->id == *match_id;
}

int compare_history_by_timestamp(const void *a, const void *b)
{
    const history_entry *ae = a;
    const history_entry *be = b;
    
    // returns time difference between two timestamps in descending order
    return compare_datetime_desc(&ae->timestamp, &be->timestamp);
}

#pragma endregion filter_and_compare_functions

#pragma region core

void history_init()
{
    // on program closure free history vector
    atexit(history_free);

    // initialize history vector
    history = VEC_INIT(history_entry);

    if (access(HISTORY_PATH, F_OK) == 0)
    {
        // if file exists load history vector from it
        load_history();
    }
    else
    {
        // if file doesn't exist set last_match_id to zero
        last_match_id = 0;
    }
}

void add_history_entry(game_data *game)
{
    // increase last match id and create an entry
    last_match_id++; 
    history_entry entry;
    
    // adds game info for each player (1 history entry for each user with the same match_id)
    for (size_t i = 0; i < game->players_count; i++)
    {
        entry = (history_entry){
            .id = last_match_id, // match id
            .user = game->players[i].id, // current player's id
            .players = game->players_count, // count of players
            .timestamp = game->timestamp, // timestamp of the game
            .points = game->players[i].deck.length, // points of the current player
            .steals = game->players[i].steals, // steals made by the current player
            .won = i == game->winner}; // true if current player index equals winner index
        // append the created entry to the history vector
        VEC_APPEND(history_entry, history, entry);
    };

    // save history to file
    save_history();
}

#pragma endregion core

#pragma region persistence

void save_history()
{
    // opens the history file in write mode
    FILE *f = fopen(HISTORY_PATH, "wb");
    check_null_pointer(f);
    // writes last match id
    fwrite(&last_match_id, sizeof(matchid), 1, f);
    // writes history count
    fwrite(&history->count, sizeof(size_t), 1, f);
    // writes history array
    fwrite(history->elements, history->element_size, history->count, f);
    fclose(f);
}

void load_history()
{
    // opens the history file in read mode
    FILE *f = fopen(HISTORY_PATH, "rb");
    check_null_pointer(f);
    // reads the last match id
    fread(&last_match_id, sizeof(matchid), 1, f);
    // reads the history array count
    fread(&history->count, sizeof(size_t), 1, f);

    if (history->count > history->size)
    {
        // resize vector if history count is greater then vector size
        VEC_RESIZE(history, history->count);
    }

    // reads history array
    fread(history->elements, history->element_size, history->count, f);
    fclose(f);
}

#pragma endregion persistence

#pragma region getters

history_entry *get_history_for(userid user, size_t *history_size)
{
    check_null_pointer(history_size);

    // checks if there's any match entry in history and the user actually exists
    if (history->count > 0 && user_exists(user))
    {
        // filtered array with history entries for the specified user
        history_entry *filtered = array_filter(history->elements, history->count, history->element_size, filter_by_userid, &user, history_size);
        
        // if the history's not null
        if (filtered != NULL)
        {
            // sort history entries by their timestamp in decreasing order and return the filtered list
            qsort(filtered, *history_size, sizeof(history_entry), compare_history_by_timestamp);
            return filtered;
        }
        else
        {
            // set filtered array size to 0 and return NULL
            *history_size = 0;
            return NULL;
        }
    }
    else
    {
        // if there isn't any history entry return null with history array size set to 0
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
    
    // returns history as json
    return json;
}

void history_free()
{
    VEC_FREE(history);
}
