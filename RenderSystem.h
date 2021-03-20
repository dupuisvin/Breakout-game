#ifndef M_RENDER_SYSTEM
#define M_RENDER_SYSTEM

#include <entt/entity/registry.hpp>
#include <SDL.h>

namespace SDLEngine
{
    class RenderWindow;
    struct Position;
    struct CollisionAABB;
    struct CollisionCircle;

    //System in charge of rendering all the appropriate entities
    class RenderSystem
    {
    public:

        //Initiliazed the rendering layers
        void Init(RenderWindow &window);

        //Render the approparite entities to the specified window
        void Render(entt::registry& reg, RenderWindow &w);

    private:

        void DrawDebugCollRect(const Position& pos, const CollisionAABB& coll, SDL_Renderer* renderer);
        void DrawDebugCollCircle(const Position& pos, const CollisionCircle& coll, SDL_Renderer* renderer);

        struct TextureDeleter
        {
            void operator()(SDL_Texture* p) { SDL_DestroyTexture(p); }
        };
        std::array<std::unique_ptr<SDL_Texture, TextureDeleter>, 5> RenderLayers;
    };
}


#endif // !M_RENDER_SYSTEM

