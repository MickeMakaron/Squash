#include "physics.h"
#include "CommonMath.h"

#include "Ball.h"
#include "ScenePlane.h"

#include <iostream>

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

        const sf::Vector3f BALL_SPIN_VELOCITY = cross(LOA, ball.getAngularVelocity()) * BALL_RADIUS;
        const sf::Vector3f BALL_VELOCITY = ball.getVelocity() + BALL_SPIN_VELOCITY;

        const sf::Vector3f PLANE_VELOCITY = plane.getVelocity();


        const float BALL_MASS = ball.getMass();
        const float PLANE_MASS = plane.getMass();
        const sf::Vector3f FRICTION_DIRECTION = normalize(cross(cross(BALL_VELOCITY - PLANE_VELOCITY, LOA), LOA));

        const float BALL_LOA_SPEED_PRE = dot(BALL_VELOCITY, LOA);
        const float PLANE_LOA_SPEED_PRE = dot(PLANE_VELOCITY, LOA);

        const float BALL_LOA_SPEED_POST = plane.isMassive() ?
              -COLLISION_FACTOR * BALL_LOA_SPEED_PRE
            : BALL_LOA_SPEED_PRE * ((BALL_MASS - COLLISION_FACTOR * PLANE_MASS) / (BALL_MASS + PLANE_MASS))
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
        sf::Vector3f ballAngularAcceleration(0.f, 0.f, 0.f);
        if(std::fabs(BALL_FRICTION_DIRECTION_SPEED_POST_NO_ROLL) < std::fabs(BALL_FRICTION_DIRECTION_SPEED_POST_ROLL))
        {
            // Roll condition false
            ballAcceleration += LOA * BALL_LOA_SPEED_DELTA + FRICTION_DIRECTION * BALL_FRICTION_DIRECTION_SPEED_DELTA_NO_ROLL;

//            if(length2(FRICTION_DIRECTION) > 0.01f)
            {
                ballAngularAcceleration += cross(-LOA, FRICTION_DIRECTION) * (FRICTION_FACTOR * BALL_LOA_SPEED_DELTA / (0.4f  * BALL_RADIUS));
//                ballAngularAcceleration = cross(-LOA, FRICTION_DIRECTION) * (FRICTION_FACTOR * BALL_LOA_SPEED_DELTA / (0.4f  * BALL_RADIUS)) - ball.getAngularVelocity();
            }
            std::cout << "NO ROLL" << std::endl;
        }
        else
        {
            // Roll condition true
            ballAcceleration += LOA * BALL_LOA_SPEED_DELTA + FRICTION_DIRECTION * BALL_FRICTION_DIRECTION_SPEED_DELTA_ROLL;

            const sf::Vector3f BALL_ANGULAR_VELOCITY = ball.getAngularVelocity();
            const sf::Vector3f NEW_FRICTION_DIRECTION = normalize(BALL_FRICTION_DIRECTION_VELOCITY_POST_ROLL);
//            ballAngularAcceleration = cross(-LOA, NEW_FRICTION_DIRECTION) * (BALL_FRICTION_DIRECTION_SPEED_POST_ROLL / BALL_RADIUS) - ball.getAngularVelocity();
            ballAngularAcceleration += cross(-LOA, FRICTION_DIRECTION) * (BALL_FRICTION_DIRECTION_SPEED_DELTA_ROLL / BALL_RADIUS);

//            if(ballAngularAcceleration.y < 0.f)
//                int ferp  = 0;
//            if((ball.getVelocity().x + ballAcceleration.x) > 0.f)
            if((ball.getAngularVelocity().y + ballAngularAcceleration.y) > 0.f)
                int flerp = 0;
            std::cout << "ROLL" << std::endl;
        }

        std::cout << "Pre linear v: " << ball.getVelocity().x << ", " << ball.getVelocity().y << ", " << ball.getVelocity().z << std::endl;
        std::cout << "Pre angular v: " << ball.getAngularVelocity().x << ", " << ball.getAngularVelocity().y << ", " << ball.getAngularVelocity().z << std::endl;
        std::cout << "Pre surface v: " << BALL_VELOCITY.x << ", " << BALL_VELOCITY.y << ", " << BALL_VELOCITY.z << std::endl;
        std::cout << "Linear Acc: " << ballAcceleration.x << ", " << ballAcceleration.y << ", " << ballAcceleration.z << std::endl;
        std::cout << "Angular Acc: " << ballAngularAcceleration.x << ", " << ballAngularAcceleration.y << ", " << ballAngularAcceleration.z << std::endl;
        ball.accelerate(ballAcceleration);
        ball.accelerateAngular(ballAngularAcceleration);

        return true;
    }

    return false;
}

