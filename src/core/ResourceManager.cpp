#include "core/ResourceManager.h"

#include <algorithm>
#include <filesystem>
#include <iostream>

namespace fresh
{

ResourceManager& ResourceManager::getInstance()
{
    static ResourceManager instance;
    return instance;
}

void ResourceManager::initialize(const std::string& assetDir)
{
    std::lock_guard<std::mutex> lock(mutex);
    assetDirectory = assetDir;

    std::cout << "ResourceManager initialized with asset directory: " << assetDirectory
              << std::endl;

    // Register default loaders
    registerLoader(ResourceType::Texture,
                   [](const std::string& path) { return std::make_shared<TextureResource>(path); });

    registerLoader(ResourceType::Mesh,
                   [](const std::string& path) { return std::make_shared<MeshResource>(path); });

    registerLoader(ResourceType::Audio, [](const std::string& path) {
        return std::make_shared<AudioClipResource>(path);
    });
}

void ResourceManager::shutdown()
{
    std::lock_guard<std::mutex> lock(mutex);

    std::cout << "ResourceManager shutting down. Unloading " << resources.size() << " resources..."
              << std::endl;

    for (auto& [path, resource] : resources) {
        if (resource && resource->isLoaded()) {
            resource->unload();
        }
    }

    resources.clear();
}

void ResourceManager::unload(const std::string& path)
{
    std::lock_guard<std::mutex> lock(mutex);

    auto it = resources.find(path);
    if (it != resources.end()) {
        if (it->second && it->second->isLoaded()) {
            it->second->unload();
        }
        resources.erase(it);
    }
}

void ResourceManager::unloadType(ResourceType type)
{
    std::lock_guard<std::mutex> lock(mutex);

    auto it = resources.begin();
    while (it != resources.end()) {
        if (it->second && it->second->getType() == type) {
            if (it->second->isLoaded()) {
                it->second->unload();
            }
            it = resources.erase(it);
        } else {
            ++it;
        }
    }
}

void ResourceManager::unloadUnused()
{
    std::lock_guard<std::mutex> lock(mutex);

    auto it = resources.begin();
    while (it != resources.end()) {
        if (it->second.use_count() <= 1) { // Only ResourceManager holds reference
            if (it->second->isLoaded()) {
                it->second->unload();
            }
            it = resources.erase(it);
        } else {
            ++it;
        }
    }
}

void ResourceManager::hotReload(const std::string& path)
{
    std::lock_guard<std::mutex> lock(mutex);

    auto it = resources.find(path);
    if (it != resources.end() && it->second) {
        std::cout << "Hot reloading resource: " << path << std::endl;
        it->second->unload();
        it->second->load();
    }
}

size_t ResourceManager::getTotalMemoryUsage() const
{
    std::lock_guard<std::mutex> lock(mutex);

    size_t total = 0;
    for (const auto& [path, resource] : resources) {
        if (resource && resource->isLoaded()) {
            total += resource->getMemoryUsage();
        }
    }
    return total;
}

size_t ResourceManager::getLoadedResourceCount() const
{
    std::lock_guard<std::mutex> lock(mutex);

    size_t count = 0;
    for (const auto& [path, resource] : resources) {
        if (resource && resource->isLoaded()) {
            count++;
        }
    }
    return count;
}

void ResourceManager::registerLoader(
    ResourceType type, std::function<std::shared_ptr<Resource>(const std::string&)> loader)
{
    std::lock_guard<std::mutex> lock(mutex);
    loaders[type] = loader;
}

void ResourceManager::scanDirectory(const std::string& directory, bool recursive)
{
    std::lock_guard<std::mutex> lock(mutex);

    std::cout << "Scanning directory: " << directory << std::endl;

    try {
        if (recursive) {
            for (const auto& entry : std::filesystem::recursive_directory_iterator(directory)) {
                if (entry.is_regular_file()) {
                    std::string path = entry.path().string();
                    ResourceType type = detectResourceType(path);
                    if (type != ResourceType::Unknown) {
                        // Register but don't load yet
                        std::cout << "  Found: " << path << std::endl;
                    }
                }
            }
        } else {
            for (const auto& entry : std::filesystem::directory_iterator(directory)) {
                if (entry.is_regular_file()) {
                    std::string path = entry.path().string();
                    ResourceType type = detectResourceType(path);
                    if (type != ResourceType::Unknown) {
                        std::cout << "  Found: " << path << std::endl;
                    }
                }
            }
        }
    } catch (const std::filesystem::filesystem_error& e) {
        std::cerr << "Error scanning directory: " << e.what() << std::endl;
    }
}

std::vector<std::string> ResourceManager::getResourcesOfType(ResourceType type) const
{
    std::lock_guard<std::mutex> lock(mutex);

    std::vector<std::string> result;
    for (const auto& [path, resource] : resources) {
        if (resource && resource->getType() == type) {
            result.push_back(path);
        }
    }
    return result;
}

bool ResourceManager::exists(const std::string& path) const
{
    std::lock_guard<std::mutex> lock(mutex);
    return resources.find(path) != resources.end();
}

void ResourceManager::printStats() const
{
    std::lock_guard<std::mutex> lock(mutex);

    std::cout << "=== Resource Manager Statistics ===" << std::endl;
    std::cout << "Total resources: " << resources.size() << std::endl;
    std::cout << "Loaded resources: " << getLoadedResourceCount() << std::endl;
    std::cout << "Memory usage: " << (getTotalMemoryUsage() / 1024 / 1024) << " MB" << std::endl;

    // Count by type
    std::map<ResourceType, int> typeCounts;
    for (const auto& [path, resource] : resources) {
        if (resource) {
            typeCounts[resource->getType()]++;
        }
    }

    std::cout << "\nResources by type:" << std::endl;
    for (const auto& [type, count] : typeCounts) {
        std::cout << "  Type " << static_cast<int>(type) << ": " << count << std::endl;
    }
}

std::shared_ptr<Resource> ResourceManager::loadResource(const std::string& path, ResourceType type)
{
    auto it = loaders.find(type);
    if (it != loaders.end()) {
        auto resource = it->second(path);
        if (resource) {
            resource->load();
            return resource;
        }
    }
    return nullptr;
}

ResourceType ResourceManager::detectResourceType(const std::string& path) const
{
    std::string ext = std::filesystem::path(path).extension().string();
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

    if (ext == ".png" || ext == ".jpg" || ext == ".jpeg" || ext == ".bmp" || ext == ".tga") {
        return ResourceType::Texture;
    } else if (ext == ".obj" || ext == ".fbx" || ext == ".gltf" || ext == ".glb") {
        return ResourceType::Mesh;
    } else if (ext == ".wav" || ext == ".ogg" || ext == ".mp3") {
        return ResourceType::Audio;
    } else if (ext == ".glsl" || ext == ".vert" || ext == ".frag") {
        return ResourceType::Shader;
    } else if (ext == ".asset" || ext == ".pack") {
        return ResourceType::AssetPack;
    }

    return ResourceType::Unknown;
}

// TextureResource implementation
TextureResource::TextureResource(const std::string& path)
{
    this->path = path;
    this->name = std::filesystem::path(path).stem().string();
    this->type = ResourceType::Texture;
}

void TextureResource::load()
{
    // TODO: Implement actual texture loading (using stb_image or similar)
    std::cout << "Loading texture: " << path << std::endl;
    loaded = true;
}

void TextureResource::unload()
{
    if (data) {
        delete[] data;
        data = nullptr;
    }
    loaded = false;
}

size_t TextureResource::getMemoryUsage() const
{
    return width * height * channels;
}

// MeshResource implementation
MeshResource::MeshResource(const std::string& path)
{
    this->path = path;
    this->name = std::filesystem::path(path).stem().string();
    this->type = ResourceType::Mesh;
}

void MeshResource::load()
{
    // TODO: Implement actual mesh loading (using tinyobjloader or similar)
    std::cout << "Loading mesh: " << path << std::endl;
    loaded = true;
}

void MeshResource::unload()
{
    vertices.clear();
    indices.clear();
    loaded = false;
}

size_t MeshResource::getMemoryUsage() const
{
    return vertices.size() * sizeof(float) + indices.size() * sizeof(uint32_t);
}

// AudioClipResource implementation
AudioClipResource::AudioClipResource(const std::string& path)
{
    this->path = path;
    this->name = std::filesystem::path(path).stem().string();
    this->type = ResourceType::Audio;
}

void AudioClipResource::load()
{
    // TODO: Implement actual audio loading
    std::cout << "Loading audio: " << path << std::endl;
    loaded = true;
}

void AudioClipResource::unload()
{
    samples.clear();
    loaded = false;
}

size_t AudioClipResource::getMemoryUsage() const
{
    return samples.size() * sizeof(int16_t);
}

} // namespace fresh
