#include "emittersUI.h"
#include "userinterface.h"
#include "imgui.h"
#include "render/server/resourceserver.h"

namespace ParticleEditor
{
EmittersUI::EmittersUI()
{
}

EmittersUI::EmittersUI(UserInterface* ui, int id, bool dummy)
{
	this->ui = ui;
	this->id = id;
	this->dummy = dummy;
	name = "New Emitter";

	//gradient = ImGradient();

	dupIcon = Render::ResourceServer::Instance()->LoadTexture("engine/toolkit/particleeditor/resources/textures/copy.png");
	deleteIcon = Render::ResourceServer::Instance()->LoadTexture("engine/toolkit/particleeditor/resources/textures/delete.png");

	visibleIcon = Render::ResourceServer::Instance()->LoadTexture("engine/toolkit/particleeditor/resources/textures/visible.png");
	notVisibleIcon = Render::ResourceServer::Instance()->LoadTexture("engine/toolkit/particleeditor/resources/textures/cancel.png");

	activeIcon = Render::ResourceServer::Instance()->LoadTexture("engine/toolkit/particleeditor/resources/textures/active.png");
	inactiveIcon = Render::ResourceServer::Instance()->LoadTexture("engine/toolkit/particleeditor/resources/textures/inactive.png");
}

EmittersUI::~EmittersUI()
{
}

void EmittersUI::DrawEmitter()
{
	if (!dummy)
	{
		ImVec2 cursorBegin = ImGui::GetCursorPos();
		std::string n = "##emitterframe" + std::to_string(id);
		ImGui::BeginChild(n.c_str(), ImVec2(ImGui::GetWindowContentRegionWidth(), 40), true);
		{
			
			if (ev.active)
			{			
				ImGui::ImageButton((void*)activeIcon->GetHandle(), ImVec2(15, 15));
			}
			else
			{
				if (ImGui::ImageButton((void*)inactiveIcon->GetHandle(), ImVec2(15, 15)))
				{
					ev.active = true;
					ui->UpdateActiveEmitter(this->id);
				}
			}
			ui->Tooltip("Set this emitter to active");

			ImGui::SameLine();

			n = "##name" + std::to_string(id);
			ImGui::InputText(n.c_str(), (char*)ev.name.AsCharPtr(), 512);

			ImGui::SameLine(ImGui::GetWindowWidth() - 115);
			if (ev.visible)
			{
				if (ImGui::ImageButton((void*)visibleIcon->GetHandle(), ImVec2(20, 20)))
				{
					ev.visible = false;
				}
			}
			else
			{
				if (ImGui::ImageButton((void*)notVisibleIcon->GetHandle(), ImVec2(20, 20)))
				{
					ev.visible = true;
				}
			}
			ui->Tooltip("Set this emitters visibility");

		
			ImGui::SameLine(ImGui::GetWindowWidth() - 80);
			if (ImGui::ImageButton((void*)dupIcon->GetHandle(), ImVec2(20, 20)))
			{
				ui->DuplicateEmitter(std::make_shared<ParticleEditor::EmittersUI>(*this));
			}
			ui->Tooltip("Duplicate this emitter");
			ImGui::SameLine();
			if (ImGui::ImageButton((void*)deleteIcon->GetHandle(), ImVec2(20, 20)))
			{
				ui->RemoveEmitter(this->id);
			}
			ui->Tooltip("Delete this emitter");

			ImGui::SameLine(1);
			n = "drag" + std::to_string(id);
		
			if (ImGui::InvisibleButton(n.c_str(), ImVec2(ImGui::GetContentRegionAvailWidth() - 1, 30)) && ImGui::IsMouseDoubleClicked(0))
			{
				ev.active = true;
				ui->UpdateActiveEmitter(this->id);
			}
		}
		ImGui::EndChild();

	}
}

}

