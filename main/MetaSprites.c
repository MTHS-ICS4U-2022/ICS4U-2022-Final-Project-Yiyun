// Copyright (c) 2020 St. Mother Teresa HS All rights reserved.
//
// Created by: Mr. Coxall
// Created on: Sep 2020
// This is a game Meta Sprite helper functions

void set_meta_sprite_tile(UINT8 startMetaSpritePosition,
                          UINT8 SpriteIndexForPart1,
                          UINT8 SpriteIndexForPart2,
                          UINT8 SpriteIndexForPart3,
                          UINT8 SpriteIndexForPart4) {
    // same as built in set_sprite_tile
    //   except it loads 4 sprites to create a Meta sprite
    set_sprite_tile(startMetaSpritePosition + 0, SpriteIndexForPart1);
    set_sprite_tile(startMetaSpritePosition + 1, SpriteIndexForPart2);
    set_sprite_tile(startMetaSpritePosition + 2, SpriteIndexForPart3);
    set_sprite_tile(startMetaSpritePosition + 3, SpriteIndexForPart4);
}

void move_meta_sprite(UINT8 startMetaSpritePosition,
                      UINT8 xPosition,
                      UINT8 yPosition) {
    // same as built in move_sprite
    //   except it moves the 4 piece Meta sprite
    move_sprite(startMetaSpritePosition + 0, xPosition + 0, yPosition - 8);
    move_sprite(startMetaSpritePosition + 1, xPosition + 8, yPosition - 8);
    move_sprite(startMetaSpritePosition + 2, xPosition + 0, yPosition - 0);
    move_sprite(startMetaSpritePosition + 3, xPosition + 8, yPosition - 0);
}

void scroll_meta_sprite(UINT8 startMetaSpriteIndex,
                        int xPosition,
                        int yPosition) {
    // same as built in scroll_sprite
    //   except it scrolls the 4 piece Meta sprite
    scroll_sprite(startMetaSpriteIndex + 0, xPosition, yPosition);
    scroll_sprite(startMetaSpriteIndex + 1, xPosition, yPosition);
    scroll_sprite(startMetaSpriteIndex + 2, xPosition, yPosition);
    scroll_sprite(startMetaSpriteIndex + 3, xPosition, yPosition);
}