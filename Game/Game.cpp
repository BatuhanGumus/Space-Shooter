#include "../Engine/ArtemisEngine.h"
#include "../Engine/FontManager.h"

#include "Enteties/Platform.h"
#include "Enteties/PlayerShip.h"
#include "Enteties/EnemyShip.h"


void ArtemisEngine::Awake()
{
	Sprite* dotSprite = new Sprite("Game/Assets/Textures/dot.png", 19, 19);
	Sprite* platformSprite = new Sprite("Game/Assets/Textures/platform.png", 80, 320);
	Sprite* playerShipSprite = new Sprite("Game/Assets/PNG/playerShip1_blue.png", 75, 99);
	Sprite* enemyShipSprite = new Sprite("Game/Assets/PNG/Enemies/enemyBlack3.png", 84, 103);

	FontManager::LoadFont("Cut_Deep" ,"Game/Assets/fonts/Cut_deep.otf", 20);

	/*
	
	for (int i = 0; i <= 8; i++)
	{
		for (int j = 0; j <= 6; j++)
		{
			new GameObject("dot_" + i, dotSprite, new Transform(new Vector2(-4 + i, -3 + j), new Vector2(0.2, 0.2)), Static);
		}
	}
	*/

	//new Platform("plt", platformSprite, new Vector2(2, -2), new Vector2(1, 1));
	new PlayerShip("PlayerShip", playerShipSprite, new Vector2(0, -2.3), new Vector2(0.6, 0.6));

	new EnemyShip("enemyShip", enemyShipSprite, new Vector2(0, 1.3), new Vector2(0.5, 0.5));
	new EnemyShip("enemyShip", enemyShipSprite, new Vector2(1, 2.2), new Vector2(0.5, 0.5));
	new EnemyShip("enemyShip", enemyShipSprite, new Vector2(-1, 2.2), new Vector2(0.5, 0.5));
	new EnemyShip("enemyShip", enemyShipSprite, new Vector2(2, 1.6), new Vector2(0.5, 0.5));
	new EnemyShip("enemyShip", enemyShipSprite, new Vector2(-2, 1.6), new Vector2(0.5, 0.5));

}
