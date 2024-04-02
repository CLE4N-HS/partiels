#include "map.h"
#include "editor.h"
#include "CustomMath.h"
#include "gamepadx.h"
#include "finish.h"
#include "hud.h"
#include "particlesSystemManager.h"

#define NB_VERTEX 100
#define SECONDS_BETWEEN 0.04197f
#define MOVING_PLATFORMS_SPEED 320.f

typedef struct {
	sfBool canShow;
	sfVector2f origin;
	sfVector2f pos;
	sfVector2f scale;
	float timer;
}xboxAButton;
xboxAButton xboxA;

sfSprite* mapSprite;

sfTexture* castleTexture;
sfTexture* slingshotTexture;
sfTexture* leftMovingTexture;
sfTexture* rightMovingTexture;
sfTexture* musicBlocTexture;
sfTexture* doorsTexture;
sfTexture* spawnTexture;
sfTexture* objectsTexture;

sfTexture* xboxATexture;

sfVertexArray* vArray;

typedef struct {
	sfBool isInSlingshot;
	playerType type;
	sfVector2f forward;
	sfVector2f basePos;
	sfVector2f oldPlayerPos;
	sfVertex vertex[3];
	sfBool canLaunch;
}Slingshot;
Slingshot slingshot;

sfIntRect pressedButtons;


float changeMapTimer;

sfBool isMapF;
sfVector2f frogDoorPos;
sfVector2f astronautDoorPos;

void initMap()
{
	mapSprite = sfSprite_create();

	nbMap = 1;
	loadMap(1);
	changeMapTimer = 0.f;

	isMapF = sfFalse;
	frogDoorPos = VECTOR2F_NULL;
	astronautDoorPos = VECTOR2F_NULL;

	castleTexture = GetTexture("castleTiles");
	slingshotTexture = GetTexture("slingshot");
	leftMovingTexture = GetTexture("leftMoving");
	rightMovingTexture = GetTexture("rightMoving");
	musicBlocTexture = GetTexture("musicBloc");
	doorsTexture = GetTexture("doors");
	spawnTexture = GetTexture("spawn");
	objectsTexture = GetTexture("objects");
	
	xboxATexture = GetTexture("xboxA");

	vArray = sfVertexArray_create();
	sfVertexArray_setPrimitiveType(vArray, sfLineStrip);

	xboxA.canShow = sfFalse;
	xboxA.origin = vector2f(64.f, 64.f);
	xboxA.scale = VECTOR2F_NULL;
	xboxA.pos = VECTOR2F_NULL;
	xboxA.timer = 0.f;

	slingshot.isInSlingshot = sfFalse;
	slingshot.type = FROG;
	slingshot.forward = VECTOR2F_NULL;
	slingshot.basePos = VECTOR2F_NULL;
	slingshot.oldPlayerPos = VECTOR2F_NULL;
	for (int i = 0; i < 3; i++)
	{
		slingshot.vertex[i].color = color(48, 22, 8, 255);
		slingshot.vertex[i].position = VECTOR2F_NULL;
	}
	slingshot.canLaunch = sfFalse;

	pressedButtons = IntRect(0, 0, 0, 0);

	sfSprite_setTexture(mapSprite, castleTexture, sfFalse);
}

void updateSlingshot(Window* _window)
{
	float dt = getDeltaTime();

	playerType _type = slingshot.type;

	slingshot.vertex[1].position = getPlayerPos(_type);

	customAttract(getPlayerPos(_type), pGetPlayerVelocity(_type), slingshot.basePos, GetSqrMagnitude(CreateVector(slingshot.basePos, getPlayerPos(_type))) * 1.2f, 1000.f, dt);


	float dtt = 0.f;
	sfVertex vertex;
	for (int k = 0; k < NB_VERTEX; k++)
	{
		sfVector2f futurePos = MultiplyVector(CreateVector(getPlayerPos(_type), slingshot.basePos), 30.f);
		futurePos.y += 0.5f * GRAVITY * dtt * 100.f;

		vertex.color = sfWhite;
		vertex.position = AddVectors(getPlayerPos(_type), MultiplyVector(futurePos, dtt / 2.f * 30.f));
		sfVertexArray_append(vArray, vertex);

		dtt = (float)k * SECONDS_BETWEEN / NB_VERTEX;

	}



	//float t = 0.f;
	//sfVector2f tmpVelocity = CreateVector(getPlayerPos(_type), slingshot.basePos);
	//sfVector2f newVelocity = tmpVelocity;
	//for (int i = 0; i < NB_VERTEX; i++)
	//{
	//
	//	sfVector2f nextPos = MultiplyVector(tmpVelocity, 123.f);
	//	tmpVelocity = MultiplyVector(newVelocity, 1.f - SLINGSHOT_DRAG * dt * 1.f);
	//	float a = GRAVITY * t * 250.f;
	//	nextPos.y += a;
	//	sfVertex vertex;
	//	vertex.color = sfWhite;
	//	vertex.position = AddVectors(getPlayerPos(_type), MultiplyVector(nextPos, t / 2.f));
	//	sfVertexArray_append(vArray, vertex);
	//	t = i * 0.1f / NB_VERTEX;
	//}


	if (isButtonPressed(0, B) || sfKeyboard_isKeyPressed(sfKeyBackspace))
	{
		setPlayerPos(_type, slingshot.oldPlayerPos);
		slingshot.isInSlingshot = sfFalse;
	}

	if (!slingshot.canLaunch && !isButtonPressed(0, A) && !sfKeyboard_isKeyPressed(sfKeyEnter))
	{
		slingshot.canLaunch = sfTrue;
	}

	if (slingshot.canLaunch && (isButtonPressed(0, A) || sfKeyboard_isKeyPressed(sfKeyEnter)))
	{
		customAddForce(pGetPlayerVelocity(_type), MultiplyVector(CreateVector(getPlayerPos(_type), slingshot.basePos), 30.f));
		slingshot.isInSlingshot = sfFalse;
		setPlayerLauchingTimer(_type, 0.f);
	}
}

