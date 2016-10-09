#include "physics.h"
#include "CommonMath.h"

#include "Ball.h"
#include "ScenePlane.h"

bool handleCollision(Ball& ball, const ScenePlane& plane)
{
    static const float COLLISION_FACTOR = 0.8f;

    const sf::Vector3f BALL_CENTER = ball.getPosition();
    const float BALL_RADIUS = ball.getRadius();
    const sf::Vector3f PLANE_NORMAL = plane.getNormal();
    const float DISTANCE = dot(BALL_CENTER, PLANE_NORMAL) - plane.getD();
    if(std::fabs(DISTANCE) < BALL_RADIUS)
    {
        const sf::Vector3f BALL_VELOCITY = ball.getVelocity();
        const sf::Vector3f PLANE_VELOCITY = plane.getVelocity();
        const float BALL_MASS = ball.getMass();
        const float PLANE_MASS = plane.getMass();

        const float BALL_LOA_SPEED_PRE = dot(BALL_VELOCITY, PLANE_NORMAL);
        const float PLANE_LOA_SPEED_PRE = dot(PLANE_VELOCITY, PLANE_NORMAL);

        const float BALL_LOA_SPEED_POST
            = BALL_LOA_SPEED_PRE * ((BALL_MASS - COLLISION_FACTOR * PLANE_MASS) / (BALL_MASS + PLANE_MASS))
            + PLANE_LOA_SPEED_PRE * ((1 + COLLISION_FACTOR) * PLANE_MASS / (BALL_MASS + PLANE_MASS));

        // We don't care about the plane's post-collision velocity.

        const sf::Vector3f BALL_ACCELERATION = PLANE_NORMAL * (BALL_LOA_SPEED_POST - BALL_LOA_SPEED_PRE);

        ball.move(PLANE_NORMAL * (BALL_RADIUS - DISTANCE));
        ball.accelerate(BALL_ACCELERATION);
//        ball.accelerate(PLANE_NORMAL * length(ball.getVelocity()) * 1.5f);


        return true;
    }

    return false;
}
