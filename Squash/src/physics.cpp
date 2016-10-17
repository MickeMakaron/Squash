#include "physics.h"
#include "CommonMath.h"

#include "Ball.h"
#include "ScenePlane.h"

#include <cassert>
#include <iostream>
#define DEBUG_OUTPUT

void applyMagnusForce(Ball& ball, float dt)
{
    static const float AIR_DENSITY = 1.21f;

    const float FORCE_MAGNITUDE = 0.5f * AIR_DENSITY * pow(ball.getRadius(), 3) * PI;
    const sf::Vector3f FORCE_DIRECTION = cross(-ball.getVelocity(), ball.getAngularVelocity());

    const sf::Vector3f ACCELERATION = FORCE_MAGNITUDE * FORCE_DIRECTION  / ball.getMass();
    ball.accelerate(ACCELERATION * dt);
}

float calcDeltaSpeed_LineOfAction(float m1, float m2, float v1, float v2, float e)
{
	return (m1 - e*m2) * v1 / (m1 + m2) + (1 + e)*m2 * v2 / (m1 + m2) - v1;
}

float calcDeltaSpeed_LineOfAction(float v, float e)
{
	return -e*v - v;
}

float calcDeltaSpeed_Friction_NoRoll(float v_n, float u_p, float v_p, float friction)
{
	return (u_p - v_p) * friction;
}

float calcDeltaSpeed_Friction_WithRoll(sf::Vector3f v_plane_pre, sf::Vector3f r, sf::Vector3f w_pre, sf::Vector3f friction_direction)
{
	return (2.f / 7.f) * length(v_plane_pre - cross(r, w_pre));
}

bool findCollisionPoint(const Ball& ball, const ScenePlane& plane, float dt, float& timeUntilCollision)
{
    float ballSpeedLoa = dot(ball.getVelocity(), plane.getNormal());

    float distanceTravelledLoa = ballSpeedLoa * dt;

	sf::Vector3f posA = ball.getPosition();
	float distanceA = dot(posA, plane.getNormal()) - plane.getD();

	if(std::fabs(distanceA) < ball.getRadius())
    {
        timeUntilCollision = -(ball.getRadius() - std::fabs(distanceA)) / std::fabs(ballSpeedLoa);
        if(std::isfinite(timeUntilCollision))
        {
            if(timeUntilCollision > dt)
                return false;
        }
        else
            return false;
        return true;
    }

    sf::Vector3f posB = ball.getPosition() + ball.getVelocity() * dt;
    float distanceB = dot(posB, plane.getNormal()) - plane.getD();
    float intersectionDepth = 0.f;
    if(distanceA < 0.f)
    {
        if(distanceTravelledLoa <= 0.f)
            return false;

        intersectionDepth = ball.getRadius() + distanceB;
    }
    else if(distanceA >= 0.f)
    {
        if(distanceTravelledLoa >= 0.f)
            return false;

        intersectionDepth = ball.getRadius() - distanceB;
    }

    timeUntilCollision = (std::fabs(distanceTravelledLoa) - intersectionDepth) / std::fabs(ballSpeedLoa);
    if(std::isfinite(timeUntilCollision))
    {
        if(timeUntilCollision > dt)
            return false;
    }
    else
        return false;

    return true;
}


void handleCollisions(Ball& ball, const std::vector<ScenePlane>& planes, float& dt, std::vector<size_t>& collidingPlaneIndices)
{
    float minTimeUntilCollision = std::numeric_limits<float>::max();
    std::vector<size_t> planeIndices;
    for(size_t i = 0; i < planes.size(); i++)
    {
        float timeUntilCollision;
        if(findCollisionPoint(ball, planes[i], dt, timeUntilCollision))
        {
            float d = minTimeUntilCollision - timeUntilCollision;
            if(std::fabs(d) < 0.00001f)
            {
                minTimeUntilCollision = std::min(minTimeUntilCollision, timeUntilCollision);
                planeIndices.push_back(i);
            }
            else if(timeUntilCollision < minTimeUntilCollision)
            {
                minTimeUntilCollision = timeUntilCollision;
                planeIndices.clear();
                planeIndices.push_back(i);
            }
        }
    }

    if(planeIndices.empty())
        return;

    assert(minTimeUntilCollision <= dt);

    ball.move(minTimeUntilCollision);
    ball.rotate(minTimeUntilCollision);
    applyMagnusForce(ball, minTimeUntilCollision);
    dt -= minTimeUntilCollision;

    for(size_t i : planeIndices)
        handleCollision(ball, planes[i], dt);

    collidingPlaneIndices.insert(collidingPlaneIndices.end(), planeIndices.begin(), planeIndices.end());
    handleCollisions(ball, planes, dt, collidingPlaneIndices);
}

