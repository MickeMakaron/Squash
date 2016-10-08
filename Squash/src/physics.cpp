#include "physics.h"
#include "CommonMath.h"

#include "Ball.h"

void handleCollision(Ball& ball, const std::vector<Plane>& planes)
{
    for(const Plane& plane : planes)
    {
        const sf::Vector3f CENTER = ball.getPosition();
        const float RADIUS = ball.getRadius();
        const sf::Vector3f PLANE_NORMAL = plane.getNormal();
        float distance = dot(CENTER, PLANE_NORMAL) - plane.getD();
        if(std::fabs(distance) < RADIUS)
        {
            ball.move(PLANE_NORMAL * (RADIUS - distance));
            ball.accelerate(PLANE_NORMAL * length(ball.getVelocity()) * 1.5f);
        }
    }
}
