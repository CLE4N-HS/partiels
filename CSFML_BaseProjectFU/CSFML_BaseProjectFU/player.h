#pragma once
#include "tools.h"
#include "stateManager.h"

#define PLAYER_SCALE 3.f
#define GRAVITY 30.f
#define LERP_VIEW_TIMER 1.f


typedef enum {
	FROG,
	ASTRONAUT
}playerType;


void initPlayer();

void updatePlayer(Window* _window);

void displayPlayer(Window* _window);

sfVector2f getPlayerPosInBounds();

sfVector2f getLerpView();