float calcExitSpeed_LineOfAction(float m1, float m2, float v1, float v2, float e)
{
	return (m1 - e*m2) * v1 / (m1 + m2) + (1 + e)*m2 * v2 / (m1 + m2);
}

float calcExitSpeed_LineOfAction(float v, float e)
{
	return -e*v;
}

float calcExitSpeed_Friction_NoRoll(float v_n, float u_p, float v_p, float friction)
{
	return v_n + (u_p - v_p) * friction;
}

float calcExitSpeed_Friction_WithRoll(float v_n)
{
	return 5.f*v_n/7.f;
}

bool handleCollision2(Ball& ball, const ScenePlane& plane)
{
	static const float COLLISION_FACTOR = 0.8f;
	static const float FRICTION_FACTOR = 0.1f;

	float distance = dot(ball.getPosition(), plane.getNormal()) - plane.getD();

	// If close enough, COLLIDE!
	if(std::fabs(distance) <= ball.getRadius())
	{

		// CP = Collision point
		sf::Vector3f vectorCP = plane.getNormal() * (-ball.getRadius());
		sf::Vector3f velocityAtCP = ball.getVelocity() + cross(ball.getAngularVelocity(), vectorCP);

		// Important directions
		sf::Vector3f lineOfAction = plane.getNormal();
		sf::Vector3f vectorFriction = cross(normalize(cross(velocityAtCP - plane.getVelocity(), lineOfAction)), lineOfAction);

		// Collapse into one dimension
		float pre_vel_p = dot(velocityAtCP, lineOfAction);
		float pre_vel_n = dot(velocityAtCP, vectorFriction);

		// (Future note: velocity of what, the ball itself or the collision point? Hmmm)
		float post_vel_p;
		if(plane.isMassive())
		{
			post_vel_p = calcExitSpeed_LineOfAction(pre_vel_p, COLLISION_FACTOR);
		}
		else
		{
			post_vel_p = calcExitSpeed_LineOfAction(ball.getMass(), plane.getMass(), pre_vel_p, dot(plane.getVelocity(), lineOfAction), COLLISION_FACTOR);
		}

		// We must compare the resulting speed of both Roll and No Roll conditions
		// to find which gives the smallest result (which is the one we want)
		float post_vel_n_NoRoll = calcExitSpeed_Friction_NoRoll(pre_vel_n, post_vel_p, pre_vel_p, FRICTION_FACTOR);
		float post_vel_n_Roll	= calcExitSpeed_Friction_WithRoll(pre_vel_n);

		sf::Vector3f resultingVelocity;
		sf::Vector3f resultingAngularVelocity;
		if (fabs(post_vel_n_NoRoll) < fabs(post_vel_n_Roll))
		{
			// No roll condition!

			resultingVelocity = lineOfAction * post_vel_p + vectorFriction * post_vel_n_NoRoll;

			//if(length2(vectorFriction) >= 0.01)
				resultingAngularVelocity = 5 * FRICTION_FACTOR * (post_vel_p - pre_vel_p) / (2 * ball.getRadius()) * cross(-lineOfAction, vectorFriction);

			std::cout << "-----------\nNO ROLL!\nVel: (" <<
				resultingVelocity.x << ", " << resultingVelocity.y << ", " << resultingVelocity.z << ")\nRot: (" <<
				resultingAngularVelocity.x << ", " << resultingAngularVelocity.y << ", " << resultingAngularVelocity.z << ")" << std::endl;
		}
		else
		{
			// Roll condition!

			resultingVelocity = lineOfAction * post_vel_p + vectorFriction * post_vel_n_Roll;

			// The cross is an assumption at this point!
			resultingAngularVelocity = (pre_vel_n - post_vel_n_Roll) / ball.getRadius() * normalize(cross(resultingVelocity, lineOfAction)); 

			std::cout << "-----------\nROLL!\nVel: (" <<
				resultingVelocity.x << ", " << resultingVelocity.y << ", " << resultingVelocity.z << ")\nRot: (" <<
				resultingAngularVelocity.x << ", " << resultingAngularVelocity.y << ", " << resultingAngularVelocity.z << ")" << std::endl;
		}


		ball.accelerate(resultingVelocity - ball.getVelocity());
		ball.accelerateAngular(resultingAngularVelocity - ball.getAngularVelocity());

		return true;
	}

	return false;
}
