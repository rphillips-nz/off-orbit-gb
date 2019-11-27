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

const UINT8 random_sides[RANDOM_SIDES_COUNT] = {0, 1, 2, 3};
const UINT8 random_flips[RANDOM_FLIPS_COUNT] = {0, 1};
const INT8 random_velocities[RANDOM_VELOCITIES_COUNT] = {-1, 1, -1, -1, 1, 2, -2, -2, -2, 2, 2, 2, -2, 2, 2, 2, -1, -1, -2, 2, -1, 1, -1, -2, 1, -1, -2, 1, 2, -1, -1, 1, -2, 1, 2, 1, 2, 2, 2, -1, 2, 2, -2, 1, 2, 1, 2, 2, 1, 1, 1, -2, 1, -2, 1, -2, 2, -2, 2, -2, -2, 1, 1, -2, -1, 1, -2, 1, -2, -1, -2, 2, 1, 2, 1, -1, -1, 1, -2, 2, 1, 1, 1, 2, 2, -2, 2, -1, -1, 2, 1, -2, 1, -1, -1, 2, -1, 2, 2, 2};
const INT8 random_positive_velocities[RANDOM_VELOCITIES_COUNT] = {1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 2, 2, 1, 1, 1, 2, 1, 1, 2, 1, 2, 1, 1, 1, 2, 1, 2, 1, 2, 2, 2, 1, 2, 2, 2, 1, 2, 1, 2, 2, 1, 1, 1, 2, 1, 2, 1, 2, 2, 2, 2, 2, 2, 1, 1, 2, 1, 1, 2, 1, 2, 1, 2, 2, 1, 2, 1, 1, 1, 1, 2, 2, 1, 1, 1, 2, 2, 2, 2, 1, 1, 2, 1, 2, 1, 1, 1, 2, 1, 2, 2, 2};
const INT8 random_negative_velocities[RANDOM_VELOCITIES_COUNT] = {-1, -1, -1, -1, -1, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -1, -1, -2, -2, -1, -1, -1, -2, -1, -1, -2, -1, -2, -1, -1, -1, -2, -1, -2, -1, -2, -2, -2, -1, -2, -2, -2, -1, -2, -1, -2, -2, -1, -1, -1, -2, -1, -2, -1, -2, -2, -2, -2, -2, -2, -1, -1, -2, -1, -1, -2, -1, -2, -1, -2, -2, -1, -2, -1, -1, -1, -1, -2, -2, -1, -1, -1, -2, -2, -2, -2, -1, -1, -2, -1, -2, -1, -1, -1, -2, -1, -2, -2, -2};
const UINT8 random_world_x[RANDOM_WORLD_X_COUNT] = {108, 176, 64, 56, 0, 148, 136, 140, 84, 60, 16, 112, 152, 44, 28, 156, 68, 172, 144, 80, 164, 20, 36, 76, 32, 88, 24, 100, 104, 8, 120, 116, 40, 160, 4, 92, 96, 128, 168, 12, 132, 48, 72, 52, 124};
const UINT8 random_world_y[RANDOM_WORLD_Y_COUNT] = {120, 92, 164, 4, 80, 76, 20, 44, 148, 12, 168, 48, 136, 68, 128, 104, 72, 100, 144, 56, 32, 160, 124, 88, 8, 156, 16, 108, 28, 60, 112, 152, 36, 0, 64, 40, 132, 52, 24, 96, 84, 140, 116};

UINT8 random_sides_index;
UINT8 random_flips_index;
UINT8 random_velocities_index;
UINT8 random_positive_velocities_index;
UINT8 random_negative_velocities_index;
UINT8 random_world_x_index;
UINT8 random_world_y_index;
