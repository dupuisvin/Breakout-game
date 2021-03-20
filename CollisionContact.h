#ifndef M_COLLISION_CONTACT
#define M_COLLISION_CONTACT

#include "entt/fwd.hpp"
#include <glm/vec2.hpp>

namespace SDLEngine
{
    //Represents a collision contact between two objects with the minimal vector along one axis to separe the two objects
    struct CollisionContact
    {
        glm::vec2 CollVec = { 0.0f, 0.0f };
        bool Collision = false;
    };
}

#endif // !M_COLLISION_CONTACT
