#include "config.h"
#include "lightserver.h"
#include "renderdevice.h"

namespace Render
{

LightServer::LightServer() : pointLightBuffer(0), spotLightBuffer(0), visiblePointLightIndicesBuffer(0), visibleSpotLightIndicesBuffer(0), workGroupsX(0), workGroupsY(0), tileLights(512)
{
	// Generate our shader storage buffers
	glGenBuffers(1, &pointLightBuffer);
	glGenBuffers(1, &spotLightBuffer);
	glGenBuffers(1, &visiblePointLightIndicesBuffer);
	glGenBuffers(1, &visibleSpotLightIndicesBuffer);

    this->oneOverThree = 1.0f/3.0f;
}

void LightServer::AddPointLight(const PointLight& pLight)
{
	this->pointLights.Append(pLight);
	this->UpdatePointLightBuffer();
}

LightServer::PointLight& LightServer::GetPointLightAtIndex(const int& index)
{
	return this->pointLights[index];
}

void LightServer::AddSpotLight(SpotLight& sLight)
{
    this->CalculateSpotlight(sLight);
	this->spotLights.Append(sLight);
	this->UpdateSpotLightBuffer();
}

void LightServer::CalculateSpotlight(SpotLight& sLight)
{
	/// calculate the radius of the bottom cirlce
	float radius = (float)tan(Math::Deg2Rad(sLight.angle)) * sLight.length;

	/// Get perpendicular direction
	Math::vec4 m = Math::vec4::normalize(Math::vec4::cross3(sLight.coneDirection, sLight.position));
	Math::vec4 Q1 = sLight.position + sLight.coneDirection * sLight.length - m * radius;
	/// Get perpendicular, -direction
	m = Math::vec4::normalize(Math::vec4::cross3(sLight.coneDirection * -1.0f, sLight.position));
	Math::vec4 Q2 = sLight.position + sLight.coneDirection * sLight.length - m * radius;

	/// Calculate the Mid Point of the Sphere
	sLight.midPoint = (sLight.position + Q1 + Q2) * this->oneOverThree;
	sLight.midPoint.set_w(1.0f);

	/// Calculate the radius for the sphere
	sLight.fRadius = (sLight.midPoint - sLight.position).length();
}

LightServer::SpotLight& LightServer::GetSpotLightAtIndex(const int& index)
{
	return this->spotLights[index];
}

void LightServer::UpdateWorkGroups()
{
	// Define work group sizes in x and y direction based off screen size and tile size (in pixels)
	workGroupsX = (GLuint)(RenderDevice::Instance()->GetRenderResolution().x + (RenderDevice::Instance()->GetRenderResolution().x % 16)) / 16;
	workGroupsY = (GLuint)(RenderDevice::Instance()->GetRenderResolution().y + (RenderDevice::Instance()->GetRenderResolution().y % 16)) / 16;

	size_t numberOfTiles = workGroupsX * workGroupsY;

	// Bind visible Point light indices buffer
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, visiblePointLightIndicesBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, numberOfTiles * sizeof(VisibleIndex) * tileLights, 0, GL_STATIC_DRAW);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, visibleSpotLightIndicesBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, numberOfTiles * sizeof(VisibleIndex) * tileLights, 0, GL_STATIC_DRAW);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void LightServer::UpdatePointLightBuffer()
{
	//size_t numberOfTiles = workGroupsX * workGroupsY;
	size_t numPointLights = this->pointLights.Size();

	// Bind light buffer
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, pointLightBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, numPointLights * sizeof(PointLight), &this->pointLights[0], GL_STATIC_DRAW);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void LightServer::UpdateSpotLightBuffer()
{
	//size_t numberOfTiles = workGroupsX * workGroupsY;
	size_t numSpotLights = this->spotLights.Size();

	// Bind light buffer
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, spotLightBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, numSpotLights * sizeof(SpotLight), &this->spotLights[0], GL_STATIC_DRAW);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void LightServer::CreateSpotLight(Math::point color,
								  Math::point position,
								  Math::vec4 direction,
								  float length,
								  float angle)
{
	SpotLight sLight = SpotLight();
	sLight.color = color;
	sLight.position = position;
	sLight.coneDirection = direction;
	sLight.length = length;
	sLight.angle = angle;
	this->AddSpotLight(sLight);
}

void LightServer::CreatePointLight(Math::point color, Math::point position, float radius)
{
	PointLight pLight;
	pLight.position = position;
	pLight.color = color;
	pLight.radiusAndPadding.set_x(radius);
	LightServer::Instance()->AddPointLight(pLight);
}
}