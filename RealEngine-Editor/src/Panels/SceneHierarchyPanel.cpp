#include "SceneHierarchyPanel.h"
#include <cstring>

/* The Microsoft C++ compiler is non-compliant with the C++ standard and needs
 * the following definition to disable a security warning on std::strncpy().
 */
#ifdef _MSVC_LANG
	#define _CRT_SECURE_NO_WARNINGS
#endif

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

#include <glm/gtc/type_ptr.hpp>

#include "RealEngine/Scene/Components.h"
#include "RealEngine/Scene/SceneCamera.h"

#include "RealEngine/Scripting/ScriptEngine.h"
#include "RealEngine/UI/UI.h"

#include "RealEngine/Utils/PlatformUtils.h"
#include "RealEngine/Utils/FileFormats.h"


namespace RealEngine {
	SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& context) {
		SetContext(context);
	}

	void SceneHierarchyPanel::SetContext(const Ref<Scene>& context) {
		m_Context = context;
		m_SelectionContext = {};
	}

	void SceneHierarchyPanel::OnImGuiRender() {
		RE_PROFILE_FUNCTION();
		
		ImGui::Begin("Scene Hierarchy");

		if (m_Context) {
			m_Context->m_Registry.each([&](auto entityID) {
				Entity entity{ entityID , m_Context.get() };

				Relationship& relationship = entity.GetRelationship();
				if (relationship.parent == 0) {
					DrawEntityNode(entity, relationship);
				}
			});

			if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
				m_SelectionContext = {};

			// Right-click on blank space
			if (ImGui::BeginPopupContextWindow(0, 1, false)) {
				if (ImGui::MenuItem("Create Empty Entity"))
					m_SelectionContext = m_Context->CreateEntity("Empty Entity");
				if (ImGui::MenuItem("Create Sprite")) {
					m_SelectionContext = m_Context->CreateEntity("Sprite");
					m_SelectionContext.AddComponent<SpriteRendererComponent>();
				}
				if (ImGui::MenuItem("Create Circle")) {
					m_SelectionContext = m_Context->CreateEntity("Circle");
					m_SelectionContext.AddComponent<CircleRendererComponent>();
				}
				if (ImGui::MenuItem("Create Camera")) {
					m_SelectionContext = m_Context->CreateEntity("Camera");
					m_SelectionContext.AddComponent<CameraComponent>();
				}

				ImGui::EndPopup();
			}

			ImGui::End();

			ImGui::Begin("Properties");
			if (m_SelectionContext) {
				DrawComponents(m_SelectionContext);
			}
		}

		ImGui::End();
	}

	void SceneHierarchyPanel::SetSelectedEntity(Entity entity) {
		m_SelectionContext = entity;
	}

	void SceneHierarchyPanel::DrawEntityNode(Entity entity, Relationship& relationship) {
		RE_PROFILE_FUNCTION();
		
		auto& tag = entity.GetComponent<TagComponent>().Tag;

		bool hasChildren = !relationship.children.empty();

		ImGuiTreeNodeFlags flags = m_SelectionContext == entity ? ImGuiTreeNodeFlags_Selected : 0 | ImGuiTreeNodeFlags_OpenOnArrow;
		flags |= ImGuiTreeNodeFlags_SpanAvailWidth;

		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());

		if (ImGui::BeginDragDropSource()) {
			//This is the payload
			uint64_t id = entity.GetUUID();
			ImGui::SetDragDropPayload("CONTENT_BROWSER_ENTITY", &id, sizeof(uint64_t));

			//Thumbnail
			ImGui::Text(entity.GetName().c_str());

			ImGui::EndDragDropSource();
		}

		if (ImGui::BeginDragDropTarget()) {
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ENTITY")) {
				UUID childID = UUID(*(uint64_t*)payload->Data);

				Relationship& childRelationship = m_Context->GetRelationshipByUUID(childID);
				UUID childsParent = childRelationship.parent;

				//Clear parent's child list of the moved child if child has parent
				if (childsParent != 0) {
					Relationship& childParentRelationship = m_Context->GetRelationshipByUUID(childsParent);
					childParentRelationship.children.erase(childID);
				}

				//Set the correct child and parent relationship
				childRelationship.parent = entity.GetUUID();
				relationship.children.insert(childID);

				//Sets TreeNodeEx to open
				ImGui::GetStateStorage()->SetInt(ImGui::GetItemID(), true);
			}
			ImGui::EndDragDropTarget();
		}

		if (ImGui::IsMouseReleased(0)) {
			if(ImGui::IsItemHovered())
				m_SelectionContext = entity;
		}

		bool entityDeleted = false;
		if (ImGui::BeginPopupContextItem()) {
			if (ImGui::MenuItem("Delete Entity"))
				entityDeleted = true;

			ImGui::EndPopup();	
		}

		if (opened) {
			if (hasChildren) {
				for (UUID childID : relationship.children) {
					Entity childEntity = m_Context->GetEntityByUUID(childID);
					DrawEntityNode(childEntity, childEntity.GetRelationship());
				}
			}
			ImGui::TreePop();
		}
		

		if (entityDeleted) {
			m_Context->DestroyEntity(entity);
			if (m_SelectionContext == entity)
				m_SelectionContext = {};
		}
	}

	template<typename UIPayload>
	static bool DrawButtonLabel(const std::string& buttonLabel, const std::string& textLabel, UIPayload uiPayload) {
		RE_PROFILE_FUNCTION();
		
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4.0f, 4.0f));
		
		bool isPressed = ImGui::Button(buttonLabel.c_str(), ImVec2(ImGui::CalcItemWidth(), 0));
		uiPayload();
		ImGui::SameLine();
		ImGui::Text(textLabel.c_str());
		ImGui::PopStyleVar();

		return isPressed;
	}

	static void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f) {
		RE_PROFILE_FUNCTION();
		
		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[0];

		ImGui::PushID(label.c_str());

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("X", buttonSize))
			values.x = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Y", buttonSize))
			values.y = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Z", buttonSize))
			values.z = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();

		ImGui::PopStyleVar();

		ImGui::Columns(1);

		ImGui::PopID();
	}	

	//For some weird fricken reason I can't move this to be a private variable 
	//because the application crashes when I try to delete the layers on close
	struct ComponentCopyBuffer m_ComponentCopyBuffer;
	template<typename T, typename UIFunction>
	void SceneHierarchyPanel::DrawComponent(const std::string& name, Entity entity, UIFunction uiFunction) {
		RE_PROFILE_FUNCTION();
		
		const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;
		if (entity.HasComponent<T>()) {
			ImGuiIO& io = ImGui::GetIO();
			auto boldFont = io.Fonts->Fonts[0];
			
			auto& component = entity.GetComponent<T>();
			ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
			float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
			ImGui::Separator();
			bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, name.c_str());
			ImGui::PopStyleVar();

			ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f);
			ImGui::PushFont(boldFont);
			if (ImGui::Button("...", ImVec2{ lineHeight, lineHeight })) {
				ImGui::OpenPopup("ComponentSettings");
			}
			ImGui::PopFont();

			bool removeComponent = false;
			if (ImGui::BeginPopup("ComponentSettings")) {
				if (ImGui::MenuItem("Remove component"))
					removeComponent = true;

				if (ImGui::MenuItem(("Copy " + name).c_str())) {
					m_ComponentCopyBuffer.ComponentID = typeid(T).hash_code();
					m_ComponentCopyBuffer.EntityID = entity.GetUUID();
				}

				//Checks if EntityID isn't set
				//Checks if the entity isn't just setting it's own component
				//Checks if the component hash is the same
				if (m_ComponentCopyBuffer.EntityID != 0 &&
					m_ComponentCopyBuffer.EntityID != entity.GetUUID() &&
					m_ComponentCopyBuffer.ComponentID == typeid(T).hash_code()) {
					Entity copyEntity = m_Context->GetEntityByUUID(m_ComponentCopyBuffer.EntityID);
					//Check if not either the entity was deleted or the component was deleted
					if (!copyEntity || !copyEntity.HasComponent<T>()) {
						m_ComponentCopyBuffer.ComponentID = 0;
						m_ComponentCopyBuffer.EntityID = 0;
					}
					else if(ImGui::MenuItem(("Paste " + name).c_str())) {
						entity.GetComponent<T>() = copyEntity.GetComponent<T>(); 
					}
				}

				ImGui::EndPopup();
			}

			if (open) {
				uiFunction(component);
				ImGui::TreePop();
			}

			if (removeComponent)
				entity.RemoveComponent<T>();
		}
	}

	void SceneHierarchyPanel::DrawComponents(Entity entity) {
		RE_PROFILE_FUNCTION();
	
		if (entity.HasComponent<TagComponent>()) {
			auto& tag = entity.GetComponent<TagComponent>().Tag;

			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strncpy_s(buffer, sizeof(buffer), tag.c_str(), sizeof(buffer));
			if (ImGui::InputText("##Tag", buffer, sizeof(buffer))) {
				tag = std::string(buffer);
			}
		}

		ImGui::SameLine();
		ImGui::PushItemWidth(-1);

		if (ImGui::Button("Add Component"))
			ImGui::OpenPopup("AddComponent");

		if (ImGui::BeginPopup("AddComponent")) {
			DisplayAddComponentEntry<TransformComponent>("Transform");
			DisplayAddComponentEntry<CameraComponent>("Camera");
			DisplayAddComponentEntry<ScriptComponent>("Script");
			DisplayAddComponentEntry<SpriteRendererComponent>("Sprite Renderer");
			DisplayAddComponentEntry<CircleRendererComponent>("Circle Renderer");
			DisplayAddComponentEntry<Rigidbody2DComponent>("Rigidbody 2D");
			DisplayAddComponentEntry<BoxCollider2DComponent>("Box Collider 2D");
			DisplayAddComponentEntry<CircleCollider2DComponent>("Circle Collider 2D");

			ImGui::EndPopup();
		}

		ImGui::PopItemWidth();

		DrawComponent<TransformComponent>("Transform", entity, [](auto& component) {
			DrawVec3Control("Translation", component.Translation);
			glm::vec3 rotation = glm::degrees(component.Rotation);
			DrawVec3Control("Rotation", rotation);
			component.Rotation = glm::radians(rotation);
			DrawVec3Control("Scale", component.Scale, 1.0f);
		});

		DrawComponent<CameraComponent>("Camera", entity, [](CameraComponent& component) {
			SceneCamera& camera = component.Camera;

			ImGui::Checkbox("Primary", &component.Primary);

			const char* projectionTypeStrings[] = { "Perspective", "Orthographic" };
			const char* currentProjectionTypeString = projectionTypeStrings[(int)camera.GetProjectionType()];
			if (ImGui::BeginCombo("Projection", currentProjectionTypeString)) {
				for (int i = 0; i < 2; i++) {
					bool isSelected = currentProjectionTypeString == projectionTypeStrings[i];
					if (ImGui::Selectable(projectionTypeStrings[i], isSelected)) {
						currentProjectionTypeString = projectionTypeStrings[i];
						camera.SetProjectionType((SceneCamera::ProjectionType)i);
					}

					if (isSelected)
						ImGui::SetItemDefaultFocus();
				}

				ImGui::EndCombo();
			}

			if (camera.GetProjectionType() == SceneCamera::ProjectionType::Perspective) {
				float perspectiveVerticalFov = glm::degrees(camera.GetPerspectiveVerticalFov());
				if (ImGui::DragFloat("Vertical FOV", &perspectiveVerticalFov))
					camera.SetPerspectiveVerticalFov(glm::radians(perspectiveVerticalFov));

				float perspectiveNear = camera.GetPerspectiveNearClip();
				if (ImGui::DragFloat("Near", &perspectiveNear))
					camera.SetPerspectiveNearClip(perspectiveNear);

				float perspectiveFar = camera.GetPerspectiveFarClip();
				if (ImGui::DragFloat("Far", &perspectiveFar))
					camera.SetPerspectiveFarClip(perspectiveFar);
			}

			if (camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic) {
				float orthoSize = camera.GetOrthographicSize();
				if (ImGui::DragFloat("Size", &orthoSize))
					camera.SetOrthographicSize(orthoSize);

				float orthoNear = camera.GetOrthgraphicNearClip();
				if (ImGui::DragFloat("Near", &orthoNear))
					camera.SetOrthgraphicNearClip(orthoNear);

				float orthoFar = camera.GetOrthgraphicFarClip();
				if (ImGui::DragFloat("Far", &orthoFar))
					camera.SetOrthgraphicFarClip(orthoFar);

				ImGui::Checkbox("Fixed Aspect Ratio", &component.FixedAspectRatio);
			}
		});

		DrawComponent<ScriptComponent>("Script", entity, [entity, scene = m_Context](auto& component) mutable {
			bool scriptClassExists = ScriptEngine::EntityClassExists(component.ClassName);

			static char buffer[64];
			strcpy_s(buffer, sizeof(buffer), component.ClassName.c_str());

			UI::ScopedStyleColor textColor(ImGuiCol_Text, ImVec4(0.9f, 0.2f, 0.3f, 1.0f), !scriptClassExists);

			if (ImGui::InputText("Class", buffer, sizeof(buffer))) {
				component.ClassName = buffer;
				return;
			}

			// Fields
			bool sceneRunning = scene->IsRunning();
			if (sceneRunning) {
				Ref<ScriptInstance> scriptInstance = ScriptEngine::GetEntityScriptInstance(entity.GetUUID());
				if (scriptInstance) {
					const auto& fields = scriptInstance->GetScriptClass()->GetFields();
					for (const auto& [name, field] : fields) {
						switch (field.Type) {
							case ScriptFieldType::Float: {
								float data = scriptInstance->GetFieldValue<float>(name);
								if (ImGui::DragFloat(name.c_str(), &data)) {
									scriptInstance->SetFieldValue(name, data);
								}
								break;
							}
							case ScriptFieldType::Double: {
								double data = scriptInstance->GetFieldValue<double>(name);
								if (ImGui::DragDouble(name.c_str(), &data)) {
									scriptInstance->SetFieldValue(name, data);
								}
								break;
							}
							case ScriptFieldType::Bool: {
								bool data = scriptInstance->GetFieldValue<bool>(name);
								if (ImGui::Checkbox(name.c_str(), &data)) {
									scriptInstance->SetFieldValue(name, data);
								}
								break;
							}
							case ScriptFieldType::Int: {
								int data = scriptInstance->GetFieldValue<int>(name);
								if (ImGui::DragInt(name.c_str(), &data)) {
									scriptInstance->SetFieldValue(name, data);
								}
								break;
							}
							case ScriptFieldType::UInt: {
								uint32_t data = scriptInstance->GetFieldValue<uint32_t>(name);
								if (ImGui::DragU32Int(name.c_str(), &data)) {
									scriptInstance->SetFieldValue(name, data);
								}
								break;
							}
							case ScriptFieldType::Vector2: {
								glm::vec2 data = scriptInstance->GetFieldValue<glm::vec2>(name);
								if (ImGui::DragFloat2(name.c_str(), glm::value_ptr(data), 0.01f)) {
									scriptInstance->SetFieldValue(name, data);
								}
								break;
							}
							case ScriptFieldType::Vector2Int: {
								glm::ivec2 data = scriptInstance->GetFieldValue<glm::ivec2>(name);
								if (ImGui::DragInt2(name.c_str(), glm::value_ptr(data), 0.01f)) {
									scriptInstance->SetFieldValue(name, data);
								}
								break;
							}
							case ScriptFieldType::Vector3: {
								glm::vec3 data = scriptInstance->GetFieldValue<glm::vec3>(name);
								if (ImGui::DragFloat3(name.c_str(), glm::value_ptr(data), 0.01f)) {
									scriptInstance->SetFieldValue(name, data);
								}
								break;
							}
							case ScriptFieldType::Vector3Int: {
								glm::ivec3 data = scriptInstance->GetFieldValue<glm::ivec3>(name);
								if (ImGui::DragInt3(name.c_str(), glm::value_ptr(data), 0.01f)) {
									scriptInstance->SetFieldValue(name, data);
								}
								break;
							}
							case ScriptFieldType::Vector4: {
								glm::vec4 data = scriptInstance->GetFieldValue<glm::vec4>(name);
								if (ImGui::DragFloat4(name.c_str(), glm::value_ptr(data), 0.01f)) {
									scriptInstance->SetFieldValue(name, data);
								}
								break;
							}
							case ScriptFieldType::Vector4Int: {
								glm::ivec4 data = scriptInstance->GetFieldValue<glm::ivec4>(name);
								if (ImGui::DragInt4(name.c_str(), glm::value_ptr(data), 0.01f)) {
									scriptInstance->SetFieldValue(name, data);
								}
								break;
							}
							case ScriptFieldType::Color: {
								glm::vec4 data = scriptInstance->GetFieldValue<glm::vec4>(name) / glm::vec4(255.0f);
								if (ImGui::ColorEdit4(name.c_str(), glm::value_ptr(data))) {
									data *= 255.0f;
									scriptInstance->SetFieldValue(name, data);
								}
								break;
							}
							case ScriptFieldType::Entity: {
								Entity entt = scene->GetEntityByUUID(scriptInstance->GetFieldValue<UUID>(name));
								std::string buttonLabel = "Entity: ";
								if (entt)
									buttonLabel += entt.GetComponent<TagComponent>().Tag;
								else
									buttonLabel += "Entity Deleted";

								DrawButtonLabel(buttonLabel, name, [scriptInstance, name]() {
									if (ImGui::BeginDragDropTarget()) {
										if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ENTITY")) {
											uint64_t* id = (uint64_t*)payload->Data;

											scriptInstance->SetFieldValue(name, *id);
										}
										ImGui::EndDragDropTarget();
									}
								});
								break;
							}
						}
					}
				}
			}
			else {
				if (scriptClassExists) {
					Ref<ScriptClass> entityClass = ScriptEngine::GetEntityClass(component.ClassName);
					const auto& fields = entityClass->GetFields();

					auto& entityFields = ScriptEngine::GetScriptFieldMap(entity);
					for (const auto& [name, field] : fields) {
						// Field has been set in editor
						if (entityFields.find(name) != entityFields.end()) {
							ScriptFieldInstance& scriptField = entityFields.at(name);

							// Display control to set it maybe
							switch (field.Type) {
								case ScriptFieldType::Float: {
									float data = scriptField.GetValue<float>();
									if (ImGui::DragFloat(name.c_str(), &data))
										scriptField.SetValue(data);
									break;
								}
								case ScriptFieldType::Double: {
									double data = scriptField.GetValue<double>();
									if (ImGui::DragDouble(name.c_str(), &data))
										scriptField.SetValue(data);
									break;
								}
								case ScriptFieldType::Bool: {
									bool data = scriptField.GetValue<bool>();
									if (ImGui::Checkbox(name.c_str(), &data))
										scriptField.SetValue(data);
									break;
								}
								case ScriptFieldType::Int: {
									int data = scriptField.GetValue<int>();
									if (ImGui::DragInt(name.c_str(), &data))
										scriptField.SetValue(data);
									break;
								}
								case ScriptFieldType::UInt: {
									uint32_t data = scriptField.GetValue<uint32_t>();
									if (ImGui::DragU32Int(name.c_str(), &data))
										scriptField.SetValue(data);
									break;
								}
								case ScriptFieldType::Vector2: {
									glm::vec2 data = scriptField.GetValue<glm::vec2>();
									if (ImGui::DragFloat2(name.c_str(), glm::value_ptr(data), 0.01f))
										scriptField.SetValue(data);
									break;
								}
								case ScriptFieldType::Vector2Int: {
									glm::ivec2 data = scriptField.GetValue<glm::ivec2>();
									if (ImGui::DragInt2(name.c_str(), glm::value_ptr(data), 0.01f))
										scriptField.SetValue(data);
									break;
								}
								case ScriptFieldType::Vector3: {
									glm::vec3 data = scriptField.GetValue<glm::vec3>();
									if (ImGui::DragFloat3(name.c_str(), glm::value_ptr(data), 0.01f))
										scriptField.SetValue(data);
									break;
								}
								case ScriptFieldType::Vector3Int: {
									glm::ivec3 data = scriptField.GetValue<glm::ivec3>();
									if (ImGui::DragInt3(name.c_str(), glm::value_ptr(data), 0.01f))
										scriptField.SetValue(data);
									break;
								}
								case ScriptFieldType::Vector4: {
									glm::vec4 data = scriptField.GetValue<glm::vec4>();
									if (ImGui::DragFloat4(name.c_str(), glm::value_ptr(data), 0.01f))
										scriptField.SetValue(data);
									break;
								}
								case ScriptFieldType::Vector4Int: {
									glm::ivec4 data = scriptField.GetValue<glm::ivec4>();
									if (ImGui::DragInt4(name.c_str(), glm::value_ptr(data), 0.01f))
										scriptField.SetValue(data);
									break;
								}
								case ScriptFieldType::Color: {
									glm::vec4 data = scriptField.GetValue<glm::vec4>() / glm::vec4(255.0f);
									if (ImGui::ColorEdit4(name.c_str(), glm::value_ptr(data))) {
										data *= 255.0f;
										scriptField.SetValue(data);
									}
									break;
								}
								case ScriptFieldType::Entity: {
									Entity entt = scene->GetEntityByUUID(scriptField.GetValue<UUID>());
									std::string& enttName = name + ": ";
									std::string buttonLabel = "Entity: ";
									if (entt)
										buttonLabel += entt.GetComponent<TagComponent>().Tag;
									else
										buttonLabel += "Entity Deleted";

									DrawButtonLabel(buttonLabel, name, [&scriptField]() {
										if (ImGui::BeginDragDropTarget()) {
											if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ENTITY")) {
												uint64_t* id = (uint64_t*)payload->Data;

												scriptField.SetValue(*id);
											}
											ImGui::EndDragDropTarget();
										}
									});
									break;
								}
							}
						}
						else {
							Ref<ScriptInstance> scriptInstance = ScriptEngine::GetEntityScriptInstance(entity.GetUUID());
							// Display control to set it maybe
							switch (field.Type) {
								case ScriptFieldType::Float: {
									float data = 0.0f;
									if (ImGui::DragFloat(name.c_str(), &data)) {
										ScriptFieldInstance& fieldInstance = entityFields[name];
										fieldInstance.Field = field;
										fieldInstance.SetValue(data);
									}
									break;
								}
								case ScriptFieldType::Double: {
									double data = 0.0;
									if (ImGui::DragDouble(name.c_str(), &data)) {
										ScriptFieldInstance& fieldInstance = entityFields[name];
										fieldInstance.Field = field;
										fieldInstance.SetValue(data);
									}
									break;
								}
								case ScriptFieldType::Bool: {
									bool data = false;
									if (ImGui::Checkbox(name.c_str(), &data)) {
										ScriptFieldInstance& fieldInstance = entityFields[name];
										fieldInstance.Field = field;
										fieldInstance.SetValue(data);
									}
									break;
								}
								case ScriptFieldType::Int: {
									int data = 0;
									if (ImGui::DragInt(name.c_str(), &data)) {
										ScriptFieldInstance& fieldInstance = entityFields[name];
										fieldInstance.Field = field;
										fieldInstance.SetValue(data);
									}
									break;
								}
								case ScriptFieldType::UInt: {
									uint32_t data = 0;
									if (ImGui::DragU32Int(name.c_str(), &data)) {
										ScriptFieldInstance& fieldInstance = entityFields[name];
										fieldInstance.Field = field;
										fieldInstance.SetValue(data);
									}
									break;
								}
								case ScriptFieldType::Vector2: {
									glm::vec2 data = glm::vec2{ 0.0f };
									if (ImGui::DragFloat2(name.c_str(), glm::value_ptr(data), 0.01f)) {
										ScriptFieldInstance& fieldInstance = entityFields[name];
										fieldInstance.Field = field;
										fieldInstance.SetValue(data);
									}
									break;
								}
								case ScriptFieldType::Vector2Int: {
									glm::ivec2 data = glm::ivec2{ 0 };
									if (ImGui::DragInt2(name.c_str(), glm::value_ptr(data), 0.01f)) {
										ScriptFieldInstance& fieldInstance = entityFields[name];
										fieldInstance.Field = field;
										fieldInstance.SetValue(data);
									}
									break;
								}
								case ScriptFieldType::Vector3: {
									glm::vec3 data = glm::vec3{ 0.0f };
									if (ImGui::DragFloat3(name.c_str(), glm::value_ptr(data), 0.01f)) {
										ScriptFieldInstance& fieldInstance = entityFields[name];
										fieldInstance.Field = field;
										fieldInstance.SetValue(data);
									}
									break;
								}
								case ScriptFieldType::Vector3Int: {
									glm::ivec3 data = glm::ivec3{ 0 };
									if (ImGui::DragInt3(name.c_str(), glm::value_ptr(data), 0.01f)) {
										ScriptFieldInstance& fieldInstance = entityFields[name];
										fieldInstance.Field = field;
										fieldInstance.SetValue(data);
									}
									break;
								}
								case ScriptFieldType::Vector4: {
									glm::vec4 data = glm::vec4{ 0.0f };
									if (ImGui::DragFloat4(name.c_str(), glm::value_ptr(data), 0.01f)) {
										ScriptFieldInstance& fieldInstance = entityFields[name];
										fieldInstance.Field = field;
										fieldInstance.SetValue(data);
									}
									break;
								}
								case ScriptFieldType::Vector4Int: {
									glm::ivec4 data = glm::ivec4{ 0 };
									if (ImGui::DragInt4(name.c_str(), glm::value_ptr(data), 0.01f)) {
										ScriptFieldInstance& fieldInstance = entityFields[name];
										fieldInstance.Field = field;
										fieldInstance.SetValue(data);
									}
									break;
								}
								case ScriptFieldType::Color: {
									glm::vec4 data = glm::vec4{ 0.0f, 0.0f, 0.0f, 1.0f };
									if (ImGui::ColorEdit4(name.c_str(), glm::value_ptr(data))) {
										ScriptFieldInstance& fieldInstance = entityFields[name];
										fieldInstance.Field = field;
										data *= 255.0f;
										fieldInstance.SetValue(data);
									}
									break;
								}
								case ScriptFieldType::Entity: {
									DrawButtonLabel("Entity: None", name, [&fieldInstance = entityFields[name], name]() {
										if (ImGui::BeginDragDropTarget()) {
											if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ENTITY")) {
												uint64_t* id = (uint64_t*)payload->Data;

												fieldInstance.SetValue(*id);
											}
											ImGui::EndDragDropTarget();
										}
									});
									break;
								}
							}
						}
					}
				}
			}
		});

		DrawComponent<SpriteRendererComponent>("Sprite Renderer", entity, [](auto& component) {
			ImGui::ColorEdit4("Color", glm::value_ptr(component.Color));

			if (ImGui::Button("Texture", ImVec2(100.0f, 0.0f))) {
				std::filesystem::path texturePath = FileDialogs::OpenFile("Image Files");
				if (!texturePath.empty()) {
					if (FileExtenstion::DoesExtensionExist(FileExtenstion::STBI_IMAGE_EXTENSTIONS, texturePath.extension().string()))
						component.Texture = Texture2D::Create(texturePath);
					else
						RE_CORE_WARN("{0} is not a proper image extention", texturePath.extension().string());
				}
			}
			
			if (ImGui::BeginDragDropTarget()) {
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_IMAGE")) {
					const wchar_t* path = (const wchar_t*)payload->Data;
					std::filesystem::path texturePath(path);
					component.Texture = Texture2D::Create(texturePath.string());
				}
				ImGui::EndDragDropTarget();
			}

			ImGui::DragFloat("Tiling Factor", &component.TilingFactor, 0.1f, 0.0f, 100.0f);
		});

		DrawComponent<CircleRendererComponent>("Circle Renderer", entity, [](auto& component) {
			ImGui::ColorEdit4("Color", glm::value_ptr(component.Color));
			ImGui::DragFloat("Thickness", &component.Thickness, 0.025f, 0.0f, 1.0f);
			ImGui::DragFloat("Fade", &component.Fade, 0.00025f, 0.0f, 1.0f);
		});

		DrawComponent<Rigidbody2DComponent>("Rigidbody 2D", entity, [](auto& component) {
			const char* bodyTypeStrings[] = { "Static", "Dynamic", "Kinematic" };
			const char* currentBodyTypeString = bodyTypeStrings[(int)component.Type];
			if (ImGui::BeginCombo("Body Type", currentBodyTypeString)) {
				for (int i = 0; i < 3; i++) {
					bool isSelected = currentBodyTypeString == bodyTypeStrings[i];
					if (ImGui::Selectable(bodyTypeStrings[i], isSelected)) {
						currentBodyTypeString = bodyTypeStrings[i];
						component.Type = (Rigidbody2DComponent::BodyType)i;
					}

					if (isSelected)
						ImGui::SetItemDefaultFocus();
				}

				ImGui::EndCombo();
			}

			ImGui::Checkbox("Fixed Rotation", &component.FixedRotation);
		});

		DrawComponent<BoxCollider2DComponent>("Box Collider 2D", entity, [](auto& component) {
			ImGui::DragFloat2("Offset", glm::value_ptr(component.Offset), 0.01f);
			ImGui::DragFloat2("Size", glm::value_ptr(component.Size), 0.01f);
			ImGui::Checkbox("Sensor", &component.IsSensor);
			ImGui::DragFloat("Density", &component.Density, 0.01f, 0.0f, 1.0f);
			ImGui::DragFloat("Friction", &component.Friction, 0.01f, 0.0f, 1.0f);
			ImGui::DragFloat("Restitution", &component.Restitution, 0.01f, 0.0f, 1.0f);
			ImGui::DragFloat("Restitution Threshold", &component.RestitutionThreshold, 0.01f, 0.0f);
		});

		DrawComponent<CircleCollider2DComponent>("Circle Collider 2D", entity, [](auto& component) {
			ImGui::DragFloat2("Offset", glm::value_ptr(component.Offset), 0.01f);
			ImGui::DragFloat("Radius", &component.Radius, 0.01f);
			ImGui::Checkbox("Sensor", &component.IsSensor);
			ImGui::DragFloat("Density", &component.Density, 0.01f, 0.0f, 1.0f);
			ImGui::DragFloat("Friction", &component.Friction, 0.01f, 0.0f, 1.0f);
			ImGui::DragFloat("Restitution", &component.Restitution, 0.01f, 0.0f, 1.0f);
			ImGui::DragFloat("Restitution Threshold", &component.RestitutionThreshold, 0.01f, 0.0f);
		});
	}

	template<typename T>
	void SceneHierarchyPanel::DisplayAddComponentEntry(const std::string& entryName) {
		RE_PROFILE_FUNCTION();
	
		if (!m_SelectionContext.HasComponent<T>()) {
			if (ImGui::MenuItem(entryName.c_str())) {
				m_SelectionContext.AddComponent<T>();
				ImGui::CloseCurrentPopup();
			}
		}
	}
}
