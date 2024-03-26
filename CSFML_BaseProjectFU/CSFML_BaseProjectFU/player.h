#pragma once
#include "tools.h"
#include "stateManager.h"

#define PLAYER_SCALE 3.f
#define GRAVITY 30.f


void initPlayer();

void updatePlayer(Window* _window);

void displayPlayer(Window* _window);
