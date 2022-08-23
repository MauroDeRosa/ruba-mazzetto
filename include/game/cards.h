#ifndef __CARDS_H__
#define __CARDS_H__

/**
 * @file cards.h
 * @author Mauro De Rosa (it.mauro.derosa@gmail.com)
 * @brief cards and deck data structures and functions
 * @version 1.0.0 alpha
 * @date 2022-07-28
 * 
 * @copyright Copyright (c) 2022
 * @license licensed under MIT license
 * 
 */

#include <stdbool.h>
#include <stdlib.h>

#define DECK_SIZE 40 ///< maximum deck size

/**
 * @brief cards suit enum (starting by 1, 0 is reserved for invalid suit)
 */
typedef enum {
    CARD_SUIT_DENARI=1,
    CARD_SUIT_SPADE,
    CARD_SUIT_BASTONI,
    CARD_SUIT_COPPE
} card_suit; 

/**
 * @brief card data structure
 */
typedef struct
{
    card_suit suit; ///< suit of the card
    short unsigned value; ///< value of the card
} card;

/**
 * @brief deck data structure
 */
typedef struct {
    card cards[DECK_SIZE]; ///< cards of the deck (maximum is defined by DECK_SIZE) @see DECK_SIZE
    size_t length; ///< number of cards actually contained by the deck
} deck;

/**
 * @brief check if a card is a valid card
 * 
 * @param c the card to check
 * @return true valid
 * @return false invalid
 */
bool is_card_valid(card c);

/**
 * @brief compare two cards by value
 * 
 * @param a first card
 * @param b second card
 * @return true the two cards have same value
 * @return false the two cards have different value
 */
bool is_card_equal(card a, card b);

/**
 * @brief string representation of a card
 * 
 * @param c the card to represent
 */
const char *card_string(card c);

/**
 * @brief check if a deck is empty
 * 
 * @param d the deck to check
 * @return true empty
 * @return false at least one card
 */
bool is_deck_empty(deck *d);

/**
 * @brief initialize an empty deck
 * 
 * @param d the deck to initialize
 */
void deck_init_empty(deck *d);

/**
 * @brief initialize a full deck
 * 
 * @param d the deck to fill
 */
void deck_init_full(deck *d);

/**
 * @brief shuffle an initialized deck
 * 
 * @param d the deck to shuffle
 */
void deck_shuffle(deck *d);

/**
 * @brief get the card on top of the deck without removing it
 * 
 * @param d deck
 * @return card the card on top of the deck
 */
card deck_top(deck *d);

/**
 * @brief remove the card on top of the deck and get it
 * 
 * @param d deck
 * @return card the card on top of the deck
 */
card deck_pop(deck *d);

/**
 * @brief get card at the given index from the deck
 * 
 * @param d deck
 * @param index index of the card in the deck
 * @return card the card at given index
 */
card deck_at(deck *d, size_t index);

/**
 * @brief remove a card at a given index from the deck and get it
 * 
 * @param d the deck
 * @param index index of the card in the deck
 * @return card the card at given index
 */
card deck_pop_index(deck *d, size_t index);

/**
 * @brief put a card on top of the deck
 * 
 * @param destination the deck where the card will be put on
 * @param c the card to put on top
 */
void deck_append_card(deck *destination, card c);

/**
 * @brief put a deck on top of another deck
 * 
 * @param destination the destination deck
 * @param source the deck to put on
 */
void deck_append(deck *destination, deck* source);

/**
 * @brief json representation of a deck
 *
 * @param d the deck to format in json
 * @return char* the json string (must be freed)
 */
char *deck_json(deck *d);

#endif/* __CARDS_H__ */
