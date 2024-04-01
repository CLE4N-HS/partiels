#include "editor.h"
//#include "math.h"
#include "fontManager.h"
#include "pause.h"
#include "gamepadx.h"
#include "viewManager.h"
#include "map.h"

sfSprite* tileCursor;
sfSprite* hudEditor;
sfRectangleShape* rectCursor;

sfSprite* otherSprite;
sfTexture* slingshotTexture;

int currentTile;
sfIntRect tileCursorRect;
sfBool canPlaceTile;

sfText* txtVisible;
sfText* txtCollision;
sfText* txtNbMap;

sfBool isVisible;
sfBool isCollisionEditor;
sfBool isEditorHUD;

void initEditor()
{
	otherSprite = sfSprite_create();
	slingshotTexture = GetTexture("slingshot");
	
	//initParallax();

	//initMap();
	/// TILE CURSOR
	tileCursor = sfSprite_create();
	sfSprite_setTexture(tileCursor, GetTexture("castleTiles"), sfFalse);
	currentTile = 2;
	tileCursorRect = IntRect(0 * BLOCK_SIZE, 1 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE);

	hudEditor = sfSprite_create();
	sfSprite_setTexture(hudEditor, GetTexture("castleTiles"), sfFalse);
	sfSprite_setPosition(hudEditor, vector2f(0.f, 0.f));
	sfSprite_setScale(hudEditor, vector2f(2.f, 2.f));

	rectCursor = sfRectangleShape_create();
	sfRectangleShape_setSize(rectCursor, vector2f(1920.f, 1080.f));
	sfRectangleShape_setFillColor(rectCursor, color(0.f, 0.f, 0.f, 200.f));

	//initView(); 

	txtVisible = sfText_create();
	sfText_setFont(txtVisible, GetFont("riangriung"));
	sfText_setString(txtVisible, "N : Visible : YES");
	sfText_setPosition(txtVisible, vector2f(20.f, 920.f));

	txtCollision = sfText_create();
	sfText_setFont(txtCollision, GetFont("riangriung"));
	sfText_setString(txtCollision, "B : Collision : YES");
	sfText_setPosition(txtCollision, vector2f(20.f, 950.f));

	txtNbMap = sfText_create();
	sfText_setFont(txtNbMap, GetFont("GingerSoda"));
	sfText_setString(txtNbMap, "Nb Map : 1 (M to save)");
	sfText_setPosition(txtNbMap, vector2f(20.f, 980.f));
	
	isVisible = sfTrue;
	isCollisionEditor = sfTrue;
	isEditorHUD = sfFalse;
	canPlaceTile = sfFalse;
}

