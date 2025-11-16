#include "ui/InspectorPanel.h"

#include "core/Logger.h"
#include "ecs/EntityManager.h"
#include "ecs/TransformComponent.h"
#include "ecs/RendererComponent.h"
#include "ecs/MaterialComponent.h"
#include "physics/PhysicsComponent.h"
#include "ui/SceneHierarchyPanel.h"

#include <imgui.h>

namespace fresh
{

InspectorPanel::InspectorPanel()
    : m_visible(true), m_inspectedNode(nullptr), m_entityManager(nullptr)
{
}

InspectorPanel::~InspectorPanel() {}

bool InspectorPanel::initialize(ecs::EntityManager* entityManager)
{
    m_entityManager = entityManager;
    LOG_INFO_C("Inspector Panel initialized", "InspectorPanel");
    return true;
}

void InspectorPanel::render()
{
    if (!m_visible) {
        return;
    }

    ImGui::Begin("Inspector", &m_visible);

    if (!m_inspectedNode) {
        ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), "No object selected");
        ImGui::Text("Select an object in the Scene Hierarchy");
        ImGui::End();
        return;
    }

    // Object name
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 0.5f, 1.0f));
    ImGui::Text("Object: %s", m_inspectedNode->name.c_str());
    ImGui::PopStyleColor();
    ImGui::Separator();

    // Visibility checkbox
    ImGui::Checkbox("Visible", &m_inspectedNode->visible);
    ImGui::Separator();

    // Transform properties
    if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen)) {
        renderTransformProperties();
    }

    // Physics properties (if physics component exists)
    if (m_entityManager && m_inspectedNode->userData) {
        ecs::Entity* entity = static_cast<ecs::Entity*>(m_inspectedNode->userData);
        if (entity && entity->isValid() && m_entityManager->hasComponent<physics::PhysicsComponent>(*entity)) {
            if (ImGui::CollapsingHeader("Physics")) {
                renderPhysicsProperties();
            }
        }
    }

    // Renderer properties (if renderer component exists)
    if (m_entityManager && m_inspectedNode->userData) {
        ecs::Entity* entity = static_cast<ecs::Entity*>(m_inspectedNode->userData);
        if (entity && entity->isValid() && m_entityManager->hasComponent<ecs::RendererComponent>(*entity)) {
            if (ImGui::CollapsingHeader("Renderer")) {
                renderRendererProperties();
            }
        }
    }

    // Material properties (if material component exists)
    if (m_entityManager && m_inspectedNode->userData) {
        ecs::Entity* entity = static_cast<ecs::Entity*>(m_inspectedNode->userData);
        if (entity && entity->isValid() && m_entityManager->hasComponent<ecs::MaterialComponent>(*entity)) {
            if (ImGui::CollapsingHeader("Material")) {
                renderMaterialProperties();
            }
        }
    }

    // Component management
    if (ImGui::CollapsingHeader("Components")) {
        renderComponentProperties();
    }

    ImGui::End();
}

