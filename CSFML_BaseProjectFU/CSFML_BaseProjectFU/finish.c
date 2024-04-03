#include "finish.h"
#include "viewManager.h"
#include "CustomMath.h"
#include "map.h"
#include "textureManager.h"

typedef struct {
	sfView* view;
	sfVector2f pos;
	sfVector2f size;
}Views;
Views v[2];

float finishTimer;
sfVector2f frogFinishScale;
sfVector2f astronautFinishScale;

int nbDisplay;

sfSprite* starSprite;
sfTexture* starTexture;

sfRectangleShape* opaqueRectangle;

sfShader* starShader;
sfRenderStates starRenderState;

sfBool isShaderOk;

sfGlslVec2 res = { 1920, 1080 };

float saveRotatePos;

sfColor opaqueColor;

float opaqueTimer;

void initFinsih()
{
	starSprite = sfSprite_create();
	starTexture = GetTexture("star");
	sfSprite_setTexture(starSprite, starTexture, sfTrue);
	sfSprite_setOrigin(starSprite, vector2f(200.f, 200.f));

	opaqueRectangle = sfRectangleShape_create();
	sfRectangleShape_setPosition(opaqueRectangle, VECTOR2F_NULL);
	sfRectangleShape_setSize(opaqueRectangle, vector2f(NB_BLOCKS_X * BLOCK_SIZE * BLOCK_SCALE, NB_BLOCKS_Y * BLOCK_SIZE * BLOCK_SCALE));
	opaqueColor = color(0, 0, 0, 0);
	opaqueTimer = 0.f;

	if (sfShader_isAvailable())
	{
		isShaderOk = sfTrue;

		starShader = sfShader_createFromFile(NULL, NULL, "../Ressources/Shaders/star.frag");

		starRenderState.shader = starShader;
		starRenderState.blendMode = sfBlendAlpha;
		starRenderState.texture = starTexture;
		starRenderState.transform = sfTransform_Identity;
 
	}
	else
	{
		isShaderOk = sfFalse;
	}


	for (int i = 0; i < 2; i++)
	{
		v[i].view = sfView_create();
		v[i].pos = VECTOR2F_NULL;
		v[i].size = vector2f(960.f, 1080.f);
	}
	
	sfView_setViewport(v[0].view, FlRect(0.f, 0.f, 0.5f, 1.f));
	sfView_setViewport(v[1].view, FlRect(0.5f, 0.f, 0.5f, 1.f));
	
	finishTimer = 0.f;
	frogFinishScale = vector2f(PLAYER_SCALE, PLAYER_SCALE);
	astronautFinishScale = vector2f(PLAYER_SCALE, PLAYER_SCALE);

	saveRotatePos = 0.f;

	nbDisplay = 0;
}

void updateFinish(Window* _window)
{
	float dt = getDeltaTime();

	finishTimer += dt;

	if (finishTimer >= FINISH_TIMER_DURATION) {
		finishTimer = FINISH_TIMER_DURATION;

		opaqueTimer += dt;

		if (opaqueTimer >= OPAQUE_TIMER_DURATION) {
			opaqueTimer = OPAQUE_TIMER_DURATION;
			setMapFinishedState(sfFalse);
			setEndState(END_SCREEN);
			changeState(_window, MENU);
		}

		opaqueColor = color_lerp(color(0, 0, 0, 0), color(0, 0, 0, 255), opaqueTimer / OPAQUE_TIMER_DURATION);
	}
	finishTimer = MIN(finishTimer, FINISH_TIMER_DURATION);

	setPlayerPos(FROG, LerpVector(getPlayerPos(FROG), getFinishPlayerPos(0), finishTimer / FINISH_TIMER_DURATION));
	setPlayerPos(ASTRONAUT, LerpVector(getPlayerPos(ASTRONAUT), getFinishPlayerPos(1), finishTimer / FINISH_TIMER_DURATION));

	frogFinishScale = LerpVector(vector2f(PLAYER_SCALE, PLAYER_SCALE), VECTOR2F_NULL, finishTimer / FINISH_TIMER_DURATION);
	astronautFinishScale = LerpVector(vector2f(PLAYER_SCALE, PLAYER_SCALE), VECTOR2F_NULL, finishTimer / FINISH_TIMER_DURATION);

	v[0].size = LerpVector(vector2f(960.f, 1080.f), vector2f(16.f * BLOCK_SCALE * BLOCK_SIZE / 4.f / 2.f, 9.f * BLOCK_SCALE * BLOCK_SIZE / 4.f), finishTimer / FINISH_TIMER_DURATION);
	v[1].size = LerpVector(vector2f(960.f, 1080.f), vector2f(16.f * BLOCK_SCALE * BLOCK_SIZE / 4.f / 2.f, 9.f * BLOCK_SCALE * BLOCK_SIZE / 4.f), finishTimer / FINISH_TIMER_DURATION);


	nbDisplay = 0;
}

void displayFinish(Window* _window)
{
	if (nbDisplay < 0 || nbDisplay > 1)
		return;

	if (isShaderOk) {
		float fTimer = finishTimer / FINISH_TIMER_DURATION;
		for (int i = 0; i < 2; i++)
		{
			if (fTimer < 0.5f) sfSprite_setScale(starSprite, vector2f(Lerp(0.f, 1.f, fTimer * 2.f), Lerp(0.f, 1.f, fTimer * 2.f)));
			else sfSprite_setScale(starSprite, vector2f(Lerp(1.f, 0.f, (fTimer - 0.5f) * 2), Lerp(1.f, 0.f, (fTimer - 0.5f) * 2.f)));
			sfSprite_setRotation(starSprite, 0.f + finishTimer * finishTimer  * finishTimer * 10.f);
			sfSprite_setPosition(starSprite, getFinishPlayerPos(i));
			sfShader_setTextureUniform(starShader, "u_texture", starTexture);
			sfShader_setFloatUniform(starShader, "u_timer", fTimer);
			sfRenderTexture_drawSprite(_window->renderTexture, starSprite, &starRenderState);
		}
	}

	if (finishTimer >= FINISH_TIMER_DURATION) {
		sfRectangleShape_setFillColor(opaqueRectangle, opaqueColor);
		sfRenderTexture_drawRectangleShape(_window->renderTexture, opaqueRectangle, NULL);
	}

	sfView_setCenter(v[nbDisplay].view, v[nbDisplay].pos);
	sfView_setSize(v[nbDisplay].view, v[nbDisplay].size);
	sfRenderTexture_setView(_window->renderTexture, v[nbDisplay].view);

	nbDisplay++;
	if (nbDisplay > 1)
		nbDisplay = 0;
}

void setFinishViewPos(int _nb, sfVector2f _pos)
{
	v[_nb].pos = _pos;
}

sfVector2f getFinishPlayerScale(playerType _type)
{
	if (_type == FROG) return frogFinishScale;
	if (_type == ASTRONAUT) return astronautFinishScale;
	return vector2f(PLAYER_SCALE, PLAYER_SCALE);
}