void updateEditor(Window* _window)
{
	//updateView(_window);
	//updateParallax();

	static float timer = 0.f;
	timer += getDeltaTime();
	float dt = getDeltaTime();
	static float saveTimer = 0.f;
	saveTimer += dt;
	
	if (!isEditorHUD)
	{
		/// CHANGE TILE
		if (sfKeyboard_isKeyPressed(sfKeyLeft) && timer > 0.1f)
		{
			currentTile--;
			if (currentTile < 0)
				currentTile = NB_MAX_TILES;
			timer = 0.f;
		}
		else if (sfKeyboard_isKeyPressed(sfKeyRight) && timer > 0.1f)
		{
			currentTile++;
			if (currentTile > NB_MAX_TILES)
				currentTile = 0;
			timer = 0.f;
		}
		else if (sfKeyboard_isKeyPressed(sfKeyUp) && timer > 0.1f)
		{
			currentTile -= 8;
			if (currentTile < 0)
				currentTile = NB_MAX_TILES;
			timer = 0.f;
		}
		else if (sfKeyboard_isKeyPressed(sfKeyDown) && timer > 0.1f)
		{
			currentTile += 8;
			if (currentTile > NB_MAX_TILES)
				currentTile = 0;
			timer = 0.f;
		}

		if (sfKeyboard_isKeyPressed(sfKeySpace) && timer > 0.1f)
		{
			isEditorHUD = !isEditorHUD;
			timer = 0.f;
		}
	
		/// SET TILE CURSOR RECT
		switch (currentTile)
		{
			case T_NOTILE: break;
			case T_TLLIGHTWALL: tileCursorRect = IntRect(0 * BLOCK_SIZE, 0 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); break;
			case T_TLIGHTWALL: tileCursorRect = IntRect(1 * BLOCK_SIZE, 0 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); break;
			case T_TRLIGHTWALL: tileCursorRect = IntRect(2 * BLOCK_SIZE, 0 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); break;
			case T_LIGHTWALL1: tileCursorRect = IntRect(3 * BLOCK_SIZE, 0 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); break;
			case T_BRICKWALL1: tileCursorRect = IntRect(4 * BLOCK_SIZE, 0 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); break;
			case T_SIGN: tileCursorRect = IntRect(5 * BLOCK_SIZE, 0 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); break;
			case T_LEVER: tileCursorRect = IntRect(6 * BLOCK_SIZE, 0 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); break;
			case T_BASETORCH: tileCursorRect = IntRect(7 * BLOCK_SIZE, 0 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); break;

			case T_LLIGHTWALL: tileCursorRect = IntRect(0 * BLOCK_SIZE, 1 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); break;
			case T_VOID: tileCursorRect = IntRect(1 * BLOCK_SIZE, 1 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); break;
			case T_RLIGHTWALL: tileCursorRect = IntRect(2 * BLOCK_SIZE, 1 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); break;
			case T_LIGHTWALL2: tileCursorRect = IntRect(3 * BLOCK_SIZE, 1 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); break;
			case T_BRICKWALL2: tileCursorRect = IntRect(4 * BLOCK_SIZE, 1 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); break;
			case T_SWORD: tileCursorRect = IntRect(5 * BLOCK_SIZE, 1 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); break;
			case T_LIGHTSPIKES: tileCursorRect = IntRect(6 * BLOCK_SIZE, 1 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); break;
			case T_SKULLS: tileCursorRect = IntRect(7 * BLOCK_SIZE, 1 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); break;

			case T_BLLIGHTWALL: tileCursorRect = IntRect(0 * BLOCK_SIZE, 2 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); break;
			case T_BLIGHTWALL: tileCursorRect = IntRect(1 * BLOCK_SIZE, 2 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); break;
			case T_BRLIGHTWALL: tileCursorRect = IntRect(2 * BLOCK_SIZE, 2 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); break;
			case T_DARKWALL1: tileCursorRect = IntRect(3 * BLOCK_SIZE, 2 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); break;
			case T_SIDEBARRILS: tileCursorRect = IntRect(4 * BLOCK_SIZE, 2 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); break;
			case T_LITTLEBARRIL: tileCursorRect = IntRect(5 * BLOCK_SIZE, 2 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); break;
			case T_LEFTCHAIN: tileCursorRect = IntRect(6 * BLOCK_SIZE, 2 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); break;
			case T_RIGHTCHAIN: tileCursorRect = IntRect(7 * BLOCK_SIZE, 2 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); break;

			case T_TLDARKWALL: tileCursorRect = IntRect(0 * BLOCK_SIZE, 3 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); break;
			case T_TDARKWALL: tileCursorRect = IntRect(1 * BLOCK_SIZE, 3 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); break;
			case T_TRDARKWALL: tileCursorRect = IntRect(2 * BLOCK_SIZE, 3 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); break;
			case T_DARKWALL2: tileCursorRect = IntRect(3 * BLOCK_SIZE, 3 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); break;
			case T_BIGBARRIL: tileCursorRect = IntRect(4 * BLOCK_SIZE, 3 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); break;
			case T_POT: tileCursorRect = IntRect(5 * BLOCK_SIZE, 3 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); break;
			case T_BOTHCHAIN: tileCursorRect = IntRect(6 * BLOCK_SIZE, 3 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); break;
			case T_DARKSPIKES: tileCursorRect = IntRect(7 * BLOCK_SIZE, 3 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); break;

			case T_LDARKWALL: tileCursorRect = IntRect(0 * BLOCK_SIZE, 4 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); break;
			case T_EMPTY: tileCursorRect = IntRect(1 * BLOCK_SIZE, 4 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); break;
			case T_RDARKWALL: tileCursorRect = IntRect(2 * BLOCK_SIZE, 4 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); break;
			case T_LEFTPLATFORM: tileCursorRect = IntRect(3 * BLOCK_SIZE, 4 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); break;
			case T_PLATFORM: tileCursorRect = IntRect(4 * BLOCK_SIZE, 4 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); break;
			case T_RIGHTPLATFORM: tileCursorRect = IntRect(5 * BLOCK_SIZE, 4 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); break;
			case T_LEFTWEB: tileCursorRect = IntRect(6 * BLOCK_SIZE, 4 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); break;
			case T_RIGHTWEB: tileCursorRect = IntRect(7 * BLOCK_SIZE, 4 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); break;

			case T_BLDARKWALL: tileCursorRect = IntRect(0 * BLOCK_SIZE, 5 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); break;
			case T_BDARKWALL: tileCursorRect = IntRect(1 * BLOCK_SIZE, 5 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); break;
			case T_BRDARKWALL: tileCursorRect = IntRect(2 * BLOCK_SIZE, 5 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); break;
			case T_TOPWALL: tileCursorRect = IntRect(3 * BLOCK_SIZE, 5 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); break;
			case T_MIDWALL: tileCursorRect = IntRect(4 * BLOCK_SIZE, 5 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); break;
			case T_LITTLEWALL: tileCursorRect = IntRect(5 * BLOCK_SIZE, 5 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); break;
			case T_BIGWALL: tileCursorRect = IntRect(6 * BLOCK_SIZE, 5 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); break;
			case T_LITTLESKULL: tileCursorRect = IntRect(7 * BLOCK_SIZE, 5 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); break;

			case T_LITTLEBOX:  tileCursorRect = IntRect(0 * BLOCK_SIZE, 6 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); break;
			case T_BIGBOX:  tileCursorRect = IntRect(1 * BLOCK_SIZE, 6 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); break;
			case T_TORCH:  tileCursorRect = IntRect(2 * BLOCK_SIZE, 6 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); break;
			case T_BOTTOMWALL: tileCursorRect = IntRect(3 * BLOCK_SIZE, 6 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); break;
			case T_SLINGSHOT: tileCursorRect = IntRect(0, 0, 66, 209); break;
			case T_LLEFTMOVING: tileCursorRect = IntRect(0, 0, 32, 32); break;
			case T_LMOVING: tileCursorRect = IntRect(0, 32, 32, 32); break;
			case T_LRIGHTMOVING: tileCursorRect = IntRect(0, 64, 32, 32); break;

			case T_RLEFTMOVING: tileCursorRect = IntRect(0, 0, 32, 32); break;
			case T_RMOVING: tileCursorRect = IntRect(0, 32, 32, 32); break;
			case T_RRIGHTMOVING: tileCursorRect = IntRect(0, 64, 32, 32); break;
			case T_MUSICBLOC: tileCursorRect = IntRect(0, 0, 32, 32); break;
			case T_DOOR: tileCursorRect = IntRect(0, 0, 32, 32); break;
			case T_FROGSPAWN: tileCursorRect = IntRect(0, 0, 32, 32); break;
			case T_ASTRONAUTSPAWN: tileCursorRect = IntRect(32, 0, 32, 32); break;

			case T_GLOCK: tileCursorRect = IntRect(0, 0, 32, 32); break;
			case T_BLOCK: tileCursorRect = IntRect(32, 0, 32, 32); break;
			case T_RLOCK: tileCursorRect = IntRect(64, 0, 32, 32); break;
			case T_YLOCK: tileCursorRect = IntRect(96, 0, 32, 32); break;

			case T_GKEY: tileCursorRect = IntRect(0, 32, 32, 32); break;
			case T_BKEY: tileCursorRect = IntRect(32, 32, 32, 32); break;
			case T_RKEY: tileCursorRect = IntRect(64, 32, 32, 32); break;
			case T_YKEY: tileCursorRect = IntRect(96, 32, 32, 32); break;

			case T_GEXBLOCK: tileCursorRect = IntRect(0, 64, 32, 32); break;
			case T_BEXBLOCK: tileCursorRect = IntRect(32, 64, 32, 32); break;
			case T_REXBLOCK: tileCursorRect = IntRect(64, 64, 32, 32); break;
			case T_YEXBLOCK: tileCursorRect = IntRect(96, 64, 32, 32); break;

			case T_GBUTTON: tileCursorRect = IntRect(0, 96, 32, 32); break;
			case T_BBUTTON: tileCursorRect = IntRect(32, 96, 32, 32); break;
			case T_RBUTTON: tileCursorRect = IntRect(64, 96, 32, 32); break;
			case T_YBUTTON: tileCursorRect = IntRect(96, 96, 32, 32); break;

			default: break;
		}

		if (sfMouse_isButtonPressed(sfMouseRight))
		{
			currentTile = T_NOTILE;
		}

		if (sfMouse_isButtonPressed(sfMouseLeft))
		{
			int y = getfWorldMousePos(_window->renderWindow).y / BLOCK_SIZE / BLOCK_SCALE;
			int x = getfWorldMousePos(_window->renderWindow).x / BLOCK_SIZE / BLOCK_SCALE;
			if (x < 0 || y < 0 || y >= NB_BLOCKS_Y ) {
				printf("DEPASSEMENT DE TABLEAU ");
				//ToggleFullscreen(_window);
				return;
			}
			b[y][x].type = currentTile;
			b[y][x].isSolid = sfFalse;
			b[y][x].timer = 0.f;
			int j = y;
			int i = x;

			//for (int j = 0; j < NB_BLOCKS_Y; j++)
			//{
			//	for (int i = 0; i < NB_BLOCKS_X; i++)
			//	{
					switch (b[j][i].type)
					{
					case T_NOTILE: break;
					case T_TLLIGHTWALL: b[j][i].rect = IntRect(0 * BLOCK_SIZE, 0 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); b[j][i].isSolid = sfTrue; break;
					case T_TLIGHTWALL: b[j][i].rect = IntRect(1 * BLOCK_SIZE, 0 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); b[j][i].isSolid = sfTrue; break;
					case T_TRLIGHTWALL: b[j][i].rect = IntRect(2 * BLOCK_SIZE, 0 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); b[j][i].isSolid = sfTrue; break;
					case T_LIGHTWALL1: b[j][i].rect = IntRect(3 * BLOCK_SIZE, 0 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); b[j][i].isSolid = sfTrue; break;
					case T_BRICKWALL1: b[j][i].rect = IntRect(4 * BLOCK_SIZE, 0 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); b[j][i].isSolid = sfTrue; break;
					case T_SIGN: b[j][i].rect = IntRect(5 * BLOCK_SIZE, 0 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); break;
					case T_LEVER: b[j][i].rect = IntRect(6 * BLOCK_SIZE, 0 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); break;
					case T_BASETORCH: b[j][i].rect = IntRect(7 * BLOCK_SIZE, 0 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); break;

					case T_LLIGHTWALL: b[j][i].rect = IntRect(0 * BLOCK_SIZE, 1 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); b[j][i].isSolid = sfTrue; break;
					case T_VOID: b[j][i].rect = IntRect(1 * BLOCK_SIZE, 1 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); break;
					case T_RLIGHTWALL: b[j][i].rect = IntRect(2 * BLOCK_SIZE, 1 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); b[j][i].isSolid = sfTrue;  break;
					case T_LIGHTWALL2: b[j][i].rect = IntRect(3 * BLOCK_SIZE, 1 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); b[j][i].isSolid = sfTrue; break;
					case T_BRICKWALL2: b[j][i].rect = IntRect(4 * BLOCK_SIZE, 1 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); b[j][i].isSolid = sfTrue; break;
					case T_SWORD: b[j][i].rect = IntRect(5 * BLOCK_SIZE, 1 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); break;
					case T_LIGHTSPIKES: b[j][i].rect = IntRect(6 * BLOCK_SIZE, 1 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); break;
					case T_SKULLS: b[j][i].rect = IntRect(7 * BLOCK_SIZE, 1 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); break;

					case T_BLLIGHTWALL: b[j][i].rect = IntRect(0 * BLOCK_SIZE, 2 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); b[j][i].isSolid = sfTrue; break;
					case T_BLIGHTWALL: b[j][i].rect = IntRect(1 * BLOCK_SIZE, 2 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); b[j][i].isSolid = sfTrue; break;
					case T_BRLIGHTWALL: b[j][i].rect = IntRect(2 * BLOCK_SIZE, 2 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE);b[j][i].isSolid = sfTrue;  break;
					case T_DARKWALL1: b[j][i].rect = IntRect(3 * BLOCK_SIZE, 2 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); b[j][i].isSolid = sfTrue; break;
					case T_SIDEBARRILS: b[j][i].rect = IntRect(4 * BLOCK_SIZE, 2 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); break;
					case T_LITTLEBARRIL: b[j][i].rect = IntRect(5 * BLOCK_SIZE, 2 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); break;
					case T_LEFTCHAIN: b[j][i].rect = IntRect(6 * BLOCK_SIZE, 2 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); break;
					case T_RIGHTCHAIN: b[j][i].rect = IntRect(7 * BLOCK_SIZE, 2 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); break;

					case T_TLDARKWALL: b[j][i].rect = IntRect(0 * BLOCK_SIZE, 3 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); b[j][i].isSolid = sfTrue; break;
					case T_TDARKWALL: b[j][i].rect = IntRect(1 * BLOCK_SIZE, 3 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); b[j][i].isSolid = sfTrue; break;
					case T_TRDARKWALL: b[j][i].rect = IntRect(2 * BLOCK_SIZE, 3 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); b[j][i].isSolid = sfTrue; break;
					case T_DARKWALL2: b[j][i].rect = IntRect(3 * BLOCK_SIZE, 3 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); b[j][i].isSolid = sfTrue; break;
					case T_BIGBARRIL: b[j][i].rect = IntRect(4 * BLOCK_SIZE, 3 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); break;
					case T_POT: b[j][i].rect = IntRect(5 * BLOCK_SIZE, 3 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); break;
					case T_BOTHCHAIN: b[j][i].rect = IntRect(6 * BLOCK_SIZE, 3 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); break;
					case T_DARKSPIKES: b[j][i].rect = IntRect(7 * BLOCK_SIZE, 3 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); break;

					case T_LDARKWALL: b[j][i].rect = IntRect(0 * BLOCK_SIZE, 4 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); b[j][i].isSolid = sfTrue; break;
					case T_EMPTY: b[j][i].rect = IntRect(1 * BLOCK_SIZE, 4 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); break;
					case T_RDARKWALL: b[j][i].rect = IntRect(2 * BLOCK_SIZE, 4 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); b[j][i].isSolid = sfTrue; break;
					case T_LEFTPLATFORM: b[j][i].rect = IntRect(3 * BLOCK_SIZE, 4 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); b[j][i].isSolid = sfTrue; break;
					case T_PLATFORM: b[j][i].rect = IntRect(4 * BLOCK_SIZE, 4 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); b[j][i].isSolid = sfTrue; break;
					case T_RIGHTPLATFORM: b[j][i].rect = IntRect(5 * BLOCK_SIZE, 4 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); b[j][i].isSolid = sfTrue; break;
					case T_LEFTWEB: b[j][i].rect = IntRect(6 * BLOCK_SIZE, 4 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); break;
					case T_RIGHTWEB: b[j][i].rect = IntRect(7 * BLOCK_SIZE, 4 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); break;

					case T_BLDARKWALL: b[j][i].rect = IntRect(0 * BLOCK_SIZE, 5 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); b[j][i].isSolid = sfTrue; break;
					case T_BDARKWALL: b[j][i].rect = IntRect(1 * BLOCK_SIZE, 5 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); b[j][i].isSolid = sfTrue; break;
					case T_BRDARKWALL: b[j][i].rect = IntRect(2 * BLOCK_SIZE, 5 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); b[j][i].isSolid = sfTrue; break;
					case T_TOPWALL: b[j][i].rect = IntRect(3 * BLOCK_SIZE, 5 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); break;
					case T_MIDWALL: b[j][i].rect = IntRect(4 * BLOCK_SIZE, 5 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); break;
					case T_LITTLEWALL: b[j][i].rect = IntRect(5 * BLOCK_SIZE, 5 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); break;
					case T_BIGWALL: b[j][i].rect = IntRect(6 * BLOCK_SIZE, 5 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); break;
					case T_LITTLESKULL: b[j][i].rect = IntRect(7 * BLOCK_SIZE, 5 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); break;

					case T_LITTLEBOX:  b[j][i].rect = IntRect(0 * BLOCK_SIZE, 6 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); break;
					case T_BIGBOX:  b[j][i].rect = IntRect(1 * BLOCK_SIZE, 6 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); break;
					case T_TORCH:  b[j][i].rect = IntRect(2 * BLOCK_SIZE, 6 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); break;
					case T_BOTTOMWALL: b[j][i].rect = IntRect(3 * BLOCK_SIZE, 6 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); b[j][i].isSolid = sfTrue; break;
					case T_SLINGSHOT: b[j][i].rect = IntRect(0, 0, 66, 209); break;
					case T_LLEFTMOVING: b[j][i].rect = IntRect(0, 0, 32, 32); b[j][i].isSolid = sfTrue; break;
					case T_LMOVING: b[j][i].rect = IntRect(0, 32, 32, 32); b[j][i].isSolid = sfTrue; break;
					case T_LRIGHTMOVING: b[j][i].rect = IntRect(0, 64, 32, 32); b[j][i].isSolid = sfTrue; break;

					case T_RLEFTMOVING: b[j][i].rect = IntRect(0, 0, 32, 32); b[j][i].isSolid = sfTrue; break;
					case T_RMOVING: b[j][i].rect = IntRect(0, 32, 32, 32); b[j][i].isSolid = sfTrue; break;
					case T_RRIGHTMOVING: b[j][i].rect = IntRect(0, 64, 32, 32); b[j][i].isSolid = sfTrue; break;
					case T_MUSICBLOC: b[j][i].rect = IntRect(0, 0, 32, 32); b[j][i].isSolid = sfTrue; break;
					case T_DOOR: b[j][i].rect = IntRect(0, 0, 32, 32); break;
					case T_FROGSPAWN: b[j][i].rect = IntRect(0, 0, 32, 32); break;
					case T_ASTRONAUTSPAWN: b[j][i].rect = IntRect(32, 0, 32, 32); break;

					case T_GLOCK: b[j][i].rect = IntRect(0, 0, 32, 32); b[j][i].isSolid = sfTrue; break;
					case T_BLOCK: b[j][i].rect = IntRect(32, 0, 32, 32); b[j][i].isSolid = sfTrue; break;
					case T_RLOCK: b[j][i].rect = IntRect(64, 0, 32, 32); b[j][i].isSolid = sfTrue; break;
					case T_YLOCK: b[j][i].rect = IntRect(96, 0, 32, 32); b[j][i].isSolid = sfTrue; break;

					case T_GKEY: b[j][i].rect = IntRect(0, 32, 32, 32); break;
					case T_BKEY: b[j][i].rect = IntRect(32, 32, 32, 32); break;
					case T_RKEY: b[j][i].rect = IntRect(64, 32, 32, 32); break;
					case T_YKEY: b[j][i].rect = IntRect(96, 32, 32, 32); break;

					case T_GEXBLOCK: b[j][i].rect = IntRect(0, 64, 32, 32); b[j][i].isSolid = sfTrue; break;
					case T_BEXBLOCK: b[j][i].rect = IntRect(32, 64, 32, 32); b[j][i].isSolid = sfTrue; break;
					case T_REXBLOCK: b[j][i].rect = IntRect(64, 64, 32, 32); b[j][i].isSolid = sfTrue; break;
					case T_YEXBLOCK: b[j][i].rect = IntRect(96, 64, 32, 32); b[j][i].isSolid = sfTrue; break;

					case T_GBUTTON: b[j][i].rect = IntRect(0, 96, 32, 32); break;
					case T_BBUTTON: b[j][i].rect = IntRect(32, 96, 32, 32); break;
					case T_RBUTTON: b[j][i].rect = IntRect(64, 96, 32, 32); break;
					case T_YBUTTON: b[j][i].rect = IntRect(96, 96, 32, 32); break;

					default: break;
					//}
				//}
			}
		}

		if (sfKeyboard_isKeyPressed(sfKeyN) && timer > 0.2f)
		{
			isVisible = !isVisible;
			if (isVisible)
				sfText_setString(txtVisible, "N : Visible : YES");
			else
				sfText_setString(txtVisible, "N : Visible : NO");
			timer = 0.f;
		}

		if (sfKeyboard_isKeyPressed(sfKeyB) && timer > 0.2f)
		{
			isCollisionEditor = !isCollisionEditor;
			if(isCollisionEditor)
	 			sfText_setString(txtCollision, "B : Collision : YES"); 
			else
				sfText_setString(txtCollision, "B : Collision : NO");
			timer = 0.f;
		}
	}
	else
	{
		if (sfKeyboard_isKeyPressed(sfKeySpace) && timer > 0.5f)
		{
			isEditorHUD = !isEditorHUD;
			timer = 0.f;
		}

		if (canPlaceTile && !sfMouse_isButtonPressed(sfMouseLeft)) {
			canPlaceTile = sfFalse;
			isEditorHUD = sfFalse;
		}

		if (sfMouse_isButtonPressed(sfMouseLeft))
		{
			timer = 0.f;
			canPlaceTile = sfTrue;
			float mousePosX = getfMousePos(_window->renderWindow).x;
			float mousePosY = getfMousePos(_window->renderWindow).y;

			if ((mousePosX > 0.f * 2.f && mousePosX < 32.f * 2.f) && (mousePosY > 0.f * 2.f && mousePosY < 32.f * 2.f))
				currentTile = T_TLLIGHTWALL;
			else if ((mousePosX > 32.f * 2.f && mousePosX < 64.f * 2.f) && (mousePosY > 0.f * 2.f && mousePosY < 32.f * 2.f))
				currentTile = T_TLIGHTWALL;
			else if ((mousePosX > 64.f * 2.f && mousePosX < 96.f * 2.f) && (mousePosY > 0.f * 2.f && mousePosY < 32.f * 2.f))
				currentTile = T_TRLIGHTWALL;
			else if ((mousePosX > 96.f * 2.f && mousePosX < 128.f * 2.f) && (mousePosY > 0.f * 2.f && mousePosY < 32.f * 2.f))
				currentTile = T_LIGHTWALL1;
			else if ((mousePosX > 128.f * 2.f && mousePosX < 160.f * 2.f) && (mousePosY > 0.f * 2.f && mousePosY < 32.f * 2.f))
				currentTile = T_BRICKWALL1;
			else if ((mousePosX > 160.f * 2.f && mousePosX < 192.f * 2.f) && (mousePosY > 0.f * 2.f && mousePosY < 32.f * 2.f))
				currentTile = T_SIGN;
			else if ((mousePosX > 192.f * 2.f && mousePosX < 224.f * 2.f) && (mousePosY > 0.f * 2.f && mousePosY < 32.f * 2.f))
				currentTile = T_LEVER;
			else if ((mousePosX > 224.f * 2.f && mousePosX < 256.f * 2.f) && (mousePosY > 0.f * 2.f && mousePosY < 32.f * 2.f))
				currentTile = T_BASETORCH;

			else if ((mousePosX > 0.f * 2.f && mousePosX < 32.f * 2.f) && (mousePosY > 32.f * 2.f && mousePosY < 64.f * 2.f))
				currentTile = T_LLIGHTWALL;
			else if ((mousePosX > 32.f * 2.f && mousePosX < 64.f * 2.f) && (mousePosY > 32.f * 2.f && mousePosY < 64.f * 2.f))
				currentTile = T_VOID;
			else if ((mousePosX > 64.f * 2.f && mousePosX < 96.f * 2.f) && (mousePosY > 32.f * 2.f && mousePosY < 64.f * 2.f))
				currentTile = T_RLIGHTWALL;
			else if ((mousePosX > 96.f * 2.f && mousePosX < 128.f * 2.f) && (mousePosY > 32.f * 2.f && mousePosY < 64.f * 2.f))
				currentTile = T_LIGHTWALL2;
			else if ((mousePosX > 128.f * 2.f && mousePosX < 160.f * 2.f) && (mousePosY > 32.f * 2.f && mousePosY < 64.f * 2.f))
				currentTile = T_BRICKWALL2;
			else if ((mousePosX > 160.f * 2.f && mousePosX < 192.f * 2.f) && (mousePosY > 32.f * 2.f && mousePosY < 64.f * 2.f))
				currentTile = T_SWORD;
			else if ((mousePosX > 192.f * 2.f && mousePosX < 224.f * 2.f) && (mousePosY > 32.f * 2.f && mousePosY < 64.f * 2.f))
				currentTile = T_LIGHTSPIKES;
			else if ((mousePosX > 224.f * 2.f && mousePosX < 256.f * 2.f) && (mousePosY > 32.f * 2.f && mousePosY < 64.f * 2.f))
				currentTile = T_SKULLS;

			else if ((mousePosX > 0.f * 2.f && mousePosX < 32.f * 2.f) && (mousePosY > 64.f * 2.f && mousePosY < 96.f * 2.f))
				currentTile = T_BLLIGHTWALL;
			else if ((mousePosX > 32.f * 2.f && mousePosX < 64.f * 2.f) && (mousePosY > 64.f * 2.f && mousePosY < 96.f * 2.f))
				currentTile = T_BLIGHTWALL;
			else if ((mousePosX > 64.f * 2.f && mousePosX < 96.f * 2.f) && (mousePosY > 64.f * 2.f && mousePosY < 96.f * 2.f))
				currentTile = T_BRLIGHTWALL;
			else if ((mousePosX > 96.f * 2.f && mousePosX < 128.f * 2.f) && (mousePosY > 64.f * 2.f && mousePosY < 96.f * 2.f))
				currentTile = T_DARKWALL1;
			else if ((mousePosX > 128.f * 2.f && mousePosX < 160.f * 2.f) && (mousePosY > 64.f * 2.f && mousePosY < 96.f * 2.f))
				currentTile = T_SIDEBARRILS;
			else if ((mousePosX > 160.f * 2.f && mousePosX < 192.f * 2.f) && (mousePosY > 64.f * 2.f && mousePosY < 96.f * 2.f))
				currentTile = T_LITTLEBARRIL;
			else if ((mousePosX > 192.f * 2.f && mousePosX < 224.f * 2.f) && (mousePosY > 64.f * 2.f && mousePosY < 96.f * 2.f))
				currentTile = T_LEFTCHAIN;
			else if ((mousePosX > 224.f * 2.f && mousePosX < 256.f * 2.f) && (mousePosY > 64.f * 2.f && mousePosY < 96.f * 2.f))
				currentTile = T_RIGHTCHAIN;

			else if ((mousePosX > 0.f * 2.f && mousePosX < 32.f * 2.f) && (mousePosY > 96.f * 2.f && mousePosY < 128.f * 2.f))
				currentTile = T_TLDARKWALL;
			else if ((mousePosX > 32.f * 2.f && mousePosX < 64.f * 2.f) && (mousePosY > 96.f * 2.f && mousePosY < 128.f * 2.f))
				currentTile = T_TDARKWALL;
			else if ((mousePosX > 64.f * 2.f && mousePosX < 96.f * 2.f) && (mousePosY > 96.f * 2.f && mousePosY < 128.f * 2.f))
				currentTile = T_TRDARKWALL;
			else if ((mousePosX > 96.f * 2.f && mousePosX < 128.f * 2.f) && (mousePosY > 96.f * 2.f && mousePosY < 128.f * 2.f))
				currentTile = T_DARKWALL2;
			else if ((mousePosX > 128.f * 2.f && mousePosX < 160.f * 2.f) && (mousePosY > 96.f * 2.f && mousePosY < 128.f * 2.f))
				currentTile = T_BIGBARRIL;
			else if ((mousePosX > 160.f * 2.f && mousePosX < 192.f * 2.f) && (mousePosY > 96.f * 2.f && mousePosY < 128.f * 2.f))
				currentTile = T_POT;
			else if ((mousePosX > 192.f * 2.f && mousePosX < 224.f * 2.f) && (mousePosY > 96.f * 2.f && mousePosY < 128.f * 2.f))
				currentTile = T_BOTHCHAIN;
			else if ((mousePosX > 224.f * 2.f && mousePosX < 256.f * 2.f) && (mousePosY > 96.f * 2.f && mousePosY < 128.f * 2.f))
				currentTile = T_DARKSPIKES;

			else if ((mousePosX > 0.f * 2.f && mousePosX < 32.f * 2.f) && (mousePosY > 128.f * 2.f && mousePosY < 160.f * 2.f))
				currentTile = T_LDARKWALL;
			else if ((mousePosX > 32.f * 2.f && mousePosX < 64.f * 2.f) && (mousePosY > 128.f * 2.f && mousePosY < 160.f * 2.f))
				currentTile = T_EMPTY;
			else if ((mousePosX > 64.f * 2.f && mousePosX < 96.f * 2.f) && (mousePosY > 128.f * 2.f && mousePosY < 160.f * 2.f))
				currentTile = T_RDARKWALL;
			else if ((mousePosX > 96.f * 2.f && mousePosX < 128.f * 2.f) && (mousePosY > 128.f * 2.f && mousePosY < 160.f * 2.f))
				currentTile = T_LEFTPLATFORM;
			else if ((mousePosX > 128.f * 2.f && mousePosX < 160.f * 2.f) && (mousePosY > 128.f * 2.f && mousePosY < 160.f * 2.f))
				currentTile = T_PLATFORM;
			else if ((mousePosX > 160.f * 2.f && mousePosX < 192.f * 2.f) && (mousePosY > 128.f * 2.f && mousePosY < 160.f * 2.f))
				currentTile = T_RIGHTPLATFORM;
			else if ((mousePosX > 192.f * 2.f && mousePosX < 224.f * 2.f) && (mousePosY > 128.f * 2.f && mousePosY < 160.f * 2.f))
				currentTile = T_LEFTWEB;
			else if ((mousePosX > 224.f * 2.f && mousePosX < 256.f * 2.f) && (mousePosY > 128.f * 2.f && mousePosY < 160.f * 2.f))
				currentTile = T_RIGHTWEB;

			else if ((mousePosX > 0.f * 2.f && mousePosX < 32.f * 2.f) && (mousePosY > 160.f * 2.f && mousePosY < 192.f * 2.f))
				currentTile = T_BLDARKWALL;
			else if ((mousePosX > 32.f * 2.f && mousePosX < 64.f * 2.f) && (mousePosY > 160.f * 2.f && mousePosY < 192.f * 2.f))
				currentTile = T_BDARKWALL;
			else if ((mousePosX > 64.f * 2.f && mousePosX < 96.f * 2.f) && (mousePosY > 160.f * 2.f && mousePosY < 192.f * 2.f))
				currentTile = T_BRDARKWALL;
			else if ((mousePosX > 96.f * 2.f && mousePosX < 128.f * 2.f) && (mousePosY > 160.f * 2.f && mousePosY < 192.f * 2.f))
				currentTile = T_TOPWALL;
			else if ((mousePosX > 128.f * 2.f && mousePosX < 160.f * 2.f) && (mousePosY > 160.f * 2.f && mousePosY < 192.f * 2.f))
				currentTile = T_MIDWALL;
			else if ((mousePosX > 160.f * 2.f && mousePosX < 192.f * 2.f) && (mousePosY > 160.f * 2.f && mousePosY < 192.f * 2.f))
				currentTile = T_LITTLEWALL;
			else if ((mousePosX > 192.f * 2.f && mousePosX < 224.f * 2.f) && (mousePosY > 160.f * 2.f && mousePosY < 192.f * 2.f))
				currentTile = T_BIGWALL;
			else if ((mousePosX > 224.f * 2.f && mousePosX < 256.f * 2.f) && (mousePosY > 160.f * 2.f && mousePosY < 192.f * 2.f))
				currentTile = T_LITTLESKULL;

			else if ((mousePosX > 0.f * 2.f && mousePosX < 32.f * 2.f) && (mousePosY > 192.f * 2.f && mousePosY < 224.f * 2.f))
				currentTile = T_LITTLEBOX;
			else if ((mousePosX > 32.f * 2.f && mousePosX < 64.f * 2.f) && (mousePosY > 192.f * 2.f && mousePosY < 224.f * 2.f))
				currentTile = T_BIGBOX;
			else if ((mousePosX > 64.f * 2.f && mousePosX < 96.f * 2.f) && (mousePosY > 192.f * 2.f && mousePosY < 224.f * 2.f))
				currentTile = T_TORCH;
			else if ((mousePosX > 96.f * 2.f && mousePosX < 128.f * 2.f) && (mousePosY > 192.f * 2.f && mousePosY < 224.f * 2.f))
				currentTile = T_BOTTOMWALL;
			else if ((mousePosX > 128.f * 2.f && mousePosX < 160.f * 2.f) && (mousePosY > 192.f * 2.f && mousePosY < 224.f * 2.f))
				currentTile = T_SLINGSHOT;
			else if ((mousePosX > 160.f * 2.f && mousePosX < 192.f * 2.f) && (mousePosY > 192.f * 2.f && mousePosY < 224.f * 2.f))
				currentTile = T_LLEFTMOVING;
			else if ((mousePosX > 192.f * 2.f && mousePosX < 224.f * 2.f) && (mousePosY > 192.f * 2.f && mousePosY < 224.f * 2.f))
				currentTile = T_LMOVING;
			else if ((mousePosX > 224.f * 2.f && mousePosX < 256.f * 2.f) && (mousePosY > 192.f * 2.f && mousePosY < 224.f * 2.f))
				currentTile = T_LRIGHTMOVING;

			else if ((mousePosX > 0.f * 2.f && mousePosX < 32.f * 2.f) && (mousePosY > 224.f * 2.f && mousePosY < 256.f * 2.f))
				currentTile = T_RLEFTMOVING;
			else if ((mousePosX > 32.f * 2.f && mousePosX < 64.f * 2.f) && (mousePosY > 224.f * 2.f && mousePosY < 256.f * 2.f))
				currentTile = T_RMOVING;
			else if ((mousePosX > 64.f * 2.f && mousePosX < 96.f * 2.f) && (mousePosY > 224.f * 2.f && mousePosY < 256.f * 2.f))
				currentTile = T_RRIGHTMOVING;
			else if ((mousePosX > 96.f * 2.f && mousePosX < 128.f * 2.f) && (mousePosY > 224.f * 2.f && mousePosY < 256.f * 2.f))
				currentTile = T_MUSICBLOC;
			else if ((mousePosX > 128.f * 2.f && mousePosX < 160.f * 2.f) && (mousePosY > 224.f * 2.f && mousePosY < 256.f * 2.f))
				currentTile = T_DOOR;
			else if ((mousePosX > 160.f * 2.f && mousePosX < 192.f * 2.f) && (mousePosY > 224.f * 2.f && mousePosY < 256.f * 2.f))
				currentTile = T_FROGSPAWN;
			else if ((mousePosX > 192.f * 2.f && mousePosX < 224.f * 2.f) && (mousePosY > 224.f * 2.f && mousePosY < 256.f * 2.f))
				currentTile = T_ASTRONAUTSPAWN;

			else if (mousePosX > 576.f && mousePosX < 640.f && mousePosY > 0.f && mousePosY < 64.f) currentTile = T_GLOCK;
			else if (mousePosX > 640.f && mousePosX < 704.f && mousePosY > 0.f && mousePosY < 64.f) currentTile = T_BLOCK;
			else if (mousePosX > 704.f && mousePosX < 768.f && mousePosY > 0.f && mousePosY < 64.f) currentTile = T_RLOCK;
			else if (mousePosX > 768.f && mousePosX < 832.f && mousePosY > 0.f && mousePosY < 64.f) currentTile = T_YLOCK;

			else if (mousePosX > 576.f && mousePosX < 640.f && mousePosY > 64.f && mousePosY < 128.f) currentTile = T_GKEY;
			else if (mousePosX > 640.f && mousePosX < 704.f && mousePosY > 64.f && mousePosY < 128.f) currentTile = T_BKEY;
			else if (mousePosX > 704.f && mousePosX < 768.f && mousePosY > 64.f && mousePosY < 128.f) currentTile = T_RKEY;
			else if (mousePosX > 768.f && mousePosX < 832.f && mousePosY > 64.f && mousePosY < 128.f) currentTile = T_YKEY;

			else if (mousePosX > 576.f && mousePosX < 640.f && mousePosY > 128.f && mousePosY < 192.f) currentTile = T_GEXBLOCK;
			else if (mousePosX > 640.f && mousePosX < 704.f && mousePosY > 128.f && mousePosY < 192.f) currentTile = T_BEXBLOCK;
			else if (mousePosX > 704.f && mousePosX < 768.f && mousePosY > 128.f && mousePosY < 192.f) currentTile = T_REXBLOCK;
			else if (mousePosX > 768.f && mousePosX < 832.f && mousePosY > 128.f && mousePosY < 192.f) currentTile = T_YEXBLOCK;

			else if (mousePosX > 576.f && mousePosX < 640.f && mousePosY > 192.f && mousePosY < 256.f) currentTile = T_GBUTTON;
			else if (mousePosX > 640.f && mousePosX < 704.f && mousePosY > 192.f && mousePosY < 256.f) currentTile = T_BBUTTON;
			else if (mousePosX > 704.f && mousePosX < 768.f && mousePosY > 192.f && mousePosY < 256.f) currentTile = T_RBUTTON;
			else if (mousePosX > 768.f && mousePosX < 832.f && mousePosY > 192.f && mousePosY < 256.f) currentTile = T_YBUTTON;


			else
				canPlaceTile = sfFalse;
		}
	}

	// to remove improve
	if (sfKeyboard_isKeyPressed(sfKeyEscape) && timer > 0.2f)
	{
		isEditor = sfFalse;
		timer = 0.f;
	}

		//if (getPause())
		//	updatePause(_window);

		/// CHANGE MAPS
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
	if (key > 0 && saveTimer > 0.5f) {
		saveTimer = 0.f;
		if (sfKeyboard_isKeyPressed(sfKeyC)) {
			copyMap(key);

			switch (nbMap)
			{
			case 1: sfText_setString(txtNbMap, "Copied into : 1 (M to save)"); break;
			case 2: sfText_setString(txtNbMap, "Copied into : 2 (M to save)"); break;
			case 3: sfText_setString(txtNbMap, "Copied into : 3 (M to save)"); break;
			case 4: sfText_setString(txtNbMap, "Copied into : 4 (M to save)"); break;
			case 5: sfText_setString(txtNbMap, "Copied into : 5 (M to save)"); break;
			case 6: sfText_setString(txtNbMap, "Copied into : 6 (M to save)"); break;
			case 7: sfText_setString(txtNbMap, "Copied into : 7 (M to save)"); break;
			case 8: sfText_setString(txtNbMap, "Copied into : 8 (M to save)"); break;
			case 9: sfText_setString(txtNbMap, "Copied into : 9 (M to save)"); break;
			default:
				break;
			}
		}
		else {
			loadMap(key);
			nbMap = key;

			switch (nbMap)
			{
			case 1: sfText_setString(txtNbMap, "Nb Map : 1 (M to save)"); break;
			case 2: sfText_setString(txtNbMap, "Nb Map : 2 (M to save)"); break;
			case 3: sfText_setString(txtNbMap, "Nb Map : 3 (M to save)"); break;
			case 4: sfText_setString(txtNbMap, "Nb Map : 4 (M to save)"); break;
			case 5: sfText_setString(txtNbMap, "Nb Map : 5 (M to save)"); break;
			case 6: sfText_setString(txtNbMap, "Nb Map : 6 (M to save)"); break;
			case 7: sfText_setString(txtNbMap, "Nb Map : 7 (M to save)"); break;
			case 8: sfText_setString(txtNbMap, "Nb Map : 8 (M to save)"); break;
			case 9: sfText_setString(txtNbMap, "Nb Map : 9 (M to save)"); break;
			default:
				break;
			}
		}
	}

	if (sfKeyboard_isKeyPressed(sfKeyM) && saveTimer > 0.5f) {
		saveTimer = 0.f;
		saveMap(nbMap);
		sfText_setString(txtNbMap, "Map saved", nbMap);
	}

	if (isButtonPressed(0, START) && timer > 0.2f) {
		timer = 0.f;
		togglePause();
	}

	if (sfKeyboard_isKeyPressed(sfKeyQ)) SetViewPosition(mainView, vector2f(GetViewPosition(mainView).x - 1000.f * dt, GetViewPosition(mainView).y));
	if (sfKeyboard_isKeyPressed(sfKeyD)) SetViewPosition(mainView, vector2f(GetViewPosition(mainView).x + 1000.f * dt, GetViewPosition(mainView).y));
	if (sfKeyboard_isKeyPressed(sfKeyS)) SetViewPosition(mainView, vector2f(GetViewPosition(mainView).x, GetViewPosition(mainView).y + 1000.f * dt));
	if (sfKeyboard_isKeyPressed(sfKeyZ)) SetViewPosition(mainView, vector2f(GetViewPosition(mainView).x, GetViewPosition(mainView).y - 1000.f * dt));
}

