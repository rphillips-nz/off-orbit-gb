#include <gb/gb.h>

UBYTE fade_index;

void fade_out() {
	for (fade_index = 0; fade_index < 3; fade_index ++) {
		switch (fade_index) {
			case 0:
				BGP_REG = 0xF9;
				break;
			case 1:
				BGP_REG = 0xFE;
				break;
			case 2:
				BGP_REG = 0xFF;
				break;
		}

		wait_vbl_done();
		wait_vbl_done();
		wait_vbl_done();
	}
}

void fade_in() {
	for (fade_index = 0; fade_index < 3; fade_index++) {
		switch (fade_index) {
			case 0:
				BGP_REG = 0xFE;
				break;
			case 1:
				BGP_REG = 0xF9;
				break;
			case 2:
				BGP_REG = 0xE4;
				break;
		}

		wait_vbl_done();
		wait_vbl_done();
		wait_vbl_done();
	}
}

void fade_out_white() {
	for (fade_index = 0; fade_index < 3; fade_index ++) {
		switch (fade_index) {
			case 0:
				BGP_REG = 0x90;
				break;
			case 1:
				BGP_REG = 0x80;
				break;
			case 2:
				BGP_REG = 0x00;
				break;
		}

		wait_vbl_done();
		wait_vbl_done();
		wait_vbl_done();
	}
}

void fade_in_white() {
	for (fade_index = 0; fade_index < 3; fade_index++) {
		switch (fade_index) {
			case 0:
				BGP_REG = 0x80;
				break;
			case 1:
				BGP_REG = 0x90;
				break;
			case 2:
				BGP_REG = 0xE4;
				break;
		}

		wait_vbl_done();
		wait_vbl_done();
		wait_vbl_done();
	}
}
