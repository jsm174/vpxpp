#pragma once

#include <SDL2/SDL.h>

class BgfxWindow
{
public:
	BgfxWindow();
	virtual ~BgfxWindow();

	bool Create(int width, int height);
	void Run();

	virtual void Render();

private:
	void ShowStatsWindow();
	
	void Destroy();

	int m_width;
	int m_height;

	SDL_Window* m_pWindow;
};