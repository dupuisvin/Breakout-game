#include "RenderSystem.h"
#include "RenderWindow.h"
#include "Position.h"
#include "Sprite.h"
#include "Active.h"

#include <entt/entt.hpp>
#include <stdexcept>

using namespace SDLEngine;

#ifdef DRAW_DEBUG_INFO

#include "CollisionAABB.h"
#include "CollisionCircle.h"
#include "PaddleCollisionObj.h"

using namespace Breakout;

static void DrawDebugCollRect(const Position& pos, const CollisionAABB& coll, SDL_Renderer* renderer, SDL_Texture* layer)
{
    SDL_FRect rect;
    rect.x = pos.Pos.x + coll.PosOffset.x;
    rect.y = pos.Pos.y + coll.PosOffset.y;
    rect.w = coll.Width;
    rect.h = coll.Height;
    SDL_SetRenderTarget(renderer, layer);
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderDrawRectF(renderer, &rect);
}

static void DrawDebugCollCircle(const Position& pos, const CollisionCircle& coll, SDL_Renderer* renderer, SDL_Texture* layer)
{
    glm::vec circleCenter = pos.Pos + coll.PosOffset;
    SDL_SetRenderTarget(renderer, layer);
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderDrawLineF(renderer, circleCenter.x, circleCenter.y, circleCenter.x, circleCenter.y + coll.Radius);
    SDL_RenderDrawLineF(renderer, circleCenter.x, circleCenter.y, circleCenter.x, circleCenter.y - coll.Radius);
    SDL_RenderDrawLineF(renderer, circleCenter.x, circleCenter.y, circleCenter.x + coll.Radius, circleCenter.y);
    SDL_RenderDrawLineF(renderer, circleCenter.x, circleCenter.y, circleCenter.x - coll.Radius, circleCenter.y);
}

static void DrawDebugObjects(entt::registry &reg, SDL_Renderer *renderer, SDL_Texture *layer)
{
    auto debugViewAABB = reg.view <CollisionAABB, Position, Active>();
    debugViewAABB.each([&](const CollisionAABB& coll, const Position& pos)
        {
            DrawDebugCollRect(pos, coll, renderer, layer);
        });

    auto debugViewCircles = reg.view <CollisionCircle, Position, Active>();
    debugViewCircles.each([&](const CollisionCircle& coll, const Position& pos)
        {
            DrawDebugCollCircle(pos, coll, renderer, layer);
        });

    auto debugViewPaddle = reg.view<PaddleCollisionObj, Position, Active>();
    debugViewPaddle.each([&](const PaddleCollisionObj& paddleColl, const Position& pos)
        {
            const auto& centerAABB = reg.get<CollisionAABB>(paddleColl.CenterAABB);
            const auto& leftCircle = reg.get<CollisionCircle>(paddleColl.LeftCircle);
            const auto& rightCircle = reg.get<CollisionCircle>(paddleColl.RightCircle);
            DrawDebugCollRect(pos, centerAABB, renderer, layer);
            DrawDebugCollCircle(pos, leftCircle, renderer, layer);
            DrawDebugCollCircle(pos, rightCircle, renderer, layer);
        });

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
}

#endif

void RenderSystem::ClearLayers(RenderWindow& w)
{
    //Clear all layers
    for (auto& layer : RenderLayers)
    {
        if (SDL_SetRenderTarget(w.GetRenderer(), layer.get()) != 0)
            throw std::runtime_error(SDL_GetError());
        SDL_RenderClear(w.GetRenderer());
    }

    //Reset target
    if (SDL_SetRenderTarget(w.GetRenderer(), nullptr) != 0)
        throw std::runtime_error(SDL_GetError());
}

void RenderSystem::Init(RenderWindow& window)
{
    //Create all layer textures
    for (auto& layer : RenderLayers)
    {
        layer = std::unique_ptr<SDL_Texture, TextureDeleter>(SDL_CreateTexture(window.GetRenderer(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, window.GetWidth(), window.GetHeight()));
        if (!layer)
            throw std::runtime_error(SDL_GetError());
        SDL_SetTextureBlendMode(layer.get(), SDL_BLENDMODE_BLEND);
    }
}

void RenderSystem::Render(entt::registry& reg, RenderWindow& w)
{
    ClearLayers(w);
    RenderSpritesToLayers(reg, w);

#ifdef DRAW_DEBUG_INFO
    DrawDebugObjects(reg, w.GetRenderer(), RenderLayers[4].get());
#endif // DRAW_DEBUG_INFO

    RenderLayersToScreen(w);
} 

void RenderSystem::RenderLayersToScreen(RenderWindow& w)
{
    //Reset Target
    if (SDL_SetRenderTarget(w.GetRenderer(), nullptr) != 0)
        throw std::runtime_error(SDL_GetError());

    //Render all layers
    SDL_RenderClear(w.GetRenderer());
    for (const auto& layer : RenderLayers)
    {
        if (SDL_RenderCopy(w.GetRenderer(), layer.get(), nullptr, nullptr) != 0)
            throw std::runtime_error(SDL_GetError());
    }

    SDL_RenderPresent(w.GetRenderer());
}

void RenderSystem::RenderSpritesToLayers(entt::registry& reg, RenderWindow& w)
{
    //Draw each texture to its specified layer
    auto view = reg.view<Sprite, Position, Active>();
    view.each([&](const Sprite& sprite, const Position& pos)
        {
            auto scaleVec = w.GetScale();
            SDL_Rect srcQuad = { 0, 0, sprite.Rect.w, sprite.Rect.h };
            SDL_Rect dstQuad = {
                static_cast<int>(pos.Pos.x * scaleVec.x),
                static_cast<int>(pos.Pos.y * scaleVec.y),
                static_cast<int>(sprite.Rect.w * scaleVec.x),
                static_cast<int>(sprite.Rect.h * scaleVec.y) };
            SDL_SetRenderTarget(w.GetRenderer(), RenderLayers[sprite.Layer].get());
            SDL_RenderCopyEx(w.GetRenderer(), sprite.Texture.get(), &srcQuad, &dstQuad, 0.0, nullptr, SDL_FLIP_NONE);
            SDL_SetTextureAlphaMod(sprite.Texture.get(), SDL_ALPHA_OPAQUE);
        });
}

