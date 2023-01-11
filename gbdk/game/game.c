// Copyright (c) 2022 St. Mother Teresa HS All rights reserved.
//
// Created by: Yiyun Qin
// Created on: Dec 2022
// This program shows the background without moving.

#include <stdio.h>
#include <gb/gb.h>

#include "flappyBirdSky.c"

void main() {
    set_sprite_data(0, 0, flappyBirdSky);
	set_sprite_tile(0, 0);
	move_sprite(0, 88, 78);

	SHOW_SPRITES;
}
	