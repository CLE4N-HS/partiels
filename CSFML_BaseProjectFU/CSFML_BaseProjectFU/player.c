#include "player.h"
#include "textureManager.h"
#include "gamepadx.h"
#include "map.h"
#include "CustomMath.h"

typedef enum {
	FROG,
	ASTRONAUT
}playerType;

typedef enum {
	NO_ANIM,
	IDLE,
	RUN,
	JUMP,
	FALL,
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

	float speed;
	float animTimer;
	sfBool isFlipped;
}Players;
Players p[2];

sfRectangleShape* pRectangle;

void initPlayer()
{
	pRectangle = sfRectangleShape_create();
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

		p[i].sprite = sfSprite_create();
		sfSprite_setOrigin(p[i].sprite, vector2f(16.f, 16.f));
	}
}

void updatePlayer(Window* _window)
{
	float dt = getDeltaTime();

	// movement
	float xStickPos = getStickPos(0, sfTrue, sfTrue);
	float yStickPos = getStickPos(0, sfTrue, sfFalse);

	if (((xStickPos < -50.f && yStickPos > -50.f && yStickPos < 50.f) || sfKeyboard_isKeyPressed(sfKeyQ) )&& !isCollision(p[FROG].bounds, sfTrue, sfTrue)) {
		p[FROG].velocity.x = -p[FROG].speed;
		p[FROG].anim = RUN;
		p[FROG].isFlipped = sfTrue;
	}
	else if (((xStickPos > 50.f && yStickPos > -50.f && yStickPos < 50.f) || sfKeyboard_isKeyPressed(sfKeyD)) && !isCollision(p[FROG].bounds, sfTrue, sfFalse)) {
		p[FROG].velocity.x = p[FROG].speed;
		p[FROG].anim = RUN;
		p[FROG].isFlipped = sfFalse;
	}
	else {
		p[FROG].anim = IDLE;
		if (!isGrounded(p[FROG].pos))
		{
			p[FROG].anim = FALL;
			p[FROG].velocity.y += GRAVITY * dt;
		}
		else p[FROG].velocity.y = 0.f;
		p[FROG].velocity.x = 0.f;
	}

	if (((yStickPos < -50.f && xStickPos > -50.f && xStickPos < 50.f) || sfKeyboard_isKeyPressed(sfKeyS)) && !isCollision(p[ASTRONAUT].bounds, sfFalse, sfFalse)) {
		p[ASTRONAUT].velocity.y = p[ASTRONAUT].speed;
		p[ASTRONAUT].anim = FALL;
	}
	else if (((yStickPos > 50.f && xStickPos > -50.f && xStickPos < 50.f) || sfKeyboard_isKeyPressed(sfKeyZ)) && !isCollision(p[ASTRONAUT].bounds, sfFalse, sfTrue)) {
		p[ASTRONAUT].velocity.y = -p[ASTRONAUT].speed;
		p[ASTRONAUT].anim = JUMP;
	}
	else {
		p[ASTRONAUT].anim = IDLE;
		p[ASTRONAUT].velocity = VECTOR2F_NULL;
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
			default:
				break;
			}

			if (p[i].isFlipped) {
				sfSprite_setScale(p[i].sprite, vector2f(-PLAYER_SCALE, PLAYER_SCALE));
			}
			else {
				sfSprite_setScale(p[i].sprite, vector2f(PLAYER_SCALE, PLAYER_SCALE));
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
		default:
			break;
		}



		p[i].pos = AddVectors(p[i].pos, MultiplyVector(p[i].velocity, dt));

	}
}

void displayPlayer(Window* _window)
{
	for (int i = 0; i < 2; i++)
	{
		//sfSprite_setOrigin(p[i].sprite, p[i].origin);
		//sfSprite_setTextureRect(p[i].sprite, p[i].rect);
		sfSprite_setPosition(p[i].sprite, p[i].pos);
		sfSprite_setTextureRect(p[i].sprite, p[i].rect);
		sfRenderTexture_drawSprite(_window->renderTexture, p[i].sprite, NULL);

		p[i].bounds = sfSprite_getGlobalBounds(p[i].sprite);
	}

	sfRectangleShape_setPosition(pRectangle, vector2f(p[FROG].bounds.left, p[FROG].bounds.top));
	sfRectangleShape_setSize(pRectangle, vector2f(p[FROG].bounds.width, p[FROG].bounds.height));
	sfRectangleShape_setFillColor(pRectangle, sfRed);
	//sfRenderTexture_drawRectangleShape(_window->renderTexture, pRectangle, NULL);

	sfRectangleShape_setPosition(pRectangle, vector2f(tmpRect.left, tmpRect.top));
	sfRectangleShape_setSize(pRectangle, vector2f(tmpRect.width, tmpRect.height));
	sfRectangleShape_setFillColor(pRectangle, sfBlue);
	sfRenderTexture_drawRectangleShape(_window->renderTexture, pRectangle, NULL);

}
