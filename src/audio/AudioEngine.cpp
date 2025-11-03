#include "audio/AudioEngine.h"
#include "core/ResourceManager.h"
#include <algorithm>
#include <iostream>

namespace fresh {

AudioEngine& AudioEngine::getInstance() {
    static AudioEngine instance;
    return instance;
}

bool AudioEngine::initialize() {
    if (initialized) return true;
    
    std::cout << "Initializing AudioEngine..." << std::endl;
    
    // TODO: Initialize audio library (OpenAL, FMOD, etc.)
    // For now, this is a stub implementation
    
    initialized = true;
    std::cout << "AudioEngine initialized successfully" << std::endl;
    return true;
}

void AudioEngine::shutdown() {
    if (!initialized) return;
    
    std::cout << "Shutting down AudioEngine..." << std::endl;
    
    stopAll();
    stopMusic();
    
    // TODO: Cleanup audio library resources
    
    initialized = false;
}

void AudioEngine::update(float deltaTime) {
    if (!initialized) return;
    
    // Update 3D audio based on listener position
    for (auto& [id, source] : activeSources) {
        if (source.is3D && source.isPlaying) {
            // TODO: Update 3D audio attenuation based on distance
            float distance = glm::distance(listener.position, source.position);
            float attenuation = 1.0f;
            
            if (distance > source.minDistance) {
                attenuation = source.minDistance / 
                    std::min(distance, source.maxDistance);
            }
            
            // Apply attenuation to volume
            float effectiveVolume = source.volume * attenuation * effectsVolume * masterVolume;
            if (muted) effectiveVolume = 0.0f;
            
            // TODO: Set platform-specific audio source volume
        }
    }
    
    // Handle music fading
    if (fadingMusic) {
        fadeTime += deltaTime;
        float t = fadeTime / fadeDuration;
        
        if (t >= 1.0f) {
            fadingMusic = false;
            if (!fadeIn) {
                stopMusic();
            }
        } else {
            float fadeVolume = fadeIn ? t : (1.0f - t);
            setMusicVolume(musicVolume * fadeVolume);
        }
    }
    
    // Cleanup finished sounds
    cleanupFinishedSources();
}

int AudioEngine::play2D(const std::string& path, float volume, bool loop) {
    if (!initialized) return -1;
    
    std::cout << "Playing 2D audio: " << path << std::endl;
    
    int id = getNextSourceID();
    AudioSource source;
    // source.clip = ResourceManager::getInstance().load<AudioClipResource>(path);
    source.volume = volume;
    source.loop = loop;
    source.is3D = false;
    source.isPlaying = true;
    source.sourceID = id;
    
    activeSources[id] = source;
    
    // TODO: Actually play the audio using audio library
    
    return id;
}

int AudioEngine::play3D(const std::string& path, const glm::vec3& position,
                       float volume, bool loop) {
    if (!initialized) return -1;
    
    std::cout << "Playing 3D audio: " << path << " at (" 
              << position.x << ", " << position.y << ", " << position.z << ")" << std::endl;
    
    int id = getNextSourceID();
    AudioSource source;
    // source.clip = ResourceManager::getInstance().load<AudioClipResource>(path);
    source.position = position;
    source.volume = volume;
    source.loop = loop;
    source.is3D = true;
    source.isPlaying = true;
    source.sourceID = id;
    
    activeSources[id] = source;
    
    // TODO: Actually play the audio using audio library with 3D positioning
    
    return id;
}

void AudioEngine::stop(int soundID) {
    auto it = activeSources.find(soundID);
    if (it != activeSources.end()) {
        it->second.isPlaying = false;
        // TODO: Stop platform-specific audio source
        activeSources.erase(it);
    }
}

void AudioEngine::stopAll() {
    for (auto& [id, source] : activeSources) {
        source.isPlaying = false;
        // TODO: Stop platform-specific audio source
    }
    activeSources.clear();
}

void AudioEngine::pause(int soundID) {
    auto it = activeSources.find(soundID);
    if (it != activeSources.end()) {
        it->second.isPlaying = false;
        // TODO: Pause platform-specific audio source
    }
}

void AudioEngine::resume(int soundID) {
    auto it = activeSources.find(soundID);
    if (it != activeSources.end()) {
        it->second.isPlaying = true;
        // TODO: Resume platform-specific audio source
    }
}

void AudioEngine::setVolume(int soundID, float volume) {
    auto it = activeSources.find(soundID);
    if (it != activeSources.end()) {
        it->second.volume = volume;
        // TODO: Update platform-specific audio source volume
    }
}

bool AudioEngine::isPlaying(int soundID) const {
    auto it = activeSources.find(soundID);
    return (it != activeSources.end()) && it->second.isPlaying;
}

void AudioEngine::playMusic(const std::string& path, float volume, bool loop) {
    stopMusic();
    
    std::cout << "Playing music: " << path << std::endl;
    
    musicSource.volume = volume;
    musicSource.loop = loop;
    musicSource.isPlaying = true;
    musicVolume = volume;
    
    // TODO: Load and play music file
}

void AudioEngine::stopMusic() {
    if (musicSource.isPlaying) {
        musicSource.isPlaying = false;
        // TODO: Stop music playback
    }
}

void AudioEngine::setMusicVolume(float volume) {
    musicVolume = volume;
    // TODO: Update music volume
}

void AudioEngine::fadeMusicIn(float duration) {
    fadingMusic = true;
    fadeIn = true;
    fadeTime = 0.0f;
    fadeDuration = duration;
}

void AudioEngine::fadeMusicOut(float duration) {
    fadingMusic = true;
    fadeIn = false;
    fadeTime = 0.0f;
    fadeDuration = duration;
}

void AudioEngine::setListenerPosition(const glm::vec3& position) {
    listener.position = position;
    // TODO: Update platform-specific listener position
}

void AudioEngine::setListenerVelocity(const glm::vec3& velocity) {
    listener.velocity = velocity;
    // TODO: Update platform-specific listener velocity (for doppler effect)
}

void AudioEngine::setListenerOrientation(const glm::vec3& forward, const glm::vec3& up) {
    listener.forward = forward;
    listener.up = up;
    // TODO: Update platform-specific listener orientation
}

void AudioEngine::setMasterVolume(float volume) {
    masterVolume = std::clamp(volume, 0.0f, 1.0f);
    // TODO: Update all active sources
}

void AudioEngine::setMuted(bool isMuted) {
    muted = isMuted;
    // TODO: Mute/unmute all audio sources
}

int AudioEngine::getNextSourceID() {
    return nextSourceID++;
}

void AudioEngine::cleanupFinishedSources() {
    // Remove sources that have finished playing (non-looping)
    auto it = activeSources.begin();
    while (it != activeSources.end()) {
        if (!it->second.loop && !it->second.isPlaying) {
            it = activeSources.erase(it);
        } else {
            ++it;
        }
    }
}

} // namespace fresh