void updateMap(Window* _window)
{
	float dt = getDeltaTime();

	playerType _viewFocus = getViewFocus();
	sfFloatRect _bounds = getPlayerBounds(_viewFocus);
	xboxA.canShow = sfFalse;

	int nbPlayerAtDoors = 0;

	for (int j = 0; j < NB_BLOCKS_Y; j++)
	{
		for (int i = 0; i < NB_BLOCKS_X; i++)
		{
			sfFloatRect blockBounds = FlRect(b[j][i].pos.x, b[j][i].pos.y, BLOCK_SIZE * BLOCK_SCALE, BLOCK_SIZE * BLOCK_SCALE);
			switch (b[j][i].type)
			{
			case T_SLINGSHOT:
				if (!slingshot.isInSlingshot && getPlayerLauchingTimer(_viewFocus) >= LAUNCHING_TIMER_DURATION && sfFloatRect_intersects(&_bounds, &blockBounds, NULL))
				{
					xboxA.canShow = sfTrue;
					xboxA.pos = AddVectors(b[j][i].pos, vector2f(64.f, 64.f));
					xboxA.timer += dt;
					float sin = (fabs(sinf(xboxA.timer)) + 0.5f) * 0.5f;
					xboxA.scale = vector2f(sin / 2.f, sin / 2.f);

					if (isButtonPressed(0, A) || sfKeyboard_isKeyPressed(sfKeyEnter))
					{
						slingshot.type = _viewFocus;
						slingshot.isInSlingshot = sfTrue;
						slingshot.basePos = AddVectors(b[j][i].pos, vector2f(64.f, 64.f));
						slingshot.oldPlayerPos = getPlayerPos(slingshot.type);
						slingshot.vertex[0].position = AddVectors(b[j][i].pos, vector2f(56.f * 2.f, 38.f));
						slingshot.vertex[2].position = AddVectors(b[j][i].pos, vector2f(14.f, 36.f));
						slingshot.canLaunch = sfFalse;
						setPlayerPos(slingshot.type, slingshot.basePos);
					}
				}
				break;
			case T_LLEFTMOVING:
				b[j][i].timer += dt;
				Animator(&b[j][i].rect, &b[j][i].timer, 3, 0, 0.1f, 0.f);
				break;
			case T_LMOVING:
				b[j][i].timer += dt;
				Animator(&b[j][i].rect, &b[j][i].timer, 3, 0, 0.1f, 0.f);
				break;
			case T_LRIGHTMOVING:
				b[j][i].timer += dt;
				Animator(&b[j][i].rect, &b[j][i].timer, 3, 0, 0.1f, 0.f);
				break;
			case T_RLEFTMOVING:
				b[j][i].timer += dt;
				Animator(&b[j][i].rect, &b[j][i].timer, 3, 0, 0.1f, 0.f);
				break;
			case T_RMOVING:
				b[j][i].timer += dt;
				Animator(&b[j][i].rect, &b[j][i].timer, 3, 0, 0.1f, 0.f);
				break;
			case T_RRIGHTMOVING:
				b[j][i].timer += dt;
				Animator(&b[j][i].rect, &b[j][i].timer, 3, 0, 0.1f, 0.f);
				break;
			case T_MUSICBLOC:
				if (b[j][i].timer > 0.f)
					b[j][i].timer -= dt;
				break;
			case T_DOOR:
				if (sfFloatRect_intersects(&blockBounds, pGetPlayerBounds(FROG), NULL)) {
					b[j][i].rect = IntRect(32, 0, 32, 32);
					nbPlayerAtDoors++;
					frogDoorPos = AddVectors(b[j][i].pos, vector2f(BLOCK_SIZE * BLOCK_SCALE / 2.f, BLOCK_SIZE * BLOCK_SCALE / 2.f));
				}
				else if (sfFloatRect_intersects(&blockBounds, pGetPlayerBounds(ASTRONAUT), NULL)) {
					b[j][i].rect = IntRect(64, 0, 32, 32);
					nbPlayerAtDoors++;
					astronautDoorPos = AddVectors(b[j][i].pos, vector2f(BLOCK_SIZE * BLOCK_SCALE / 2.f, BLOCK_SIZE * BLOCK_SCALE / 2.f));
				}
				else
					b[j][i].rect = IntRect(0, 0, 32, 32);
				break;
			case T_GKEY:
				keyBounds(&blockBounds);
				if (sfFloatRect_intersects(&blockBounds, pGetPlayerBounds(FROG), NULL)) {
					collectKey(FROG, 0);
					b[j][i].type = T_EMPTY;
					b[j][i].rect = IntRect(1 * BLOCK_SIZE, 4 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE);
				}
				else if (sfFloatRect_intersects(&blockBounds, pGetPlayerBounds(ASTRONAUT), NULL)) {
					collectKey(ASTRONAUT, 0);
					b[j][i].type = T_EMPTY;
					b[j][i].rect = IntRect(1 * BLOCK_SIZE, 4 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE);
				}
				break;
			case T_BKEY:
				keyBounds(&blockBounds);
				if (sfFloatRect_intersects(&blockBounds, pGetPlayerBounds(FROG), NULL)) {
					collectKey(FROG, 1);
					b[j][i].type = T_EMPTY;
					b[j][i].rect = IntRect(1 * BLOCK_SIZE, 4 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE);
				}
				else if (sfFloatRect_intersects(&blockBounds, pGetPlayerBounds(ASTRONAUT), NULL)) {
					collectKey(ASTRONAUT, 1);
					b[j][i].type = T_EMPTY;
					b[j][i].rect = IntRect(1 * BLOCK_SIZE, 4 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE);
				}
				break;
			case T_RKEY:
				keyBounds(&blockBounds);
				if (sfFloatRect_intersects(&blockBounds, pGetPlayerBounds(FROG), NULL)) {
					collectKey(FROG, 2);
					b[j][i].type = T_EMPTY;
					b[j][i].rect = IntRect(1 * BLOCK_SIZE, 4 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE);
				}
				else if (sfFloatRect_intersects(&blockBounds, pGetPlayerBounds(ASTRONAUT), NULL)) {
					collectKey(ASTRONAUT, 2);
					b[j][i].type = T_EMPTY;
					b[j][i].rect = IntRect(1 * BLOCK_SIZE, 4 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE);
				}
				break;
			case T_YKEY:
				keyBounds(&blockBounds);
				if (sfFloatRect_intersects(&blockBounds, pGetPlayerBounds(FROG), NULL)) {
					collectKey(FROG, 3);
					b[j][i].type = T_EMPTY;
					b[j][i].rect = IntRect(1 * BLOCK_SIZE, 4 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE);
				}
				else if (sfFloatRect_intersects(&blockBounds, pGetPlayerBounds(ASTRONAUT), NULL)) {
					collectKey(ASTRONAUT, 3);
					b[j][i].type = T_EMPTY;
					b[j][i].rect = IntRect(1 * BLOCK_SIZE, 4 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE);
				}
				break;
			case T_GBUTTON:
				buttonBounds(&blockBounds);
				if (sfFloatRect_intersects(&blockBounds, pGetPlayerBounds(_viewFocus), NULL)) {
					pressedButtons.left = 1;
					b[j][i].type = T_GPRESSEDBUTTON;
					b[j][i].isSolid = sfFalse;
					b[j][i].rect.top += 64;
				}
				break;
			case T_BBUTTON:
				buttonBounds(&blockBounds);
				if (sfFloatRect_intersects(&blockBounds, pGetPlayerBounds(_viewFocus), NULL)) {
					pressedButtons.top = 1;
					b[j][i].type = T_BPRESSEDBUTTON;
					b[j][i].isSolid = sfFalse;
					b[j][i].rect.top += 64;
				}
				break;
			case T_RBUTTON:
				buttonBounds(&blockBounds);
				if (sfFloatRect_intersects(&blockBounds, pGetPlayerBounds(_viewFocus), NULL)) {
					pressedButtons.width = 1;
					b[j][i].type = T_RPRESSEDBUTTON;
					b[j][i].isSolid = sfFalse;
					b[j][i].rect.top += 64;
				}
				break;
			case T_YBUTTON:
				buttonBounds(&blockBounds);
				if (sfFloatRect_intersects(&blockBounds, pGetPlayerBounds(_viewFocus), NULL)) {
					pressedButtons.height = 1;
					b[j][i].type = T_YPRESSEDBUTTON;
					b[j][i].isSolid = sfFalse;
					b[j][i].rect.top += 64;
				}
				break;
			case T_GREVERSEDBUTTON:
				reversedButtonBounds(&blockBounds);
				if (sfFloatRect_intersects(&blockBounds, pGetPlayerBounds(_viewFocus), NULL)) {
					pressedButtons.left = 1;
					b[j][i].type = T_GREVERSEDPRESSEDBUTTON;
					b[j][i].isSolid = sfFalse;
					b[j][i].rect.top += 64;
				}
				break;
			case T_BREVERSEDBUTTON:
				reversedButtonBounds(&blockBounds);
				if (sfFloatRect_intersects(&blockBounds, pGetPlayerBounds(_viewFocus), NULL)) {
					pressedButtons.top = 1;
					b[j][i].type = T_BREVERSEDPRESSEDBUTTON;
					b[j][i].isSolid = sfFalse;
					b[j][i].rect.top += 64;
				}
				break;
			case T_RREVERSEDBUTTON:
				reversedButtonBounds(&blockBounds);
				if (sfFloatRect_intersects(&blockBounds, pGetPlayerBounds(_viewFocus), NULL)) {
					pressedButtons.width = 1;
					b[j][i].type = T_RREVERSEDPRESSEDBUTTON;
					b[j][i].isSolid = sfFalse;
					b[j][i].rect.top += 64;
				}
				break;
			case T_YREVERSEDBUTTON:
				reversedButtonBounds(&blockBounds);
				if (sfFloatRect_intersects(&blockBounds, pGetPlayerBounds(_viewFocus), NULL)) {
					pressedButtons.height = 1;
					b[j][i].type = T_YREVERSEDPRESSEDBUTTON;
					b[j][i].isSolid = sfFalse;
					b[j][i].rect.top += 64;
				}
				break;
			case T_GEXBLOCK:
				if (pressedButtons.left == 1) {
					b[j][i].type = T_GOPENEDEXBLOCK;
					b[j][i].isSolid = sfFalse;
					b[j][i].rect = IntRect(0, 224, 32, 32);
				}
				break;
			case T_BEXBLOCK:
				if (pressedButtons.top == 1) {
					b[j][i].type = T_BOPENEDEXBLOCK;
					b[j][i].isSolid = sfFalse;
					b[j][i].rect = IntRect(32, 224, 32, 32);
				}
				break;
			case T_REXBLOCK:
				if (pressedButtons.width== 1) {
					b[j][i].type = T_ROPENEDEXBLOCK;
					b[j][i].isSolid = sfFalse;
					b[j][i].rect = IntRect(64, 224, 32, 32);
				}
				break;
			case T_YEXBLOCK:
				if (pressedButtons.height == 1) {
					b[j][i].type = T_GOPENEDEXBLOCK;
					b[j][i].isSolid = sfFalse;
					b[j][i].rect = IntRect(96, 224, 32, 32);
				}
				break;
			default:
				break;
			}
		}
	}

	// finish condition
	if (nbPlayerAtDoors >= 2)
	{
		isMapF = sfTrue;
		setFinishViewPos(0, frogDoorPos);
		setFinishViewPos(1, astronautDoorPos);
		setAnimPlayer(FROG, THROW);
		setAnimPlayer(ASTRONAUT, THROW);
	}

	// reset xboxA button
	if (!xboxA.canShow) {
		xboxA.scale = VECTOR2F_NULL;
		xboxA.timer = 0.f;
	}

	changeMapTimer += dt;

	int key = 0;
	if (sfKeyboard_isKeyPressed(sfKeyNumpad1)) key = 1;
	else if (sfKeyboard_isKeyPressed(sfKeyNumpad2)) key = 2;
	else if (sfKeyboard_isKeyPressed(sfKeyNumpad3)) key = 3;
	else if (sfKeyboard_isKeyPressed(sfKeyNumpad4)) key = 4;
	else if (sfKeyboard_isKeyPressed(sfKeyNumpad5)) key = 5;
	else if (sfKeyboard_isKeyPressed(sfKeyNumpad6)) key = 6;
	else if (sfKeyboard_isKeyPressed(sfKeyNumpad7)) key = 7;
	else if (sfKeyboard_isKeyPressed(sfKeyNumpad8)) key = 8;
	else if (sfKeyboard_isKeyPressed(sfKeyNumpad9)) key = 9;

	if (key > 0 && changeMapTimer > 0.5f) {
		changeMapTimer = 0.f;
		loadMap(key);
		setPlayerSpawnPos();
		setupKeys();
		pressedButtons = IntRect(0, 0, 0, 0);
		nbMap = key;
	}
}

