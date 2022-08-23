#ifndef __HISTORY_H__
#define __HISTORY_H__

/**
 * @file history.h
 * @author Mauro De Rosa (it.mauro.derosa@gmail.com)
 * @brief match history module
 * @version 1.0.0 alpha
 * @date 2022-08-22
 * 
 * @copyright Copyright (c) 2022
 * @license licensed under MIT license
 * 
 */

#include <stdlib.h>
#include <game/user.h>
#include <game/game.h>
#include <utils/timeutils.h>

typedef size_t matchid;

/**
 * @brief single match history data for one user
 */
typedef struct
{
    matchid id; ///< the match id
    userid user; ///< the user id
    datetime timestamp; ///< when the match was made
    size_t players; ///< redundant player count for faster statistic filtering
    size_t points; ///< points of the user
    size_t steals; ///< number of steals made by the user
    bool won; ///< has the user won?
} history_entry;

/**
 * @brief initialize history table and load it from file if yet exists
 * 
 */
void history_init();

/**
 * @brief adds game informations to the history table
 * 
 * @param game pointer to the ended game
 */
void add_history_entry(game_data *game);

/**
 * @brief replace any match played by a certain user with an USERID_INVALID
 * 
 * @param id the id of the user to remove from table
 */
void remove_user_from_history(userid id);

/**
 * @brief Get the match history for the user with the given userid
 * 
 * @param user the user id of which you want the history
 * @param history_size pointer to a size_t which will contain the filtered history size
 * @return history_entry* an allocated array of matches played by user (must be freed)
 */
history_entry *get_history_for(userid user, size_t *history_size);

/**
 * @brief json representation on an history entry
 * 
 * @param entry the entry to represent
 * @return char* the json string
 */
char *history_json(history_entry *entry);


#endif /* __HISTORY_H__ */
