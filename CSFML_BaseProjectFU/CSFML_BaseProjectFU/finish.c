#include "finish.h"
#include "viewManager.h"

typedef struct {
	sfView* view;
	sfVector2f pos;
	sfVector2f size;
}Views;
Views v[2];

int nbDisplay;

void initFinsih()
{
	for (int i = 0; i < 2; i++)
	{
		v[i].view = sfView_create();
		v[i].pos = VECTOR2F_NULL;
		v[i].size = vector2f(960.f, 1080.f);
	}
	
	sfView_setViewport(v[0].view, FlRect(0.f, 0.f, 0.5f, 1.f));
	sfView_setViewport(v[1].view, FlRect(0.5f, 0.f, 0.5f, 1.f));
	nbDisplay = 0;
}

void updateFinish(Window* _window)
{
	nbDisplay = 0;
}

void displayFinish(Window* _window)
{
	if (nbDisplay < 0 || nbDisplay > 1)
		return;

	sfView_setCenter(v[nbDisplay].view, v[nbDisplay].pos);
	sfView_setSize(v[nbDisplay].view, v[nbDisplay].size);
	sfRenderTexture_setView(_window->renderTexture, v[nbDisplay].view);

	nbDisplay++;
}

void setFinishViewPos(int _nb, sfVector2f _pos)
{
	v[_nb].pos = _pos;
}
