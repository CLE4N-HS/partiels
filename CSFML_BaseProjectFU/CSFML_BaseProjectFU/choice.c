#include "choice.h"
#include "fontManager.h"
#include "gamepadx.h"

sfText* choiceText;

int nbChoice;

char buffer[3];

void initChoice()
{
	choiceText = sfText_create();
	sfText_setFont(choiceText, GetFont("GingerSoda"));
	sfText_setCharacterSize(choiceText, 60);
	nbChoice = 0;
}

void updateChoice(Window* _window)
{
	static float timer = 0.f;
	timer += getDeltaTime();

	if (timer > 0.2f)
	{
		if (isMovedOrPressed(sfTrue, sfTrue, 50.f, sfKeyRight)) {
			nbChoice++;
			if (nbChoice > 9)
				nbChoice = 0;
			timer = 0.f;
		}
		if (isMovedOrPressed(sfTrue, sfTrue, -50.f, sfKeyLeft)) {
			nbChoice--;
			if (nbChoice < 0)
				nbChoice = 9;
			timer = 0.f;
		}
		if (isMovedOrPressed(sfTrue, sfFalse, 50.f, sfKeyUp)) {
			nbChoice -= 3;
			if (nbChoice == 6)
				nbChoice = 7;
			else if (nbChoice < 0)
				nbChoice = 9;
			timer = 0.f;
		}
		if (isMovedOrPressed(sfTrue, sfFalse, -50.f, sfKeyDown)) {
			nbChoice += 3;
			if (nbChoice > 11)
				nbChoice = 1;
			else if (nbChoice > 9)
				nbChoice = 9;
			timer = 0.f;
		}
	}

	if (timer > 0.4f)
	{
		if (isButtonPressed(0, A) || sfKeyboard_isKeyPressed(sfKeyEnter))
		{
			timer = 0.f;
			isChoice = sfFalse;
			if (nbChoice >= 0 && nbChoice <= 8) {
				changeState(_window, GAME);
			}
			else {
				nbChoice = 0;
			}
		}

		if (isButtonPressed(0, B) || sfKeyboard_isKeyPressed(sfKeyEscape))
		{
			timer = 0.f;
			nbChoice = 0;
			isChoice = sfFalse;
		}
	}


}

void displayChoice(Window* _window)
{
	sfFloatRect tmpRect;

	sfText_setString(choiceText, "Choose your map");
	sfText_setPosition(choiceText, vector2f(960.f, 200.f));
	sfText_setFillColor(choiceText, sfWhite);
	tmpRect = sfText_getLocalBounds(choiceText);
	sfText_setOrigin(choiceText, TEXT_STRING_ORIGIN);
	sfText_setOrigin(choiceText, TEXT_ORIGIN);
	sfRenderTexture_drawText(_window->renderTexture, choiceText, NULL);

	for (int i = 0; i < 9; i++)
	{
		sprintf(buffer, "%d", i + 1);
		sfText_setString(choiceText, buffer);
		sfText_setPosition(choiceText, vector2f(860.f + (i % 3) * 100.f, 400.f + (i / 3) * 100.f));
		if (i == nbChoice) sfText_setFillColor(choiceText, sfRed);
		else sfText_setFillColor(choiceText, sfWhite);
		tmpRect = sfText_getLocalBounds(choiceText);
		sfText_setOrigin(choiceText, TEXT_STRING_ORIGIN);
		sfText_setOrigin(choiceText, TEXT_ORIGIN);
		sfRenderTexture_drawText(_window->renderTexture, choiceText, NULL);
	}

	sfText_setString(choiceText, "retour");
	sfText_setPosition(choiceText, vector2f(960.f, 800.f));
	if (nbChoice == 9) sfText_setFillColor(choiceText, sfRed);
	else sfText_setFillColor(choiceText, sfWhite);
	tmpRect = sfText_getLocalBounds(choiceText);
	sfText_setOrigin(choiceText, TEXT_STRING_ORIGIN);
	sfText_setOrigin(choiceText, TEXT_ORIGIN);
	sfRenderTexture_drawText(_window->renderTexture, choiceText, NULL);
}

int getMapToLoad()
{
	if (nbChoice >= 0 && nbChoice <= 8)
		return nbChoice + 1;
	return 1;
}