void displayMap(Window* _window)
{
	for (int j = 0; j < NB_BLOCKS_Y; j++)
	{
		for (int i = 0; i < NB_BLOCKS_X; i++)
		{
			sfSprite_setPosition(mapSprite, b[j][i].pos);
			sfSprite_setScale(mapSprite, vector2f(BLOCK_SCALE, BLOCK_SCALE));
			sfSprite_setColor(mapSprite, color(255, 255, 255, 255));

			blockType tmpType = b[j][i].type;

			if (tmpType == T_SLINGSHOT) {
				if (!slingshot.isInSlingshot) {
					sfSprite_setTexture(mapSprite, slingshotTexture, sfFalse);
					sfSprite_setScale(mapSprite, vector2f(2.f, 1.f));
					sfSprite_setTextureRect(mapSprite, IntRect(66, 0, 44, 209));
					sfSprite_setPosition(mapSprite, b[j][i].pos);
					sfRenderTexture_drawSprite(_window->renderTexture, mapSprite, NULL);
				}
				sfSprite_setTexture(mapSprite, slingshotTexture, sfFalse);
				sfSprite_setScale(mapSprite, vector2f(2.f, 1.f));
			}
			else if (tmpType >= T_LLEFTMOVING && tmpType <= T_LRIGHTMOVING) sfSprite_setTexture(mapSprite, leftMovingTexture, sfFalse);
			else if (tmpType >= T_RLEFTMOVING && tmpType <= T_RRIGHTMOVING) sfSprite_setTexture(mapSprite, rightMovingTexture, sfFalse);
			else if (tmpType == T_MUSICBLOC) {
				if (b[j][i].timer > 0.f) sfSprite_setPosition(mapSprite, AddVectors(b[j][i].pos, vector2f(0.f, b[j][i].timer * BLOCK_SIZE * 2)));
				sfSprite_setTexture(mapSprite, musicBlocTexture, sfFalse);
			}
			else if (tmpType == T_DOOR) sfSprite_setTexture(mapSprite, doorsTexture, sfFalse);
			else if (tmpType == T_FROGSPAWN || tmpType == T_ASTRONAUTSPAWN) sfSprite_setTexture(mapSprite, spawnTexture, sfFalse);
			else if (tmpType >= T_GLOCK && tmpType <= T_YOPENEDEXBLOCK) sfSprite_setTexture(mapSprite, objectsTexture, sfFalse);
			else sfSprite_setTexture(mapSprite, castleTexture, sfFalse);

			sfSprite_setTextureRect(mapSprite, b[j][i].rect);
			sfRenderTexture_drawSprite(_window->renderTexture, mapSprite, NULL);


			//switch (b[j][i].type)
			//{
			//case T_SLINGSHOT:
			//	if (!slingshot.isInSlingshot) {
			//		sfSprite_setTexture(mapSprite, slingshotTexture, sfFalse);
			//		sfSprite_setScale(mapSprite, vector2f(2.f, 1.f));
			//		sfSprite_setTextureRect(mapSprite, IntRect(66, 0, 44, 209));
			//		sfSprite_setPosition(mapSprite, b[j][i].pos);
			//		sfRenderTexture_drawSprite(_window->renderTexture, mapSprite, NULL);
			//	}
			//	sfSprite_setTexture(mapSprite, slingshotTexture, sfFalse);
			//	sfSprite_setScale(mapSprite, vector2f(2.f, 1.f));
			//	break;
			//case T_LLEFTMOVING:
			//	sfSprite_setTexture(mapSprite, leftMovingTexture, sfFalse);
			//	break;
			//case T_LMOVING:
			//	sfSprite_setTexture(mapSprite, leftMovingTexture, sfFalse);
			//	break;
			//case T_LRIGHTMOVING:
			//	sfSprite_setTexture(mapSprite, leftMovingTexture, sfFalse);
			//	break;
			//case T_RLEFTMOVING:
			//	sfSprite_setTexture(mapSprite, rightMovingTexture, sfFalse);
			//	break;
			//case T_RMOVING:
			//	sfSprite_setTexture(mapSprite, rightMovingTexture, sfFalse);
			//	break;
			//case T_RRIGHTMOVING:
			//	sfSprite_setTexture(mapSprite, rightMovingTexture, sfFalse);
			//	break;
			//case T_MUSICBLOC:
			//	if (b[j][i].timer > 0.f) sfSprite_setPosition(mapSprite, AddVectors(b[j][i].pos, vector2f(0.f, b[j][i].timer * BLOCK_SIZE * 2)));
			//	sfSprite_setTexture(mapSprite, musicBlocTexture, sfFalse);
			//	break;
			//case T_DOOR:
			//	sfSprite_setTexture(mapSprite, doorsTexture, sfFalse);
			//	break;
			//case T_FROGSPAWN:
			//	sfSprite_setTexture(mapSprite, spawnTexture, sfFalse);
			//	break;
			//case T_ASTRONAUTSPAWN:
			//	sfSprite_setTexture(mapSprite, spawnTexture, sfFalse);
			//	break;
			//default:
			//	sfSprite_setTexture(mapSprite, castleTexture, sfFalse);
			//	sfSprite_setScale(mapSprite, vector2f(BLOCK_SCALE, BLOCK_SCALE));
			//	break;
			//}
			//sfSprite_setTextureRect(mapSprite, b[j][i].rect);
			//sfRenderTexture_drawSprite(_window->renderTexture, mapSprite, NULL);
		}
	}

	if (slingshot.isInSlingshot) {
		sfRenderTexture_drawVertexArray(_window->renderTexture, vArray, NULL);
		sfVertexArray_clear(vArray);

		sfVertexArray_append(vArray, slingshot.vertex[0]);
		sfVertexArray_append(vArray, slingshot.vertex[1]);
		sfRenderTexture_drawVertexArray(_window->renderTexture, vArray, NULL);
		sfVertexArray_clear(vArray);
	}
}

