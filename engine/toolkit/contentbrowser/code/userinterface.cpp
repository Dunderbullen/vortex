#include "userinterface.h"
#include "render/server/resourceserver.h"
#include "imgui.h"
#include "application.h"
#include "render/server/frameserver.h"
#include "toolkit/tools/style.h"
#include "nfd.h"
#include "render/resources/surface.h"
#include <dirent.h>
#include "render/frame/flatgeometrylitpass.h"
#include "render/properties/graphicsproperty.h"
#include "render/resources/modelinstance.h"
#include "application/game/entity.h"

using namespace Render;

namespace Toolkit
{
	UserInterface::UserInterface(ContentBrowser::Application* app)
	{
		this->application = app;
		this->savePath = "";
		this->browseButtonTextureHandle = Render::ResourceServer::Instance()->LoadTexture("engine/toolkit/leveleditor/resources/textures/texture.png")->GetHandle();
		this->selectedNode = nullptr;
		//Setup ImGui Stuff
		SetupImGuiStyle();
		ImGui::LoadDock("engine/toolkit/contentbrowser/layout/default.layout");
	}

	UserInterface::~UserInterface()
	{

	}

	void UserInterface::BrowseDirectory(const char* directory, Util::Array<std::string>& outArray)
	{
		DIR *dir;
		struct dirent *ent;
		int i = 0;
		if ((dir = opendir(directory)) != NULL)
		{
			//list the files and directories within directory
			while ((ent = readdir(dir)) != NULL) 
			{
				//skip the . and .. directories
				std::string n = ent->d_name;
				if (n != "." && n != "..")
					outArray.Append(n);

				i++;
			}
			
		}
		else
		{
			//could not open directory
			_assert2(false, "Could not open directory!");
			return;
		}

		closedir(dir);
		return;
	}

	void UserInterface::ImGuiListDirectory(const char* label, int* selectedItem, Util::Array<std::string>& list)
	{
		//We have to convert from std::string to const char* for imgui to accept it. This is ugly as hell, but it works.
		Util::Array<const char *> cStrArray;
		for (int i = 0; i < list.Size(); ++i)
		{
			cStrArray.Append(list[i].c_str());
		}

		ImGui::ListBox(label, selectedItem, &cStrArray[0], (int)list.Size());
	}