void InspectorPanel::renderTransformProperties()
{
    // Try to get transform component from entity
    ecs::TransformComponent* transform = nullptr;
    
    if (m_entityManager && m_inspectedNode && m_inspectedNode->userData) {
        ecs::Entity* entity = static_cast<ecs::Entity*>(m_inspectedNode->userData);
        if (entity && entity->isValid()) {
            transform = m_entityManager->getComponent<ecs::TransformComponent>(*entity);
        }
    }

    if (transform) {
        // Position
        ImGui::Text("Position");
        float position[3] = {transform->position.x, transform->position.y, transform->position.z};
        if (ImGui::DragFloat3("##pos", position, 0.1f)) {
            transform->position = glm::vec3(position[0], position[1], position[2]);
        }

        // Rotation (as Euler angles in degrees)
        glm::vec3 euler = transform->getEulerAngles();
        ImGui::Text("Rotation");
        float rotation[3] = {euler.x, euler.y, euler.z};
        if (ImGui::DragFloat3("##rot", rotation, 1.0f, -180.0f, 180.0f)) {
            transform->setEulerAngles(glm::vec3(rotation[0], rotation[1], rotation[2]));
        }

        // Scale
        ImGui::Text("Scale");
        float scale[3] = {transform->scale.x, transform->scale.y, transform->scale.z};
        if (ImGui::DragFloat3("##scale", scale, 0.1f, 0.01f, 100.0f)) {
            transform->scale = glm::vec3(scale[0], scale[1], scale[2]);
        }
    } else {
        // No transform component - show placeholder
        ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.7f, 1.0f), "No Transform Component");
        ImGui::Text("Position: N/A");
        ImGui::Text("Rotation: N/A");
        ImGui::Text("Scale: N/A");
        
        // Option to add transform component
        if (m_entityManager && m_inspectedNode && m_inspectedNode->userData) {
            ecs::Entity* entity = static_cast<ecs::Entity*>(m_inspectedNode->userData);
            if (entity && entity->isValid()) {
                if (ImGui::Button("+ Add Transform Component")) {
                    auto* newTransform = new ecs::TransformComponent();
                    m_entityManager->addComponent<ecs::TransformComponent>(*entity, newTransform);
                    LOG_INFO_C("Added Transform Component", "InspectorPanel");
                }
            }
        }
    }
}

void InspectorPanel::renderComponentProperties()
{
    if (!m_entityManager || !m_inspectedNode || !m_inspectedNode->userData) {
        ImGui::Text("No entity manager or entity available");
        return;
    }

    ecs::Entity* entity = static_cast<ecs::Entity*>(m_inspectedNode->userData);
    if (!entity || !entity->isValid()) {
        ImGui::Text("Invalid entity");
        return;
    }

    ImGui::Indent();
    ImGui::Text("Add Component:");
    ImGui::Separator();

    // Add component buttons
    if (!m_entityManager->hasComponent<ecs::TransformComponent>(*entity)) {
        if (ImGui::Button("+ Transform Component", ImVec2(-1, 0))) {
            auto* newComp = new ecs::TransformComponent();
            m_entityManager->addComponent<ecs::TransformComponent>(*entity, newComp);
            LOG_INFO_C("Added Transform Component", "InspectorPanel");
        }
    }

    if (!m_entityManager->hasComponent<physics::PhysicsComponent>(*entity)) {
        if (ImGui::Button("+ Physics Component", ImVec2(-1, 0))) {
            auto* newComp = new physics::PhysicsComponent();
            m_entityManager->addComponent<physics::PhysicsComponent>(*entity, newComp);
            LOG_INFO_C("Added Physics Component", "InspectorPanel");
        }
    }

    if (!m_entityManager->hasComponent<ecs::RendererComponent>(*entity)) {
        if (ImGui::Button("+ Renderer Component", ImVec2(-1, 0))) {
            auto* newComp = new ecs::RendererComponent();
            m_entityManager->addComponent<ecs::RendererComponent>(*entity, newComp);
            LOG_INFO_C("Added Renderer Component", "InspectorPanel");
        }
    }

    if (!m_entityManager->hasComponent<ecs::MaterialComponent>(*entity)) {
        if (ImGui::Button("+ Material Component", ImVec2(-1, 0))) {
            auto* newComp = new ecs::MaterialComponent();
            m_entityManager->addComponent<ecs::MaterialComponent>(*entity, newComp);
            LOG_INFO_C("Added Material Component", "InspectorPanel");
        }
    }

    ImGui::Unindent();
}

