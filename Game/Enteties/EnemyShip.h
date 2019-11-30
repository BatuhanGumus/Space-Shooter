#pragma once
#include "../../Engine/Vector2.h"
#include "../../Engine/Sprite.h"
#include "Ship.h"

class EnemyShip : public Ship
{
public:
	EnemyShip(const char* name, Sprite* sprite, Vector2* pos, Vector2* scale);
	~EnemyShip();

	void Update() override;
	void Damage(int dmg);

	static int EnemyShipCount;
private:
	
	Vector2 distToText;
	static  Sprite* laserSprite;
	double timeSinceShot;
	double randTime;
	void RandTimeForShot();
};

