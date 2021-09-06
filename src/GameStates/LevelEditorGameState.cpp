#include "LevelEditorGameState.h"

#include "TextureLoader.h"
#include "RenderWindow.h"
#include "Active.h"
#include "Sprite.h"
#include "Position.h"
#include "Sound.h"
#include "MouseEvents.h"
#include "KeyboardEvents.h"
#include "GameStatesEvents.h"
#include "CollisionUtils.h"
#include <entt/entt.hpp>
#include <SDL_mixer.h>

#include <filesystem>

namespace fs = std::filesystem;

using namespace SDLEngine;
using namespace Breakout;

LevelEditorGameState::LevelEditorGameState(entt::dispatcher& dispatcher) :
    GameState(dispatcher),
    SoundSys(Registry, dispatcher)
{
}

void LevelEditorGameState::AddRemoveBrick(glm::vec2 mousePos, bool bAdd)
{
    //We could use a hierarchical structure to simplify finding the brick at the mouse position if this gets too slow
    auto brickView = Registry.view<Brick, Position, CollisionAABB>();
    for (auto entity : brickView)
    {
        const auto& brick = Registry.get<Brick>(entity);
        const auto& pos   = Registry.get<Position>(entity);
        const auto& coll  = Registry.get<CollisionAABB>(entity);

        if (CollisionUtils::IsInsideRect(mousePos, pos.Pos, coll))
        {
            Registry.destroy(entity);
            break;
        }
    }

    if (bAdd)
    {
        glm::vec2 brickPos = { std::floor(mousePos.x / 32.0f)*32.0f, std::floor(mousePos.y / 32.0f) * 32.0f };
        auto newBrick = Registry.create();
        Registry.emplace<Brick>(newBrick, CurrentBrickType);
        Registry.emplace<Position>(newBrick, brickPos.x, brickPos.y);
        auto brickSprite = Level.GetBrickSprite(CurrentBrickType);
        Registry.emplace<CollisionAABB>(newBrick, static_cast<float>(brickSprite.Rect.w), static_cast<float>(brickSprite.Rect.h));
        Registry.emplace<Sprite>(newBrick, std::move(brickSprite));
        Registry.emplace<Active>(newBrick);
    }
}

void LevelEditorGameState::ConnectEvents()
{
    Dispatcher.sink<MouseButtonUpEvent>().connect<&LevelEditorGameState::HandleMouseButtonUp>(*this);
    Dispatcher.sink<KeyDownEvent>().connect<&LevelEditorGameState::HandleKeyboardEvents>(*this);
    Dispatcher.sink<MouseMoveEvent>().connect<&LevelEditorGameState::HandleMouseMoveEvent>(*this);
}

void LevelEditorGameState::DisconnectEvents()
{
    Dispatcher.sink<MouseButtonUpEvent>().disconnect<&LevelEditorGameState::HandleMouseButtonUp>(*this);
    Dispatcher.sink<KeyDownEvent>().disconnect<&LevelEditorGameState::HandleKeyboardEvents>(*this);
    Dispatcher.sink<MouseMoveEvent>().disconnect<&LevelEditorGameState::HandleMouseMoveEvent>(*this);
}

void LevelEditorGameState::HandleMouseButtonUp(const MouseButtonUpEvent& event)
{
    switch (event.Button)
    {
    case SDL_BUTTON_LEFT:
    case SDL_BUTTON_RIGHT:
    {
        glm::vec2 mousePos = { event.PosX, event.PosY };
        if (CollisionUtils::IsInsideRect(mousePos, BrickZonePos, BrickZone))
            AddRemoveBrick(mousePos, event.Button == SDL_BUTTON_LEFT);
    }
    break;
    }
}

