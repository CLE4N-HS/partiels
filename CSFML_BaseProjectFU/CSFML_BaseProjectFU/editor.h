#pragma once
#include "tools.h"
#include "stateManager.h"
#include "map.h"
#include "textureManager.h"

#define SAVE_PATH "../Ressources/Saves/"
#define NB_MAX_TILES 30

void initEditor();
void updateEditor(Window* _window);
void displayEditor(Window* _window);
void deinitEditor();

void saveMap(int _nbMap);
void copyMap(int _MapToCopy);