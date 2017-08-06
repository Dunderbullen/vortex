#include "config.h"
#include "rigidbodyinspector.h"
#include "imgui.h"

namespace LevelEditor
{
	RigidbodyInspector::RigidbodyInspector()
    {
        this->name = "Rigidbody";
    }
    
	RigidbodyInspector::~RigidbodyInspector()
    {
        //Empty
    }
	
    void RigidbodyInspector::DrawGUI()
    {
        if(this->property == nullptr)
        {
            //Draw some indicator that something has gone wrong!
            return;
        }
		
        BeginAttribute("Kinematic", "If set to true, this entity will\nnot be affected external forces such as gravity\nor collisions but will still won't pass through\ncolliders when translated.");
		{
			bool isKinematic = this->property->IsKinematic();

			if (ImGui::Checkbox("##isKinematic", &isKinematic))
			{
				this->property->SetKinematic(isKinematic);
			}
		}
		EndAttribute();

		BeginAttribute("Is Debris?", "If set to true, this entity will\nonly collide with static entites and use a less expensive\nintegration method for physics calculations.");
		{
			bool isDebris = this->property->IsDebris();

			if (ImGui::Checkbox("##isDebris", &isDebris))
			{
				this->property->SetIsDebris(isDebris);
			}
		}
		EndAttribute();

        BeginAttribute("Mass");
        {
            static float var = this->property->GetMass();
			if (ImGui::InputFloat("##floatP", &var, 0.05f))
			{
				this->property->SetMass(var);
			}
        }
        EndAttribute();


        
        ImGui::NextColumn();
    }

    void RigidbodyInspector::Update()
    {
        if(this->property == nullptr)
            return;
    }

    void RigidbodyInspector::SetProperty(const Ptr<Game::BaseProperty>& property)
    {
        this->property = property.downcast<Property::Rigidbody>();

        if(this->property != nullptr)
        {
			
        }
		else
		{
            _warning("Could not set inspector property to RigidbodyProperty in RigidbodyInspector::SetProperty!");
		}
    }

	Ptr<Game::BaseProperty> RigidbodyInspector::CreateNewProperty()
	{
		return Property::Rigidbody::Create();
	}

}