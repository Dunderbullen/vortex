#include "config.h" 
#include "modelinstance.h"
#include "render/resources/meshresource.h"
#include "render/server/resourceserver.h"
#include "render/resources/surface.h"

namespace Render
{

__ImplementClass(Render::ModelInstance, 'MDLI', Core::RefCounted);

///////////////////////////////////////////////////////////////////

ModelNode::~ModelNode()
{
	if (surface != nullptr)
	{
		surface->RemoveModelNode(this);
	}
}

//////////////////////////////////////////////////////////////////

ModelInstance::ModelInstance()
{
	mesh = nullptr;
}

ModelInstance::~ModelInstance()
{
	for (size_t i = 0; i < this->modelNodes.Size(); ++i)
	{
		delete this->modelNodes[i];
	}

	//if (this->material != nullptr)
	//{
		//HACK: Implement this
		//this->surfaces->removeModelInstance(this);
	//}
}

//const Util::Array<Ptr<Surface>>& ModelInstance::GetSurfaces()
//{
	//return this->surfaces;
//}

//void ModelInstance::SetSurface(const Util::String& name)
//{
	//if (this->surfaces.Size() < 1)
	//{
	//	this->surfaces.Append(ResourceServer::Instance()->LoadSurface(name));
	//	this->surfaces[0]->getModelInstances().Append(std::make_pair(this, 0));
	//}
	//else
	//{
	//	this->surfaces[0] = ResourceServer::Instance()->LoadSurface(name);
	//	this->surfaces[0]->getModelInstances().Append(std::make_pair(this, 0));
	//}	
//}

//void ModelInstance::SetSurfaceList(Util::Array<Ptr<Surface>> list)
//{
//	for (size_t i = 0; i < this->modelNodes.Size(); ++i)
//	{
//		modelNodes[i]->surface = list[i].get();
//	}
//}

Ptr<MeshResource> ModelInstance::GetMesh()
{
	return this->mesh;
}

void ModelInstance::SetMesh(const char* file)
{
	this->mesh = ResourceServer::Instance()->LoadMesh(file);

	for (size_t i = 0; i < this->modelNodes.Size(); ++i)
	{
		delete this->modelNodes[i];
	}
	this->modelNodes.Clear();

	//Fill up modelnodes with placeholder surface	
	for (uint i = 0; i < this->mesh->getNumPrimitiveGroups(); i++)
	{
		ModelNode* node = new ModelNode();
		node->modelInstance = this;
		node->primitiveGroup = i;
		
		auto surface = ResourceServer::Instance()->LoadSurface("resources/surfaces/placeholder.surface");
		node->surface = surface.get();
		surface->AppendModelNode(node);

		this->modelNodes.Append(node);
	}
}

void ModelInstance::AddGraphicsProperty(GraphicsProperty* gp)
{
	auto it = this->graphicsProperties.Find(gp);
	if (it == nullptr)
	{
		this->graphicsProperties.Append(gp);
	}
	else
	{
		printf("WARNING: GraphicsProperty already registered to this Model!\n");
	}
}

void ModelInstance::RemoveGraphicsProperty(GraphicsProperty* gp)
{
	auto it = this->graphicsProperties.Find(gp);
	if (it != nullptr)
	{
		//Erase and move last element to this position.
		//Destroys sorting!
		it = this->graphicsProperties.EraseSwap(it);
	}
}

Util::Array<GraphicsProperty*>& ModelInstance::GetGraphicsProperties()
{
	return this->graphicsProperties;
}

}