void displayObjects(Window* _window)
{
	if (slingshot.isInSlingshot) {
		sfVertexArray_append(vArray, slingshot.vertex[2]);
		sfVertexArray_append(vArray, slingshot.vertex[1]);
		sfRenderTexture_drawVertexArray(_window->renderTexture, vArray, NULL);
		sfVertexArray_clear(vArray);
	}

	for (int j = 0; j < NB_BLOCKS_Y; j++)
	{
		for (int i = 0; i < NB_BLOCKS_X; i++)
		{
			switch (b[j][i].type)
			{
			case T_SLINGSHOT:
				if (slingshot.isInSlingshot) {
					sfSprite_setTexture(mapSprite, slingshotTexture, sfFalse);
					sfSprite_setScale(mapSprite, vector2f(2.f, 1.f));
					sfSprite_setTextureRect(mapSprite, IntRect(66, 0, 44, 209));
					sfSprite_setPosition(mapSprite, b[j][i].pos);
					sfRenderTexture_drawSprite(_window->renderTexture, mapSprite, NULL);
				}
				break;
			default:
				break;
			}
		}
	}

	if (xboxA.canShow)
	{
		sfSprite_setTexture(mapSprite, xboxATexture, sfTrue);
		sfSprite_setOrigin(mapSprite, xboxA.origin);
		sfSprite_setScale(mapSprite, xboxA.scale);
		sfSprite_setPosition(mapSprite, xboxA.pos);
		sfRenderTexture_drawSprite(_window->renderTexture, mapSprite, NULL);
	}

	sfSprite_setOrigin(mapSprite, VECTOR2F_NULL);
}

void defaultMap()
{
	for (int j = 0; j < NB_BLOCKS_Y; j++)
	{
		for (int i = 0; i < NB_BLOCKS_X; i++)
		{
			//if (j > 3) {
			//	b[j][i].type = T_TLIGHTWALL;
			//	b[j][i].isSolid = sfTrue;
			//	b[j][i].rect = IntRect(1 * BLOCK_SIZE, 0 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE);
			//}
			//else {
			//	b[j][i].type = T_DARKWALL2;
			//	b[j][i].isSolid = sfFalse;
			//	b[j][i].rect = IntRect(4 * BLOCK_SIZE, 5 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE);
			//}
			//if (i <= 1)
			//{
			//	b[j][i].isSolid = sfTrue;
			//}
			b[j][i].isSolid = sfFalse;
			b[j][i].type = T_LITTLEWALL;
			b[j][i].rect = IntRect(0, 0, BLOCK_SIZE * BLOCK_SCALE, BLOCK_SIZE * BLOCK_SCALE);
			b[j][i].pos = vector2f(i * BLOCK_SIZE * BLOCK_SCALE, j * BLOCK_SIZE * BLOCK_SCALE);
			b[j][i].timer = 0.f;
		}
	}
}

