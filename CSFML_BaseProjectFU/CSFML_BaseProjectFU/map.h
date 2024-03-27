#pragma once
#include "tools.h"
#include "stateManager.h"

#define BLOCK_SIZE 32

#define BLOCK_SCALE 6

#define NB_BLOCKS_X 30
#define NB_BLOCKS_Y 17

int nbMap;

typedef enum {
	T_NOTILE,

	T_TLLIGHTWALL,
	T_TLIGHTWALL,
	T_TRLIGHTWALL,
	T_LIGHTWALL1,
	T_BRICKWALL1,
	T_SIGN,
	T_LEVER,
	T_BASETORCH,

	T_LLIGHTWALL,
	T_VOID,
	T_RLIGHTWALL,
	T_LIGHTWALL2,
	T_BRICKWALL2,
	T_SWORD,
	T_LIGHTSPIKES,
	T_SKULLS,

	T_BLLIGHTWALL,
	T_BLIGHTWALL,
	T_BRLIGHTWALL,
	T_DARKWALL1,
	T_SIDEBARRILS,
	T_LITTLEBARRIL,
	T_LEFTCHAIN,
	T_RIGHTCHAIN,

	T_TLDARKWALL,
	T_TDARKWALL,
	T_TRDARKWALL,
	T_DARKWALL2,
	T_BIGBARRIL,
	T_POT,
	T_BOTHCHAIN,
	T_DARKSPIKES,

	T_LDARKWALL,
	T_EMPTY,
	T_RDARKWALL,
	T_LEFTPLATFORM,
	T_PLATFORM,
	T_RIGHTPLATFORM,
	T_LEFTWEB,
	T_RIGHTWEB,

	T_BLDARKWALL,
	T_BDARKWALL,
	T_BRDARKWALL,
	T_TOPWALL,
	T_MIDWALL,
	T_LITTLEWALL,
	T_BIGWALL,
	T_LITTLESKULL,

	T_LITTLEBOX,
	T_BIGBOX,
	T_TORCH,
	T_BOTTOMWALL,
	T_SLINGSHOT
}blockType;

typedef struct {
	blockType type;
	sfVector2f pos;
	sfIntRect rect;
	sfBool isSolid;
}Blocks;
Blocks b[NB_BLOCKS_Y][NB_BLOCKS_X];

sfFloatRect tmpRect;
sfFloatRect tmpRect2;
sfFloatRect tmpPlayerRect;
sfFloatRect tmpPlayerRect2;

void initMap();

void updateMap(Window* _window);

void displayMap(Window* _window);

void displayObjects(Window* _window);

void defaultMap();

void loadMap(int _nbMap);

sfVector2i getPlayerBlockPos(sfVector2f _pos);

sfBool isGrounded(sfVector2f _pos);

sfBool isCollision2(sfFloatRect _rect, sfBool _XAxis, sfBool _UpOrLeft);

sfBool isCollision(sfFloatRect _rect, sfBool _XAxis, sfBool _UpOrLeft);

sfBool isSomeoneInSlingshot();

//playerType getWhoIsInSlingshot();
