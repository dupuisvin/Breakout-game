#include "RenderWindow.h"
#include <stdexcept>

using namespace SDLEngine;
using namespace std::string_literals;

RenderWindow::RenderWindow(std::string windowName) :
    WindowName(windowName)
{
}

void RenderWindow::Init()
{
	WindowPtr.reset(SDL_CreateWindow(
		WindowName.c_str(),
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		DEFAULT_SCREEN_WIDTH,
		DEFAULT_SCREEN_HEIGHT,
		SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI));

	if (WindowPtr)
	{
		RendererPtr.reset(SDL_CreateRenderer(WindowPtr.get(), -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC));
		if (!RendererPtr)
			throw std::runtime_error("Renderer could not bbe created; SDL_Error: % s\n"s + std::string(SDL_GetError()));
	}
	else
		throw std::runtime_error("Window could not be created; SDL_Error: % s\n"s + std::string(SDL_GetError()));
}

void RenderWindow::UpdateWindowSize(int nWidth, int nHeight)
{
	Width = nWidth;
	Height = nHeight;
	Scale = glm::vec2(
		nWidth / static_cast<float>(DEFAULT_SCREEN_WIDTH),
		nHeight / static_cast<float>(DEFAULT_SCREEN_HEIGHT));
}