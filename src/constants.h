#ifndef _OO_CONSTANTS_H_
#define _OO_CONSTANTS_H_



// Top corner of the screen is actually (8, 16) rather than (0, 0)
#define SCREEN_MIN_X 8u
#define SCREEN_MIN_Y 16u
#define SCREEN_MAX_X 168u
#define SCREEN_MAX_Y 160u

// Screen is 160x144 and add 8 for each side
#define WORLD_MIN_X 0u
#define WORLD_MIN_Y 8u
#define WORLD_MAX_X 176u
#define WORLD_MAX_Y 168u

#define MAX_VELOCITY 4
#define ACCELERATION 1

#define MAX_HEALTH 3u
#define FIRE_RATE 20u
#define ANIMATION_RATE 4u
#define BULLET_COUNT 2u
#define ENEMY_COUNT 6u
#define SMALL_ENEMY_COUNT 2u

#define BACKGROUND_DATA_TILES 12u
#define FONT_DATA_TILES 37u

#define ABS(x)    (((x) < 0) ? -(x) : (x))
#define MAX(a, b) ((a < b) ? (b) : (a))
#define MIN(a, b) ((a < b) ? (a) : (b))



#endif
