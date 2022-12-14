/**
 * @file player.c
 * @author Mauro De Rosa (it.mauro.derosa@gmail.com)
 * @brief player module implementation @see player.h
 * @version 1.0.0 alpha
 * @date 2022-08-23
 * 
 * @copyright Copyright (c) 2022
 * @license licensed under MIT license
 * 
 */

#include <game/player.h>
#include <types/memory.h>
#include <types/array.h>
#include <io/log.h>
#include <string.h>
#include <stdio.h>

bool is_hand_empty(player *p)
{
    check_null_pointer(p);
    return p->hand.length == 0;
}

card hand_at(player *p, size_t hand_index)
{
    check_null_pointer(p);
    
    // check if the player has the card at the given index
    if(hand_index >= p->hand.length)
    {
        log_error("index out of bound");
    }
    
    // returns the player card at the given index
    return p->hand.cards[hand_index];
}

card hand_throw(player *p, size_t hand_index)
{
    check_null_pointer(p);

    if (hand_index >= p->hand.length)
    {
        log_error("index out of bound");
    }
    
    //takes the card, removes it from the hand and return it
    card c = hand_at(p, hand_index);
    array_delete(hand_index, p->hand.cards, &(p->hand.length), sizeof(card));
    return c;
}


#pragma region json

char *player_json(player *p)
{
    check_null_pointer(p);

    char *hand_str = player_hand_json(&p->hand);
    char *player_json = memory_allocate_zero(500, sizeof(char));
    sprintf(player_json, "{"
                         "\"id\":%zu,"
                         "\"username\":\"%s\","
                         "\"deck\":{\"length\":%zu,\"top\":\"%s\"},"
                         "\"hand\":%s,"
                         "\"points\":%zu,"
                         "\"steals\":%zu"
                         "}",
                         p->id,
                         p->username,
                         p->deck.length,
                         p->deck.length > 1 ? card_string(deck_top(&p->deck)) : "ZZ",
                         hand_str, p->deck.length, p->steals);
    free(hand_str);
    
    // returns player's info as json
    return player_json;
}

char *player_hand_json(player_hand *ph)
{
    check_null_pointer(ph);
    char *json = memory_allocate_zero(25, sizeof(char)), tmp_card[10] = {0};

    strcat(json, "[");

    for (size_t i = 0; i < ph->length; i++)
    {
        sprintf(tmp_card, "\"%s\"%c",
                card_string(ph->cards[i]),
                i == ph->length - 1 ? '\0' : ',');
        strcat(json, tmp_card);
    }

    strcat(json, "]");
    
    // returns player's hand as json
    return json;
}

#pragma endregion json
