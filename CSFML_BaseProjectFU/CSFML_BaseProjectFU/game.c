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
#include "finish.h"
#include "hud.h"
#include "gamepadx.h"

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
	initFinsih();
	initHud();
	if (isEditor) initEditor();

	//Sleep(2000); // A enlever juste pour le test Thread

	w.state = sfTrue;

}

void updateGame(Window* _window)
{
	timer += getDeltaTime();

	if (/*isKeyboardOrControllerButtonPressed(sfKeyEscape, SELECT_XBOX) */(isButtonPressed(0, START) || isButtonPressed(0, BACK)) && timer > 0.2f && !isMapFinished())
	{
		togglePause();
		timer = 0.0f;
	}

	updatePlayer(_window);

	if (isMapFinished())
	{
		updateFinish(_window);
	}
	else
	{
		updateMap(_window);
	}

	
}

void displayGame(Window* _window)
{

	sfRenderTexture_drawSprite(_window->renderTexture, spBG1, NULL);
	
	sfRenderTexture_drawSprite(_window->renderTexture, spBG2, NULL);

	displayMap(_window);
	displayPlayer(_window);
	displayObjects(_window);

	if (isMapFinished())
		displayFinish(_window);
	else
		displayHud(_window);
}

void deinitGame()
{
	deinitPause();
	sfSprite_destroy(spBG1);
	sfSprite_destroy(spBG2);
	RemoveAllTextureButALL();
}