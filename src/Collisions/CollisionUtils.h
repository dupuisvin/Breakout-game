#ifndef M_COLLISION_UTILS
#define M_COLLISION_UTILS

#include "CollisionContact.h"
#include "CollisionCircle.h"
#include "CollisionAABB.h"
#include <glm/geometric.hpp>

namespace SDLEngine
{
    class CollisionUtils
    {
    public:

        // Distance squared between two 2D positions
        static inline float DistanceSquared(float x1, float y1, float x2, float y2)
        {
            const float dy = y2 - y1;
            const float dx = x2 - x1;
            return dy * dy + dx * dx;
        }

        // Get the closest point on an AABB from a circle
        static glm::vec2 GetClosestPtOnRect(glm::vec2 circlePos, const CollisionCircle& circle, glm::vec2 rectPos, const CollisionAABB& rect)
        {
            glm::vec2 retPt;

            //Find closest x offset
            if (circlePos.x < rectPos.x)
                retPt.x = rectPos.x;
            else if (circlePos.x > rectPos.x + rect.Width)
                retPt.x = rectPos.x + rect.Width;
            else
                retPt.x = circlePos.x;

            //Find closest y offset
            if (circlePos.y < rectPos.y)
                retPt.y = rectPos.y;
            else if (circlePos.y > rectPos.y + rect.Height)
                retPt.y = rectPos.y + rect.Height;
            else
                retPt.y = circlePos.y;

            return retPt;
        }

        // Return true if the specified position (px,py) is inside the specified AABB
        // px, py the position to validate
        // rx, ry the AABB top left corner position
        // rw, rh the width and height of the AABB
        static inline bool IsInsideRect(float px, float py, float rx, float ry, float rw, float rh)
        {
            if (px < rx      ||
                px > rx + rw ||
                py < ry      ||
                py > ry + rh )
                return false;
            return true;
        }

        // Return true if the specified position (px,py) is inside the specified AABB
        // pos     the position to validate
        // collPos the AABB top left corner position
        // coll    the AABB
        static inline bool IsInsideRect(glm::vec2 pos, glm::vec2 collPos, const CollisionAABB &coll)
        {
            return IsInsideRect(pos.x, pos.y, collPos.x, collPos.y, coll.Width, coll.Height);
        }

        //Check for possible collision between a circle and an AABB and return the contact between them (if there is one)
        static CollisionContact Collision(glm::vec2 circlerCorner, const CollisionCircle& circle, glm::vec2 rectCorner, const CollisionAABB& rect)
        {
            CollisionContact contact;

            glm::vec2 circlePos = circlerCorner + circle.PosOffset;
            glm::vec2 rectPos = rectCorner + rect.PosOffset;

            //Closest point on collision box
            glm::vec2 pos = GetClosestPtOnRect(circlePos, circle, rectPos, rect);

            //If the closest point is inside the circle
            if (DistanceSquared(circlePos.x, circlePos.y, pos.x, pos.y) < circle.Radius * circle.Radius)
            {
                float nOffX, nOffY;

                float nLeft = rectPos.x - (circlePos.x + circle.Radius);
                float nRight = (rectPos.x + rect.Width) - (circlePos.x - circle.Radius);
                if (std::abs(nLeft) < std::abs(nRight))
                    nOffX = nLeft;
                else
                    nOffX = nRight;

                float nTop = rectPos.y - (circlePos.y + circle.Radius);
                float nBottom = (rectPos.y + rect.Height) - (circlePos.y - circle.Radius);
                if (std::abs(nTop) < std::abs(nBottom))
                    nOffY = nTop;
                else
                    nOffY = nBottom;

                contact.Collision = true;
                contact.CollVec = { nOffX, nOffY };
                contact.CollVec *= glm::vec2(1.001, 1.001);
            }

            return contact;
        }

        //Check for possible collision between two circles and return the contact between them (if there is one)
        static CollisionContact Collision(glm::vec2 c1Pos, CollisionCircle c1, glm::vec2 c2Pos, CollisionCircle c2)
        {
            CollisionContact contact;

            glm::vec2 c1Center = c1Pos + c1.PosOffset;
            glm::vec2 c2Center = c2Pos + c2.PosOffset;
            float totalRadiusSquared = c1.Radius + c2.Radius;
            totalRadiusSquared *= totalRadiusSquared;
            float dSquared = DistanceSquared(c1Center.x, c1Center.y, c2Center.x, c2Center.y);
            if (dSquared < totalRadiusSquared)
            {
                glm::vec2 v1 = glm::vec2(c1Center.x, c1Center.y);
                glm::vec2 v2 = glm::vec2(c2Center.x, c2Center.y);
                glm::vec2 n = glm::normalize(v1 - v2);
                n *= (c1.Radius + c2.Radius) - sqrt(dSquared);
                contact.CollVec = n * glm::vec2(1.001, 1.001);
                contact.Collision = true;
            }

            return contact;
        }


        //Check for possible collision between two AABB and return the contact between them (if there is one)
        static bool IsCollision(glm::vec2 c1Pos, CollisionAABB c1, glm::vec2 c2Pos, CollisionAABB c2)
        {
            return ((c1Pos.x < c2Pos.x + c2.Width && c1Pos.x + c1.Width > c2Pos.x) &&
                    (c1Pos.y < c2Pos.y + c2.Height && c1Pos.y + c1.Height > c2Pos.y));
        }
    };
}

#endif // !#define M_COLLISION_UTILS

