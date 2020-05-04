#ifndef _OO_PLAYER_H_
#define _OO_PLAYER_H_



#include <gb/gb.h>

void set_up_player();

void turn_player(UINT8 jpad);

void destroy_player();

void start_destroying_player();

void reset_player();

void animate_player(UINT8 jpad);

void update_player(UINT8 jpad);

#endif
