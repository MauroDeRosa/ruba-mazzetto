#ifndef __STATISTICS_H__
#define __STATISTICS_H__

/**
 * @file statistics.h
 * @author Mauro De Rosa (it.mauro.derosa@gmail.com)
 * @brief user statistics module
 * @version 0.1
 * @date 2022-08-22
 * 
 * @copyright Copyright (c) 2022
 * @license licensed under MIT license
 * 
 */

#include <stdlib.h>
#include <game/user.h>

/**
 * @brief statistic data for a single category
 */
typedef struct
{
    size_t matches;        ///< number of played matches
    size_t wins;           ///< number of wins
    size_t losses;         ///< number of losses
    double win_rate;             ///< win/losses rate
    size_t max_win_streak; ///< number of wins in a row
    size_t min_points;     ///< minimum end game points
    size_t max_points;     ///< maximum end game points
    double avg_points;      ///< average end game points
    size_t min_steals;     ///< minimum end game points
    size_t max_steals;     ///< maximum end game points
    double avg_steals;      ///< average end game points
} statistics_data;

/**
 * @brief statistic entry for each user containing all the three game categories (2 / 3 / 4 players)
 */
typedef struct
{
    userid id; ///< the id of the user
    statistics_data players[3]; ///< the statistics for each category
} statistics_entry;

/**
 * @brief statistic entry for each user containing a single game category
 */
typedef struct 
{
    userid id;
    statistics_data data;
} leaderboard_entry;


/**
 * @brief initialize statistics vector and load data from file if exists
 */
void statistics_init();

/**
 * @brief Get static_entry for user by the given id
 * 
 * @param user the user id
 * @return statistics_entry* the statistic_entry pointer (NULL if entry doesn't exist)
 */
statistics_entry *get_statistics_for(userid user);

/**
 * @brief updates statistic_entry for user of the given id
 * 
 * @param user the user id
 */
void update_statistics_for(userid user);

/**
 * @brief json representation of statistics_entry type
 * 
 * @param data pointer to the statistics_entry to represent
 * @return char* json string (must be freed)
 */
char *statistics_entry_json(statistics_entry *entry);

/**
 * @brief Get leaderboard array for based on players count
 * 
 * @param players_count players count category (i.e. 2 players, 3 players, 4 players)
 * @param leaderboard_size how many players are in the leaderboard
 * @return leaderboard_entry* 
 */
leaderboard_entry *get_leaderboard(size_t players_count, size_t *leaderboard_size);

/**
 * @brief json representation of a leaderboard
 * 
 * @param leaderboard pointer to the leaderboard to represent
 * @param leaderboard_size size of the leaderboard array
 * @return char* json string (must be freed)
 */
char *leaderboard_json(leaderboard_entry *leaderboard, size_t leaderboard_size);

#endif /* __STATISTICS_H__ */
