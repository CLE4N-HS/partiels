#pragma once
#include "tools.h"
#include "stateManager.h"

void initHud();

void displayHud(Window* _window);

void setupKeys();

void collectKey(int _player, int _key);

sfBool canOpenLock(int _player, int _key);
