#ifndef _OO_BULLET_H_
#define _OO_BULLET_H_

#include <gb/gb.h>
#include "game-character.h"

void set_up_bullet(GameCharacter* bullet);

void update_bullet(GameCharacter* bullet);

void fire_bullet(uint8_t direction);

#endif
