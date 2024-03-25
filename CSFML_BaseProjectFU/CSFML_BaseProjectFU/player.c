#include "player.h"
#include "textureManager.h"
#include "gamepadx.h"

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
	sfVector2f pos;

	float speed;
	float animTimer;
	sfBool isFlipped;
}Players;
Players p[2];

void initPlayer()
{
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

		p[i].anim = IDLE;
		p[i].lastAnim = NO_ANIM;
		p[i].rect = IntRect(0, 0, 32, 32);
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
		default:
			break;
		}
	}

	float xStickPos = getStickPos(0, sfTrue, sfTrue);
	float yStickPos = getStickPos(0, sfTrue, sfFalse);

	if ((xStickPos < -50.f && yStickPos > -50.f && yStickPos < 50.f ) || sfKeyboard_isKeyPressed(sfKeyQ)) {
		p[FROG].pos.x -= p[FROG].speed * dt;
		p[FROG].anim = RUN;
		p[FROG].isFlipped = sfTrue;
	}
	else if ((xStickPos > 50.f && yStickPos > -50.f && yStickPos < 50.f) || sfKeyboard_isKeyPressed(sfKeyD)) {
		p[FROG].pos.x += p[FROG].speed * dt;
		p[FROG].anim = RUN;
		p[FROG].isFlipped = sfFalse;
	}
	else {
		p[FROG].anim = IDLE;
	}

	if ((yStickPos < -50.f && xStickPos > -50.f && xStickPos < 50.f) || sfKeyboard_isKeyPressed(sfKeyS)) {
		p[ASTRONAUT].pos.y += p[ASTRONAUT].speed * dt;
		p[ASTRONAUT].anim = FALL;
	}
	else if ((yStickPos > 50.f && xStickPos > -50.f && xStickPos < 50.f) || sfKeyboard_isKeyPressed(sfKeyZ)) {
		p[ASTRONAUT].pos.y -= p[ASTRONAUT].speed * dt;
		p[ASTRONAUT].anim = JUMP;
	}
	else {
		p[ASTRONAUT].anim = IDLE;
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
	}
}
