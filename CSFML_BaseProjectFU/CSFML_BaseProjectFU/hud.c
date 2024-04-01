#include "hud.h"
#include "textureManager.h"
#include "player.h"
#include "viewManager.h"
#include "map.h"
#include "particlesSystemManager.h"

sfSprite* hudSprite;

typedef struct {
	sfVector2f pos;
	sfIntRect rect;
}PlayersHud;
PlayersHud hud[2][4];

sfIntRect availableKeys;

void initHud()
{
	hudSprite = sfSprite_create();
	sfSprite_setTexture(hudSprite, GetTexture("keys"), sfFalse);

	setupKeys();
}

void displayHud(Window* _window)
{
	sfRenderTexture_setView(_window->renderTexture, sfRenderTexture_getDefaultView(_window->renderTexture));
	int j = getViewFocus();
	for (int i = 0; i < 4; i++)
	{
		if (hud[j][i].pos.x < -10.f)
			continue;

		sfSprite_setPosition(hudSprite, hud[j][i].pos);
		sfSprite_setTextureRect(hudSprite, hud[j][i].rect);
		sfRenderTexture_drawSprite(_window->renderTexture, hudSprite, NULL);
	}
	sfRenderTexture_setView(_window->renderTexture, mainView->view);
}

void setupKeys()
{
	availableKeys = getKeysAvailable();


	for (int j = 0; j < 2; j++)
	{
		int nbKeys = 0;

		for (int i = 0; i < 4; i++)
		{
			switch (i)
			{
			case 0:
				if (availableKeys.left == 1) {
					hud[j][i].pos = vector2f(22.f + 44.f * nbKeys, 22.f);
					nbKeys++;
				}
				else
					hud[j][i].pos = vector2f(-100.f, 22.f);
				break;
			case 1:
				if (availableKeys.top == 1) {
					hud[j][i].pos = vector2f(22.f + 44.f * nbKeys, 22.f);
					nbKeys++;
				}
				else
					hud[j][i].pos = vector2f(-100.f, 22.f);
				break;
			case 2:
				if (availableKeys.width == 1) {
					hud[j][i].pos = vector2f(22.f + 44.f * nbKeys, 22.f);
					nbKeys++;
				}
				else
					hud[j][i].pos = vector2f(-100.f, 22.f);
				break;
			case 3:
				if (availableKeys.height == 1) {
					hud[j][i].pos = vector2f(22.f + 44.f * nbKeys, 22.f);
					nbKeys++;
				}
				else
					hud[j][i].pos = vector2f(-100.f, 22.f);
				break;
			default:
				break;
			}

			hud[j][i].rect = IntRect(44 * i, 0, 44, 40);
		}
	}
}

void collectKey(int _player, int _key)
{
	hud[_player][_key].rect.top = 40;

	sfColor keyColor;
	switch (_key)
	{
	case 0: keyColor = color(115, 205, 75, 255); break;
	case 1: keyColor = color(30, 167, 225, 255); break;
	case 2: keyColor = color(232, 106, 23, 255); break;
	case 3: keyColor = color(255, 204, 0, 255); break;
	default:
		keyColor = color(0, 0, 0, 0);
		break;
	}
	CreateParticles(getPlayerPos(_player), vector2f(10.f, 10.f), vector2f(10.f, 10.f), vector2f(5.f, 5.f), 0.f, 360.f, 0.f, 0.f, 100.f, 200.f, 5.f, keyColor, color(keyColor.r, keyColor.g, keyColor.b, 0), 0.4f, 0.6f, 50, "no", IntRect(0, 0, 0, 0), NULL, 0.f, 0.f, 0.f);
}

sfBool canOpenLock(int _player, int _key)
{
	if (hud[_player][_key].rect.top > 10)
		return sfTrue;

	return sfFalse;
}
