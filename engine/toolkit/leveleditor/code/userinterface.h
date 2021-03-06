/*
 * TODO: Add Delete buttons for lights
 */

#pragma once
#include "GL/glew.h"
#include <string>
#include <render/server/lightserver.h>
#include "foundation/math/point.h"
#include "undo.h"
#include "application/properties/particleemitter.h"
#include "toolhandler.h"

namespace LevelEditor { class Application; }

namespace Interface
{
	class InterfaceManager;
}

namespace Toolkit
{

	class UserInterface
	{
	public:
		UserInterface(LevelEditor::Application* app);
		~UserInterface();

		void Run();

		double frameTime;
	private:
		Edit::CommandManager* commandManager;
		LevelEditor::Application* application;
		
		void RenderDocks();
		void ExecShortCuts();
		void ShowFileMenu();

		void ParticlesSettings(Ptr<Game::ParticleEmitter> emitter);
		
		double prevFPSTime;
		double currentFPS;
		
		std::string consoleBuffer;
		std::string a;

		///Texture Handles
		//Toolbar
		GLuint selectToolTextureHandle;
		GLuint translateToolTextureHandle;
		GLuint rotateToolTextureHandle;
		GLuint scaleToolTextureHandle;
		GLuint entityToolTextureHandle;
		GLuint brushToolTextureHandle;
		GLuint polygonEditTextureHandle;
	
		GLuint particleCount;

	};

}
