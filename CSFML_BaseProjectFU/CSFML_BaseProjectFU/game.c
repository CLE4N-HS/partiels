#include "game.h"
#include "textureManager.h"
#include "soundManager.h"
#include "fontManager.h"
#include "viewManager.h"
#include "stateManager.h"
#include "Gamepad.h"
#include "pause.h"
#include "player.h"
#include "editor.h"

#include <Windows.h>

//sfSprite* sprite;
sfSprite* spBG1;
sfSprite* spBG2;



sfBool gamepadChangePos;
sfVector2f BG1Pos;
sfVector2f BG2Pos;



float timer;
int nbreJoueur;
int waveCount;

float waveTimer;
float soloTimer;
float defaultwaveTimer;
float defaultsoloTimer;
float bossTimer;

sfBool phaseWave;
sfBool phaseBoss;
sfBool phaseBossCompleted;
sfBool gameover;

void initGame(Window* _window)
{
	Texture_Onload(GAME);
	Sound_Onload(GAME);
	Font_Onload(GAME);

	spBG1 = sfSprite_create();
	spBG2 = sfSprite_create();
	
	sfSprite_setTexture(spBG1, GetTexture("BG1"), sfTrue);
	sfSprite_setTexture(spBG2, GetTexture("BG2"), sfTrue);

	sfSprite_setPosition(spBG2, vector2f(0.0f, -1080.0f));
	


	sfVector3f ambientLight = { 0.1f, 0.1f, 0.3f };
	
	BG1Pos = vector2f(0.0f, 0.0f);
	BG2Pos = vector2f(0.0f, -1080.0f);

	SetViewPosition(mainView, vector2f(960.f, 540.f));
	
	
	GamepadDetection();

	initPlayer();
	initMap();
	if (isEditor) initEditor();

	//Sleep(2000); // A enlever juste pour le test Thread

	w.state = sfTrue;

}

void updateGame(Window* _window)
{
	timer += getDeltaTime();
	
	for (int i = 0; i < /*8*/nbPlayer; i++)
	{
		if (Gamepad_isButtonPressed(i, OPTION) && timer > 0.2f)
		{
			togglePause();
			timer = 0.0f;
		}
	}

	if (isKeyboardOrControllerButtonPressed(sfKeyEscape, SELECT_XBOX) && timer > 0.2f)
	{
		togglePause();
		timer = 0.0f;
	}


	updateMap(_window);
	updatePlayer(_window);
	
}

void displayGame(Window* _window)
{

	sfRenderTexture_drawSprite(_window->renderTexture, spBG1, NULL);
	
	sfRenderTexture_drawSprite(_window->renderTexture, spBG2, NULL);

	displayMap(_window);
	displayPlayer(_window);
	displayObjects(_window);
}

void deinitGame()
{
	deinitPause();
	sfSprite_destroy(spBG1);
	sfSprite_destroy(spBG2);
	RemoveAllTextureButALL();
}