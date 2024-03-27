#include "map.h"
#include "editor.h"
#include "CustomMath.h"
#include "gamepadx.h"

#define NB_VERTEX 100

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


float changeMapTimer;

void initMap()
{
	mapSprite = sfSprite_create();

	nbMap = 1;
	loadMap(1);
	changeMapTimer = 0.f;

	castleTexture = GetTexture("castleTiles");
	slingshotTexture = GetTexture("slingshot");
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

	sfSprite_setTexture(mapSprite, castleTexture, sfFalse);
}

void updateSlingshot(Window* _window)
{
	float dt = getDeltaTime();

	playerType _type = slingshot.type;

	slingshot.vertex[1].position = getPlayerPos(_type);

	customAttract(getPlayerPos(_type), pGetPlayerVelocity(_type), slingshot.basePos, /*600.f*/ GetSqrMagnitude(CreateVector(slingshot.basePos, getPlayerPos(_type))) * 0.02f, 1000.f, dt);


	float t = 0.f;
	sfVector2f tmpVelocity = CreateVector(getPlayerPos(_type), slingshot.basePos);
	sfVector2f newVelocity = tmpVelocity;
	for (int i = 0; i < NB_VERTEX; i++)
	{

		sfVector2f nextPos = MultiplyVector(tmpVelocity, 100.f);
		tmpVelocity = MultiplyVector(newVelocity, 1.f - SLINGSHOT_DRAG * dt * 2.f);
		float a = GRAVITY * t * 200.f;
		nextPos.y += a;
		sfVertex vertex;
		vertex.color = sfWhite;
		vertex.position = AddVectors(slingshot.basePos, MultiplyVector(nextPos, t / 2.f));
		sfVertexArray_append(vArray, vertex);
		t = i * 0.1f / NB_VERTEX;
	}


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
		customAddForce(pGetPlayerVelocity(_type), MultiplyVector(CreateVector(getPlayerPos(_type), slingshot.basePos), 10.f));
		slingshot.isInSlingshot = sfFalse;
		setPlayerLauchingTimer(_type, 0.f);
	}
}

void updateMap(Window* _window)
{
	float dt = getDeltaTime();

	playerType _viewFocus = getViewFocus();
	sfFloatRect _bounds = getPlayerRect(_viewFocus);
	xboxA.canShow = sfFalse;

	for (int j = 0; j < NB_BLOCKS_Y; j++)
	{
		for (int i = 0; i < NB_BLOCKS_X; i++)
		{
			sfFloatRect blockRect = FlRect(b[j][i].pos.x, b[j][i].pos.y, BLOCK_SIZE * BLOCK_SCALE, BLOCK_SIZE * BLOCK_SCALE);
			switch (b[j][i].type)
			{
			case T_SLINGSHOT:
				if (!slingshot.isInSlingshot && getPlayerLauchingTimer(_viewFocus) >= LAUNCHING_TIMER_DURATION && sfFloatRect_intersects(&_bounds, &blockRect, NULL))
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
			default:
				break;
			}
		}
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
		nbMap = key;
	}
}