void loadMap(int _nbMap)
{

	FILE* file;
	switch (_nbMap)
	{
	case 1:
		file = fopen(SAVE_PATH"map1.bin", "rb");
		if (file == NULL) {
			defaultMap();
			file = fopen(SAVE_PATH"map1.bin", "wb");
			fwrite(b, sizeof(Blocks), NB_BLOCKS_X * NB_BLOCKS_Y, file);
			fclose(file);
		}
		else {
			fread(&b, sizeof(Blocks), NB_BLOCKS_X * NB_BLOCKS_Y, file);
			fclose(file);
		}
		break;
	case 2:
		file = fopen(SAVE_PATH"map2.bin", "rb");
		if (file == NULL) {
			defaultMap();
			file = fopen(SAVE_PATH"map2.bin", "wb");
			fwrite(b, sizeof(Blocks), NB_BLOCKS_X * NB_BLOCKS_Y, file);
			fclose(file);
		}
		else {
			fread(&b, sizeof(Blocks), NB_BLOCKS_X * NB_BLOCKS_Y, file);
			fclose(file);
		}
		break;
	case 3:
		file = fopen(SAVE_PATH"map3.bin", "rb");
		if (file == NULL) {
			defaultMap();
			file = fopen(SAVE_PATH"map3.bin", "wb");
			fwrite(b, sizeof(Blocks), NB_BLOCKS_X * NB_BLOCKS_Y, file);
			fclose(file);
		}
		else {
			fread(&b, sizeof(Blocks), NB_BLOCKS_X * NB_BLOCKS_Y, file);
			fclose(file);
		}
		break;
	case 4:
		file = fopen(SAVE_PATH"map4.bin", "rb");
		if (file == NULL) {
			defaultMap();
			file = fopen(SAVE_PATH"map4.bin", "wb");
			fwrite(b, sizeof(Blocks), NB_BLOCKS_X * NB_BLOCKS_Y, file);
			fclose(file);
		}
		else {
			fread(&b, sizeof(Blocks), NB_BLOCKS_X * NB_BLOCKS_Y, file);
			fclose(file);
		}
		break;
	case 5:
		file = fopen(SAVE_PATH"map5.bin", "rb");
		if (file == NULL) {
			defaultMap();
			file = fopen(SAVE_PATH"map5.bin", "wb");
			fwrite(b, sizeof(Blocks), NB_BLOCKS_X * NB_BLOCKS_Y, file);
			fclose(file);
		}
		else {
			fread(&b, sizeof(Blocks), NB_BLOCKS_X * NB_BLOCKS_Y, file);
			fclose(file);
		}
		break;
	case 6:
		file = fopen(SAVE_PATH"map6.bin", "rb");
		if (file == NULL) {
			defaultMap();
			file = fopen(SAVE_PATH"map6.bin", "wb");
			fwrite(b, sizeof(Blocks), NB_BLOCKS_X * NB_BLOCKS_Y, file);
			fclose(file);
		}
		else {
			fread(&b, sizeof(Blocks), NB_BLOCKS_X * NB_BLOCKS_Y, file);
			fclose(file);
		}
		break;
	case 7:
		file = fopen(SAVE_PATH"map7.bin", "rb");
		if (file == NULL) {
			defaultMap();
			file = fopen(SAVE_PATH"map7.bin", "wb");
			fwrite(b, sizeof(Blocks), NB_BLOCKS_X * NB_BLOCKS_Y, file);
			fclose(file);
		}
		else {
			fread(&b, sizeof(Blocks), NB_BLOCKS_X * NB_BLOCKS_Y, file);
			fclose(file);
		}
		break;
	case 8:
		file = fopen(SAVE_PATH"map8.bin", "rb");
		if (file == NULL) {
			defaultMap();
			file = fopen(SAVE_PATH"map8.bin", "wb");
			fwrite(b, sizeof(Blocks), NB_BLOCKS_X * NB_BLOCKS_Y, file);
			fclose(file);
		}
		else {
			fread(&b, sizeof(Blocks), NB_BLOCKS_X * NB_BLOCKS_Y, file);
			fclose(file);
		}
		break;
	case 9:
		file = fopen(SAVE_PATH"map9.bin", "rb");
		if (file == NULL) {
			defaultMap();
			file = fopen(SAVE_PATH"map9.bin", "wb");
			fwrite(b, sizeof(Blocks), NB_BLOCKS_X * NB_BLOCKS_Y, file);
			fclose(file);
		}
		else {
			fread(&b, sizeof(Blocks), NB_BLOCKS_X * NB_BLOCKS_Y, file);
			fclose(file);
		}
		break;
	default:
		break;
	}
}

sfVector2i getPlayerBlockPos(sfVector2f _pos)
{
	sfVector2f fPos = MultiplyVector(_pos, 1.f / BLOCK_SIZE / BLOCK_SCALE);
	sfVector2i iPos = vector2i(0, 0);
	iPos.x = (int)fPos.x;
	iPos.y = (int)fPos.y;
	return iPos;
}

sfBool isGrounded(sfVector2f _pos, sfVector2f* _velocity, sfVector2f* _drag)
{
	sfVector2i blockPos = getPlayerBlockPos(vector2f(_pos.x - 32.f, _pos.y + 48.f)); // offset to not count the alpha 0
	sfVector2i blockPos2 = getPlayerBlockPos(vector2f(_pos.x + 32.f, _pos.y + 48.f));

	if (blockPos.y < 0 || blockPos.y >= NB_BLOCKS_Y || blockPos.x < 0 || blockPos.x >= NB_BLOCKS_X || blockPos2.y < 0 || blockPos2.y >= NB_BLOCKS_Y || blockPos2.x < 0 || blockPos2.x >= NB_BLOCKS_X) // out of array
		return sfFalse;

	//tmpRect = FlRect(b[blockPos.y][blockPos.x].pos.x, b[blockPos.y][blockPos.x].pos.y, BLOCK_SIZE * BLOCK_SCALE, BLOCK_SIZE * BLOCK_SCALE);
	//tmpRect2 = FlRect(b[blockPos2.y][blockPos2.x].pos.x, b[blockPos2.y][blockPos2.x].pos.y, BLOCK_SIZE * BLOCK_SCALE, BLOCK_SIZE * BLOCK_SCALE);

	if (b[blockPos.y][blockPos.x].isSolid || b[blockPos2.y][blockPos2.x].isSolid) {
		switch (b[blockPos.y][blockPos.x].type)
		{
		case T_LLEFTMOVING:
			_velocity->x = -MOVING_PLATFORMS_SPEED;
			_velocity->y = 0.f;
			break;
		case T_LMOVING:
			_velocity->x = -MOVING_PLATFORMS_SPEED;
			_velocity->y = 0.f;
			break;
		case T_LRIGHTMOVING:
			_velocity->x = -MOVING_PLATFORMS_SPEED;
			_velocity->y = 0.f;
			break;
		case T_RLEFTMOVING:
			_velocity->x = MOVING_PLATFORMS_SPEED;
			_velocity->y = 0.f;
			break;
		case T_RMOVING:
			_velocity->x = MOVING_PLATFORMS_SPEED;
			_velocity->y = 0.f;
			break;
		case T_RRIGHTMOVING:
			_velocity->x = MOVING_PLATFORMS_SPEED;
			_velocity->y = 0.f;
			break;
		case T_MUSICBLOC:
			if (getPlayerMusicBlocTimer(FROG) >= 0.9f) {
				setPlayerMusicBlocTimer(FROG, 0.f); // TODO : DYNAMIC
				b[blockPos.y][blockPos.x].timer = MUSIC_BLOC_TIMER_DURATION;
			}
			//_drag->y = 1.1f;
			_velocity->x = 0.f;
			break;
		default:
			break;
		}

		switch (b[blockPos2.y][blockPos2.x].type)
		{
		case T_LLEFTMOVING:
			_velocity->x = -MOVING_PLATFORMS_SPEED;
			_velocity->y = 0.f;
			break;
		case T_LMOVING:
			_velocity->x = -MOVING_PLATFORMS_SPEED;
			_velocity->y = 0.f;
			break;
		case T_LRIGHTMOVING:
			_velocity->x = -MOVING_PLATFORMS_SPEED;
			_velocity->y = 0.f;
			break;
		case T_RLEFTMOVING:
			_velocity->x = MOVING_PLATFORMS_SPEED;
			_velocity->y = 0.f;
			break;
		case T_RMOVING:
			_velocity->x = MOVING_PLATFORMS_SPEED;
			_velocity->y = 0.f;
			break;
		case T_RRIGHTMOVING:
			_velocity->x = MOVING_PLATFORMS_SPEED;
			_velocity->y = 0.f;
			break;
		case T_MUSICBLOC:
			if (getPlayerMusicBlocTimer(FROG) >= 0.9f) {
				setPlayerMusicBlocTimer(FROG, 0.f); // TODO : DYNAMIC
				b[blockPos2.y][blockPos2.x].timer = MUSIC_BLOC_TIMER_DURATION;
			}
			//_drag->y = 1.1f;
			_velocity->x = 0.f;
			break;
		default:
			break;
		}


		return sfTrue;
	}

	return sfFalse;
}

