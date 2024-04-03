#pragma once
#include "tools.h"
#include "stateManager.h"
#include "player.h"

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
	T_SLINGSHOT,
	T_LLEFTMOVING,
	T_LMOVING,
	T_LRIGHTMOVING,

	T_RLEFTMOVING,
	T_RMOVING,
	T_RRIGHTMOVING,
	T_MUSICBLOC,
	T_DOOR,
	T_FROGSPAWN,
	T_ASTRONAUTSPAWN,
	T_IHAVENTDECIDEDYET,

	T_GLOCK, // A Green Lock, no weapons are involved here
	T_BLOCK,
	T_RLOCK,
	T_YLOCK,

	T_GKEY,
	T_BKEY,
	T_RKEY,
	T_YKEY,

	T_GEXBLOCK,
	T_BEXBLOCK,
	T_REXBLOCK,
	T_YEXBLOCK,

	T_GBUTTON,
	T_BBUTTON,
	T_RBUTTON,
	T_YBUTTON,

	T_GPRESSEDBUTTON,
	T_BPRESSEDBUTTON,
	T_RPRESSEDBUTTON,
	T_YPRESSEDBUTTON,

	T_GREVERSEDBUTTON,
	T_BREVERSEDBUTTON,
	T_RREVERSEDBUTTON,
	T_YREVERSEDBUTTON,

	T_GREVERSEDPRESSEDBUTTON,
	T_BREVERSEDPRESSEDBUTTON,
	T_RREVERSEDPRESSEDBUTTON,
	T_YREVERSEDPRESSEDBUTTON,

	T_GOPENEDEXBLOCK,
	T_BOPENEDEXBLOCK,
	T_ROPENEDEXBLOCK,
	T_YOPENEDEXBLOCK,

}blockType;

typedef struct {
	blockType type;
	sfVector2f pos;
	sfIntRect rect;
	sfBool isSolid;
	float timer;
}Blocks;
Blocks b[NB_BLOCKS_Y][NB_BLOCKS_X];

sfFloatRect tmpRect;
sfFloatRect tmpRect2;
sfFloatRect tmpPlayerRect;
sfFloatRect tmpPlayerRect2;

void initMap();

void updateSlingshot(Window* _window);

void updateMap(Window* _window);

void displayMap(Window* _window);

void displayObjects(Window* _window);

void defaultMap();

void loadMap(int _nbMap);

sfVector2i getPlayerBlockPos(sfVector2f _pos);

sfBool isGrounded(sfVector2f _pos, sfVector2f* _velocity, sfVector2f* _drag);

sfBool isCollision3(sfFloatRect _rect, sfVector2f* _velocity);

sfBool isCollision2(sfFloatRect _rect, sfBool _XAxis, sfBool _UpOrLeft);

sfBool isCollision(sfFloatRect _rect, sfBool _XAxis, sfBool _UpOrLeft);

sfBool isSomeoneInSlingshot();

playerType getWhoIsInSlingshot();

sfVector2f getSlingshotBasePos();

sfBool isMapFinished();

void setMapFinishedState(sfBool _isMapF);

sfVector2f getFinishPlayerPos(int _nb);

sfIntRect getKeysAvailable();

void delock(int _j, int _i);

void keyBounds(sfFloatRect* _bounds);

void buttonBounds(sfFloatRect* _bounds);

void reversedButtonBounds(sfFloatRect* _bounds);
