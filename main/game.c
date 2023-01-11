// Copyright (c) 2022 St. Mother Teresa HS All rights reserved.
//
// Created by: Yiyun Qin
// Created on: Dec 2022
// This program shows the background without moving.

#include <stdio.h>
#include <gb/gb.h>

#include "Background.c"
#include "SpaceAliens.c"
#include "MetaSprites.c"

void main() {
	uint8_t joypadData;

    // load spritesheet referenced as #0
	// load 16 sprites from it
	// find references in SpaceAliens
	set_sprite_data(0, 16, SpaceAliens);

    // load sprite for meta sprites(many parts)
	set_meta_sprite_tile(0, 1, 2, 3, 4);

	// move to (88, 78)
	move_meta_sprite(0, 76, 144);

	// background
    set_bkg_data(0, 16, SpaceAliens);
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
            scroll_meta_sprite(0, 0, -1);
		}

		// scroll background -1 in X and 0 in Y
		scroll_bkg(1, 0);

		// wait until end of frame(1/60th second)
		wait_vbl_done();
	}
}