sfBool isCollision3(sfFloatRect _rect, sfVector2f* _velocity)
{
	if (_velocity->x > EPSILON)
	{
		if (isCollision2(_rect, sfTrue, sfFalse))
		{
			_velocity->x = 0.f;
			return sfTrue;
		}
	}
	if (_velocity->x < -EPSILON)
	{
		if (isCollision2(_rect, sfTrue, sfTrue))
		{
			_velocity->x = 0.f;
			return sfTrue;
		}
	}
	if (_velocity->y > EPSILON)
	{
		if (isCollision2(_rect, sfFalse, sfFalse))
		{
			_velocity->y = 0.f;
			return sfTrue;
		}
	}
	if (_velocity->y < -EPSILON)
	{
		if (isCollision2(_rect, sfFalse, sfTrue))
		{
			_velocity->y = 0.f;
			return sfTrue;
		}
	}
	return sfFalse;
}

sfBool isCollision2(sfFloatRect _rect, sfBool _XAxis, sfBool _UpOrLeft)
{
	sfVector2f playerPos = VECTOR2F_NULL;
	sfVector2f playerPos2 = VECTOR2F_NULL;
	if (_XAxis) {
		if (_UpOrLeft) {
			playerPos = vector2f(_rect.left + 12.f, _rect.top + 12.f);
			playerPos2 = vector2f(_rect.left + 12.f, _rect.top + 84.f);
		}
		else {
			playerPos = vector2f(_rect.left + 84.f, _rect.top + 12.f);
			playerPos2 = vector2f(_rect.left + 84.f, _rect.top + 94.f);
		}
	}
	else {
		if (_UpOrLeft) {
			playerPos = vector2f(_rect.left + 12.f, _rect.top + 12.f);
			playerPos2 = vector2f(_rect.left + 84.f, _rect.top + 94.f);
		}
		else {
			playerPos = vector2f(_rect.left + 12.f, _rect.top + 94.f);
			playerPos2 = vector2f(_rect.left + 84.f, _rect.top + 94.f);
		}
	}

	sfVector2i blockPos = getPlayerBlockPos(playerPos);
	sfVector2i blockPos2 = getPlayerBlockPos(playerPos2);

	if (blockPos.y < 0 || blockPos.y >= NB_BLOCKS_Y || blockPos.x < 0 || blockPos.x >= NB_BLOCKS_X || blockPos2.y < 0 || blockPos2.y >= NB_BLOCKS_Y || blockPos2.x < 0 || blockPos2.x >= NB_BLOCKS_X) // out of array
		return sfFalse;

	tmpPlayerRect = FlRect(b[blockPos.y][blockPos.x].pos.x, b[blockPos.y][blockPos.x].pos.y, BLOCK_SIZE * BLOCK_SCALE, BLOCK_SIZE * BLOCK_SCALE);
	tmpPlayerRect2 = FlRect(b[blockPos2.y][blockPos2.x].pos.x, b[blockPos2.y][blockPos2.x].pos.y, BLOCK_SIZE * BLOCK_SCALE, BLOCK_SIZE * BLOCK_SCALE);

	
	if (_XAxis)
	{
		if (_UpOrLeft && blockPos.x > 0 && blockPos2.x > 0)
		{
			if (b[blockPos.y][blockPos.x - 1].isSolid)
			{
				sfFloatRect blockRect = FlRect(b[blockPos.y][blockPos.x - 1].pos.x, b[blockPos.y][blockPos.x - 1].pos.y, BLOCK_SIZE * BLOCK_SCALE, BLOCK_SIZE * BLOCK_SCALE);
				tmpRect = blockRect;
				if (sfFloatRect_intersects(&_rect, &blockRect, NULL))
				{
					switch (b[blockPos.y][blockPos.x - 1].type)
					{
					case T_LEFTPLATFORM: break;
					case T_PLATFORM: break;
					case T_GLOCK:
						if (canOpenLock(getViewFocus(), 0))
							delock(blockPos.y, blockPos.x - 1);
						return sfTrue;
					case T_BLOCK:
						if (canOpenLock(getViewFocus(), 1))
							delock(blockPos.y, blockPos.x - 1);
						return sfTrue;
					case T_RLOCK:
						if (canOpenLock(getViewFocus(), 2))
							delock(blockPos.y, blockPos.x - 1);
						return sfTrue;
					case T_YLOCK:
						if (canOpenLock(getViewFocus(), 3))
							delock(blockPos.y, blockPos.x - 1);
						return sfTrue;
					case T_GEXBLOCK:
						if (pressedButtons.left == 1) break;
					case T_BEXBLOCK:
						if (pressedButtons.top == 1) break;
					case T_REXBLOCK:
						if (pressedButtons.width == 1) break;
					case T_YEXBLOCK:
						if (pressedButtons.height == 1) break;
					default:
						return sfTrue;
					}
				}
			}
			if (b[blockPos2.y][blockPos2.x - 1].isSolid)
			{
				sfFloatRect blockRect2 = FlRect(b[blockPos2.y][blockPos2.x - 1].pos.x, b[blockPos2.y][blockPos2.x - 1].pos.y, BLOCK_SIZE * BLOCK_SCALE, BLOCK_SIZE * BLOCK_SCALE);
				tmpRect2 = blockRect2;
				if (sfFloatRect_intersects(&_rect, &blockRect2, NULL))
				{
					switch (b[blockPos2.y ][blockPos2.x - 1].type)
					{
					case T_LEFTPLATFORM: break;
					case T_PLATFORM: break;
					case T_GLOCK:
						if (canOpenLock(getViewFocus(), 0))
							delock(blockPos2.y, blockPos2.x - 1);
						return sfTrue;
					case T_BLOCK:
						if (canOpenLock(getViewFocus(), 1))
							delock(blockPos2.y, blockPos2.x - 1);
						return sfTrue;
					case T_RLOCK:
						if (canOpenLock(getViewFocus(), 2))
							delock(blockPos2.y, blockPos2.x - 1);
						return sfTrue;
					case T_YLOCK:
						if (canOpenLock(getViewFocus(), 3))
							delock(blockPos2.y, blockPos2.x - 1);
						return sfTrue;
					default:
						return sfTrue;
					}
				}
			}
		}
		else if (!_UpOrLeft && blockPos.x < NB_BLOCKS_X - 1 && blockPos2.x < NB_BLOCKS_X - 1)
		{
			if (b[blockPos.y][blockPos.x + 1].isSolid)
			{
				sfFloatRect blockRect = FlRect(b[blockPos.y][blockPos.x + 1].pos.x, b[blockPos.y][blockPos.x + 1].pos.y, BLOCK_SIZE * BLOCK_SCALE, BLOCK_SIZE * BLOCK_SCALE);
				tmpRect = blockRect;
				if (sfFloatRect_intersects(&_rect, &blockRect, NULL))
				{
					switch (b[blockPos.y][blockPos.x + 1].type)
					{
					case T_PLATFORM: break;
					case T_RIGHTPLATFORM: break;
					case T_GLOCK:
						if (canOpenLock(getViewFocus(), 0))
							delock(blockPos.y, blockPos.x + 1);
						return sfTrue;
					case T_BLOCK:
						if (canOpenLock(getViewFocus(), 1))
							delock(blockPos.y, blockPos.x + 1);
						return sfTrue;
					case T_RLOCK:
						if (canOpenLock(getViewFocus(), 2))
							delock(blockPos.y, blockPos.x + 1);
						return sfTrue;
					case T_YLOCK:
						if (canOpenLock(getViewFocus(), 3))
							delock(blockPos.y, blockPos.x + 1);
						return sfTrue;
					default:
						return sfTrue;
					}
				}
			}
			if (b[blockPos2.y][blockPos2.x + 1].isSolid)
			{
				sfFloatRect blockRect2 = FlRect(b[blockPos2.y][blockPos2.x + 1].pos.x, b[blockPos2.y][blockPos2.x + 1].pos.y, BLOCK_SIZE * BLOCK_SCALE, BLOCK_SIZE * BLOCK_SCALE);
				tmpRect2 = blockRect2;
				if (sfFloatRect_intersects(&_rect, &blockRect2, NULL))
				{
					switch (b[blockPos2.y][blockPos2.x + 1].type)
					{
					case T_PLATFORM: break;
					case T_RIGHTPLATFORM: break;
					case T_GLOCK:
						if (canOpenLock(getViewFocus(), 0))
							delock(blockPos2.y, blockPos2.x + 1);
						return sfTrue;
					case T_BLOCK:
						if (canOpenLock(getViewFocus(), 1))
							delock(blockPos2.y, blockPos2.x + 1);
						return sfTrue;
					case T_RLOCK:
						if (canOpenLock(getViewFocus(), 2))
							delock(blockPos2.y, blockPos2.x + 1);
						return sfTrue;
					case T_YLOCK:
						if (canOpenLock(getViewFocus(), 3))
							delock(blockPos2.y, blockPos2.x + 1);
						return sfTrue;
					default:
						return sfTrue;
					}
				}
			}
		}
	}
	else
	{
		if (_UpOrLeft && blockPos.y > 0 && blockPos2.y > 0)
		{
			if (b[blockPos.y - 1][blockPos.x].isSolid)
			{
				sfFloatRect blockRect = FlRect(b[blockPos.y - 1][blockPos.x].pos.x, b[blockPos.y - 1][blockPos.x].pos.y, BLOCK_SIZE * BLOCK_SCALE, BLOCK_SIZE * BLOCK_SCALE);
				tmpRect = blockRect;
				if (sfFloatRect_intersects(&_rect, &blockRect, NULL))
				{
					if (getViewFocus() == FROG)
						setPlayerMusicBlocTimer(FROG, MUSIC_BLOC_TIMER_DURATION);

					switch (b[blockPos.y - 1][blockPos.x].type)
					{
					case T_LEFTPLATFORM: break;
					case T_PLATFORM: break;
					case T_RIGHTPLATFORM: break;
					case T_GLOCK:
						if (canOpenLock(getViewFocus(), 0))
							delock(blockPos.y - 1, blockPos.x);
						return sfTrue;
					case T_BLOCK:
						if (canOpenLock(getViewFocus(), 1))
							delock(blockPos.y - 1, blockPos.x);
						return sfTrue;
					case T_RLOCK:
						if (canOpenLock(getViewFocus(), 2))
							delock(blockPos.y - 1, blockPos.x);
						return sfTrue;
					case T_YLOCK:
						if (canOpenLock(getViewFocus(), 3))
							delock(blockPos.y - 1, blockPos.x);
						return sfTrue;
					default:
						return sfTrue;
					}
				}
			}
			if (b[blockPos2.y - 1][blockPos2.x].isSolid)
			{
				sfFloatRect blockRect2 = FlRect(b[blockPos2.y - 1][blockPos2.x].pos.x, b[blockPos2.y - 1][blockPos2.x].pos.y, BLOCK_SIZE * BLOCK_SCALE, BLOCK_SIZE * BLOCK_SCALE);
				tmpRect = blockRect2;
				if (sfFloatRect_intersects(&_rect, &blockRect2, NULL))
				{
					if (getViewFocus() == FROG)
						setPlayerMusicBlocTimer(FROG, MUSIC_BLOC_TIMER_DURATION);

					switch (b[blockPos2.y - 1][blockPos2.x].type)
					{
					case T_LEFTPLATFORM: break;
					case T_PLATFORM: break;
					case T_RIGHTPLATFORM: break;
					case T_GLOCK:
						if (canOpenLock(getViewFocus(), 0))
							delock(blockPos2.y - 1, blockPos2.x);
						return sfTrue;
					case T_BLOCK:
						if (canOpenLock(getViewFocus(), 1))
							delock(blockPos2.y - 1, blockPos2.x);
						return sfTrue;
					case T_RLOCK:
						if (canOpenLock(getViewFocus(), 2))
							delock(blockPos2.y - 1, blockPos2.x);
						return sfTrue;
					case T_YLOCK:
						if (canOpenLock(getViewFocus(), 3))
							delock(blockPos2.y - 1, blockPos2.x);
						return sfTrue;
					default:
						return sfTrue;
					}
				}
			}
		}
		else if (!_UpOrLeft && blockPos.y < NB_BLOCKS_Y - 1 && blockPos2.y < NB_BLOCKS_Y - 1)
		{
			if (b[blockPos.y + 1][blockPos.x].isSolid)
			{
				sfFloatRect blockRect = FlRect(b[blockPos.y + 1][blockPos.x].pos.x, b[blockPos.y + 1][blockPos.x].pos.y, BLOCK_SIZE * BLOCK_SCALE, BLOCK_SIZE * BLOCK_SCALE);
				tmpRect = blockRect;
				if (sfFloatRect_intersects(&_rect, &blockRect, NULL))
				{
					switch (b[blockPos.y + 1][blockPos.x].type)
					{
					case T_GLOCK:
						if (canOpenLock(getViewFocus(), 0))
							delock(blockPos.y + 1, blockPos.x);
						return sfTrue;
					case T_BLOCK:
						if (canOpenLock(getViewFocus(), 1))
							delock(blockPos.y + 1, blockPos.x);
						return sfTrue;
					case T_RLOCK:
						if (canOpenLock(getViewFocus(), 2))
							delock(blockPos.y + 1, blockPos.x);
						return sfTrue;
					case T_YLOCK:
						if (canOpenLock(getViewFocus(), 3))
							delock(blockPos.y + 1, blockPos.x);
						return sfTrue;
					default:
						return sfTrue;
					}
				}
			}
			if (b[blockPos2.y + 1][blockPos2.x].isSolid)
			{
				sfFloatRect blockRect2 = FlRect(b[blockPos2.y + 1][blockPos2.x].pos.x, b[blockPos2.y + 1][blockPos2.x].pos.y, BLOCK_SIZE * BLOCK_SCALE, BLOCK_SIZE * BLOCK_SCALE);
				tmpRect = blockRect2;
				if (sfFloatRect_intersects(&_rect, &blockRect2, NULL))
				{
					switch (b[blockPos2.y + 1][blockPos2.x].type)
					{
					case T_GLOCK:
						if (canOpenLock(getViewFocus(), 0))
							delock(blockPos2.y + 1, blockPos2.x);
						return sfTrue;
					case T_BLOCK:
						if (canOpenLock(getViewFocus(), 1))
							delock(blockPos2.y + 1, blockPos2.x);
						return sfTrue;
					case T_RLOCK:
						if (canOpenLock(getViewFocus(), 2))
							delock(blockPos2.y + 1, blockPos2.x);
						return sfTrue;
					case T_YLOCK:
						if (canOpenLock(getViewFocus(), 3))
							delock(blockPos2.y + 1, blockPos2.x);
						return sfTrue;
					default:
						return sfTrue;
					}
				}
			}
		}
	}

	return sfFalse;
}


