#include "Engine.h"
#include <iostream>
#include "SDL_image.h"
#include "Input.h"
#include <algorithm>
#include "Debug.h"

using namespace ArtemisEngine;

SDL_Renderer* Engine::renderer = nullptr;
std::vector<GameObject*> Engine::gameObjects;
std::queue<GameObject*> Engine::gameObjectsToAdd;
std::queue<GameObject*> Engine::gameObjectsToRemove;
std::vector<Text*> Engine::textsToRender;

int Engine::pixW;
int Engine::pixH;
double Engine::pixPerWorld;

Engine::Engine(const char* title, int xPos, int yPos, int widthPX, int heightPX, bool fullScreen)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
        Debug::Log("Subsystem Initialised!");

		int flags = 0;
		if (fullScreen == true)
		{
			flags = SDL_WINDOW_FULLSCREEN;
		}

		window = SDL_CreateWindow(title, xPos, yPos, widthPX, heightPX, flags);

		if (window)
		{
            Debug::Log("Window created!");
		}

		renderer = SDL_CreateRenderer(window, -1, 0);

		if (renderer)
		{
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            Debug::Log("Renderer created!");
		}

		if (TTF_Init() == -1)
		{
            Debug::Log("Text Renderer Init Error: " + std::string( SDL_GetError()), Debug::Warning);
		}

		isRunning = true;
	}
	else
	{
		isRunning = false;
	}

	pixW = widthPX;
	pixH = heightPX;

	physicsEngine = new Physics();
}


Engine::~Engine()
{
}

void Engine::HandleEvents()
{
	Input::ClearInputBuffer();

	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			isRunning = false;
			break;
		case SDL_KEYDOWN:
			Input::PassInFrameInput(event.key);
			break;
		case SDL_KEYUP:
			Input::PassInFrameInput(event.key);
			break;
		default:
			break;
		}
	}

}


void Engine::Update()
{
	for (int i = 0; i < gameObjects.size(); i++)
	{
        gameObjects[i]->UpdateComponents();
	}
}

void Engine::Render()
{
	SDL_RenderClear(renderer);

	for (int i = 0; i < gameObjects.size(); i++)
	{
        gameObjects[i]->RenderComponents();
	}

	for (int i = 0; i < textsToRender.size(); i++)
	{
		textsToRender[i]->Render();
	}

	SDL_RenderPresent(renderer);
}

void Engine::Clean()
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	TTF_Quit();
	SDL_Quit();
    Debug::Log("Game cleaned!");
}

bool Engine::IsGameRunning()
{
	return isRunning;
}

void Engine::GameObjectCreated(GameObject *gameObject)
{
    gameObjectsToAdd.push(gameObject);
}

void Engine::UpdateGameObjectList()
{
    while(!gameObjectsToRemove.empty())
    {
        auto found = std::find(gameObjects.begin(), gameObjects.end(), gameObjectsToRemove.front());
        if (found != gameObjects.end())
        {
            gameObjects.erase(found);
        }

        RigidBody* rb = gameObjectsToRemove.front()->GetComponent<RigidBody>();
        if (rb != nullptr)
        {
            Physics::RigidBodyDeleted(rb);
        }

        delete gameObjectsToRemove.front();
        gameObjectsToRemove.pop();
    }

    while(!gameObjectsToAdd.empty())
    {
        gameObjects.push_back(gameObjectsToAdd.front());
        gameObjectsToAdd.pop();
    }
}

void Engine::Destroy(GameObject *gameObject)
{
    gameObjectsToRemove.push(gameObject);
}