void displayMap(Window* _window)
{
	for (int j = 0; j < NB_BLOCKS_Y; j++)
	{
		for (int i = 0; i < NB_BLOCKS_X; i++)
		{
			switch (b[j][i].type)
			{
			case T_SLINGSHOT:
				if (!slingshot.isInSlingshot) {
					sfSprite_setTexture(mapSprite, slingshotTexture, sfFalse);
					sfSprite_setScale(mapSprite, vector2f(2.f, 1.f));
					sfSprite_setTextureRect(mapSprite, IntRect(66, 0, 44, 209));
					sfSprite_setPosition(mapSprite, b[j][i].pos);
					sfRenderTexture_drawSprite(_window->renderTexture, mapSprite, NULL);
				}
				sfSprite_setTexture(mapSprite, slingshotTexture, sfFalse);
				sfSprite_setScale(mapSprite, vector2f(2.f, 1.f));
				break;
			default:
				sfSprite_setTexture(mapSprite, castleTexture, sfFalse);
				sfSprite_setScale(mapSprite, vector2f(BLOCK_SCALE, BLOCK_SCALE));
				break;
			}
			sfSprite_setTextureRect(mapSprite, b[j][i].rect);
			sfSprite_setPosition(mapSprite, b[j][i].pos);
			sfRenderTexture_drawSprite(_window->renderTexture, mapSprite, NULL);
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
	sfVector2i iPos;
	iPos.x = (int)fPos.x;
	iPos.y = (int)fPos.y;
	return iPos;
}

sfBool isGrounded(sfVector2f _pos)
{
	sfVector2i blockPos = getPlayerBlockPos(vector2f(_pos.x - 32.f, _pos.y + 48.f)); // offset to not count the alpha 0
	sfVector2i blockPos2 = getPlayerBlockPos(vector2f(_pos.x + 32.f, _pos.y + 48.f));

	//tmpRect = FlRect(b[blockPos.y][blockPos.x].pos.x, b[blockPos.y][blockPos.x].pos.y, BLOCK_SIZE * BLOCK_SCALE, BLOCK_SIZE * BLOCK_SCALE);
	//tmpRect2 = FlRect(b[blockPos2.y][blockPos2.x].pos.x, b[blockPos2.y][blockPos2.x].pos.y, BLOCK_SIZE * BLOCK_SCALE, BLOCK_SIZE * BLOCK_SCALE);

	if (b[blockPos.y][blockPos.x].isSolid || b[blockPos2.y][blockPos2.x].isSolid) {
		return sfTrue;
	}

	return sfFalse;
}

sfBool isCollision3(sfFloatRect _rect, sfVector2f* _velocity)
{

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

	tmpPlayerRect = FlRect(b[blockPos.y][blockPos.x].pos.x, b[blockPos.y][blockPos.x].pos.y, BLOCK_SIZE * BLOCK_SCALE, BLOCK_SIZE * BLOCK_SCALE);
	tmpPlayerRect2 = FlRect(b[blockPos2.y][blockPos2.x].pos.x, b[blockPos2.y][blockPos2.x].pos.y, BLOCK_SIZE * BLOCK_SCALE, BLOCK_SIZE * BLOCK_SCALE);


	if (blockPos.y < 0 || blockPos.y >= NB_BLOCKS_Y || blockPos.x < 0 || blockPos.x >= NB_BLOCKS_X || blockPos2.y < 0 || blockPos2.y >= NB_BLOCKS_Y || blockPos2.x < 0 || blockPos2.x >= NB_BLOCKS_X) // out of array
		return sfFalse;

	if (_XAxis)
	{
		if (_UpOrLeft && blockPos.x > 0 && blockPos2.x > 0)
		{
			if (b[blockPos.y][blockPos.x - 1].isSolid)
			{
				sfFloatRect blockRect = FlRect(b[blockPos.y][blockPos.x - 1].pos.x, b[blockPos.y][blockPos.x - 1].pos.y, BLOCK_SIZE * BLOCK_SCALE, BLOCK_SIZE * BLOCK_SCALE);
				tmpRect = blockRect;
				if (sfFloatRect_intersects(&_rect, &blockRect, NULL))
					return sfTrue;
			}
			if (b[blockPos2.y][blockPos2.x - 1].isSolid)
			{
				sfFloatRect blockRect2 = FlRect(b[blockPos2.y][blockPos2.x - 1].pos.x, b[blockPos2.y][blockPos2.x - 1].pos.y, BLOCK_SIZE * BLOCK_SCALE, BLOCK_SIZE * BLOCK_SCALE);
				tmpRect2 = blockRect2;
				if (sfFloatRect_intersects(&_rect, &blockRect2, NULL))
					return sfTrue;
			}
		}
		else if (!_UpOrLeft && blockPos.x < NB_BLOCKS_X - 1 && blockPos2.x < NB_BLOCKS_X - 1)
		{
			if (b[blockPos.y][blockPos.x + 1].isSolid)
			{
				sfFloatRect blockRect = FlRect(b[blockPos.y][blockPos.x + 1].pos.x, b[blockPos.y][blockPos.x + 1].pos.y, BLOCK_SIZE * BLOCK_SCALE, BLOCK_SIZE * BLOCK_SCALE);
				tmpRect = blockRect;
				if (sfFloatRect_intersects(&_rect, &blockRect, NULL))
					return sfTrue;
			}
			if (b[blockPos2.y][blockPos2.x + 1].isSolid)
			{
				sfFloatRect blockRect2 = FlRect(b[blockPos2.y][blockPos2.x + 1].pos.x, b[blockPos2.y][blockPos2.x + 1].pos.y, BLOCK_SIZE * BLOCK_SCALE, BLOCK_SIZE * BLOCK_SCALE);
				tmpRect2 = blockRect2;
				if (sfFloatRect_intersects(&_rect, &blockRect2, NULL))
					return sfTrue;
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
					return sfTrue;
			}
			if (b[blockPos2.y - 1][blockPos2.x].isSolid)
			{
				sfFloatRect blockRect2 = FlRect(b[blockPos2.y - 1][blockPos2.x].pos.x, b[blockPos2.y - 1][blockPos2.x].pos.y, BLOCK_SIZE * BLOCK_SCALE, BLOCK_SIZE * BLOCK_SCALE);
				tmpRect = blockRect2;
				if (sfFloatRect_intersects(&_rect, &blockRect2, NULL))
					return sfTrue;
			}
		}
		else if (!_UpOrLeft && blockPos.y < NB_BLOCKS_Y - 1 && blockPos2.y < NB_BLOCKS_Y - 1)
		{
			if (b[blockPos.y + 1][blockPos.x].isSolid)
			{
				sfFloatRect blockRect = FlRect(b[blockPos.y + 1][blockPos.x].pos.x, b[blockPos.y + 1][blockPos.x].pos.y, BLOCK_SIZE * BLOCK_SCALE, BLOCK_SIZE * BLOCK_SCALE);
				tmpRect = blockRect;
				if (sfFloatRect_intersects(&_rect, &blockRect, NULL))
					return sfTrue;
			}
			if (b[blockPos2.y + 1][blockPos2.x].isSolid)
			{
				sfFloatRect blockRect2 = FlRect(b[blockPos2.y + 1][blockPos2.x].pos.x, b[blockPos2.y + 1][blockPos2.x].pos.y, BLOCK_SIZE * BLOCK_SCALE, BLOCK_SIZE * BLOCK_SCALE);
				tmpRect = blockRect2;
				if (sfFloatRect_intersects(&_rect, &blockRect2, NULL))
					return sfTrue;
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