sfBool isCollision(sfFloatRect _rect, sfBool _XAxis, sfBool _UpOrLeft)
{
	sfVector2f playerPos = vector2f(_rect.left + 48.f, _rect.top + 48.f);
	sfVector2f playerPos2 = vector2f(_rect.left + 48.f, _rect.top + 48.f);
	sfVector2i blockPos = getPlayerBlockPos(playerPos);
	sfVector2i blockPos2 = getPlayerBlockPos(playerPos2);

	tmpPlayerRect = FlRect(b[blockPos.y][blockPos.x].pos.x, b[blockPos.y][blockPos.x].pos.y, BLOCK_SIZE * BLOCK_SCALE, BLOCK_SIZE * BLOCK_SCALE);


	if (blockPos.y < 0 || blockPos.y >= NB_BLOCKS_Y || blockPos.x < 0 || blockPos.x >= NB_BLOCKS_X) // out of array
		return sfFalse;

	if (_XAxis)
	{
		if (_UpOrLeft && blockPos.x > 0)
		{
			if (b[blockPos.y][blockPos.x - 1].isSolid)
			{
				sfFloatRect blockRect = FlRect(b[blockPos.y][blockPos.x - 1].pos.x, b[blockPos.y][blockPos.x - 1].pos.y, BLOCK_SIZE * BLOCK_SCALE, BLOCK_SIZE * BLOCK_SCALE);
				tmpRect = blockRect;
				if (sfFloatRect_intersects(&_rect, &blockRect, NULL))
					return sfTrue;
			}
		}
		else if (!_UpOrLeft && blockPos.x < NB_BLOCKS_X - 1)
		{
			if (b[blockPos.y][blockPos.x + 1].isSolid)
			{
				sfFloatRect blockRect = FlRect(b[blockPos.y][blockPos.x + 1].pos.x, b[blockPos.y][blockPos.x + 1].pos.y, BLOCK_SIZE * BLOCK_SCALE, BLOCK_SIZE * BLOCK_SCALE);
				tmpRect = blockRect;
				if (sfFloatRect_intersects(&_rect, &blockRect, NULL))
					return sfTrue;
			}
		}
	}
	else
	{
		if (_UpOrLeft && blockPos.y > 0)
		{
			if (b[blockPos.y - 1][blockPos.x].isSolid)
			{
				sfFloatRect blockRect = FlRect(b[blockPos.y - 1][blockPos.x].pos.x, b[blockPos.y - 1][blockPos.x].pos.y, BLOCK_SIZE * BLOCK_SCALE, BLOCK_SIZE * BLOCK_SCALE);
				tmpRect = blockRect;
				if (sfFloatRect_intersects(&_rect, &blockRect, NULL))
					return sfTrue;
			}
		}
		else if (!_UpOrLeft && blockPos.y < NB_BLOCKS_Y - 1)
		{
			if (b[blockPos.y + 1][blockPos.x].isSolid)
			{
				sfFloatRect blockRect = FlRect(b[blockPos.y + 1][blockPos.x].pos.x, b[blockPos.y + 1][blockPos.x].pos.y, BLOCK_SIZE * BLOCK_SCALE, BLOCK_SIZE * BLOCK_SCALE);
				tmpRect = blockRect;
				if (sfFloatRect_intersects(&_rect, &blockRect, NULL))
					return sfTrue;
			}
		}
	}

	return sfFalse;
}

