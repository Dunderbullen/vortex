#pragma once
#include <cstddef>
#include "GL/glew.h"
#include <string>
#include <fstream>
#include "foundation/math/vector4.h"
#include <map>
#include <utility>
#include <foundation/math/bbox.h>
#include "foundation/util/array.h"
#include "vertexcomponent.h"

namespace Render
{

class MeshResource
{
public:
	MeshResource();
	~MeshResource();

	struct OBJVertex
	{
		bool operator==(const OBJVertex& rhs) const
		{
			return (pos == rhs.pos && uv == rhs.uv && normal == rhs.normal);
		}

		GLfloat pos[3];
		GLfloat uv[2];
		GLfloat normal[3];
	};

	bool loadMesh(const char* filename);

	bool loadMeshFromFile(const char* filename);
	
	/// DEPRECATED AND OBSOLETE. DO NOT USE AS IT WILL PROBABLY CAUSE MEMLEAKS
	bool loadMeshFromOBJ(const char* filename);

	void createQuad();
	///Bind VAO
	void Bind();
	/// Unbind VAO
	void Unbind();
	///Draw elements
	void Draw();

	void* getMesh();
	void* getIndices();

	///Total amount of indices in mesh
	size_t getNumIndices() { return this->numIndices; }
	///Total amount of vertices in mesh
	uint getNumVertices() { return this->numVertices; }
	///Returns the amount of floats that a vertex contains
	uint getVertexWidth() { return this->vertexWidth; }

	///Returns the base bbox for this entity.
	Math::bbox getBaseBBox() { return this->bbox; }

	std::string GetFileName() { return this->name; }
private:
	std::string name;

	//TEMPORARY: for obj loading
	Util::Array<OBJVertex> OBJvertexBuffer;
	Util::Array<unsigned int> OBJindexBuffer;

	Math::bbox bbox;

	GLuint vao[1];
	GLuint ib[1];
	GLuint vbo[1];

	void* mesh;
	void* indices;
	
	bool rawMode;
	bool N2File = false;

	void ReadHeaderData();
	void ReadPrimitiveGroups();
	void SetupVertices();

	void SetupVertexBuffer();
	void SetupIndexBuffer();

	uchar* mapPtr;
	void* groupDataPtr;
	void* vertexDataPtr;
	void* indexDataPtr;

	uint numGroups;
	uint numVertices;
	uint vertexWidth;
	size_t numIndices;
	uint numEdges;
	uint vertexComponentMask;

	size_t groupDataSize;
	size_t vertexDataSize;
	size_t indexDataSize;

	Util::Array<VertexComponent> vertexComponents;

};

}