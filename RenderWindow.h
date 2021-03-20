#ifndef M_RENDER_WINDOW
#define M_RENDER_WINDOW

#include "SDL.h"
#include <glm/vec2.hpp>
#include <string>
#include <memory>

namespace SDLEngine
{
    class RenderWindow
    {
    public:

        static constexpr int DEFAULT_SCREEN_WIDTH = 1024;
        static constexpr int DEFAULT_SCREEN_HEIGHT = 512;

        RenderWindow(std::string windowName);

        SDL_Renderer* GetRenderer() { return RendererPtr.get(); }
        SDL_Window* GetWindow() { return WindowPtr.get(); }

        //Initiliaze the window by creating the window and intializing the renderer
        void Init();

        int GetWidth() const { return Width; }
        int GetHeight() const { return Height; }

        //Get the scale of the window (CurrentWidth/DefaultWidth, CurrentHeight/DefaultHeight)
        glm::vec2 GetScale() const { return Scale; }

        //Update the size properties of the window
        void UpdateWindowSize(int nWidth, int nHeight);

    private:

		struct SDLWindowDeleter
		{
			void operator()(SDL_Window* p) const noexcept { SDL_DestroyWindow(p); }
		};

		struct SDLRendererDeleter
		{
			void operator()(SDL_Renderer* p) const noexcept { SDL_DestroyRenderer(p); }
		};

        std::unique_ptr<SDL_Window, SDLWindowDeleter> WindowPtr;
        std::unique_ptr<SDL_Renderer, SDLRendererDeleter> RendererPtr;
        std::string WindowName;
        int Width = DEFAULT_SCREEN_WIDTH;
        int Height = DEFAULT_SCREEN_HEIGHT;
        glm::vec2 Scale = glm::vec2{ 1.0f, 1.0f };
    };
}


#endif // !M_RENDER_WINDOW