void InspectorPanel::renderPhysicsProperties()
{
    if (!m_entityManager || !m_inspectedNode || !m_inspectedNode->userData) {
        return;
    }

    ecs::Entity* entity = static_cast<ecs::Entity*>(m_inspectedNode->userData);
    if (!entity || !entity->isValid()) {
        return;
    }

    auto* physics = m_entityManager->getComponent<physics::PhysicsComponent>(*entity);
    if (!physics) {
        return;
    }

    ImGui::Indent();

    ImGui::Checkbox("Use Gravity", &physics->useGravity);
    ImGui::Checkbox("Is Static", &physics->isStatic);

    ImGui::Text("Mass");
    ImGui::DragFloat("##mass", &physics->mass, 0.1f, 0.1f, 1000.0f);

    ImGui::Text("Friction");
    ImGui::SliderFloat("##friction", &physics->friction, 0.0f, 1.0f);

    ImGui::Text("Restitution (Bounciness)");
    ImGui::SliderFloat("##restitution", &physics->restitution, 0.0f, 1.0f);

    ImGui::Text("Linear Drag");
    ImGui::SliderFloat("##drag", &physics->drag, 0.0f, 1.0f);

    ImGui::Text("Angular Drag");
    ImGui::SliderFloat("##angularDrag", &physics->angularDrag, 0.0f, 1.0f);

    // Velocity (read-only for now)
    ImGui::Separator();
    ImGui::Text("Velocity: (%.2f, %.2f, %.2f)", physics->velocity.x, physics->velocity.y,
                physics->velocity.z);
    ImGui::Text("Angular Velocity: (%.2f, %.2f, %.2f)", physics->angularVelocity.x,
                physics->angularVelocity.y, physics->angularVelocity.z);

    ImGui::Unindent();
}

void InspectorPanel::renderRendererProperties()
{
    if (!m_entityManager || !m_inspectedNode || !m_inspectedNode->userData) {
        return;
    }

    ecs::Entity* entity = static_cast<ecs::Entity*>(m_inspectedNode->userData);
    if (!entity || !entity->isValid()) {
        return;
    }

    auto* renderer = m_entityManager->getComponent<ecs::RendererComponent>(*entity);
    if (!renderer) {
        return;
    }

    ImGui::Indent();

    ImGui::Checkbox("Visible", &renderer->visible);
    ImGui::Checkbox("Cast Shadows", &renderer->castShadows);
    ImGui::Checkbox("Receive Shadows", &renderer->receiveShadows);

    ImGui::Text("Render Layer");
    ImGui::SliderInt("##layer", &renderer->renderLayer, 0, 31);

    ImGui::Unindent();
}

void InspectorPanel::renderMaterialProperties()
{
    if (!m_entityManager || !m_inspectedNode || !m_inspectedNode->userData) {
        return;
    }

    ecs::Entity* entity = static_cast<ecs::Entity*>(m_inspectedNode->userData);
    if (!entity || !entity->isValid()) {
        return;
    }

    auto* material = m_entityManager->getComponent<ecs::MaterialComponent>(*entity);
    if (!material) {
        return;
    }

    ImGui::Indent();

    // Shader selection
    const char* shaders[] = {"Standard", "Unlit", "Water", "Transparent", "Custom"};
    int currentShader = static_cast<int>(material->shaderType);
    ImGui::Text("Shader");
    if (ImGui::Combo("##shader", &currentShader, shaders, IM_ARRAYSIZE(shaders))) {
        material->shaderType = static_cast<ecs::MaterialComponent::ShaderType>(currentShader);
    }

    // Color
    ImGui::Text("Color");
    float color[4] = {material->color.r, material->color.g, material->color.b, material->color.a};
    if (ImGui::ColorEdit4("##color", color)) {
        material->color = glm::vec4(color[0], color[1], color[2], color[3]);
    }

    // Texture
    ImGui::Text("Texture");
    std::string buttonLabel =
        material->texturePath.empty() ? "None (Texture)" : material->texturePath;
    ImGui::Button(buttonLabel.c_str(), ImVec2(200, 0));
    if (ImGui::BeginDragDropTarget()) {
        // Texture drag-drop handling would be implemented here
        // when asset drag-drop system is complete
        ImGui::EndDragDropTarget();
    }

    // Material properties
    ImGui::Text("Metallic");
    ImGui::SliderFloat("##metallic", &material->metallic, 0.0f, 1.0f);

    ImGui::Text("Smoothness");
    ImGui::SliderFloat("##smoothness", &material->smoothness, 0.0f, 1.0f);

    ImGui::Text("Emission");
    ImGui::SliderFloat("##emission", &material->emission, 0.0f, 5.0f);

    ImGui::Unindent();
}

} // namespace fresh