void LevelEditorGameState::HandleKeyboardEvents(const KeyDownEvent& event)
{
    switch (event.KeyCode)
    {
    case SDLK_s:
        SaveLevel();
        break;
    case SDLK_1:
        CurrentBrickType = Brick::BrickType::Blue;
        Registry.replace<Sprite>(CurrentBrick, Level.GetBrickSprite(CurrentBrickType));
        break;
    case SDLK_2:
        CurrentBrickType = Brick::BrickType::Green;
        Registry.replace<Sprite>(CurrentBrick, Level.GetBrickSprite(CurrentBrickType));
        break;
    case SDLK_3:
        CurrentBrickType = Brick::BrickType::Grey;
        Registry.replace<Sprite>(CurrentBrick, Level.GetBrickSprite(CurrentBrickType));
        break;
    case SDLK_4:
        CurrentBrickType = Brick::BrickType::Purple;
        Registry.replace<Sprite>(CurrentBrick, Level.GetBrickSprite(CurrentBrickType));
        break;
    case SDLK_5:
        CurrentBrickType = Brick::BrickType::Red;
        Registry.replace<Sprite>(CurrentBrick, Level.GetBrickSprite(CurrentBrickType));
        break;
    case SDLK_6:
        CurrentBrickType = Brick::BrickType::Yellow;
        Registry.replace<Sprite>(CurrentBrick, Level.GetBrickSprite(CurrentBrickType));
        break;
    case SDLK_q:
        CurrentBackgroundIdx = (CurrentBackgroundIdx-1)%Backgrounds.size();
        Registry.replace<Sprite>(CurrentBackground, Backgrounds[CurrentBackgroundIdx].first);
        break;
    case SDLK_e:
        CurrentBackgroundIdx = (CurrentBackgroundIdx + 1) % Backgrounds.size();
        Registry.replace<Sprite>(CurrentBackground, Backgrounds[CurrentBackgroundIdx].first);
        break;
    case SDLK_ESCAPE:
        Dispatcher.trigger<PauseEvent>();
        break;
    }
}

void LevelEditorGameState::HandleMouseMoveEvent(const MouseMoveEvent& event)
{
    glm::vec2 mousePos = { event.PosX, event.PosY };
    if (CollisionUtils::IsInsideRect(mousePos, BrickZonePos, BrickZone))
    {
        auto& brickPos = Registry.get<Position>(CurrentBrick);
        brickPos = { std::floor(mousePos.x / 32.0f) * 32.0f, std::floor(mousePos.y / 32.0f) * 32.0f };;
    }
}

static fs::path GetLevelPath()
{
    //Ugly temporary way to get a valid file name
    //@todo add widget to set the filename in the level editor
    fs::path levelFolder = "Assets\\Levels";
    std::string CustomLevelName = "CustomLevel";
    int nLevelNumber = 1;
    std::string CustomLevelExt = ".json";
    fs::path levelPath = levelFolder / fs::path(CustomLevelName + std::to_string(nLevelNumber) + CustomLevelExt);
    while (fs::exists(levelPath))
    {
        ++nLevelNumber;
        levelPath = levelFolder / fs::path(CustomLevelName + std::to_string(nLevelNumber) + CustomLevelExt);
    }
    return levelPath;
}

void LevelEditorGameState::Init(RenderWindow& window)
{
    LevelLoader::LoadBrickSprites(Level, window.GetRenderer());
    LoadBackgrounds(window);
    Renderer.Init(window);

    CurrentBackground = Registry.create();
    Registry.emplace<Position>(CurrentBackground, 0.0f, 0.0f);
    Registry.emplace<Sprite>(CurrentBackground, Backgrounds[CurrentBackgroundIdx].first);
    Registry.emplace<Active>(CurrentBackground);

    CurrentBrick = Registry.create();
    Registry.emplace<Position>(CurrentBrick, 0.0f, 0.0f);
    Registry.emplace<Sprite>(CurrentBrick, Level.GetBrickSprite(CurrentBrickType));
    Registry.emplace<Active>(CurrentBrick);

    BrickZonePos = { 32.0f, 32.0f };
    BrickZone.Width = 928.0f;
    BrickZone.Height = 224.0f;

    ConnectEvents();
}

void LevelEditorGameState::LoadBackgrounds(RenderWindow& window)
{
    fs::path backgroundFolder = "Assets\\Graphics\\Background";
    for (const auto& entry : fs::directory_iterator(backgroundFolder))
        Backgrounds.emplace_back(TextureLoader::LoadFromFile(window.GetRenderer(), entry.path().string(), 0), entry.path().string());
    CurrentBackgroundIdx = 0;
}

void LevelEditorGameState::Render(RenderWindow& window)
{
    Renderer.Render(Registry, window);
}

void LevelEditorGameState::SaveLevel()
{
    auto levelPath = GetLevelPath();
    LevelAssets levelAssets;
    levelAssets.Background = Backgrounds[CurrentBackgroundIdx].second;
    auto brickView = Registry.view<Brick, Position>();
    levelAssets.Bricks.reserve(brickView.size());
    for (auto entity : brickView)
    {
        if (entity != CurrentBrick)
        {
            const auto& brick = Registry.get<Brick>(entity);
            const auto& pos = Registry.get<Position>(entity);
            levelAssets.Bricks.emplace_back(brick.Type, static_cast<int>(pos.Pos.x), static_cast<int>(pos.Pos.y));
        }

    }
    LevelLoader::SaveLevel(levelPath.string(), levelAssets, Registry);
}

void LevelEditorGameState::Uninit()
{
    Registry.clear();
    DisconnectEvents();
}
