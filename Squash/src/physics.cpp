#include "physics.h"
#include "CommonMath.h"

#include "Ball.h"
#include "ScenePlane.h"

#include <iostream>

sf::Vector3f ballAngularVelocity(0.f, -2.f, 0.f);
//sf::Vector3f ballAngularVelocity(-1.f, 0.f, 0.f);
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
        const sf::Vector3f BALL_SPIN_VELOCITY = cross(LOA, ballAngularVelocity) * BALL_RADIUS;
        const sf::Vector3f BALL_VELOCITY = ball.getVelocity() + BALL_SPIN_VELOCITY;
//        const sf::Vector3f BALL_SURFACE_VELOCITY = BALL_VELOCITY + cross(LOA, ball.getAngularVelocity()) * BALL_RADIUS;
        const sf::Vector3f PLANE_VELOCITY = plane.getVelocity();
        const float BALL_MASS = ball.getMass();
        const float PLANE_MASS = plane.getMass();
        const sf::Vector3f NORMAL_UNNORMALIZED = cross(cross(BALL_VELOCITY - PLANE_VELOCITY, LOA), LOA);
        const sf::Vector3f NORMAL = length2(NORMAL_UNNORMALIZED) < 0.01f ? sf::Vector3f() : normalize(NORMAL_UNNORMALIZED);
//        const sf::Vector3f NORMAL = normalize(cross(cross(BALL_SURFACE_VELOCITY - PLANE_VELOCITY, LOA), LOA));

        const float BALL_LOA_SPEED_PRE = dot(BALL_VELOCITY, LOA);
        const float PLANE_LOA_SPEED_PRE = dot(PLANE_VELOCITY, LOA);

        const float BALL_LOA_SPEED_POST
            = BALL_LOA_SPEED_PRE * ((BALL_MASS - COLLISION_FACTOR * PLANE_MASS) / (BALL_MASS + PLANE_MASS))
            + PLANE_LOA_SPEED_PRE * ((1 + COLLISION_FACTOR) * PLANE_MASS / (BALL_MASS + PLANE_MASS));

        const float BALL_LOA_SPEED_DELTA = BALL_LOA_SPEED_POST - BALL_LOA_SPEED_PRE;

        // We don't care about the plane's post-collision velocity.


        const float BALL_NORMAL_SPEED_PRE = dot(BALL_VELOCITY, NORMAL);
        const sf::Vector3f BALL_NORMAL_VELOCITY_PRE = NORMAL * BALL_NORMAL_SPEED_PRE;

        const float BALL_NORMAL_SPEED_DELTA_NO_ROLL = BALL_LOA_SPEED_DELTA * FRICTION_FACTOR;
        const sf::Vector3f BALL_NORMAL_VELOCITY_POST_NO_ROLL = BALL_NORMAL_VELOCITY_PRE + NORMAL * BALL_NORMAL_SPEED_DELTA_NO_ROLL;
        const float BALL_NORMAL_SPEED_POST_NO_ROLL = length(BALL_NORMAL_VELOCITY_POST_NO_ROLL);

        const float BALL_NORMAL_SPEED_DELTA_ROLL = ((5.f / 7.f) - 1.f) * BALL_NORMAL_SPEED_PRE;
        const sf::Vector3f BALL_NORMAL_VELOCITY_POST_ROLL = BALL_NORMAL_VELOCITY_PRE + NORMAL * BALL_NORMAL_SPEED_DELTA_ROLL;
        const float BALL_NORMAL_SPEED_POST_ROLL = length(BALL_NORMAL_VELOCITY_POST_ROLL);

        sf::Vector3f ballAcceleration(0.f, 0.f, 0.f);
        sf::Vector3f ballAngularAcceleration(0.f, 0.f, 0.f);
        const float BALL_SPEED = dot(ball.getVelocity(), NORMAL);
        const float SPIN_SPEED = dot(BALL_SPIN_VELOCITY, NORMAL);
        if(!(BALL_SPEED > SPIN_SPEED - 0.01f && BALL_SPEED < SPIN_SPEED + 0.01f))
//        if(BALL_NORMAL_SPEED_POST_NO_ROLL < BALL_NORMAL_SPEED_POST_ROLL)
        {
            // Roll condition false
            ballAcceleration += LOA * BALL_LOA_SPEED_DELTA + NORMAL * BALL_NORMAL_SPEED_DELTA_NO_ROLL;
//            ballAngularAcceleration

            if(length2(NORMAL) > 0.01f)
            {
                ballAngularVelocity
    //                = cross(-LOA, NORMAL) * (FRICTION_FACTOR * BALL_LOA_SPEED_DELTA / (0.4f  * BALL_RADIUS));
                    = cross(-LOA, NORMAL) * (FRICTION_FACTOR * BALL_LOA_SPEED_DELTA / (0.4f  * BALL_RADIUS));
            }
            std::cout << "NO ROLL" << std::endl;
        }
        else
        {
            // Roll condition true
            ballAcceleration += LOA * BALL_LOA_SPEED_DELTA;
            const sf::Vector3f NORMAL_ROLL = normalize(cross(cross(ball.getVelocity() - PLANE_VELOCITY, LOA), LOA));
            ballAcceleration += NORMAL_ROLL * ((5.f / 7.f) - 1.f) * dot(ball.getVelocity(), NORMAL_ROLL);

//            const sf::Vector3f BALL_SPIN_DIRECTION = normalize(BALL_SPIN_VELOCITY);
//            ballAngularAcceleration = BALL_SPIN_DIRECTION * (BALL_NORMAL_SPEED_DELTA_ROLL / BALL_RADIUS);
//            ballAngularAcceleration += ballAngularVelocity + BALL_SPIN_DIRECTION * (BALL_NORMAL_SPEED_DELTA_ROLL / BALL_RADIUS);

            ballAngularVelocity = NORMAL_ROLL * (BALL_NORMAL_SPEED_POST_ROLL / BALL_RADIUS);
//            if(length2(NORMAL) > 0.01f)
//            {
//                ballAngularVelocity = NORMAL * (BALL_NORMAL_SPEED_POST_ROLL / BALL_RADIUS);
//            }
            std::cout << "ROLL" << std::endl;
        }


        ball.move(LOA * (BALL_RADIUS - DISTANCE));
        ball.accelerate(ballAcceleration);
        ballAngularVelocity += ballAngularAcceleration;
//        ball.accelerateAngular(BALL_ANGULAR_ACCELERATION);


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

bool handleCollision2(Ball& ball, const ScenePlane& plane)
{
	static const float COLLISION_FACTOR = 0.8f;
	static const float FRICTION_FACTOR = 0.1f;

	
	// Collision point
	sf::Vector3f vectorCP = plane.getNormal() * ball.getRadius();

	sf::Vector3f velocityCP = ball.getVelocity() + cross(ballAngularVelocity, vectorCP);



}