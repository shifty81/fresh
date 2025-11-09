#include "core/SceneManager.h"

#include <iostream>

#include <glm/gtc/matrix_transform.hpp>

namespace fresh
{

// SceneNode implementation
SceneNode::SceneNode(const std::string& name) : name(name) {}

SceneNode::~SceneNode() {}

void SceneNode::addChild(std::shared_ptr<SceneNode> child)
{
    if (child) {
        child->parent = this;
        children.push_back(child);
    }
}

void SceneNode::removeChild(std::shared_ptr<SceneNode> child)
{
    auto it = std::find(children.begin(), children.end(), child);
    if (it != children.end()) {
        (*it)->parent = nullptr;
        children.erase(it);
    }
}

glm::mat4 SceneNode::getLocalTransform() const
{
    glm::mat4 transform = glm::mat4(1.0f);
    transform = glm::translate(transform, position);
    transform *= glm::mat4_cast(rotation);
    transform = glm::scale(transform, scale);
    return transform;
}

glm::mat4 SceneNode::getWorldTransform() const
{
    if (parent) {
        return parent->getWorldTransform() * getLocalTransform();
    }
    return getLocalTransform();
}

void SceneNode::update(float deltaTime)
{
    if (!active)
        return;

    for (auto& child : children) {
        if (child) {
            child->update(deltaTime);
        }
    }
}

void SceneNode::render()
{
    if (!active)
        return;

    for (auto& child : children) {
        if (child) {
            child->render();
        }
    }
}

// Scene implementation
Scene::Scene(const std::string& name) : name(name)
{
    root = std::make_shared<SceneNode>("Root");
}

Scene::~Scene()
{
    shutdown();
}

void Scene::initialize()
{
    std::cout << "Initializing scene: " << name << std::endl;
    loaded = true;
}

void Scene::shutdown()
{
    std::cout << "Shutting down scene: " << name << std::endl;
    root->clearChildren();
    loaded = false;
}

void Scene::update(float deltaTime)
{
    if (loaded && root) {
        root->update(deltaTime);
    }
}

void Scene::render()
{
    if (loaded && root) {
        root->render();
    }
}

void Scene::addNode(std::shared_ptr<SceneNode> node)
{
    if (root && node) {
        root->addChild(node);
    }
}

SceneNode* Scene::findNode(const std::string& nodeName)
{
    // Simple recursive search (could be optimized with a map)
    std::function<SceneNode*(SceneNode*, const std::string&)> search;
    search = [&](SceneNode* node, const std::string& targetName) -> SceneNode* {
        if (!node)
            return nullptr;
        if (node->getName() == targetName)
            return node;

        for (auto& child : node->getChildren()) {
            SceneNode* result = search(child.get(), targetName);
            if (result)
                return result;
        }
        return nullptr;
    };

    return search(root.get(), nodeName);
}

// SceneManager implementation
SceneManager& SceneManager::getInstance()
{
    static SceneManager instance;
    return instance;
}

Scene* SceneManager::createScene(const std::string& name)
{
    auto scene = std::make_unique<Scene>(name);
    Scene* ptr = scene.get();
    scenes[name] = std::move(scene);

    std::cout << "Created scene: " << name << std::endl;
    return ptr;
}

Scene* SceneManager::loadScene(const std::string& path)
{
    // TODO: Implement scene loading from file
    std::cout << "Loading scene from: " << path << std::endl;
    return nullptr;
}

bool SceneManager::saveScene(const std::string& path, Scene* scene)
{
    if (!scene)
        return false;

    // TODO: Implement scene saving to file
    std::cout << "Saving scene to: " << path << std::endl;
    return true;
}

void SceneManager::setActiveScene(const std::string& name)
{
    auto it = scenes.find(name);
    if (it != scenes.end()) {
        activeScene = it->second.get();
        if (!activeScene->isLoaded()) {
            activeScene->initialize();
        }
        std::cout << "Set active scene: " << name << std::endl;
    }
}

void SceneManager::unloadScene(const std::string& name)
{
    auto it = scenes.find(name);
    if (it != scenes.end()) {
        if (it->second.get() == activeScene) {
            activeScene = nullptr;
        }
        it->second->shutdown();
        scenes.erase(it);
        std::cout << "Unloaded scene: " << name << std::endl;
    }
}

Scene* SceneManager::getScene(const std::string& name)
{
    auto it = scenes.find(name);
    return (it != scenes.end()) ? it->second.get() : nullptr;
}

void SceneManager::update(float deltaTime)
{
    if (activeScene) {
        activeScene->update(deltaTime);
    }

    // Handle transitions
    if (transitioning) {
        transitionTime += deltaTime;
        if (transitionTime >= transitionDuration) {
            setActiveScene(targetScene);
            transitioning = false;
            transitionTime = 0.0f;
        }
    }
}

void SceneManager::render()
{
    if (activeScene) {
        activeScene->render();
    }
}

void SceneManager::transitionTo(const std::string& sceneName, float fadeTime)
{
    transitioning = true;
    targetScene = sceneName;
    transitionDuration = fadeTime;
    transitionTime = 0.0f;

    std::cout << "Transitioning to scene: " << sceneName << std::endl;
}

} // namespace fresh
