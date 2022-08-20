#ifndef __GAME_H__
#define __GAME_H__

#include <utils/timeutils.h>
#include <game/player.h>
#include <game/cards.h>
#include <game/user.h>
#include <stddef.h>
#include <stdbool.h>

/**
 * @brief game state enum
 */
typedef enum
{
    GAME_START, ///< game just started
    GAME_SERVE, ///< serving cards to players and putting cards onto table
    GAME_CYCLE, ///< cycle through player turns, check if a player do his move, if must serve cards or game's ended
    GAME_WAITING, ///< waiting for player move
    GAME_END ///< game ended append table cards to last taker and check who's the winner
} game_state;

/**
 * @brief game's table (implemented as a deck)
 */
typedef deck game_table;

/**
 * @brief game's data used for current game and saves
 */
typedef struct
{
    player players[4]; ///< players array @see player
    size_t players_count; ///< how many players in this match (2, 3 or 4 players)
    size_t last_take; ///< index of the last player who took cards from the game's table or from another player's deck
    size_t turn;  ///< index of the currently playing player
    deck dealer_deck; ///< the game's dealer deck (full at game start)
    game_table table; ///< the game's table (initialized with 4 cards)
    game_state state; ///< the current game's state
    userid winner; ///< the game's winner
    datetime timestamp; ///< when the game started
} game_data;

/**
 * @brief initialize a new game with 2 players
 * 
 * @param user1 the first player's user id
 * @param user2 the second player's user id
 */
void game_init_2players(userid user1, userid user2);

/**
 * @brief initialize a new game with 3 players
 * 
 * @param user1 the first player's user id
 * @param user2 the second player's user id
 * @param user3 the third player's user id
 */
void game_init_3players(userid user1, userid user2, userid user3);

/**
 * @brief initialize a new game with 4 players
 * 
 * @param user1 the first player's user id
 * @param user2 the second player's user id
 * @param user3 the third player's user id
 * @param user4 the fourth player's user id
 */
void game_init_4players(userid user1, userid user2, userid user3, userid user4);

/**
 * @brief advance through the game states (one state each call) if a game is initialized or loaded
 */
void game_update();

/**
 * @brief throw current player's card on the table from his hand
 * 
 * @param hand_index the index of the card in the player's hand
 */
void game_throw_on_table(size_t hand_index);

/**
 * @brief pick a card of the same value from the table with the current player (changes state to cycle if successfully picks)
 * 
 * @param hand_index the index of the card in the player's hand
 * @param table_index the index of the card on the table you want to pick
 */
void game_pick_from_table(size_t hand_index, size_t table_index);

/**
 * @brief steal the deck of one of the opponents if has card of same value on top with the current player (changes state to cycle if successfully steals)
 * 
 * @param hand_index 
 * @param opponent_index 
 */
void game_pick_from_opponent(size_t hand_index, size_t opponent_index);

/**
 * @brief Get the current game's data
 * 
 * @return game_data* current game data pointer
 */
game_data* get_game_data();

/**
 * @brief  initialize saves vector and load saves from binary file if exists
 */
void game_saves_init();

/**
 * @brief save the current game to saves vector and write saves vector to binary file
 */
void game_save();

/**
 * @brief load a game from the saves vector at the given index
 * 
 * @param save_index index of the saved game
 * @param user1 first player id (must be logged)
 * @param user2 second player id (must be logged)
 * @param user3 third player id (must be logged) or 0 if the loaded game has only 2 players
 * @param user4 fourth player id (must be logged) or 0 if the loaded game has only 3 players
 */
void game_load(size_t save_index, userid user1, userid user2, userid user3, userid user4);

/**
 * @brief get the savelist
 * 
 * @param list_size pointer to a size_t which will contain the savelist length
 * @return game_data* array of the game saves
 */
game_data *game_save_list(size_t *list_size);

/**
 * @brief get the game save at the given index
 * 
 * @param id index of the saved game
 * @return game_data* game save pointer
 */
game_data *game_save_by_id(size_t id);

/**
 * @brief json representation of the game_data
 *
 * @param gd game data pointer
 * @return char* json string (must be freed)
 */
char *game_data_json(game_data *gd);

/**
 * @brief json representation fo a game save
 * 
 * @param save game save pointer
 * @param id index of the game save
 * @return char* json string (must be freed)
 */
char *game_save_json(game_data *save, size_t id);

/**
 * @brief json representation of the save list
 *
 * @return char* json string (must be freed)
 */
char *game_save_list_json();

#endif /* __GAME_H__ */
