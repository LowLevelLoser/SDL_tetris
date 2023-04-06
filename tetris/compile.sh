#!/bin/bash

gcc `sdl2-config --libs --cflags` -o SDL_tetris rendering.c logic.c main.c -lpthread -lSDL2 -lSDL2_ttf