void displayEditor(Window* _window)
{

	//displayParallax(_window);
	displayMap(_window);
	/// TILE CURSOR
	if (currentTile == T_SLINGSHOT)	sfSprite_setTexture(tileCursor, GetTexture("slingshot"), sfFalse);
	else if (currentTile == T_LLEFTMOVING || currentTile == T_LMOVING || currentTile == T_LRIGHTMOVING) sfSprite_setTexture(tileCursor, GetTexture("leftMoving"), sfFalse);
	else if (currentTile == T_RLEFTMOVING || currentTile == T_RMOVING || currentTile == T_RRIGHTMOVING) sfSprite_setTexture(tileCursor, GetTexture("rightMoving"), sfFalse);
	else if (currentTile == T_MUSICBLOC) sfSprite_setTexture(tileCursor, GetTexture("musicBloc"), sfFalse);
	else if (currentTile == T_DOOR) sfSprite_setTexture(tileCursor, GetTexture("doors"), sfFalse);
	else if (currentTile == T_FROGSPAWN || currentTile == T_ASTRONAUTSPAWN) sfSprite_setTexture(tileCursor, GetTexture("spawn"), sfFalse);
	else if (currentTile >= T_GLOCK && currentTile <= T_YPRESSEDBUTTON) sfSprite_setTexture(tileCursor, GetTexture("objects"), sfFalse);
	else sfSprite_setTexture(tileCursor, GetTexture("castleTiles"), sfFalse);

	sfSprite_setTextureRect(tileCursor, tileCursorRect);
	
	sfSprite_setPosition(tileCursor, vector2f(getfWorldMousePos(_window->renderWindow).x, getfWorldMousePos(_window->renderWindow).y));
	if (currentTile != T_NOTILE)
		sfRenderTexture_drawSprite(_window->renderTexture, tileCursor, NULL);

	sfRenderTexture_setView(_window->renderTexture, sfRenderTexture_getDefaultView(_window->renderTexture));
	sfRenderTexture_drawText(_window->renderTexture, txtCollision, NULL);
	sfRenderTexture_drawText(_window->renderTexture, txtVisible, NULL);
	sfRenderTexture_drawText(_window->renderTexture, txtNbMap, NULL);
	if (isEditorHUD)
	{
		sfRenderTexture_drawRectangleShape(_window->renderTexture, rectCursor, NULL);

		sfSprite_setTexture(hudEditor, GetTexture("castleTiles"), sfTrue);
		sfSprite_setPosition(hudEditor, vector2f(0.f, 0.f));
		sfSprite_setScale(hudEditor, vector2f(2.f, 2.f));
		sfRenderTexture_drawSprite(_window->renderTexture, hudEditor, NULL);

		sfSprite_setTexture(hudEditor, GetTexture("objects"), sfTrue);
		//sfSprite_setTextureRect(hudEditor, IntRect(0, 0, 128, 128));
		sfSprite_setPosition(hudEditor, vector2f(576.f, 0.f));
		sfSprite_setScale(hudEditor, vector2f(2.f, 2.f));
		sfRenderTexture_drawSprite(_window->renderTexture, hudEditor, NULL);
	}
	sfRenderTexture_setView(_window->renderTexture, mainView->view);

	//if (getPause())
	//	displayPause(_window);
}

