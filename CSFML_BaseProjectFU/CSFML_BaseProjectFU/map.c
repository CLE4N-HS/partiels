#include "map.h"
#include "editor.h"
#include "CustomMath.h"
#include "player.h"

sfSprite* mapSprite;

float changeMapTimer;

void initMap()
{
	mapSprite = sfSprite_create();

	nbMap = 1;
	loadMap(1);
	changeMapTimer = 0.f;

	sfSprite_setTexture(mapSprite, GetTexture("castleTiles"), sfFalse);
}

void updateMap(Window* _window)
{
	float dt = getDeltaTime();
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
			sfSprite_setTextureRect(mapSprite, b[j][i].rect);
			sfSprite_setPosition(mapSprite, b[j][i].pos);
			sfSprite_setScale(mapSprite, vector2f(BLOCK_SCALE, BLOCK_SCALE));
			sfRenderTexture_drawSprite(_window->renderTexture, mapSprite, NULL);
		}
	}
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
