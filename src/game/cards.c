#include <io/log.h>
#include <game/cards.h>
#include <types/array.h>
#include <types/memory.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define CARDS_FOR_SUIT 10

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

const char cards_representation[DECK_SIZE][3] = {"D1", "D2", "D3", "D4", "D5", "D6", "D7", "D8", "D9", "DK", "S1", "S2", "S3", "S4", "S5", "S6", "S7", "S8", "S9", "SK", "B1", "B2", "B3", "B4", "B5", "B6", "B7", "B8", "B9", "BK", "C1", "C2", "C3", "C4", "C5", "C6", "C7", "C8", "C9", "CK"};

bool card_is_valid(card c)
{
    return (c.suit >= CARD_SUIT_DENARI && c.suit <= CARD_SUIT_COPPE) && (c.value >= 1 && c.value <= 10);
}

bool card_equal_by_value(card a, card b)
{
    if (card_is_valid(a) == false || card_is_valid(b) == false)
    {
        log_error("one of the cards is invalid");
    }

    return a.value == b.value;
}

const char *card_string(card c)
{
    if (card_is_valid(c))
    {
        return cards_representation[(c.suit - 1) * CARDS_FOR_SUIT + (c.value - 1)];
    }
    else
    {
        return "ZZ";
    }
}

bool deck_is_empty(deck *d)
{
    check_null_pointer(d);
    return d->length < 1;
}

void deck_init_empty(deck *d)
{
    check_null_pointer(d);
    memset(d, 0, sizeof(deck));
}

void deck_init_full(deck *d)
{
    check_null_pointer(d);
    memcpy(d->cards, cards, DECK_SIZE * sizeof(card));
    d->length = 40;
}

void deck_shuffle(deck *d)
{
    check_null_pointer(d);
    if (deck_is_empty(d) == false)
    {
        array_shuffle(d->cards, d->length, sizeof(card));
    }
    else
    {
        log_warning("deck is empty");
    }
}

card deck_top(deck *d)
{
    if (d->length < 1)
    {
        log_error("deck is empty");
    }

    return d->cards[d->length - 1];
}

card deck_pop(deck *d)
{
    if (d->length < 1)
    {
        log_error("deck is empty");
    }

    return d->cards[--(d->length)];
}

card deck_at(deck *d, size_t index)
{
    check_null_pointer(d);

    if (index >= d->length)
    {
        log_error("out of bound index");
    }

    return d->cards[index];
}

card deck_pop_index(deck *d, size_t index)
{
    if (index >= d->length)
    {
        log_error("out of bound index");
    }

    card out = deck_at(d, index);
    array_delete(index, d->cards, &d->length, sizeof(card));
    return out;
}

void deck_append_card(deck *destination, card c)
{
    check_null_pointer(destination);

    if (card_is_valid(c) == false)
    {
        log_error("invalid card");
    }

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

    if (deck_is_empty(source))
    {
        log_warning("nothing to add to destination deck");
    }
    else
    {
        memcpy(&destination->cards[destination->length], source->cards, source->length * sizeof(card));
        destination->length += source->length;
        memset(source, 0, sizeof(deck));
    }
}

#pragma region json

char *deck_json(deck *d)
{
    check_null_pointer(d);
    char *json = calloc(250, sizeof(char)), tmp_card[10] = {0};

    strcat(json, "[");

    for (size_t i = 0; i < d->length; i++)
    {
        sprintf(tmp_card, "\"%s\"%c",
                card_string(d->cards[i]),
                i == d->length - 1 ? '\0' : ',');
        strcat(json, tmp_card);
    }

    strcat(json, "]");
    return json;
}

#pragma endregion json