void deinitEditor()
{
	RemoveAllTextureButALL();
	sfSprite_destroy(tileCursor);
}


void saveMap(int _nbMap)
{
	FILE* file;
	switch (_nbMap)
	{
		case 1:
			file = fopen(SAVE_PATH"map1.bin", "wb");
			if (file != NULL) {
				fwrite(&b, sizeof(Blocks), NB_BLOCKS_X * NB_BLOCKS_Y, file);
				fclose(file); 
				int m = 4;
			}
			break;
		case 2:
			file = fopen(SAVE_PATH"map2.bin", "wb");
			if (file != NULL) {
				fwrite(&b, sizeof(Blocks), NB_BLOCKS_X * NB_BLOCKS_Y, file);
				fclose(file);
			}
			break;
		case 3:
			file = fopen(SAVE_PATH"map3.bin", "wb");
			if (file != NULL) {
				fwrite(&b, sizeof(Blocks), NB_BLOCKS_X * NB_BLOCKS_Y, file);
				fclose(file);
			}
			break;
		case 4:
			file = fopen(SAVE_PATH"map4.bin", "wb");
			if (file != NULL) {
				fwrite(&b, sizeof(Blocks), NB_BLOCKS_X * NB_BLOCKS_Y, file);
				fclose(file);
			}
			break;
		case 5:
			file = fopen(SAVE_PATH"map5.bin", "wb");
			if (file != NULL) {
				fwrite(&b, sizeof(Blocks), NB_BLOCKS_X * NB_BLOCKS_Y, file);
				fclose(file);
			}
			break;
		case 6:
			file = fopen(SAVE_PATH"map6.bin", "wb");
			if (file != NULL) {
				fwrite(&b, sizeof(Blocks), NB_BLOCKS_X * NB_BLOCKS_Y, file);
				fclose(file);
			}
			break;
		case 7:
			file = fopen(SAVE_PATH"map7.bin", "wb");
			if (file != NULL) {
				fwrite(&b, sizeof(Blocks), NB_BLOCKS_X * NB_BLOCKS_Y, file);
				fclose(file);
			}
			break;
		case 8:
			file = fopen(SAVE_PATH"map8.bin", "wb");
			if (file != NULL) {
				fwrite(&b, sizeof(Blocks), NB_BLOCKS_X * NB_BLOCKS_Y, file);
				fclose(file);
			}
			break;
		case 9:
			file = fopen(SAVE_PATH"map9.bin", "wb");
			if (file != NULL) {
				fwrite(&b, sizeof(Blocks), NB_BLOCKS_X * NB_BLOCKS_Y, file);
				fclose(file);
			}
			break;
		default:
			break;
	}
}

void copyMap(int _MapToCopy)
{
	FILE* file;
	switch (_MapToCopy)
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
		default:
			break;
	}
}
