#ifndef __GAME_H__
#define __GAME_H__

#include <utils/timeutils.h>
#include <game/player.h>
#include <game/cards.h>
#include <game/user.h>
#include <stddef.h>
#include <stdbool.h>

typedef enum
{
    GAME_START,
    GAME_SERVE,
    GAME_CYCLE,
    GAME_WAITING,
    GAME_END
} game_state;

typedef deck game_table;

typedef struct
{
    player players[4];
    size_t players_count;
    size_t last_take;
    size_t turn;
    deck dealer_deck;
    game_table table;
    game_state state;
    userid winner;
    datetime timestamp;
} game_data;

void game_init_2players(userid user1, userid user2);
void game_init_3players(userid user1, userid user2, userid user3);
void game_init_4players(userid user1, userid user2, userid user3, userid user4);

void game_state_start();
void game_state_serve();
void game_state_cycle();

void game_update();
void game_throw_on_table(size_t hand_index);
void game_pick_from_table(size_t hand_index, size_t table_index);
void game_pick_from_opponent(size_t hand_index, size_t opponent_index);
game_data* get_game_data();

void game_saves_init();
void game_save();
void game_load(size_t save_index, userid user1, userid user2, userid user3, userid user4);
game_data *game_save_list(size_t *list_size);
game_data *game_save_by_id(size_t id);


/**
 * @brief json representation of the game_data
 *
 * @param gd game data pointer
 * @return char* json string (must be freed)
 */
char *game_data_json(game_data *gd);

char *game_save_json(game_data *save, size_t id);

/**
 * @brief json representation of the save list
 *
 * @return char* json string (must be freed)
 */
char *game_save_list_json();

#endif /* __GAME_H__ */
