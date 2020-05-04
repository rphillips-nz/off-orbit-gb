#ifndef _OO_SEMI_RANDOM_H_
#define _OO_SEMI_RANDOM_H_



#include <gb/gb.h>

#define RANDOM_SIDES_COUNT 4
#define RANDOM_FLIPS_COUNT 2
#define RANDOM_VELOCITIES_COUNT 100
#define RANDOM_WORLD_X_COUNT 45
#define RANDOM_WORLD_Y_COUNT 43

#define RANDOM_SIDE (random_sides[++random_sides_index % RANDOM_SIDES_COUNT])
#define RANDOM_FLIP (random_flips[++random_flips_index % RANDOM_FLIPS_COUNT])
#define RANDOM_VELOCITY (random_velocities[++random_velocities_index % RANDOM_VELOCITIES_COUNT])
#define RANDOM_POSITIVE_VELOCITY (random_positive_velocities[++random_positive_velocities_index % RANDOM_VELOCITIES_COUNT])
#define RANDOM_NEGATIVE_VELOCITY (random_negative_velocities[++random_negative_velocities_index % RANDOM_VELOCITIES_COUNT])
#define RANDOM_WORLD_X (random_world_x[++random_world_x_index % RANDOM_WORLD_X_COUNT])
#define RANDOM_WORLD_Y (random_world_y[++random_world_y_index % RANDOM_WORLD_Y_COUNT])

extern const UINT8 random_sides[RANDOM_SIDES_COUNT];
extern const UINT8 random_flips[RANDOM_FLIPS_COUNT];
extern const INT8 random_velocities[RANDOM_VELOCITIES_COUNT];
extern const INT8 random_positive_velocities[RANDOM_VELOCITIES_COUNT];
extern const INT8 random_negative_velocities[RANDOM_VELOCITIES_COUNT];
extern const UINT8 random_world_x[RANDOM_WORLD_X_COUNT];
extern const UINT8 random_world_y[RANDOM_WORLD_Y_COUNT];

extern UINT8 random_sides_index;
extern UINT8 random_flips_index;
extern UINT8 random_velocities_index;
extern UINT8 random_positive_velocities_index;
extern UINT8 random_negative_velocities_index;
extern UINT8 random_world_x_index;
extern UINT8 random_world_y_index;



#endif
