#include "physics.h"
#include "CommonMath.h"

#include "Ball.h"
#include "ScenePlane.h"

#include <iostream>

sf::Vector3f ballAngularVelocity(0.f, -10.f, 0.f);
//sf::Vector3f ballAngularVelocity(-1.f, 0.f, 0.f);
bool handleCollision(Ball& ball, const ScenePlane& plane)
{
    static const float COLLISION_FACTOR = 0.6f;
    static const float FRICTION_FACTOR = 0.1f;

    const sf::Vector3f BALL_CENTER = ball.getPosition();
    const float BALL_RADIUS = ball.getRadius();
    const sf::Vector3f LOA = plane.getNormal();
    const float DISTANCE = dot(BALL_CENTER, LOA) - plane.getD();
    if(std::fabs(DISTANCE) < BALL_RADIUS)
    {
        ball.move(LOA * (BALL_RADIUS - DISTANCE));

        const sf::Vector3f BALL_SPIN_VELOCITY = cross(LOA, ballAngularVelocity) * BALL_RADIUS;
        const sf::Vector3f BALL_VELOCITY = ball.getVelocity() + BALL_SPIN_VELOCITY;

        const sf::Vector3f PLANE_VELOCITY = plane.getVelocity();
        if(length2(BALL_VELOCITY - PLANE_VELOCITY) < 1.f)
        {
            ball.accelerate(-ball.getVelocity());
            return true;
        }

        const float BALL_MASS = ball.getMass();
        const float PLANE_MASS = plane.getMass();
        const sf::Vector3f FRICTION_DIRECTION_UNNORMALIZED = cross(cross(BALL_VELOCITY - PLANE_VELOCITY, LOA), LOA);
        const sf::Vector3f FRICTION_DIRECTION = length2(FRICTION_DIRECTION_UNNORMALIZED) < 0.01f ? sf::Vector3f() : normalize(FRICTION_DIRECTION_UNNORMALIZED);

        const float BALL_LOA_SPEED_PRE = dot(BALL_VELOCITY, LOA);
        const float PLANE_LOA_SPEED_PRE = dot(PLANE_VELOCITY, LOA);

        const float BALL_LOA_SPEED_POST
            = BALL_LOA_SPEED_PRE * ((BALL_MASS - COLLISION_FACTOR * PLANE_MASS) / (BALL_MASS + PLANE_MASS))
            + PLANE_LOA_SPEED_PRE * ((1 + COLLISION_FACTOR) * PLANE_MASS / (BALL_MASS + PLANE_MASS));

        const float BALL_LOA_SPEED_DELTA = BALL_LOA_SPEED_POST - BALL_LOA_SPEED_PRE;

        // We don't care about the plane's post-collision velocity.


        const float BALL_FRICTION_DIRECTION_SPEED_PRE = dot(BALL_VELOCITY, FRICTION_DIRECTION);
        const sf::Vector3f BALL_FRICTION_DIRECTION_VELOCITY_PRE = FRICTION_DIRECTION * BALL_FRICTION_DIRECTION_SPEED_PRE;

        const float BALL_FRICTION_DIRECTION_SPEED_DELTA_NO_ROLL = BALL_LOA_SPEED_DELTA * FRICTION_FACTOR;
        const sf::Vector3f BALL_FRICTION_DIRECTION_VELOCITY_POST_NO_ROLL = BALL_FRICTION_DIRECTION_VELOCITY_PRE + FRICTION_DIRECTION * BALL_FRICTION_DIRECTION_SPEED_DELTA_NO_ROLL;
        const float BALL_FRICTION_DIRECTION_SPEED_POST_NO_ROLL = length(BALL_FRICTION_DIRECTION_VELOCITY_POST_NO_ROLL);

        const float BALL_FRICTION_DIRECTION_SPEED_DELTA_ROLL = ((5.f / 7.f) - 1.f) * BALL_FRICTION_DIRECTION_SPEED_PRE;
        const sf::Vector3f BALL_FRICTION_DIRECTION_VELOCITY_POST_ROLL = BALL_FRICTION_DIRECTION_VELOCITY_PRE + FRICTION_DIRECTION * BALL_FRICTION_DIRECTION_SPEED_DELTA_ROLL;
        const float BALL_FRICTION_DIRECTION_SPEED_POST_ROLL = length(BALL_FRICTION_DIRECTION_VELOCITY_POST_ROLL);

        sf::Vector3f ballAcceleration(0.f, 0.f, 0.f);
        if(BALL_FRICTION_DIRECTION_SPEED_POST_NO_ROLL < BALL_FRICTION_DIRECTION_SPEED_POST_ROLL)
        {
            // Roll condition false
            ballAcceleration += LOA * BALL_LOA_SPEED_DELTA + FRICTION_DIRECTION * BALL_FRICTION_DIRECTION_SPEED_DELTA_NO_ROLL;

            if(length2(FRICTION_DIRECTION) > 0.01f)
            {
                ballAngularVelocity = cross(-LOA, FRICTION_DIRECTION) * (FRICTION_FACTOR * BALL_LOA_SPEED_DELTA / (0.4f  * BALL_RADIUS));
            }
            std::cout << "NO ROLL" << std::endl;
        }
        else
        {
            // Roll condition true
            ballAcceleration += LOA * BALL_LOA_SPEED_DELTA + FRICTION_DIRECTION * BALL_FRICTION_DIRECTION_SPEED_DELTA_ROLL;

            ballAngularVelocity = FRICTION_DIRECTION * (BALL_FRICTION_DIRECTION_SPEED_POST_ROLL / BALL_RADIUS);

            std::cout << "ROLL" << std::endl;
        }


        ball.accelerate(ballAcceleration);

        return true;
    }

    return false;
}

float calcExitSpeed(float m1, float m2, float v1, float v2, float e)
{
	
}

bool handleCollision2(Ball& ball, const ScenePlane& plane)
{
	static const float COLLISION_FACTOR = 0.8f;
	static const float FRICTION_FACTOR = 0.1f;

	
	// CP = Collision point
	sf::Vector3f vectorCP = plane.getNormal() * (-ball.getRadius());
	sf::Vector3f velocityAtCP = ball.getVelocity() + cross(ballAngularVelocity, vectorCP);

	// Important directions
	sf::Vector3f lineOfAction = plane.getNormal();
	sf::Vector3f vectorFriction = cross(normalize(cross(velocityAtCP, lineOfAction)), lineOfAction);

	// Collapse into one dimension
	float velCP_p = dot(velocityAtCP, lineOfAction);
	float vel_n = dot(velocityAtCP, vectorFriction);

	


}