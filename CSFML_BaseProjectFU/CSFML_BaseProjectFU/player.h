#pragma once
#include "tools.h"
#include "stateManager.h"

#define PLAYER_SCALE 3.f
#define GRAVITY 30.f
#define LERP_VIEW_TIMER 1.f
#define LAUNCHING_TIMER_DURATION 1.f


typedef enum {
	FROG,
	ASTRONAUT
}playerType;


void initPlayer();

void updatePlayer(Window* _window);

void displayPlayer(Window* _window);

sfVector2f getPlayerPosInBounds(playerType _type);

sfVector2f getLerpView(sfVector2f _lastViewPos, sfVector2f _viewPos);

playerType getViewFocus();

sfFloatRect getPlayerRect(playerType _type);

sfVector2f getPlayerPos(playerType _type);

void setPlayerPos(playerType _type, sfVector2f _pos);

sfVector2f* pGetPlayerPos(playerType _type);

sfVector2f* pGetPlayerVelocity(playerType _type);

void setPlayerLauchingTimer(playerType _type, float _launchingTimer);

float getPlayerLauchingTimer(playerType _type);
