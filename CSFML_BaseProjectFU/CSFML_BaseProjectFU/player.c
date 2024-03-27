#include "player.h"
#include "textureManager.h"
#include "gamepadx.h"
#include "map.h"
#include "CustomMath.h"
#include "viewManager.h"

typedef enum {
	NO_ANIM,
	IDLE,
	RUN,
	JUMP,
	FALL,
	THROW
}playerAnim;

typedef struct {
	playerType type;
	sfSprite* sprite;
	playerAnim anim;
	playerAnim lastAnim;

	sfIntRect rect;
	sfFloatRect bounds;
	sfVector2f pos;
	sfVector2f velocity;
	sfVector2f forward;

	float speed;
	float animTimer;
	sfBool isFlipped;

	float lauchingTimer;
}Players;
Players p[2];

sfRectangleShape* pRectangle;

playerType viewFocus;
playerType lastViewFocus;
float viewTimer;
sfVector2f lastViewPos;

void initPlayer()
{
	pRectangle = sfRectangleShape_create();

	viewFocus = FROG;
	lastViewFocus = FROG;
	viewTimer = LERP_VIEW_TIMER;
	lastViewPos = VECTOR2F_NULL;


	for (int i = 0; i < 2; i++)
	{
		switch (i)
		{
		case 0:
			p[i].type = FROG;
			p[i].pos = vector2f(300.f, 520.f);
			break;
		case 1:
			p[i].type = ASTRONAUT;
			p[i].pos = vector2f(492.f, 520.f);
			break;
		default:
			break;
		}

		p[i].velocity = VECTOR2F_NULL;
		p[i].anim = IDLE;
		p[i].lastAnim = NO_ANIM;
		p[i].rect = IntRect(0, 0, 32, 32);
		p[i].bounds = FlRect(0.f, 0.f, 0.f, 0.f);
		p[i].speed = 500.f;
		p[i].animTimer = 0.f;
		p[i].isFlipped = sfFalse;
		p[i].forward = VECTOR2F_NULL;
		p[i].lauchingTimer = LAUNCHING_TIMER_DURATION;

		p[i].sprite = sfSprite_create();
		sfSprite_setOrigin(p[i].sprite, vector2f(16.f, 16.f));
	}
}

