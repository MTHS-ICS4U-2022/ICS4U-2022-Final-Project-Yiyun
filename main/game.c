// Copyright (c) 2022 St. Mother Teresa HS All rights reserved.
//
// Created by: Yiyun Qin
// Created on: Dec 2022
// This program shows the background without moving.

#include <stdio.h>
#include <gb/gb.h>
#include <stdbool.h>
#include <stdint.h>

#include "GBDK_Constants.h"
#include "Background.c"
#include "SpaceAliens.c"
#include "MetaSprites.c"
#include "BirdAndPipes.c"

INT16 birdLocation[2];
BYTE jumping;
uint8_t gravity = 4;
uint8_t currentSpeedY;
uint8_t floorY = 100;

INT8 wouldHitSurface(UINT8 positionY) {
    if (positionY >= floorY) {
        return floorY;
    } else {
        return -1;
    }
}


void jump() {
    INT8 possibleSurfaceY;
    if (jumping == 0) {
        jumping = 1;
        currentSpeedY = 8;
    }
    // gravity slows the speed
    currentSpeedY = currentSpeedY - gravity;
    birdLocation[1] = birdLocation[1] - currentSpeedY;
    possibleSurfaceY = wouldHitSurface(birdLocation[1]);
    if (possibleSurfaceY != -1) {
        // hit the surface
        jumping = 0;  // stop jumping
        move_meta_sprite(0, birdLocation[0], floorY);  // move it on the floor
    } else {
        move_meta_sprite(0, birdLocation[0], birdLocation[1]);
    }
}

void main() {
	uint8_t joypadData;

	// check if A button is single pressed
	bool aJustPressed = FALSE;
	bool aStillPressed = FALSE;

	// three registers that enable sound
	NR52_REG = 0x80;
	NR50_REG = 0x77;
	NR51_REG = 0xFF;

    // the position of the bird
	const int FIXED_X_POSITION_OF_BIRD = 60;
	birdLocation[0] = FIXED_X_POSITION_OF_BIRD;
	birdLocation[1] = 82;
	jumping = 0;
	currentSpeedY = 0;

    // load spritesheet referenced as #0
	// load 16 sprites from it
	// find references in SpaceAliens
	set_sprite_data(0, 16, BirdAndPipes);

    // load sprite for meta sprites(many parts)
	set_meta_sprite_tile(0, 2, 4, 3, 5);

	// move to (88, 78)
	move_meta_sprite(0, birdLocation[0], birdLocation[1]);

	// background
    set_bkg_data(0, 43, SpaceAliens);
	set_bkg_tiles(0, 0, 32, 18, Background);

    SHOW_BKG;
	SHOW_SPRITES;
	DISPLAY_ON;

	// game loop
	while (1) {
		// get D-pad input
		joypadData = joypad();

		// if the A is pressed, move sprite up(0, -1)
		if (joypadData & J_A) {
			if (aJustPressed == TRUE) {
				jump();
				// add this because I want it can jump again in
				// the air if I press A again
				jumping = 0;
				if (birdLocation[1] < 24) {
					birdLocation[1] = 24;
					move_meta_sprite(0, FIXED_X_POSITION_OF_BIRD, 24);
				}
			}
		} else if (jumping == 1) {
			jump();
			if (birdLocation[1] < 24) {
				birdLocation[1] = 24;
				move_meta_sprite(0, FIXED_X_POSITION_OF_BIRD, 24);
			}
		}

		// if the A is pressed
		if (joypadData & J_A) {
			if (aJustPressed == TRUE) {
				aStillPressed = TRUE;
			} else {
				aJustPressed = TRUE;
				NR10_REG=0x1C;
        		NR11_REG=0x44;
        		NR12_REG=0x42;
        		NR13_REG=0x43;
        		NR14_REG=0x86;
			}
		} else {
			aJustPressed = FALSE;
			aStillPressed = FALSE;
		}

        // prevent the bird from dropping outside of the screen
		if (birdLocation[1] > SCREEN_HEIGHT + 8) {
			birdLocation[1] = SCREEN_HEIGHT + 8;
			move_meta_sprite(0, FIXED_X_POSITION_OF_BIRD,
			(uint8_t)(SCREEN_HEIGHT + 8));
		}

		// scroll background -1 in X and 0 in Y
		scroll_bkg(1, 0);

		// wait until end of frame(1/60th second)
		wait_vbl_done();
	}
}
