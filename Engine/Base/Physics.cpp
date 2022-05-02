#include "Physics.h"
#include <cmath>
#include "GameTime.h"
#include <algorithm>

using namespace ArtemisEngine;

std::vector<RigidBody*> Physics::bodies;
std::queue<RigidBody*> Physics::bodiesToAdd;
//std::queue<RigidBody*> Physics::bodiesToRemove;

Physics::Physics()
{
}

Physics::~Physics()
{
}

void Physics::Update()
{
	for (int i = 0; i < bodies.size(); i++)
	{
		ApplyPhysics(bodies[i]);
	}
}

void Physics::ApplyPhysics(RigidBody* body)
{
	ApplyRules(body);
	*body->gameObject->transform->position += body->velocity;
	CheckCollision(body);
}

void Physics::ApplyRules(RigidBody* body)
{
	if (body->gameObject->type == Static)
	{
		return;
	}

	if (body->useGravity == true)
	{
		body->velocity.y += _gravity * GameTime::fixedDeltaTime;
	}

	if (Vector2::Magnitude(body->velocity) < body->drag  * GameTime::fixedDeltaTime)
	{
		body->velocity = 0;
	}
	else
	{
		body->velocity -= Vector2::Normalize(body->velocity) * body->drag  * GameTime::fixedDeltaTime;
	}

	if (body->maxSpeed != 0 && Vector2::Magnitude(body->velocity) > body->maxSpeed)
	{
		body->velocity = Vector2::Normalize(body->velocity) * body->maxSpeed;
	}
}

void Physics::CheckCollision(RigidBody* body)
{
	auto mainPosX = body->gameObject->transform->position->x;
    auto mainPosY = body->gameObject->transform->position->y;
    auto mainToRight = body->collider->width * body->gameObject->transform->scale->x / 2;
    auto mainToUp = body->collider->height * body->gameObject->transform->scale->y / 2;

    auto collided = false;

	for (int i = 0; i < bodies.size(); i++)
	{
		if (&bodies[i]->gameObject == &body->gameObject)
		{
			continue;
		}

        auto posX = bodies[i]->transform->position->x;
        auto posY = bodies[i]->transform->position->y;
        auto ToRight = bodies[i]->collider->width * bodies[i]->transform->scale->x / 2;
        auto ToUp = bodies[i]->collider->height * bodies[i]->transform->scale->y / 2;

		for (int j = 0; j < 4; j++)
		{
			Vector2** mainPoints = new Vector2*[2];

			switch (j)
			{
			case 0:
				mainPoints[0] = new Vector2(mainPosX + mainToRight, mainPosY + mainToUp); // top right
				mainPoints[1] = new Vector2(mainPosX + mainToRight, mainPosY - mainToUp); // bottom right
				break;
			case 1:
				mainPoints[0] = new Vector2(mainPosX + mainToRight, mainPosY - mainToUp); // bottom right
				mainPoints[1] = new Vector2(mainPosX - mainToRight, mainPosY - mainToUp); // bottom left
				break;
			case 2:
				mainPoints[0] = new Vector2(mainPosX - mainToRight, mainPosY - mainToUp); // bottom left
				mainPoints[1] = new Vector2(mainPosX - mainToRight, mainPosY + mainToUp); // top left
				break;
			case 3:
				mainPoints[0] = new Vector2(mainPosX - mainToRight, mainPosY + mainToUp); // top left
				mainPoints[1] = new Vector2(mainPosX + mainToRight, mainPosY + mainToUp); // top right
				break;
			}

            auto A1 = mainPoints[1]->y - mainPoints[0]->y;
            auto B1 = mainPoints[0]->x - mainPoints[1]->x;
            auto C1 = A1 * mainPoints[0]->x + B1 * mainPoints[0]->y;

			for (int k = 0; k < 4; k++)
			{
				Vector2** points = new Vector2*[2];

				switch (k)
				{
				case 0:
					points[0] = new Vector2(posX + ToRight, posY + ToUp); // top right
					points[1] = new Vector2(posX + ToRight, posY - ToUp); // bottom right
					break;
				case 1:
					points[0] = new Vector2(posX + ToRight, posY - ToUp); // bottom right
					points[1] = new Vector2(posX - ToRight, posY - ToUp); // bottom left
					break;
				case 2:
					points[0] = new Vector2(posX - ToRight, posY - ToUp); // bottom left
					points[1] = new Vector2(posX - ToRight, posY + ToUp); // top left
					break;
				case 3:
					points[0] = new Vector2(posX - ToRight, posY + ToUp); // top left
					points[1] = new Vector2(posX + ToRight, posY + ToUp); // top right
					break;
				}

                auto A2 = points[1]->y - points[0]->y;
                auto B2 = points[0]->x - points[1]->x;
                auto C2 = A2 * points[0]->x + B2 * points[0]->y;

                auto det = A1 * B2 - A2 * B1;

				if (det == 0)
				{
					// parallel
				}
				else
				{
                    auto x = (B2 * C1 - B1 * C2) / det;
                    auto y = (A1 * C2 - A2 * C1) / det;

					if (
						( abs(x - mainPoints[0]->x) <= 0.05 && abs(x - mainPoints[1]->x) <= 0.05 && 
						((mainPoints[0]->y <= y && y <= mainPoints[1]->y) || (mainPoints[1]->y <= y && y <= mainPoints[0]->y)))
						&&
						(abs(y - points[0]->y) <= 0.05 && abs(y - points[1]->y) <= 0.05 &&
						((points[0]->x <= x && x <= points[1]->x) || (points[1]->x <= x && x <= points[0]->x)))
						||
						(abs(y - mainPoints[0]->y) <= 0.05 && abs(y - mainPoints[1]->y) <= 0.05 &&
						((mainPoints[0]->x <= x && x <= mainPoints[1]->x) || (mainPoints[1]->x <= x && x <= mainPoints[0]->x)))
						&&
						(abs(x - points[0]->x) <= 0.05 && abs(x - points[1]->x) <= 0.05 &&
						((points[0]->y <= y && y <= points[1]->y) || (points[1]->y <= y && y <= points[0]->y)))
						)
					{
						if (collided == false)
						{
							body->CallCollision(bodies[i]->gameObject);
						}
						
						collided = true;
					}
				}

				for (int v = 0; v < 2; v++)
				{
					delete points[v];
				}

				if (collided == true)
				{
					break;
				}

			} // end of other objects corners

			for (int c = 0; c < 2; c++)
			{
				delete mainPoints[c];
			}

		} // end of main objects corners

	} // end of other bodies

	if (collided == false)
	{
		body->CallCollision(nullptr);
	}

}

void Physics::RigidBodyCreated(RigidBody *rigidBody)
{
    bodiesToAdd.push(rigidBody);
}


void Physics::RigidBodyDeleted(RigidBody *rigidBody)
{
    if (auto found = std::find(bodies.begin(), bodies.end(), rigidBody); found != bodies.end())
    {
        bodies.erase(found);
    }
}

void Physics::UpdateBodyList()
{
    while(!bodiesToAdd.empty())
    {
        bodies.push_back(bodiesToAdd.front());
        bodiesToAdd.pop();
    }
}