sfBool isSomeoneInSlingshot()
{
	return slingshot.isInSlingshot;
}

playerType getWhoIsInSlingshot()
{
	return slingshot.type;
}

sfVector2f getSlingshotBasePos()
{
	return slingshot.basePos;
}

sfBool isMapFinished()
{
	return isMapF;
}

sfVector2f getFinishPlayerPos(int _nb)
{
	if (_nb == 0) return frogDoorPos;
	if (_nb == 1) return astronautDoorPos;
	return VECTOR2F_NULL;
}

sfIntRect getKeysAvailable()
{
	sfIntRect availableKeys = IntRect(0, 0, 0, 0);
	for (int j = 0; j < NB_BLOCKS_Y; j++)
	{
		for (int i = 0; i < NB_BLOCKS_X; i++)
		{
			if (b[j][i].type == T_GKEY) availableKeys.left = 1;
			if (b[j][i].type == T_BKEY) availableKeys.top = 1;
			if (b[j][i].type == T_RKEY )availableKeys.width = 1;
			if (b[j][i].type == T_YKEY) availableKeys.height = 1;
		}
	}
	return availableKeys;
}

void delock(int _j, int _i)
{
	CreateParticles(AddVectors(b[_j][_i].pos, vector2f(BLOCK_SIZE * BLOCK_SCALE / 2.f, BLOCK_SIZE * BLOCK_SCALE / 2.f)), vector2f(BLOCK_SCALE, BLOCK_SCALE), vector2f(BLOCK_SCALE, BLOCK_SCALE), vector2f(16.f, 16.f), 0.f, 360.f, 100.f, 3.f, 100.f, 200.f, 10.f, color(255, 255, 255, 255), color(255, 255, 255, 0), 1.f, 1.f, 1, "objects", b[_j][_i].rect, NULL, 0.f, 0.f, 0.f);
	b[_j][_i].type = T_EMPTY;
	b[_j][_i].rect = IntRect(1 * BLOCK_SIZE, 4 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE);
	b[_j][_i].isSolid = sfFalse;
}

void keyBounds(sfFloatRect* _bounds)
{
	_bounds->left += 3 * BLOCK_SCALE;
	_bounds->top += 8 * BLOCK_SCALE;
	_bounds->width -= 6 * BLOCK_SCALE;
	_bounds->height -= 16 * BLOCK_SCALE;
}

void buttonBounds(sfFloatRect* _bounds)
{
	_bounds->top += 11 * BLOCK_SCALE;
	_bounds->height -= 11 * BLOCK_SCALE;
}

void reversedButtonBounds(sfFloatRect* _bounds)
{
	_bounds->height -= 11 * BLOCK_SCALE;
}