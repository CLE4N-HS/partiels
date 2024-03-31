#pragma once
#include "tools.h"
#include "stateManager.h"

#define PLAYER_SCALE 3.f
#define GRAVITY 300.f
#define LERP_VIEW_TIMER 1.f
#define LAUNCHING_TIMER_DURATION 1.f
#define MUSIC_BLOC_TIMER_DURATION 1.f
#define SLINGSHOT_GRAVITY 1000.f
#define SLINGSHOT_DRAG 1.1f

typedef enum {
	FROG,
	ASTRONAUT
}playerType;

typedef enum {
	NO_ANIM,
	IDLE,
	RUN,
	JUMP,
	FALL,
	THROW
}playerAnim;


void initPlayer();

void updatePlayer(Window* _window);

void displayPlayer(Window* _window);

sfVector2f getPlayerPosInBounds(playerType _type);

sfVector2f getLerpView(sfVector2f _lastViewPos, sfVector2f _viewPos);

playerType getViewFocus();

sfVector2f getPlayerPos(playerType _type);

void setPlayerPos(playerType _type, sfVector2f _pos);

sfVector2f* pGetPlayerPos(playerType _type);

sfVector2f* pGetPlayerVelocity(playerType _type);

sfVector2f getPlayerVelocity(playerType _type);

void setPlayerLauchingTimer(playerType _type, float _launchingTimer);

float getPlayerLauchingTimer(playerType _type);

sfFloatRect getPlayerBounds(playerType _type);

sfFloatRect* pGetPlayerBounds(playerType _type);

void setPlayerMusicBlocTimer(playerType _type, float _timer);

float getPlayerMusicBlocTimer(playerType _type);

void setAnimPlayer(playerType _type, playerAnim _anim);

void setPlayerSpawnPos();
