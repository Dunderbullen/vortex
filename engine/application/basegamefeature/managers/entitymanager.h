#pragma once
#include "core/refcounted.h"
#include "core/singleton.h"
#include <unordered_map>
#include "foundation/util/linkedlist.h"
#include <memory>
#include "application/game/entity.h"

#define UPDATE_FREQUENCY 0.1f //Update frequency per second for Fixed Update

namespace Game
{
class Entity;
}

namespace BaseGameFeature
{

class EntityManager
{
	__DeclareSingleton(EntityManager)

public:
	//Returns a unique ID
	uint GetNewEntityID();

	void RegisterEntity(Ptr<Game::Entity> entity);
	
	//Deletes an entity by ID. Unregisters it from gamehandler. 
	void UnregisterEntity(const int& ID);

	//Calls all entities update and fixedUpdate functions.
	void Update();

	//Returns time since last update multiplied with UPDATE_MULTIPLIER
	double DeltaTime();

	Ptr<Game::Entity> GetEntityByID(const uint& id);

	//Update frequency multiplier for deltaTime, put this to 1.0f if you want normal speed.
	//float UPDATE_MULTIPLIER;

	//Returns std::map of registered entities.
	std::unordered_map<int, Ptr<Game::Entity>>& GetEntityList() { return EntityList; }
	
private:
	//Incrementing index for making sure entities has unique IDs. 
	//Never change this explicitly if you don't know what you're doing!
	uint entityCounter = 20000; //HACK: Start at 20k because we need to reserve low numbers...

	//Holds all units mapped: unique ID as key and pointer to their baseclass as mapped value
	std::unordered_map<int, Ptr<Game::Entity>> EntityList;

	//Used for calculating DeltaTime
	//Time variables
	double deltaTime;
	double lastTime;

	double lastUpdateTime;

	//Returns time since last update in seconds
	double TimeSinceLastUpdate();
};

}