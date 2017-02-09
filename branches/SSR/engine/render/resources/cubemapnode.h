#pragma once
#include "foundation/math/point.h"
#include "GL/glew.h"
#include <memory>
#include "shaderobject.h"
#include "render/server/renderdevice.h"

namespace Render
{

class CubeMapNode : public std::enable_shared_from_this<CubeMapNode>
{
public:
	enum CubemapShape
	{
		BOX,
		SPHERE
	};

	CubeMapNode();
	~CubeMapNode();

	void GenerateCubeMap();
	GLuint GetCubeMap();
	
	void SetPosition(Math::point pos);
	const Math::point& GetPosition() const;

	void Activate();
	void Deactivate();

	void DeleteCubeMap();

	bool IsLoaded();

	Math::vector& InnerScale() { return this->innerScale; }
	Math::vector& OuterScale() { return this->outerScale; }

	void SetShape(CubemapShape s) { this->shape = s; }
	CubemapShape GetShape() const { return this->shape; }

private:
	enum CubeFace
	{
		RIGHT = 0,
		LEFT = 1,
		TOP = 2,
		BOTTOM = 3,
		BACK = 4,
		FRONT = 5
	};

	void RenderTexture(const GLuint& framebuffer, CubeFace face, Graphics::Camera& camera);

	bool isLoaded;
	bool isActive;

	Math::point position;

	// cubemaps influence shape
	// box will use entire scale vectors, sphere will always be spherical (x as radius)
	CubemapShape shape;
	// if camera is within innerbounds, 100% of this cubemap will be used.
	Math::vector innerScale;
	// cubemaps will be blended depending on the cameras position relative to inner and outer range for k cubemaps.
	Math::vector outerScale;

	Render::Resolution resolution;
	GLuint mipLevels;

	///The cubemap
	GLuint cubeSampler;

#ifdef _LEVELEDITOR
	//Lists all textures in the cubemap so that we can inspect them within applications
	GLuint textures[6];
#endif
};
}