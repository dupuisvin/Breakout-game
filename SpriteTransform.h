#ifndef M_TRANSFORM
#define M_TRANSFORM

#include "SDL.h"
#include <glm/vec2.hpp>
#include "Interpolations.h"

struct SpriteTransform
{
    static SpriteTransform BuildFromOffset(float offX, float offY) { return SpriteTransform(offX, offY, 0.0f, 0.0f, 0.0, 0, 0, SDL_FLIP_NONE); }
    static SpriteTransform BuilFromRotation(double angle, int rotX, int rotY) { return SpriteTransform(0.0f, 0.0f, 0.0f, 0.0f, angle, rotX, rotY, SDL_FLIP_NONE); }
    static SpriteTransform BuildFromScale(float scaleX, float scaleY) { return SpriteTransform(0.0f, 0.0f, scaleX, scaleY, 0.0, 0, 0, SDL_FLIP_NONE); }
    
    SpriteTransform() = default;

    SpriteTransform(glm::vec2 off, glm::vec2 scale, double angle, int rotX, int rotY, SDL_RendererFlip flip) :
        Offset(off),
        Scaling(scale),
        RotAngle(angle),
        RotCenter{ rotX, rotY },
        Flip(flip) {}

    SpriteTransform(float offX, float offY, float scaleX, float scaleY, double angle, int rotX, int rotY, SDL_RendererFlip flip) :
        Offset(offX, offY),
        Scaling(scaleX, scaleY),
        RotAngle(angle),
        RotCenter{ rotX, rotY },
        Flip(flip) {}

    void Rotate(float angle)
    {
        RotAngle += angle;
    }

    void Translate(const glm::vec2& off)
    {
        Offset += off;
    }

    void Scale(const glm::vec2& scale)
    {
        Scaling *= scale;
    }

    SpriteTransform Interpolate(SpriteTransform& other, float t) const
    {
        SpriteTransform outTransfo;
        outTransfo.Offset.x = SDLEngine::Lerp<float>(Offset.x, other.Offset.x, t);
        outTransfo.Offset.y = SDLEngine::Lerp<float>(Offset.y, other.Offset.y, t);
        outTransfo.Scaling.x = SDLEngine::Lerp<float>(Scaling.x, other.Scaling.x, t);
        outTransfo.Scaling.y = SDLEngine::Lerp<float>(Scaling.y, other.Scaling.y, t);
        outTransfo.RotAngle = SDLEngine::Lerp<double>(RotAngle, other.RotAngle, t);
        if (Flip != other.Flip && t >= 0.5f)
            outTransfo.Flip = other.Flip;
        else
            outTransfo.Flip = Flip;
    }

    glm::vec2 Offset = { 0.0f, 0.0f };
    glm::vec2 Scaling = { 1.0f, 1.0f };
    double RotAngle = 0.0f;
    SDL_Point RotCenter = { 0, 0 };
    SDL_RendererFlip Flip = SDL_FLIP_NONE;
};

#endif // !M_TRANSFORM
