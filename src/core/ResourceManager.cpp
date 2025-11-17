#include "core/ResourceManager.h"

#include <algorithm>
#include <filesystem>
#include <iostream>

namespace fresh
{

ResourceManager::ResourceManager()
{
    startAsyncLoader();
}

ResourceManager::~ResourceManager()
{
    stopAsyncLoader();
}

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

    // Create default placeholder resources
    auto placeholderTexture = std::make_shared<TextureResource>("__placeholder_texture__");
    placeholderTexture->load();
    placeholders["texture"] = placeholderTexture;

    auto placeholderMesh = std::make_shared<MeshResource>("__placeholder_mesh__");
    placeholderMesh->load();
    placeholders["mesh"] = placeholderMesh;

    auto placeholderAudio = std::make_shared<AudioClipResource>("__placeholder_audio__");
    placeholderAudio->load();
    placeholders["audio"] = placeholderAudio;
}

void ResourceManager::shutdown()
{
    std::lock_guard<std::mutex> lock(mutex);

    std::cout << "ResourceManager shutting down. Unloading " << resources.size() << " resources..."
              << std::endl;

    // Stop async loader first
    stopAsyncLoader();

    for (auto& [path, resource] : resources) {
        if (resource && resource->isLoaded()) {
            resource->unload();
        }
    }

    resources.clear();
    placeholders.clear();
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
    std::cout << "Loading texture: " << path << std::endl;
    
    // Check if file exists
    if (!std::filesystem::exists(path)) {
        std::cerr << "ERROR: Texture file not found: " << path << std::endl;
        std::cerr << "Using placeholder texture instead" << std::endl;
        // Will be handled by ResourceManager fallback
        return;
    }
    
    try {
        // TODO: Implement actual texture loading (using stb_image)
        // For now, just mark as loaded
        loaded = true;
        std::cout << "Texture loaded successfully: " << path << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "ERROR loading texture " << path << ": " << e.what() << std::endl;
        loaded = false;
    }
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
    std::cout << "Loading mesh: " << path << std::endl;
    
    // Check if file exists
    if (!std::filesystem::exists(path)) {
        std::cerr << "ERROR: Mesh file not found: " << path << std::endl;
        std::cerr << "Using placeholder mesh instead" << std::endl;
        return;
    }
    
    try {
        // TODO: Implement actual mesh loading (using tinyobjloader)
        // For now, just mark as loaded
        loaded = true;
        std::cout << "Mesh loaded successfully: " << path << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "ERROR loading mesh " << path << ": " << e.what() << std::endl;
        loaded = false;
    }
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
    std::cout << "Loading audio: " << path << std::endl;
    
    // Check if file exists
    if (!std::filesystem::exists(path)) {
        std::cerr << "ERROR: Audio file not found: " << path << std::endl;
        std::cerr << "Using placeholder audio instead" << std::endl;
        return;
    }
    
    // Check file extension for supported formats
    std::string ext = std::filesystem::path(path).extension().string();
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
    
    if (ext != ".wav" && ext != ".ogg" && ext != ".mp3") {
        std::cerr << "WARNING: Unsupported audio format: " << ext << std::endl;
        std::cerr << "Supported formats: .wav, .ogg, .mp3" << std::endl;
    }
    
    try {
        // TODO: Implement actual audio loading (integrate with AudioEngine)
        // For now, just mark as loaded
        loaded = true;
        std::cout << "Audio loaded successfully: " << path << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "ERROR loading audio " << path << ": " << e.what() << std::endl;
        loaded = false;
    }
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

// Async loading implementation
void ResourceManager::startAsyncLoader()
{
    asyncLoaderRunning = true;
    asyncLoaderThread = std::thread(&ResourceManager::asyncLoadingWorker, this);
}

void ResourceManager::stopAsyncLoader()
{
    if (asyncLoaderRunning) {
        asyncLoaderRunning = false;
        loaderCondition.notify_all();
        if (asyncLoaderThread.joinable()) {
            asyncLoaderThread.join();
        }
    }
}

void ResourceManager::asyncLoadingWorker()
{
    while (asyncLoaderRunning) {
        LoadingRequest request;
        
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            loaderCondition.wait(lock, [this] { 
                return !loadingQueue.empty() || !asyncLoaderRunning; 
            });

            if (!asyncLoaderRunning) {
                break;
            }

            if (!loadingQueue.empty()) {
                request = std::move(loadingQueue.front());
                loadingQueue.pop();
            } else {
                continue;
            }
        }

        try {
            // Load the resource
            std::shared_ptr<Resource> resource = loadResource(request.path, request.type);
            
            if (resource) {
                // Store in resource map
                {
                    std::lock_guard<std::mutex> lock(mutex);
                    resources[request.path] = resource;
                }
                request.promise.set_value(resource);
            } else {
                // Failed to load - use placeholder
                std::cerr << "Failed to load resource: " << request.path 
                         << " - using placeholder" << std::endl;
                
                // Get appropriate placeholder
                std::shared_ptr<Resource> placeholder;
                {
                    std::lock_guard<std::mutex> lock(mutex);
                    switch (request.type) {
                        case ResourceType::Texture:
                            placeholder = placeholders["texture"];
                            break;
                        case ResourceType::Mesh:
                            placeholder = placeholders["mesh"];
                            break;
                        case ResourceType::Audio:
                            placeholder = placeholders["audio"];
                            break;
                        default:
                            break;
                    }
                }
                request.promise.set_value(placeholder);
            }
        } catch (const std::exception& e) {
            std::cerr << "Exception loading resource: " << request.path 
                     << " - " << e.what() << std::endl;
            request.promise.set_exception(std::current_exception());
        }

        completedLoadRequests++;
    }
}

bool ResourceManager::isLoading(const std::string& path) const
{
    std::lock_guard<std::mutex> lock(queueMutex);
    
    // Check if path is in loading queue
    std::queue<LoadingRequest> tempQueue = loadingQueue;
    while (!tempQueue.empty()) {
        if (tempQueue.front().path == path) {
            return true;
        }
        tempQueue.pop();
    }
    return false;
}

float ResourceManager::getLoadingProgress() const
{
    size_t total = totalLoadRequests.load();
    if (total == 0) {
        return 1.0f;
    }
    return static_cast<float>(completedLoadRequests.load()) / static_cast<float>(total);
}

} // namespace fresh
