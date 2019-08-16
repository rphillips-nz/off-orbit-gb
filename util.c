#include <gb/gb.h>
#include <rand.h>

void performant_delay(UINT8 loop_count) {
	UINT8 i;

	for (i = 0; i < loop_count; i++) {
		wait_vbl_done();
	}
}

INT16 rand_range(INT16 min_n, INT16 max_n) {
    return rand() % (max_n - min_n + 1) + min_n;
}