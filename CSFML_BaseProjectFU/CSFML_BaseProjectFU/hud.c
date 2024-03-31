#include "hud.h"
#include "textureManager.h"
#include "player.h"
#include "viewManager.h"

sfSprite* hudSprite;

typedef struct {
	sfVector2f pos;
	sfIntRect rect;
	sfBool hasGreenKey;
	sfBool hasBlueKey;
	sfBool hasRedKey;
	sfBool hasYellowKey;
}PlayersHud;
PlayersHud hud[2][4];

int nbMaxKeys;

void initHud()
{
	hudSprite = sfSprite_create();
	sfSprite_setTexture(hudSprite, GetTexture("keys"), sfFalse);

	for (int j = 0; j < 2; j++)
	{
		for (int i = 0; i < 4; i++)
		{
			hud[j][i].pos = vector2f(22.f + 44.f * i, 22.f);
			hud[j][i].rect = IntRect(44 * i, 0, 44, 40);
			hud[j][i].hasGreenKey = sfFalse;
			hud[j][i].hasBlueKey = sfFalse;
			hud[j][i].hasRedKey = sfFalse;
			hud[j][i].hasYellowKey = sfFalse;
		}
	}
}

void displayHud(Window* _window)
{
	sfRenderTexture_setView(_window->renderTexture, sfRenderTexture_getDefaultView(_window->renderTexture));
	int j = getViewFocus();
	// to change
	nbMaxKeys = 4;
	for (int i = 0; i < nbMaxKeys; i++)
	{
		sfSprite_setPosition(hudSprite, hud[j][i].pos);
		sfSprite_setTextureRect(hudSprite, hud[j][i].rect);
		sfRenderTexture_drawSprite(_window->renderTexture, hudSprite, NULL);
	}
	sfRenderTexture_setView(_window->renderTexture, mainView->view);
}
