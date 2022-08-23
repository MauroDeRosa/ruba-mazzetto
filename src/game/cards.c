/**
 * @file cards.c
 * @author Mauro De Rosa (it.mauro.derosa@gmail.com)
 * @brief card module implementation @see cards.h
 * @version 1.0.0 alpha
 * @date 2022-08-23
 * 
 * @copyright Copyright (c) 2022
 * @license licensed under MIT license
 * 
 */

#include <io/log.h>
#include <game/cards.h>
#include <types/array.h>
#include <types/memory.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define CARDS_FOR_SUIT 10 ///< how many cards are in a suit

/**
 * @brief array containing all the possible cards of a deck (lookup table)
 */
const card cards[DECK_SIZE] = {
    (card){CARD_SUIT_DENARI, 1},
    (card){CARD_SUIT_DENARI, 2},
    (card){CARD_SUIT_DENARI, 3},
    (card){CARD_SUIT_DENARI, 4},
    (card){CARD_SUIT_DENARI, 5},
    (card){CARD_SUIT_DENARI, 6},
    (card){CARD_SUIT_DENARI, 7},
    (card){CARD_SUIT_DENARI, 8},
    (card){CARD_SUIT_DENARI, 9},
    (card){CARD_SUIT_DENARI, 10},
    (card){CARD_SUIT_SPADE, 1},
    (card){CARD_SUIT_SPADE, 2},
    (card){CARD_SUIT_SPADE, 3},
    (card){CARD_SUIT_SPADE, 4},
    (card){CARD_SUIT_SPADE, 5},
    (card){CARD_SUIT_SPADE, 6},
    (card){CARD_SUIT_SPADE, 7},
    (card){CARD_SUIT_SPADE, 8},
    (card){CARD_SUIT_SPADE, 9},
    (card){CARD_SUIT_SPADE, 10},
    (card){CARD_SUIT_BASTONI, 1},
    (card){CARD_SUIT_BASTONI, 2},
    (card){CARD_SUIT_BASTONI, 3},
    (card){CARD_SUIT_BASTONI, 4},
    (card){CARD_SUIT_BASTONI, 5},
    (card){CARD_SUIT_BASTONI, 6},
    (card){CARD_SUIT_BASTONI, 7},
    (card){CARD_SUIT_BASTONI, 8},
    (card){CARD_SUIT_BASTONI, 9},
    (card){CARD_SUIT_BASTONI, 10},
    (card){CARD_SUIT_COPPE, 1},
    (card){CARD_SUIT_COPPE, 2},
    (card){CARD_SUIT_COPPE, 3},
    (card){CARD_SUIT_COPPE, 4},
    (card){CARD_SUIT_COPPE, 5},
    (card){CARD_SUIT_COPPE, 6},
    (card){CARD_SUIT_COPPE, 7},
    (card){CARD_SUIT_COPPE, 8},
    (card){CARD_SUIT_COPPE, 9},
    (card){CARD_SUIT_COPPE, 10}};

/**
 * @brief string representation of every possible card in a deck (lookup table)
 */
const char cards_representation[DECK_SIZE][3] = {"D1", "D2", "D3", "D4", "D5", "D6", "D7", "D8", "D9", "DK", "S1", "S2", "S3", "S4", "S5", "S6", "S7", "S8", "S9", "SK", "B1", "B2", "B3", "B4", "B5", "B6", "B7", "B8", "B9", "BK", "C1", "C2", "C3", "C4", "C5", "C6", "C7", "C8", "C9", "CK"};


bool is_card_valid(card c)
{
    return (c.suit >= CARD_SUIT_DENARI && c.suit <= CARD_SUIT_COPPE) && (c.value >= 1 && c.value <= 10);
}

bool is_card_equal(card a, card b)
{
    if (is_card_valid(a) == false || is_card_valid(b) == false)
    {
        log_error("one of the cards is invalid");
    }

    return a.value == b.value;
}

const char *card_string(card c)
{
    if (is_card_valid(c))
    {
        // return card string representation from the lookup table
        return cards_representation[(c.suit - 1) * CARDS_FOR_SUIT + (c.value - 1)];
    }
    else
    {
        // return value for empty card
        return "ZZ";
    }
}

bool is_deck_empty(deck *d)
{
    check_null_pointer(d);
    return d->length < 1;
}

void deck_init_empty(deck *d)
{
    check_null_pointer(d);
    //initialize an empty deck with length = 0, and all cards set to invalid values
    memset(d, 0, sizeof(deck));
}

void deck_init_full(deck *d)
{
    check_null_pointer(d);
    //copy the lookup table containing each possible card in the game and set the length to DECK_SIZE
    memcpy(d->cards, cards, DECK_SIZE * sizeof(card));
    d->length = DECK_SIZE;
}

void deck_shuffle(deck *d)
{
    check_null_pointer(d);
    if (is_deck_empty(d) == false)
    {
        // if not empty shuffle the deck
        array_shuffle(d->cards, d->length, sizeof(card));
    }
    else
    {
        log_warning("deck is empty");
    }
}

card deck_top(deck *d)
{
    check_null_pointer(d);

    if (is_deck_empty(d))
    {
        log_error("deck is empty");
    }

    // if deck's not empty return the card on the top
    return d->cards[d->length - 1];
}

card deck_pop(deck *d)
{
    check_null_pointer(d);

    if (is_deck_empty(d))
    {
        log_error("deck is empty");
    }

    // if deck's not empty remove the card on top of the deck and return it
    return d->cards[--(d->length)];
}

card deck_at(deck *d, size_t index)
{
    check_null_pointer(d);

    if (index >= d->length)
    {
        log_error("out of bound index");
    }

    // if index is valid get the card at the given index
    return d->cards[index];
}

card deck_pop_index(deck *d, size_t index)
{
    check_null_pointer(d);

    if (index >= d->length)
    {
        log_error("out of bound index");
    }
    
    // if index is valid remove the card at the given index and return it
    card out = deck_at(d, index);
    array_delete(index, d->cards, &d->length, sizeof(card));
    return out;
}

void deck_append_card(deck *destination, card c)
{
    check_null_pointer(destination);

    if (is_card_valid(c) == false)
    {
        log_error("invalid card");
    }

    // append the given card if valid on top of the deck
    destination->cards[destination->length++] = c;
}

void deck_append(deck *destination, deck *source)
{
    check_null_pointer(destination);
    check_null_pointer(source);

    if (destination->length + source->length > 40)
    {
        log_error("sum of the two decks must be less or equal to 40");
    }

    if (is_deck_empty(source))
    {
        log_warning("nothing to add to destination deck");
    }
    else
    {
        // if source deck is not empty copy the deck on top of the destination one
        memcpy(&destination->cards[destination->length], source->cards, source->length * sizeof(card));
        // increase the length of the destination deck by the source deck length
        destination->length += source->length;
        // reset the source deck as an empty deck
        memset(source, 0, sizeof(deck));
    }
}

#pragma region json

char *deck_json(deck *d)
{
    check_null_pointer(d);
    char *json = memory_allocate_zero(250, sizeof(char)), tmp_card[10] = {0};

    strcat(json, "[");

    for (size_t i = 0; i < d->length; i++)
    {
        sprintf(tmp_card, "\"%s\"%c",
                card_string(d->cards[i]),
                i == d->length - 1 ? '\0' : ',');
        strcat(json, tmp_card);
    }

    strcat(json, "]");

    // return deck as a json string array
    return json;
}

#pragma endregion json
