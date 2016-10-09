#include "physics.h"
#include "CommonMath.h"

#include "Ball.h"
#include "ScenePlane.h"

bool handleCollision(Ball& ball, const ScenePlane& plane)
{
    static const float COLLISION_FACTOR = 0.8f;
    static const float FRICTION_FACTOR = 0.1f;

    const sf::Vector3f BALL_CENTER = ball.getPosition();
    const float BALL_RADIUS = ball.getRadius();
    const sf::Vector3f LOA = plane.getNormal();
    const float DISTANCE = dot(BALL_CENTER, LOA) - plane.getD();
    if(std::fabs(DISTANCE) < BALL_RADIUS)
    {
        const sf::Vector3f BALL_VELOCITY = ball.getVelocity();
        const sf::Vector3f PLANE_VELOCITY = plane.getVelocity();
        const float BALL_MASS = ball.getMass();
        const float PLANE_MASS = plane.getMass();
        const sf::Vector3f NORMAL = normalize(cross(cross(BALL_VELOCITY - PLANE_VELOCITY, LOA), LOA));

        const float BALL_LOA_SPEED_PRE = dot(BALL_VELOCITY, LOA);
        const float PLANE_LOA_SPEED_PRE = dot(PLANE_VELOCITY, LOA);

        const float BALL_LOA_SPEED_POST
            = BALL_LOA_SPEED_PRE * ((BALL_MASS - COLLISION_FACTOR * PLANE_MASS) / (BALL_MASS + PLANE_MASS))
            + PLANE_LOA_SPEED_PRE * ((1 + COLLISION_FACTOR) * PLANE_MASS / (BALL_MASS + PLANE_MASS));

        const float BALL_LOA_SPEED_DELTA = BALL_LOA_SPEED_POST - BALL_LOA_SPEED_PRE;

        // We don't care about the plane's post-collision velocity.

        // Assume rolling condition is true
        const float BALL_NORMAL_SPEED_DELTA = ((5.f / 7.f) - 1.f) * dot(BALL_VELOCITY, NORMAL);

        const sf::Vector3f BALL_ACCELERATION = NORMAL * BALL_NORMAL_SPEED_DELTA + LOA * BALL_LOA_SPEED_DELTA;
//        const sf::Vector3f BALL_ANGULAR_ACCELERATION = BALL_NORMAL_SPEED_DELTA / BALL_RADIUS;

        ball.move(LOA * (BALL_RADIUS - DISTANCE));
        ball.accelerate(BALL_ACCELERATION);


//        sf::Vector3f ballAcceleration(0.f, 0.f, 0.f);
//        sf::Vector3f ballAngularAcceleration(0.f, 0.f, 0.f);
//        ballAcceleration += (LOA + FRICTION_FACTOR * NORMAL) * BALL_LOA_SPEED_DELTA;
//        ballAngularAcceleration
//            += cross(LOA, NORMAL) * (BALL_RADIUS * FRICTION_FACTOR * BALL_LOA_SPEED_DELTA / (0.4f * BALL_RADIUS * BALL_RADIUS));

//        const float BALL_NORMAL_SPEED = dot(BALL_VELOCITY, NORMAL);
//        const float BALL_NORMAL_ACCELERATION = ((1.f / 1.4f) - 1.f) * BALL_NORMAL_SPEED;
//        ballAcceleration += NORMAL * BALL_NORMAL_ACCELERATION;

//        ball.move(LOA * (BALL_RADIUS - DISTANCE));
//        ball.accelerate(ballAcceleration);
//        ball.accelerateAngular(ballAngularAcceleration);

//        ball.accelerate(PLANE_NORMAL * length(ball.getVelocity()) * 1.5f);


        return true;
    }

    return false;
}