bool handleCollision(Ball& ball, const ScenePlane& plane, float& dt)
{
	static const float COLLISION_FACTOR = 0.76f;
	static const float FRICTION_FACTOR = 0.11f;
	static const float FRICTION_FACTOR_ROLL = 0.056f;


//	float distance = dot(ball.getPosition(), plane.getNormal()) - plane.getD();

	// If close enough, COLLIDE!
//	if(std::fabs(distance) <= ball.getRadius())
	{
//		// Ball might have overshot the plane by a little bit, so we need to
//		// adjust speed and position to where the ball should have hit the plane.
//		float overshoot = ball.getRadius() - distance;
//
//		float velocityLength = length(ball.getVelocity());//dot(ball.getVelocity(), -plane.getNormal());
//		float assumedAcceleration = (velocityLength - length(ball.getPreviousVelocity())) / dt;
//		float distanceTravelledInOneStep = velocityLength * dt;
//
//		float actualTime = (distanceTravelledInOneStep - overshoot) / velocityLength;
//		if(!std::isfinite(actualTime))
//            actualTime = 0.f;
//
//
//
//		sf::Vector3f normVel = normalize(ball.getVelocity());
//		sf::Vector3f actualVelocity = ball.getPreviousVelocity() + normalize(ball.getVelocity() - ball.getPreviousVelocity()) * assumedAcceleration * actualTime;
//
//		ball.accelerate((actualVelocity - ball.getVelocity()));
//        ball.move(-normVel * (overshoot));


		// CP = Collision point
		sf::Vector3f vectorCP = plane.getNormal() * (-ball.getRadius());
		sf::Vector3f spinVelocityAtCP = cross(ball.getAngularVelocity(), vectorCP);
		sf::Vector3f velocityAtCP = ball.getVelocity() + spinVelocityAtCP;

		// Important directions
		sf::Vector3f lineOfAction = plane.getNormal();
		sf::Vector3f vectorFriction = cross(normalize(cross(velocityAtCP - plane.getVelocity(), lineOfAction)), lineOfAction);

		// If the ball is pretty much already rolling, recalculate the normal
		if (length2(vectorFriction) <= 1.0e-6f)
		{
			vectorFriction = cross(normalize(cross(ball.getVelocity() - plane.getVelocity(), lineOfAction)), lineOfAction);
		}

		// Collapse into one dimension
		float pre_vel_p = dot(ball.getVelocity(), lineOfAction);
		float pre_vel_n = dot(ball.getVelocity(), vectorFriction);

		// (Future note: velocity of what, the ball itself or the collision point? Hmmm)
		float delta_vel_p;
		if(plane.isMassive())
		{
			delta_vel_p = calcDeltaSpeed_LineOfAction(pre_vel_p, COLLISION_FACTOR);
		}
		else
		{
			delta_vel_p = calcDeltaSpeed_LineOfAction(ball.getMass(), plane.getMass(), pre_vel_p, dot(plane.getVelocity(), lineOfAction), COLLISION_FACTOR);
		}

		if (fabs(delta_vel_p) <= 1.0f && dot(lineOfAction, sf::Vector3f(0, 0, 1)) >= 0.9f)
		{
			delta_vel_p = -pre_vel_p;
			ball.setGrounded(true);

			sf::Vector3f v = ball.getVelocity() + plane.getNormal() * float(fabs(pre_vel_p));
			sf::Vector3f r = -plane.getNormal() * ball.getRadius();
			float slideTime = (2.f/7.f)*length(v - cross(r, ball.getAngularVelocity())) / (FRICTION_FACTOR * 9.82f);

			float delta_vel_n;
			sf::Vector3f resultingVelocity;
			sf::Vector3f resultingAngularVelocity;

			// If slideTime is greater than dt, keep sliding
			if(slideTime > dt)
			{
				delta_vel_n = FRICTION_FACTOR * 9.82f * dt;
				resultingAngularVelocity = cross(vectorFriction, lineOfAction) * 5.f * FRICTION_FACTOR * 9.82f / (2 * ball.getRadius()) * dt;

				#ifdef DEBUG_OUTPUT

				std::cout << "-----------\nCONTACT NO ROLL!\nVel: (" <<
					resultingVelocity.x << ", " << resultingVelocity.y << ", " << resultingVelocity.z << ")\nRot: (" <<
					resultingAngularVelocity.x << ", " << resultingAngularVelocity.y << ", " << resultingAngularVelocity.z << ")" << std::endl;

				#endif // DEBUG_OUTPUT
			}
			// Otherwise, start rolling!
			else
			{
				// If the velocity is very small, we are practically stationary (this is to avoid having infinite tiny movements when the ball should be still)
				if (length2(v) <= 1.0e-3f)
				{
					delta_vel_n = 0.f;
					ball.accelerate(-v);
					resultingAngularVelocity = -ball.getAngularVelocity();
				}
				else
				{
					vectorFriction = cross(normalize(cross(ball.getVelocity() - plane.getVelocity(), lineOfAction)), lineOfAction);

					delta_vel_n = FRICTION_FACTOR_ROLL * 9.82f * dt;
					resultingAngularVelocity = cross(vectorFriction, lineOfAction) * delta_vel_n / ball.getRadius() * dt;
				}

				#ifdef DEBUG_OUTPUT

				std::cout << "-----------\nCONTACT ROLL!\nVel: (" <<
					resultingVelocity.x << ", " << resultingVelocity.y << ", " << resultingVelocity.z << ")\nRot: (" <<
					resultingAngularVelocity.x << ", " << resultingAngularVelocity.y << ", " << resultingAngularVelocity.z << ")" << std::endl;

				#endif // DEBUG_OUTPUT
			}

			resultingVelocity = lineOfAction * delta_vel_p + vectorFriction * delta_vel_n;

			ball.accelerate(resultingVelocity);
			ball.accelerateAngular(resultingAngularVelocity);
			return true;
		}
		else
		{
			ball.setGrounded(false);
		}

		// We must compare the resulting speed of both Roll and No Roll conditions
		// to find which gives the smallest result (which is the one we want)
		float delta_vel_n_NoRoll = calcDeltaSpeed_Friction_NoRoll(pre_vel_n, delta_vel_p, pre_vel_p, FRICTION_FACTOR);
		float delta_vel_n_Roll	 = calcDeltaSpeed_Friction_WithRoll(ball.getVelocity() + plane.getNormal() * float(fabs(pre_vel_p)), -plane.getNormal() * ball.getRadius(), ball.getAngularVelocity(), vectorFriction);
		//float post_vel_n_NoRoll = calcExitSpeed_Friction_NoRoll(pre_vel_n, post_vel_p, pre_vel_p, FRICTION_FACTOR);
		//float post_vel_n_Roll	= calcExitSpeed_Friction_WithRoll(ball.getVelocity() + plane.getNormal() * fabs(pre_vel_p), -plane.getNormal() * ball.getRadius(), ball.getAngularVelocity(), vectorFriction);

		//float post_vel_n_Roll	= calcExitSpeed_Friction_WithRoll(pre_vel_n, dot(spinVelocityAtCP, vectorFriction));

		sf::Vector3f resultingVelocity;
		sf::Vector3f resultingAngularVelocity;
		if (fabs(delta_vel_n_NoRoll) < fabs(delta_vel_n_Roll))
		{
			// No roll condition!

			resultingVelocity = lineOfAction * (delta_vel_p) + vectorFriction * (delta_vel_n_NoRoll);

			//if(length2(vectorFriction) >= 0.01)
				resultingAngularVelocity = 5.f * FRICTION_FACTOR * (delta_vel_p) / (2.f * ball.getRadius()) * cross(-lineOfAction, vectorFriction);

            #ifdef DEBUG_OUTPUT

			std::cout << "-----------\nNO ROLL!\nVel: (" <<
				resultingVelocity.x << ", " << resultingVelocity.y << ", " << resultingVelocity.z << ")\nRot: (" <<
				resultingAngularVelocity.x << ", " << resultingAngularVelocity.y << ", " << resultingAngularVelocity.z << ")" << std::endl;

            #endif // DEBUG_OUTPUT
		}
		else
		{
			// Roll condition!

			resultingVelocity = lineOfAction * (delta_vel_p) + vectorFriction * (delta_vel_n_Roll);

			// The cross is an assumption at this point!
			//resultingAngularVelocity = (delta_vel_n_Roll) / ball.getRadius() * normalize(cross(resultingVelocity, -lineOfAction));
			resultingAngularVelocity = cross(vectorFriction, lineOfAction) * (delta_vel_n_Roll) / ball.getRadius();

			#ifdef DEBUG_OUTPUT

			std::cout << "-----------\nROLL!\nVel: (" <<
				resultingVelocity.x << ", " << resultingVelocity.y << ", " << resultingVelocity.z << ")\nRot: (" <<
				resultingAngularVelocity.x << ", " << resultingAngularVelocity.y << ", " << resultingAngularVelocity.z << ")" << std::endl;

			#endif // DEBUG_OUTPUT
		}


		ball.accelerate(resultingVelocity);
		ball.accelerateAngular(resultingAngularVelocity);

		return true;
	}

	return false;
}
