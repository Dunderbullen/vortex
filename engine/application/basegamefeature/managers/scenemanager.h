#pragma once
#include "core/singleton.h"
#include "core/refcounted.h"

namespace Render
{
	class ModelInstance;
}

namespace BaseGameFeature
{

class SceneManager
{

	__DeclareSingleton(SceneManager)

public:
	///OBSOLETE: Loads an .obj scene into the game. Make sure to only use this once per .obj/.mtl file!
	Ptr<Render::ModelInstance> LoadOBJScene(const char* filename, const char* mtlfile);

	///Loads an XML file scene
	bool LoadXMLScene(const char* filename);
	///Save an XML file scene
	bool SaveXMLScene(const char* filename);

	///Loads an JSON file scene
	bool LoadJSONScene(const char* filename);
	///Save an JSON file scene
	bool SaveJSONScene(const char* filename);


private:


};

}