void updatePlayer(Window* _window)
{
	float dt = getDeltaTime();

	// to remove
	if (sfKeyboard_isKeyPressed(sfKeyA)) p[FROG].pos = vector2f(300.f, 520.f);
	if (sfKeyboard_isKeyPressed(sfKeyE)) p[ASTRONAUT].pos = p[FROG].pos;

	// movement
	float xStickPos = getStickPos(0, sfTrue, sfTrue);
	float yStickPos = getStickPos(0, sfTrue, sfFalse);

	if (isSomeoneInSlingshot())
	{
		playerType _type = getWhoIsInSlingshot();

		p[_type].forward = Normalize(vector2f(xStickPos, -yStickPos));
		p[_type].velocity = MultiplyVector(p[_type].forward, 100.f);

		updateSlingshot(_window);

		if (p[_type].pos.x > getSlingshotBasePos().x) p[_type].isFlipped = sfTrue;
		else p[_type].isFlipped = sfFalse;

	}
	else if (viewTimer >= LERP_VIEW_TIMER && p[viewFocus].lauchingTimer >= LAUNCHING_TIMER_DURATION)
	{
		if (((xStickPos < -50.f && yStickPos > -50.f && yStickPos < 50.f) || sfKeyboard_isKeyPressed(sfKeyQ)) && !isCollision2(p[FROG].bounds, sfTrue, sfTrue)) {
			p[FROG].velocity.x = -p[FROG].speed;
			p[FROG].anim = RUN;
			p[FROG].isFlipped = sfTrue;
			viewFocus = FROG;
		}
		else if (((xStickPos > 50.f && yStickPos > -50.f && yStickPos < 50.f) || sfKeyboard_isKeyPressed(sfKeyD)) && !isCollision2(p[FROG].bounds, sfTrue, sfFalse)) {
			p[FROG].velocity.x = p[FROG].speed;
			p[FROG].anim = RUN;
			p[FROG].isFlipped = sfFalse;
			viewFocus = FROG;
		}
		else {
			p[FROG].anim = IDLE;
			p[FROG].velocity.x = 0.f;
		}

		if (((yStickPos < -50.f && xStickPos > -50.f && xStickPos < 50.f) || sfKeyboard_isKeyPressed(sfKeyS)) && !isCollision2(p[ASTRONAUT].bounds, sfFalse, sfFalse)) {
			p[ASTRONAUT].velocity.y = p[ASTRONAUT].speed;
			p[ASTRONAUT].anim = FALL;
			viewFocus = ASTRONAUT;
		}
		else if (((yStickPos > 50.f && xStickPos > -50.f && xStickPos < 50.f) || sfKeyboard_isKeyPressed(sfKeyZ)) && !isCollision2(p[ASTRONAUT].bounds, sfFalse, sfTrue)) {
			p[ASTRONAUT].velocity.y = -p[ASTRONAUT].speed;
			p[ASTRONAUT].anim = JUMP;
			viewFocus = ASTRONAUT;
		}
		else {
			p[ASTRONAUT].anim = IDLE;
			p[ASTRONAUT].velocity = VECTOR2F_NULL;
		}
	}
	else {
		p[FROG].anim = IDLE;
		if (p[viewFocus].lauchingTimer >= LAUNCHING_TIMER_DURATION)
			p[FROG].velocity.x = 0.f;

		p[ASTRONAUT].anim = IDLE;
		if (p[viewFocus].lauchingTimer >= LAUNCHING_TIMER_DURATION)
			p[ASTRONAUT].velocity = VECTOR2F_NULL;
	}

	if (p[viewFocus].lauchingTimer >= LAUNCHING_TIMER_DURATION)
	{
		if (!isGrounded(p[FROG].pos))
		{
			p[FROG].anim = FALL;
			p[FROG].velocity.y += GRAVITY * dt;
		}
		else p[FROG].velocity.y = 0.f;
	}
	else
	{
		p[viewFocus].lauchingTimer += dt;
		p[viewFocus].velocity = MultiplyVector(p[viewFocus].velocity, 1.f - SLINGSHOT_DRAG * dt);
		p[viewFocus].velocity = AddVectors(p[viewFocus].velocity, MultiplyVector(vector2f(0.f, SLINGSHOT_GRAVITY), dt));
		p[viewFocus].anim = THROW;
	}

	for (int i = 0; i < 2; i++)
	{
		if (p[i].anim != p[i].lastAnim) // change animState
		{
			switch (p[i].anim)
			{
			case IDLE:
				if (i == FROG) sfSprite_setTexture(p[i].sprite, GetTexture("frogIdle"), sfFalse);
				else sfSprite_setTexture(p[i].sprite, GetTexture("astronautIdle"), sfFalse);
				break;
			case RUN:
				sfSprite_setTexture(p[i].sprite, GetTexture("frogRun"), sfFalse);
				break;
			case JUMP:
				if (i == FROG) sfSprite_setTexture(p[i].sprite, GetTexture("frogJump"), sfTrue);
				else sfSprite_setTexture(p[i].sprite, GetTexture("astronautJump"), sfTrue);
				break;
			case FALL:
				if (i == FROG) sfSprite_setTexture(p[i].sprite, GetTexture("frogFall"), sfTrue);
				else sfSprite_setTexture(p[i].sprite, GetTexture("astronautFall"), sfTrue);
				break;
			case THROW:
				if (i == FROG) sfSprite_setTexture(p[i].sprite, GetTexture("frogThrow"), sfFalse);
				else sfSprite_setTexture(p[i].sprite, GetTexture("astronautThrow"), sfFalse);
				break;
			default:
				break;
			}




			p[i].rect.left = 0;

			p[i].lastAnim = p[i].anim;
		}

		p[i].animTimer += dt;

		switch (p[i].anim)
		{
		case IDLE:
			Animator(&p[i].rect, &p[i].animTimer, 11, 1, 0.1f, 0.f);
			break;
		case RUN:
			Animator(&p[i].rect, &p[i].animTimer, 12, 1, 0.075f, 0.f);
			break;
		case THROW:
			Animator(&p[i].rect, &p[i].animTimer, 6, 1, 0.04f, 0.f);
			break;
		default:
			break;
		}



		p[i].pos = AddVectors(p[i].pos, MultiplyVector(p[i].velocity, dt));

	}

	if (lastViewFocus != viewFocus && viewTimer >= LERP_VIEW_TIMER)
	{
		viewTimer = 0.f;
	}
	else if (viewTimer < LERP_VIEW_TIMER)
	{
		sfVector2f lastViewPos = getPlayerPosInBounds(lastViewFocus);
		sfVector2f viewPos = getPlayerPosInBounds(viewFocus);
		if (lastViewPos.x >= viewPos.x - EPSILON && lastViewPos.x <= viewPos.x + EPSILON && lastViewPos.y >= viewPos.y - EPSILON && lastViewPos.y <= viewPos.y + EPSILON)
			viewTimer = LERP_VIEW_TIMER;
		else
			SetViewPosition(mainView, getLerpView(lastViewPos, viewPos));

		viewTimer += dt;
		if (viewTimer >= LERP_VIEW_TIMER)
			lastViewFocus = viewFocus;
	}
	else
	{
		SetViewPosition(mainView, getPlayerPosInBounds(viewFocus));
	}

}

