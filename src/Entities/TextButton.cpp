#include "TextButton.h"
#include "RenderWindow.h"
#include "TextureLoader.h"
#include "CollisionUtils.h"
#include "Position.h"
#include "Active.h"
#include <entt/entt.hpp>

using namespace SDLEngine;

const SDL_Color TextButton::DEFAULT_FONT_COLOR = { 25, 25, 25, 255 };
static constexpr char LARGE_BUTTON_TEXTURE_PATH[] = "Assets/Graphics/Buttons/LargeButton.png";
static constexpr char DEFAULT_FONT[] = "Assets/Fonts/Kenney_Rocket.ttf";

int TextButton::GetTextFontSize(const std::string& text)
{
    const size_t size = text.size();
    if (size < 10)
        return 20;
    else if (size >= 10 && size < 20)
        return 16;
    return 12;
}

bool TextButton::IsPtInside(float x, float y, entt::registry& reg)
{
    const auto [pos, sprite] = reg.get<Position, Sprite>(ButtonEntity);
    return CollisionUtils::IsInsideRect(
        x,
        y,
        pos.Pos.x,
        pos.Pos.y,
        static_cast<float>(sprite.Rect.w),
        static_cast<float>(sprite.Rect.h));
}

void TextButton::Load(entt::registry& registry, RenderWindow& window, int nStartLayer)
{
    ButtonEntity = registry.create();
    auto buttonSprite = TextureLoader::LoadFromFile(window.GetRenderer(), LARGE_BUTTON_TEXTURE_PATH, nStartLayer);
    registry.emplace<Position>(
        ButtonEntity,
        PosX - buttonSprite.Rect.w / 2.0f,
        PosY - buttonSprite.Rect.h / 2.0f);
    registry.emplace<Sprite>(ButtonEntity, std::move(buttonSprite));
    registry.emplace<Active>(ButtonEntity);

    TextEntity = registry.create();
    auto startButtonTextSprite = TextureLoader::LoadFromText(
        window.GetRenderer(),
        Text.c_str(),
        DEFAULT_FONT,
        GetTextFontSize(Text),
        FontColor,
        nStartLayer+1);
    registry.emplace<Position>(
        TextEntity,
        PosX - startButtonTextSprite.Rect.w / 2.0f,
        PosY - startButtonTextSprite.Rect.h / 2.0f);
    registry.emplace<Sprite>(TextEntity, std::move(startButtonTextSprite));
    registry.emplace<Active>(TextEntity);
}

void TextButton::Unload(entt::registry& registry)
{
    registry.destroy(ButtonEntity);
    registry.destroy(TextEntity);
}