	void UserInterface::Run()
	{
		RenderDocks();

		//TODO: Make sure we're not editing a textbox before querying for shortcuts
		ExecShortCuts();

		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				ShowFileMenu();
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Edit"))
			{
				//if (ImGui::MenuItem("Undo", "CTRL+Z")) { }
				//if (ImGui::MenuItem("Redo", "CTRL+Y")) { }  // Disabled item
				ImGui::Separator();
				if (ImGui::MenuItem("Cut", "CTRL+X")) {}
				if (ImGui::MenuItem("Copy", "CTRL+C")) {}
				if (ImGui::MenuItem("Paste", "CTRL+V")) {}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Window"))
			{
				if (ImGui::BeginMenu("Show"))
				{
					ImGui::EndMenu();
				}
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();

			this->ModalWindows();
		}

	}

	void UserInterface::ShowFileMenu()
	{
		if (ImGui::BeginMenu("New..."))
		{
			if (ImGui::MenuItem("Model"))
			{
				NewModel();
			}
			if (ImGui::MenuItem("Surface"))
			{
				NewSurface();
			}
			ImGui::EndMenu();
		}
		if (ImGui::MenuItem("Open", "Ctrl+O")) 
		{
			this->openFilePopup = true;
		}
		if (ImGui::BeginMenu("Open Recent"))
		{
			ImGui::MenuItem("example1.map");
			ImGui::MenuItem("example2.map");
			ImGui::MenuItem("example3.map");
			ImGui::EndMenu();
		}
		if (ImGui::MenuItem("Save", "Ctrl+S"))
		{
			this->SaveModel();
		}
		if (ImGui::MenuItem("Save As..", "Ctrl+Shift+S")) 
		{
			this->SaveModel(true);
		}
		ImGui::Separator();
		if (ImGui::MenuItem("Import MTL..."))
		{
			this->importFilePopup = true;
		}
		ImGui::Separator();

		if (ImGui::BeginMenu("Layout"))
		{
			if (ImGui::MenuItem("Save Layout...")) { ImGui::SaveDock("engine/toolkit/contentbrowser/layout/default.layout"); }
			if (ImGui::MenuItem("Load Layout...")) { ImGui::LoadDock("engine/toolkit/contentbrowser/layout/default.layout"); }
			ImGui::EndMenu();
		}

		if (ImGui::MenuItem("Quit", "Alt+F4")) { application->shutdown = true; }
	}

	void UserInterface::ExecShortCuts()
	{
		if ((ImGui::GetIO().KeyCtrl && ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_::ImGuiKey_S))))
		{
			this->SaveModel();
		}
	}


	void UserInterface::RenderDocks()
	{		
		ImGui::RootDock(ImVec2(0.0f, 16.0f), ImVec2((float)application->window->GetWidth(), (float)application->window->GetHeight() - 16.0f));
		{
			if(ImGui::BeginDock("3D View", NULL, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse))
			{
				ImVec2 dockSize = ImGui::GetWindowSize();
				ImGui::Image((void*)Render::FrameServer::Instance()->GetFlatGeometryLitPass()->GetBuffer(), dockSize);

				if (ImGui::IsItemHovered())
				{
					application->DoPicking();
				}
			}
			ImGui::EndDock();

			if(ImGui::BeginDock("Inspector", NULL, ImGuiWindowFlags_NoSavedSettings))
			{
				if (this->application->entity != nullptr)
				{
					ImGui::Text("Model: %s", this->application->entity->FindProperty<GraphicsProperty>()->getModelInstance()->GetName().AsCharPtr());
					ImGui::Text("Mesh: %s", this->application->entity->FindProperty<GraphicsProperty>()->getModelInstance()->GetMesh()->GetPath().c_str());
					
					ImGui::SameLine();

					if (ImGui::Button("Browse...", ImVec2(48, 16)))
					{
						this->meshBrowserPopup = true;
					}
					if (ImGui::Button("EXPORT..", ImVec2(48, 16)))
					{
						this->ExportMesh();
					}

					ImGui::BeginChild("ModelNodes", ImVec2(0, 0), true);
					{
						//Unique identifier
						uint i = 2014235;
						for (auto node : this->application->entity->FindProperty<GraphicsProperty>()->getModelInstance()->GetModelNodes())
						{
							ImGui::BeginGroup();
							{
								std::string nodeName = this->application->entity->FindProperty<GraphicsProperty>()->getModelInstance()->GetMesh()->getPrimitiveGroup(node->primitiveGroup).name;

								ImGui::Text("Node: %s", nodeName.c_str());
								
								ImGui::Text("Surface: %s", node->surface->GetPath().AsCharPtr());
								ImGui::SameLine();
								ImGui::PushID(i++);
								if (ImGui::ImageButton((void*)this->browseButtonTextureHandle, ImVec2(16, 16)))
								{
									this->surfaceBrowserPopup = true;
									this->selectedNode = node;
								}
								ImGui::PopID();
								ImGui::Separator();
							}
							ImGui::EndGroup();

							if (ImGui::IsItemHovered() && !this->surfaceBrowserPopup)
							{
								this->selectedNode = node;
							}

							if (ImGui::IsItemClicked())
							{

							}

						}
						ImGui::EndChild();
					}


				}
			}
			ImGui::EndDock();
		}
	}

	void UserInterface::ModalWindows()
	{
		if (this->openFilePopup){ ImGui::OpenPopup("OpenFile"); }
		if (this->importFilePopup){ ImGui::OpenPopup("ImportFile"); }
		if (this->confirmNewModelPopup){ ImGui::OpenPopup("New Model"); }
		if (this->surfaceBrowserPopup){ ImGui::OpenPopup("Surface Browser"); }
		if (this->meshBrowserPopup){ ImGui::OpenPopup("Mesh Browser"); }

		if (ImGui::BeginPopupModal("OpenFile", &this->openFilePopup))
		{
			nfdchar_t* outpath;
			nfdresult_t result = NFD_OpenDialog("mdl", NULL, &outpath);

			if (result == NFD_OKAY)
			{
				printf("path: %s\n", outpath);
				if (this->application->entity != nullptr)
				{
					this->application->entity->Deactivate();
				}
				this->selectedNode = nullptr;

				this->application->entity = Game::Entity::Create();
				Ptr<GraphicsProperty> gp = GraphicsProperty::Create();
				this->application->entity->AddProperty(gp.downcast<Game::BaseProperty>());
				auto mdl = Render::ResourceServer::Instance()->LoadModel(outpath);
				gp->setModelInstance(mdl);
				//this->application->entity->SetTransform(Math::mat4::scaling(0.01f, 0.01f, 0.01f));
				this->application->entity->Activate();

				this->openFilePopup = false;
				free(outpath);
			}
			else if (result == NFD_CANCEL)
			{
				this->openFilePopup = false;
			}
			else
			{
				printf("Error: %s\n", NFD_GetError());
				assert(false);
				this->openFilePopup = false;
			}

			ImGui::EndPopup();
		}

		if (ImGui::BeginPopupModal("ImportFile", &this->importFilePopup))
		{
			nfdchar_t* outpath;
			nfdresult_t result = NFD_OpenDialog("mtl", NULL, &outpath);

			if (result == NFD_OKAY)
			{
				printf("path: %s\n", outpath);
				if (this->application->entity != nullptr)
				{
					//EMPTY
				}
				
				this->importFilePopup = false;
				free(outpath);
			}
			else if (result == NFD_CANCEL)
			{
				this->importFilePopup = false;
			}
			else
			{
				printf("Error: %s\n", NFD_GetError());
				assert(false);
				this->importFilePopup = false;
			}

			ImGui::EndPopup();
		}

		if (ImGui::BeginPopupModal("New Model", &this->confirmNewModelPopup, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::SetWindowSize(ImVec2(300, 300), ImGuiSetCond_::ImGuiSetCond_Once);

			ImGui::Text("Are you sure you want to create a new model?\nAny unsaved progress will be lost.\n\n");
			ImGui::Separator();

			if (ImGui::Button("Yes", ImVec2(120, 0)))
			{
				this->application->NewModel();
				this->confirmNewModelPopup = false;
			}
			ImGui::SameLine();
			if (ImGui::Button("No", ImVec2(120, 0)))
			{
				this->confirmNewModelPopup = false;
			}

			ImGui::EndPopup();
		}

		if (ImGui::BeginPopupModal("Surface Browser", &this->surfaceBrowserPopup))
		{
			Util::Array<std::string> list;

			static int selected = 0;

			BrowseDirectory("resources/surfaces/", list);
			ImGuiListDirectory("Surfaces", &selected, list);

			ImGui::Separator();

			//Unique id
			ImGui::PushID(194985);
			if (ImGui::Button("Ok", ImVec2(120, 0)))
			{
				std::string path = "resources/surfaces/" + list[selected];
				printf("surface path: %s\n", path.c_str());

				selectedNode->surface->RemoveModelNode(selectedNode);
				auto surface = Render::ResourceServer::Instance()->LoadSurface(path.c_str());
				surface->AppendModelNode(selectedNode);
				selectedNode->surface = surface.get();

				this->surfaceBrowserPopup = false;
				this->selectedNode = nullptr;
			}
			ImGui::PopID();
			ImGui::SameLine();
			ImGui::PushID(194986);
			if (ImGui::Button("Cancel", ImVec2(120, 0)))
			{
				this->surfaceBrowserPopup = false;
			}
			ImGui::PopID();

			ImGui::EndPopup();
		}

		if (ImGui::BeginPopupModal("Mesh Browser", &this->meshBrowserPopup))
		{
			Util::Array<std::string> list;

			static int selected = 0;

			BrowseDirectory("resources/meshes/", list);
			ImGuiListDirectory("Meshes", &selected, list);

			ImGui::Separator();

			//Unique id
			ImGui::PushID(194988);
			if (ImGui::Button("Ok", ImVec2(120, 0)))
			{
				std::string path = "resources/meshes/" + list[selected];
				printf("mesh path: %s\n", path.c_str());

				this->selectedNode = nullptr;

				this->application->entity->Deactivate();
				this->application->entity->FindProperty<GraphicsProperty>()->getModelInstance()->SetMesh(path.c_str());
				this->application->entity->Activate();

				this->meshBrowserPopup = false;
			}
			ImGui::PopID();
			ImGui::SameLine();
			ImGui::PushID(194989);
			if (ImGui::Button("Cancel", ImVec2(120, 0)))
			{
				this->meshBrowserPopup = false;
			}
			ImGui::PopID();

			ImGui::EndPopup();
		}
	}

	void UserInterface::NewModel()
	{
		if (this->application->entity != nullptr)
		{
			this->confirmNewModelPopup = true;
		}
		else
		{
			this->application->NewModel();
		}
	}

	void UserInterface::SaveModel(bool newPath)
	{
		if (this->savePath.empty() || newPath)
		{
			nfdchar_t* outpath = nullptr;
			nfdresult_t result = NFD_SaveDialog("mdl", NULL, &outpath);

			if (result == NFD_OKAY)
			{
				std::string newFilePath = outpath;
			
				free(outpath);

				if (newFilePath.substr(newFilePath.find_last_of(".") + 1) != "mdl") 
				{
					newFilePath.append(".mdl");
				}			
				
				this->savePath = newFilePath;
			}
			else if (result == NFD_CANCEL)
			{
				//Do nothing!
				return;
			}
			else
			{
				printf("Error: %s\n", NFD_GetError());
				assert(false);
				return;
			}
		}		
		printf("Save Path: %s\n", this->savePath.c_str());
		this->application->SaveModel(this->savePath.c_str());		
	}

	void UserInterface::ExportMesh()
	{
		std::string filepath;

		nfdchar_t* outpath = nullptr;
		nfdresult_t result = NFD_SaveDialog("mesh", NULL, &outpath);

		if (result == NFD_OKAY)
		{
			filepath = outpath;

			free(outpath);

			if (filepath.substr(filepath.find_last_of(".") + 1) != "mesh")
			{
				filepath.append(".mesh");
			}
		}
		else if (result == NFD_CANCEL)
		{
			//Do nothing!
			return;
		}
		else
		{
			printf("Error: %s\n", NFD_GetError());
			assert(false);
			return;
		}
		printf("Mesh Export Path: %s\n", filepath.c_str());
		this->application->ExportMesh(filepath.c_str());
	}

	void UserInterface::NewSurface()
	{

	}
}
