#!/bin/bash

# Created by: Mr Coxall
# Created on: Jun 2022
# Created for: To automate the generation of gameboy files

base_name=$(basename "$1" .c)

# remove old gameboy file from the simulator directory
rm ~/GBDK/web/rom/game.gb

# compile to game
/opt/gbdk/bin/lcc -Wa-l -Wl-m -Wl-j -o $base_name.gb $base_name.c

# move it to the simulator directory
cp ./$base_name.gb ~/GBDK/web/rom/game.gb