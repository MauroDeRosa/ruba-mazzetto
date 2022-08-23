#ifndef __PLAYER_H__
#define __PLAYER_H__

/**
 * @file player.h
 * @author Mauro De Rosa (it.mauro.derosa@gmail.com)
 * @brief player data structure and functions
 * @version 0.1
 * @date 2022-07-28
 * 
 * @copyright Copyright (c) 2022
 * @license licensed under MIT license
 * 
 */

#include <game/cards.h>
#include <game/user.h>
#include <stdbool.h>

#define HAND_SIZE 3 ///< maximum cards in hand

/**
 * @brief player hand cards data structure
 */
typedef struct
{
    card cards[HAND_SIZE]; ///< cards array of size HAND_SIZE @see HAND_SIZE
    size_t length; ///< how many cards are in the hand
} player_hand;

/**
 * @brief player data structure
 */
typedef struct
{
    userid id; ///< the user id associated to the player
    char username[USERNAME_SIZE]; ///< user's username
    deck deck; ///< the player's deck @see deck
    player_hand hand; ///< the player's hand @see player_hand
    size_t steals; ///< count of steals in a game
    size_t steal_streak; ///< count of consecutive steals in a game @todo give it a functionality for statistics
} player;

/**
 * @brief check if the hand is empty
 * 
 * @param p the player
 * @return true empty
 * @return false contains cards
 */
bool is_hand_empty(player *p);

/**
 * @brief get te card from the hand at the given index
 * 
 * @param p the player
 * @param index the index of the card in the hand
 * @return card 
 */
card hand_at(player *p, size_t index);

/**
 * @brief remove and get a card from the hand at the given index
 * 
 * @param p the player
 * @param index the index of the card in the hand
 * @return card 
 */
card hand_throw(player *p, size_t index);

/**
 * @brief json representation of a player @see player
 * 
 * @param p the player
 * @return char* json string
 */
char *player_json(player *p);

/**
 * @brief json representation of a player_hand @see player_hand
 * 
 * @param ph the player hand member
 * @return char* json string
 */
char *player_hand_json(player_hand *ph);

#endif /* __PLAYER_H__ */
