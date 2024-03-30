#pragma once
#include "tools.h"
#include "stateManager.h"
#include "player.h"

#define FINISH_TIMER_DURATION 5.f

void initFinsih();

void updateFinish(Window* _window);

void displayFinish(Window* _window);

void setFinishViewPos(int _nb, sfVector2f _pos);

sfVector2f getFinishPlayerScale(playerType _type);