void displayPlayer(Window* _window)
{
	for (int i = 1; i >= 0; i--)
	{
		if (p[i].isFlipped) sfSprite_setScale(p[i].sprite, vector2f(-PLAYER_SCALE, PLAYER_SCALE));
		else sfSprite_setScale(p[i].sprite, vector2f(PLAYER_SCALE, PLAYER_SCALE));
		sfSprite_setPosition(p[i].sprite, p[i].pos);
		sfSprite_setTextureRect(p[i].sprite, p[i].rect);
		sfRenderTexture_drawSprite(_window->renderTexture, p[i].sprite, NULL);

		p[i].bounds = sfSprite_getGlobalBounds(p[i].sprite);
	}

	//sfRectangleShape_setPosition(pRectangle, vector2f(p[viewFocus].bounds.left, p[viewFocus].bounds.top));
	//sfRectangleShape_setSize(pRectangle, vector2f(p[viewFocus].bounds.width, p[viewFocus].bounds.height));
	//sfRectangleShape_setFillColor(pRectangle, color(255, 0, 0, 0));
	//sfRenderTexture_drawRectangleShape(_window->renderTexture, pRectangle, NULL);
	//
	//sfRectangleShape_setPosition(pRectangle, vector2f(tmpPlayerRect.left, tmpPlayerRect.top));
	//sfRectangleShape_setSize(pRectangle, vector2f(tmpPlayerRect.width, tmpPlayerRect.height));
	//sfRectangleShape_setFillColor(pRectangle, color(255, 0, 0, 51));
	//sfRenderTexture_drawRectangleShape(_window->renderTexture, pRectangle, NULL);
	//
	//sfRectangleShape_setPosition(pRectangle, vector2f(tmpPlayerRect2.left, tmpPlayerRect2.top));
	//sfRectangleShape_setSize(pRectangle, vector2f(tmpPlayerRect2.width, tmpPlayerRect2.height));
	//sfRectangleShape_setFillColor(pRectangle, color(255, 0, 0, 51));
	//sfRenderTexture_drawRectangleShape(_window->renderTexture, pRectangle, NULL);
	//
	//sfRectangleShape_setPosition(pRectangle, vector2f(tmpRect.left, tmpRect.top));
	//sfRectangleShape_setSize(pRectangle, vector2f(tmpRect.width, tmpRect.height));
	//sfRectangleShape_setFillColor(pRectangle, color(0, 0, 255, 51));
	//sfRenderTexture_drawRectangleShape(_window->renderTexture, pRectangle, NULL);
	//
	//sfRectangleShape_setPosition(pRectangle, vector2f(tmpRect2.left, tmpRect2.top));
	//sfRectangleShape_setSize(pRectangle, vector2f(tmpRect2.width, tmpRect2.height));
	//sfRectangleShape_setFillColor(pRectangle, color(0, 255, 255, 51));
	//sfRenderTexture_drawRectangleShape(_window->renderTexture, pRectangle, NULL);
}

sfVector2f getPlayerPosInBounds(playerType _type)
{
	sfVector2f pos = p[_type].pos;

	if (pos.x > NB_BLOCKS_X * BLOCK_SCALE * BLOCK_SIZE - 960.f) pos.x = NB_BLOCKS_X * BLOCK_SCALE * BLOCK_SIZE - 960.f;
	if (pos.x < 960.f) pos.x = 960.f;
	if (pos.y > NB_BLOCKS_Y * BLOCK_SCALE * BLOCK_SIZE - 540.f) pos.y = NB_BLOCKS_Y * BLOCK_SCALE * BLOCK_SIZE - 540.f;
	if (pos.y < 540.f) pos.y = 540.f;

	return pos;
}

sfVector2f getLerpView(sfVector2f _lastViewPos, sfVector2f _viewPos)
{
	return LerpVector(_lastViewPos, _viewPos, viewTimer);
}

playerType getViewFocus()
{
	return viewFocus;
}

sfFloatRect getPlayerRect(playerType _type)
{
	return p[_type].bounds;
}

sfVector2f getPlayerPos(playerType _type)
{
	return p[_type].pos;
}

void setPlayerPos(playerType _type, sfVector2f _pos)
{
	p[_type].pos = _pos;
}

sfVector2f* pGetPlayerPos(playerType _type)
{
	return &p[_type].pos;
}

sfVector2f* pGetPlayerVelocity(playerType _type)
{
	return &p[_type].velocity;
}

void setPlayerLauchingTimer(playerType _type, float _launchingTimer)
{
	p[_type].lauchingTimer = _launchingTimer;
}

float getPlayerLauchingTimer(playerType _type)
{
	return p[_type].lauchingTimer;
}
