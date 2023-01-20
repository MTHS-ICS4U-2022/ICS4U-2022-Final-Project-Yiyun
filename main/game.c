// Copyright (c) 2022 St. Mother Teresa HS All rights reserved.
//
// Created by: Yiyun Qin
// Created on: Dec 2022
// This program shows the background without moving.

#include <stdio.h>
#include <gb/gb.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "GBDK_Constants.h"
#include "Background.c"
#include "SpaceAliens.c"
#include "MetaSprites.c"
#include "BirdAndPipes.c"

typedef enum {
	SPLASH,
	START,
	GAME,
	GAME_OVER
} screen_t;

INT16 birdLocation[2];
BYTE jumping;
BYTE alive = 0;
uint8_t gravity = 4;
int8_t currentSpeedY;
uint8_t floorY = 100;
const int FIXED_X_POSITION_OF_BIRD = 60;
int pipes[5][3] = {
	{5, 255, 255},
	{6, 255, 255},
	{7, 255, 255},
	{8, 255, 255},
	{9, 255, 255},
};
uint8_t pipesHeight = 0;
uint8_t pipesSpace = 0;

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
		birdLocation[1] -= 5;
        birdLocation[1]--;
    }
    // gravity slows the speed
    possibleSurfaceY = wouldHitSurface(birdLocation[1]);
    if (possibleSurfaceY != -1) {
        // hit the surface
        jumping = 0;  // stop jumping
        move_meta_sprite(0, birdLocation[0], floorY);  // move it on the floor
    } else {
        move_meta_sprite(0, birdLocation[0], birdLocation[1]);
    }
}

screen_t splash() {
	uint8_t joypadData;
	screen_t next_screen = GAME;

    set_sprite_data(0, 16, BirdAndPipes);

    // load sprite for meta sprites(many parts)
	set_meta_sprite_tile(0, 2, 4, 3, 5);

	// move to (60, 82)
	move_meta_sprite(0, 60, 82);

	// background
    set_bkg_data(0, 43, SpaceAliens);
	set_bkg_tiles(0, 0, 32, 18, Background);

    SHOW_BKG;
	SHOW_SPRITES;
	DISPLAY_ON;

	while(1) {
		joypadData = joypad();

		if (joypadData & J_A) {
			return next_screen;
		}

        scroll_bkg(1, 0);

		wait_vbl_done();
	}
}

screen_t game() {
	uint8_t joypadData;
	screen_t next_screen = GAME;

	// check if A button is single pressed
	bool aJustPressed = FALSE;
	bool aStillPressed = FALSE;

	// three registers that enable sound
	NR52_REG = 0x80;
	NR50_REG = 0x77;
	NR51_REG = 0xFF;

    // the position of the bird
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
				jumping = 0;
				jump();
				// add this because I want it can jump again in
				// the air if I press A again
				if (birdLocation[1] < 24) {
					birdLocation[1] = 24;
					move_meta_sprite(0, FIXED_X_POSITION_OF_BIRD, 24);
				}
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
			alive = 1;
		}

        pipesHeight = rand() % 5 + 1;
		pipesSpace = rand() % 5 + 3;
		for (int pipesCounter = pipesHeight; pipesCounter > 0; pipesCounter--) {
			pipes[pipesCounter][1] = 161;
			pipes[pipesCounter][2] = pipesCounter * 16;
            move_meta_sprite(pipes[pipesCounter][0],
			pipes[pipesCounter][1], pipes[pipesCounter][2]);

		}

		for (int pipesCounter = 0; pipesCounter < 5; pipesCounter++) {
			if (pipes[pipesCounter][1] < 255) {
            	pipes[pipesCounter][1] -= 10;
				move_meta_sprite(pipes[pipesCounter][0],
				pipes[pipesCounter][1], pipes[pipesCounter][2]);
			}
			if (pipes[pipesCounter][1] < 0) {
				pipes[pipesCounter][1] = 255;
				pipes[pipesCounter][2] = 255;
				move_meta_sprite(pipes[pipesCounter][0],
				pipes[pipesCounter][1], pipes[pipesCounter][2]);
			}
		}

		// scroll background -1 in X and 0 in Y
		scroll_bkg(1, 0);
		// bird will always move down
		birdLocation[1]++;
		birdLocation[1] += 1;
		move_meta_sprite(0, birdLocation[0], birdLocation[1]);

		if (alive == 1) {
			next_screen = GAME_OVER;
			return next_screen;
		}

		// wait until end of frame(1/60th second)
		wait_vbl_done();
	}
}

void main() {
	screen_t currentScreen = SPLASH;

	NR52_REG = 0x80;
	NR50_REG = 0x77;
	NR51_REG = 0xFF;

    set_sprite_data(0, 16, SpaceAliens);
    // load sprites for pipes
	for (int pipeCounter = 0; pipeCounter < 5; pipeCounter++) {
		set_meta_sprite_tile(pipes[pipeCounter][0], 4, 5, 6, 7);
	}

	// move pipes out of the screen
	for (int pipeCounter = 0; pipeCounter < 5; pipeCounter++) {
		pipes[pipeCounter][1] = 255;
		pipes[pipeCounter][2] = 255;
		move_meta_sprite(pipes[pipeCounter][0],
		pipes[pipeCounter][1],
		pipes[pipeCounter][2]);
	}

	while(1) {
        if (currentScreen == SPLASH) {
			currentScreen = splash();
		} else if (currentScreen == GAME) {
			currentScreen = game();
		}
	}
}