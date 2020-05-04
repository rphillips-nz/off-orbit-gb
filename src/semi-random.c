#include <gb/gb.h>
#include "semi-random.h"

const UINT8 random_sides[RANDOM_SIDES_COUNT] = {0u, 1u, 2u, 3u};
const UINT8 random_flips[RANDOM_FLIPS_COUNT] = {0u, 1u};
const INT8 random_velocities[RANDOM_VELOCITIES_COUNT] = {-1, 1, -1, -1, 1, 2, -2, -2, -2, 2, 2, 2, -2, 2, 2, 2, -1, -1, -2, 2, -1, 1, -1, -2, 1, -1, -2, 1, 2, -1, -1, 1, -2, 1, 2, 1, 2, 2, 2, -1, 2, 2, -2, 1, 2, 1, 2, 2, 1, 1, 1, -2, 1, -2, 1, -2, 2, -2, 2, -2, -2, 1, 1, -2, -1, 1, -2, 1, -2, -1, -2, 2, 1, 2, 1, -1, -1, 1, -2, 2, 1, 1, 1, 2, 2, -2, 2, -1, -1, 2, 1, -2, 1, -1, -1, 2, -1, 2, 2, 2};
const INT8 random_positive_velocities[RANDOM_VELOCITIES_COUNT] = {1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 2, 2, 1, 1, 1, 2, 1, 1, 2, 1, 2, 1, 1, 1, 2, 1, 2, 1, 2, 2, 2, 1, 2, 2, 2, 1, 2, 1, 2, 2, 1, 1, 1, 2, 1, 2, 1, 2, 2, 2, 2, 2, 2, 1, 1, 2, 1, 1, 2, 1, 2, 1, 2, 2, 1, 2, 1, 1, 1, 1, 2, 2, 1, 1, 1, 2, 2, 2, 2, 1, 1, 2, 1, 2, 1, 1, 1, 2, 1, 2, 2, 2};
const INT8 random_negative_velocities[RANDOM_VELOCITIES_COUNT] = {-1, -1, -1, -1, -1, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -1, -1, -2, -2, -1, -1, -1, -2, -1, -1, -2, -1, -2, -1, -1, -1, -2, -1, -2, -1, -2, -2, -2, -1, -2, -2, -2, -1, -2, -1, -2, -2, -1, -1, -1, -2, -1, -2, -1, -2, -2, -2, -2, -2, -2, -1, -1, -2, -1, -1, -2, -1, -2, -1, -2, -2, -1, -2, -1, -1, -1, -1, -2, -2, -1, -1, -1, -2, -2, -2, -2, -1, -1, -2, -1, -2, -1, -1, -1, -2, -1, -2, -2, -2};
const UINT8 random_world_x[RANDOM_WORLD_X_COUNT] = {108u, 176u, 64u, 56u, 0u, 148u, 136u, 140u, 84u, 60u, 16u, 112u, 152u, 44u, 28u, 156u, 68u, 172u, 144u, 80u, 164u, 20u, 36u, 76u, 32u, 88u, 24u, 100u, 104u, 8u, 120u, 116u, 40u, 160u, 4u, 92u, 96u, 128u, 168u, 12u, 132u, 48u, 72u, 52u, 124u};
const UINT8 random_world_y[RANDOM_WORLD_Y_COUNT] = {120u, 92u, 164u, 4u, 80u, 76u, 20u, 44u, 148u, 12u, 168u, 48u, 136u, 68u, 128u, 104u, 72u, 100u, 144u, 56u, 32u, 160u, 124u, 88u, 8u, 156u, 16u, 108u, 28u, 60u, 112u, 152u, 36u, 0u, 64u, 40u, 132u, 52u, 24u, 96u, 84u, 140u, 116u};

UINT8 random_sides_index;
UINT8 random_flips_index;
UINT8 random_velocities_index;
UINT8 random_positive_velocities_index;
UINT8 random_negative_velocities_index;
UINT8 random_world_x_index;
UINT8 random_world_y_index;
