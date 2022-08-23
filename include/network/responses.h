#ifndef __RESPONSES_H__
#define __RESPONSES_H__

/**
 * @file responses.h
 * @author Mauro De Rosa (it.mauro.derosa@gmail.com)
 * @brief json responses to send through the IPC server
 * @version 0.1
 * @date 2022-08-23
 * 
 * @copyright Copyright (c) 2022
 * @license licensed under MIT license
 * 
 */

#include <network/server.h>
#include <game/user.h>
#include <game/game.h>
#include <game/history.h>
#include <game/statistics.h>

/**
 * @brief generic json response containing result (string) and data (json object)
 * 
 * @param result the response's outcome (success, failed, ...)
 * @param json json object string
 */
void response_json(const char *result, const char *json);

/**
 * @brief failure json response containing data (json object)
 * 
 * @param json json object string
 */
void response_failed_json(const char *json);

/**
 * @brief success json response containing data (json object)
 * 
 * @param json json object string
 */
void response_success_json(const char *json);

/**
 * @brief generic json response containing result (string) and message (string)
 * 
 * @param result the response's outcome (success, failure, ecc.)
 * @param message string of the message to send
 */
void response_message(const char *result, const char *message);

/**
 * @brief failure json response containing message (string)
 * 
 * @param message string of the message to send
 */
void response_failed_message(const char *message);

/**
 * @brief success json response containing message (string)
 * 
 * @param message string of the message to send
 */
void response_success_message(const char *message);

/**
 * @brief quit message response
 */
void response_quit();

/**
 * @brief json response containing a user representation
 * 
 * @param u the user to represent @see user
 */
void response_user(user *u);

/**
 * @brief json response containing the given game_data values
 * 
 * @param gd the game to represent in json format @see game_data
 */
void response_game_data(game_data *gd);

/**
 * @brief json response containing history for the requested user
 * 
 * @param id user's id @see user
 * @param entries array of the user's history entries @see history_entry
 * @param entries_len size of the entries array
 * @param page history page
 */
void response_history(userid id, history_entry *entries, size_t entries_len, size_t page);

/**
 * @brief json response containing game statistics
 * 
 * @param stats pointer to a user's statistics entry @see statistics_entry
 */
void response_statistics(statistics_entry *stats);

/**
 * @brief json response containing a leaderboard
 * 
 * @param leaderboard single category leaderboard_entry array @see leaderboard_entry
 * @param leaderboard_size count of elements in the leaderboard
 */
void response_leaderboard(leaderboard_entry *leaderboard, size_t leaderboard_size);

/**
 * @brief json response containing a list of the saved games
 * 
 * @param saves array of saves @see game_data
 * @param count saves array size
 */
void response_game_saves_list(game_data *saves, size_t count);

/**
 * @brief json response containing a saved game representation
 * 
 * @param save the save to represent @see save
 * @param id the save id in saves_list (currently is the index in the vector)
 */
void response_game_save(game_data *save, size_t id);

#endif /* __RESPONSES_H__ */
