#ifndef __RESPONSES_H__
#define __RESPONSES_H__

#include <network/server.h>
#include <game/user.h>
#include <game/game.h>
#include <game/history.h>
#include <game/statistics.h>

void response_json(const char *outcome, const char *json);
void response_failed_json(const char *json);
void response_success_json(const char *json);
void response_message(const char *outcome, const char *message);
void response_failed_message(const char *message);
void response_success_message(const char *message);
void response_quit();
void response_user(user *u);
void response_game_data(game_data *gd);
void response_history(userid id, history_entry *entries, size_t entries_len, size_t page);
void response_statistics(statistics_entry *stats);
void response_leaderboard(leaderboard_entry *leaderboard, size_t leaderboard_size);
void response_game_saves_list(game_data *saves, size_t count);
void response_game_save(game_data *save, size_t id);

#endif /* __RESPONSES_H__ */
