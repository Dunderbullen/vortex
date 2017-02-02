#pragma once
#include "application/game/baseproperty.h"
#include "render/particlesystem/particlesystem.h"



namespace Render
{
	class GraphicsProperty;
	class ModelInstance;
	class TextureResource;
}

namespace Property
{

class ParticleEmitter : public Game::BaseProperty
{
public:
	ParticleEmitter();
	~ParticleEmitter();

	void FixedUpdate();
	void Update();

	void Activate();
	void Deactivate();

	void CreateEmitter(GLuint numOfParticles, const char* texturepath);
	void BindUniformBuffer();

	GLuint& GetNumberOfParticles(){ return numOfParticles; }

	Particles::ParticleState& GetState(){ return state; }

	std::shared_ptr<Render::TextureResource>& GetEmitterTexture(){ return this->texture; }

	Math::mat4 GetModelMatrix();

	GLuint* GetUniformBuffer(){ return this->ubo; }
	Particles::EmitterBuffer& GetEmitterBuffer(){ return this->buff; }
	Particles::ParticleRenderingBuffer& GetRenderBuffer(){ return this->renderBuff; }
	Particles::ParticleUISettings& GetParticleUISettings(){ return this->pSet; }

private:
	//uniformBuffer
	GLuint ubo[1];

	GLuint numOfParticles;

	//The state of the particles
	Particles::ParticleState state;

	//The emitter buffer from the Particle System
	Particles::EmitterBuffer buff;

	//The uniform render buffer
	Particles::ParticleRenderingBuffer renderBuff;

	std::shared_ptr<Render::TextureResource> texture;

	//Saved Settings
	Particles::ParticleUISettings pSet;

};
}
