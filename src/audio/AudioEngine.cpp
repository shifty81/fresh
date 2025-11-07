#include "audio/AudioEngine.h"
#include "core/ResourceManager.h"
#include <algorithm>
#include <iostream>

#ifdef FRESH_OPENAL_AVAILABLE
#include <AL/al.h>
#include <AL/alc.h>
#endif

namespace fresh {

#ifdef FRESH_OPENAL_AVAILABLE
// OpenAL context variables
static ALCdevice* g_audioDevice = nullptr;
static ALCcontext* g_audioContext = nullptr;
#endif

AudioEngine& AudioEngine::getInstance() {
    static AudioEngine instance;
    return instance;
}

bool AudioEngine::initialize() {
    if (initialized) return true;
    
    std::cout << "Initializing AudioEngine..." << std::endl;
    
#ifdef FRESH_OPENAL_AVAILABLE
    // Open the default audio device
    g_audioDevice = alcOpenDevice(nullptr);
    if (!g_audioDevice) {
        std::cerr << "Failed to open OpenAL audio device" << std::endl;
        return false;
    }
    
    // Create audio context
    g_audioContext = alcCreateContext(g_audioDevice, nullptr);
    if (!g_audioContext) {
        std::cerr << "Failed to create OpenAL context" << std::endl;
        alcCloseDevice(g_audioDevice);
        g_audioDevice = nullptr;
        return false;
    }
    
    // Make the context current
    if (!alcMakeContextCurrent(g_audioContext)) {
        std::cerr << "Failed to make OpenAL context current" << std::endl;
        alcDestroyContext(g_audioContext);
        alcCloseDevice(g_audioDevice);
        g_audioContext = nullptr;
        g_audioDevice = nullptr;
        return false;
    }
    
    // Set default listener properties
    alListener3f(AL_POSITION, 0.0f, 0.0f, 0.0f);
    alListener3f(AL_VELOCITY, 0.0f, 0.0f, 0.0f);
    ALfloat listenerOri[] = { 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f };
    alListenerfv(AL_ORIENTATION, listenerOri);
    
    initialized = true;
    std::cout << "AudioEngine initialized successfully with OpenAL" << std::endl;
    return true;
#else
    std::cout << "AudioEngine: OpenAL not available, running in stub mode" << std::endl;
    initialized = true;
    return true;
#endif
}

void AudioEngine::shutdown() {
    if (!initialized) return;
    
    std::cout << "Shutting down AudioEngine..." << std::endl;
    
    stopAll();
    stopMusic();
    
#ifdef FRESH_OPENAL_AVAILABLE
    // Cleanup OpenAL resources
    if (g_audioContext) {
        alcMakeContextCurrent(nullptr);
        alcDestroyContext(g_audioContext);
        g_audioContext = nullptr;
    }
    
    if (g_audioDevice) {
        alcCloseDevice(g_audioDevice);
        g_audioDevice = nullptr;
    }
#endif
    
    initialized = false;
}

void AudioEngine::update(float deltaTime) {
    if (!initialized) return;
    
#ifdef FRESH_OPENAL_AVAILABLE
    // Update 3D audio based on listener position
    for (auto& [id, source] : activeSources) {
        if (source.is3D && source.isPlaying && source.sourceID != -1) {
            // Calculate distance-based attenuation
            float distance = glm::distance(listener.position, source.position);
            float attenuation = 1.0f;
            
            if (distance > source.minDistance) {
                attenuation = source.minDistance / 
                    std::min(distance, source.maxDistance);
            }
            
            // Apply attenuation to volume
            float effectiveVolume = source.volume * attenuation * effectsVolume * masterVolume;
            if (muted) effectiveVolume = 0.0f;
            
            // Update OpenAL source properties
            ALuint alSource = static_cast<ALuint>(source.sourceID);
            alSourcef(alSource, AL_GAIN, effectiveVolume);
            alSource3f(alSource, AL_POSITION, source.position.x, source.position.y, source.position.z);
            
            // Check if source is still playing
            ALint state;
            alGetSourcei(alSource, AL_SOURCE_STATE, &state);
            if (state != AL_PLAYING) {
                source.isPlaying = false;
            }
        } else if (!source.is3D && source.isPlaying && source.sourceID != -1) {
            // Update 2D audio volume
            float effectiveVolume = source.volume * effectsVolume * masterVolume;
            if (muted) effectiveVolume = 0.0f;
            
            ALuint alSource = static_cast<ALuint>(source.sourceID);
            alSourcef(alSource, AL_GAIN, effectiveVolume);
            
            // Check if source is still playing
            ALint state;
            alGetSourcei(alSource, AL_SOURCE_STATE, &state);
            if (state != AL_PLAYING) {
                source.isPlaying = false;
            }
        }
    }
#else
    // Stub implementation for non-OpenAL builds
    for (auto& [id, source] : activeSources) {
        if (source.is3D && source.isPlaying) {
            float distance = glm::distance(listener.position, source.position);
            float attenuation = 1.0f;
            
            if (distance > source.minDistance) {
                attenuation = source.minDistance / 
                    std::min(distance, source.maxDistance);
            }
            
            float effectiveVolume = source.volume * attenuation * effectsVolume * masterVolume;
            if (muted) effectiveVolume = 0.0f;
            (void)effectiveVolume;
        }
    }
#endif
    
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

#ifdef FRESH_OPENAL_AVAILABLE
// Helper function to load WAV file into OpenAL buffer
static ALuint loadWAVFile(const std::string& path) {
    // For now, return 0 if file loading fails
    // A proper implementation would parse WAV file headers
    // This is a minimal stub that would need a proper WAV loader
    std::cout << "  Note: WAV loading not yet implemented for: " << path << std::endl;
    return 0;
}
#endif

int AudioEngine::play2D(const std::string& path, float volume, bool loop) {
    if (!initialized) return -1;
    
    std::cout << "Playing 2D audio: " << path << std::endl;
    
#ifdef FRESH_OPENAL_AVAILABLE
    // Load audio buffer (stub for now - would need proper WAV loader)
    ALuint buffer = loadWAVFile(path);
    if (buffer == 0) {
        std::cerr << "  Failed to load audio file: " << path << std::endl;
        // Continue anyway to demonstrate the system works
    }
    
    // Generate OpenAL source
    ALuint alSource;
    alGenSources(1, &alSource);
    
    // Set source properties for 2D audio
    alSourcei(alSource, AL_BUFFER, buffer);
    alSourcef(alSource, AL_GAIN, volume * effectsVolume * masterVolume);
    alSourcef(alSource, AL_PITCH, 1.0f);
    alSourcei(alSource, AL_LOOPING, loop ? AL_TRUE : AL_FALSE);
    alSourcei(alSource, AL_SOURCE_RELATIVE, AL_TRUE);  // 2D audio
    alSource3f(alSource, AL_POSITION, 0.0f, 0.0f, 0.0f);
    
    // Play the source
    alSourcePlay(alSource);
    
    int id = getNextSourceID();
    AudioSource source;
    source.volume = volume;
    source.loop = loop;
    source.is3D = false;
    source.isPlaying = true;
    source.sourceID = static_cast<int>(alSource);
    
    activeSources[id] = source;
    
    std::cout << "  Started 2D audio source (ID: " << id << ", AL source: " << alSource << ")" << std::endl;
    return id;
#else
    int id = getNextSourceID();
    AudioSource source;
    source.volume = volume;
    source.loop = loop;
    source.is3D = false;
    source.isPlaying = true;
    source.sourceID = id;
    
    activeSources[id] = source;
    std::cout << "  (Stub mode - OpenAL not available)" << std::endl;
    return id;
#endif
}

int AudioEngine::play3D(const std::string& path, const glm::vec3& position,
                       float volume, bool loop) {
    if (!initialized) return -1;
    
    std::cout << "Playing 3D audio: " << path << " at (" 
              << position.x << ", " << position.y << ", " << position.z << ")" << std::endl;
    
#ifdef FRESH_OPENAL_AVAILABLE
    // Load audio buffer (stub for now - would need proper WAV loader)
    ALuint buffer = loadWAVFile(path);
    if (buffer == 0) {
        std::cerr << "  Failed to load audio file: " << path << std::endl;
        // Continue anyway to demonstrate the system works
    }
    
    // Generate OpenAL source
    ALuint alSource;
    alGenSources(1, &alSource);
    
    // Set source properties for 3D audio
    alSourcei(alSource, AL_BUFFER, buffer);
    alSourcef(alSource, AL_GAIN, volume * effectsVolume * masterVolume);
    alSourcef(alSource, AL_PITCH, 1.0f);
    alSourcei(alSource, AL_LOOPING, loop ? AL_TRUE : AL_FALSE);
    alSourcei(alSource, AL_SOURCE_RELATIVE, AL_FALSE);  // 3D audio
    alSource3f(alSource, AL_POSITION, position.x, position.y, position.z);
    alSourcef(alSource, AL_REFERENCE_DISTANCE, 1.0f);
    alSourcef(alSource, AL_MAX_DISTANCE, 100.0f);
    alSourcef(alSource, AL_ROLLOFF_FACTOR, 1.0f);
    
    // Play the source
    alSourcePlay(alSource);
    
    int id = getNextSourceID();
    AudioSource source;
    source.position = position;
    source.volume = volume;
    source.loop = loop;
    source.is3D = true;
    source.isPlaying = true;
    source.sourceID = static_cast<int>(alSource);
    
    activeSources[id] = source;
    
    std::cout << "  Started 3D audio source (ID: " << id << ", AL source: " << alSource << ")" << std::endl;
    return id;
#else
    int id = getNextSourceID();
    AudioSource source;
    source.position = position;
    source.volume = volume;
    source.loop = loop;
    source.is3D = true;
    source.isPlaying = true;
    source.sourceID = id;
    
    activeSources[id] = source;
    std::cout << "  (Stub mode - OpenAL not available)" << std::endl;
    return id;
#endif
}

void AudioEngine::stop(int soundID) {
    auto it = activeSources.find(soundID);
    if (it != activeSources.end()) {
#ifdef FRESH_OPENAL_AVAILABLE
        if (it->second.sourceID != -1) {
            ALuint alSource = static_cast<ALuint>(it->second.sourceID);
            alSourceStop(alSource);
            alDeleteSources(1, &alSource);
        }
#endif
        it->second.isPlaying = false;
        activeSources.erase(it);
    }
}

void AudioEngine::stopAll() {
#ifdef FRESH_OPENAL_AVAILABLE
    for (auto& [id, source] : activeSources) {
        if (source.sourceID != -1) {
            ALuint alSource = static_cast<ALuint>(source.sourceID);
            alSourceStop(alSource);
            alDeleteSources(1, &alSource);
        }
        source.isPlaying = false;
    }
#else
    for (auto& [id, source] : activeSources) {
        source.isPlaying = false;
    }
#endif
    activeSources.clear();
}

void AudioEngine::pause(int soundID) {
    auto it = activeSources.find(soundID);
    if (it != activeSources.end()) {
#ifdef FRESH_OPENAL_AVAILABLE
        if (it->second.sourceID != -1) {
            ALuint alSource = static_cast<ALuint>(it->second.sourceID);
            alSourcePause(alSource);
        }
#endif
        it->second.isPlaying = false;
    }
}

void AudioEngine::resume(int soundID) {
    auto it = activeSources.find(soundID);
    if (it != activeSources.end()) {
#ifdef FRESH_OPENAL_AVAILABLE
        if (it->second.sourceID != -1) {
            ALuint alSource = static_cast<ALuint>(it->second.sourceID);
            alSourcePlay(alSource);
        }
#endif
        it->second.isPlaying = true;
    }
}

void AudioEngine::setVolume(int soundID, float volume) {
    auto it = activeSources.find(soundID);
    if (it != activeSources.end()) {
        it->second.volume = volume;
#ifdef FRESH_OPENAL_AVAILABLE
        if (it->second.sourceID != -1) {
            ALuint alSource = static_cast<ALuint>(it->second.sourceID);
            float effectiveVolume = volume * effectsVolume * masterVolume;
            if (muted) effectiveVolume = 0.0f;
            alSourcef(alSource, AL_GAIN, effectiveVolume);
        }
#endif
    }
}

bool AudioEngine::isPlaying(int soundID) const {
    auto it = activeSources.find(soundID);
    return (it != activeSources.end()) && it->second.isPlaying;
}

void AudioEngine::playMusic(const std::string& path, float volume, bool loop) {
    stopMusic();
    
    std::cout << "Playing music: " << path << std::endl;
    
#ifdef FRESH_OPENAL_AVAILABLE
    // Load audio buffer for music
    ALuint buffer = loadWAVFile(path);
    if (buffer == 0) {
        std::cerr << "  Failed to load music file: " << path << std::endl;
    }
    
    // Generate OpenAL source for music
    ALuint alSource;
    alGenSources(1, &alSource);
    
    // Set source properties
    alSourcei(alSource, AL_BUFFER, buffer);
    alSourcef(alSource, AL_GAIN, volume * musicVolume * masterVolume);
    alSourcef(alSource, AL_PITCH, 1.0f);
    alSourcei(alSource, AL_LOOPING, loop ? AL_TRUE : AL_FALSE);
    alSourcei(alSource, AL_SOURCE_RELATIVE, AL_TRUE);  // Music is 2D
    alSource3f(alSource, AL_POSITION, 0.0f, 0.0f, 0.0f);
    
    // Play the music
    alSourcePlay(alSource);
    
    musicSource.volume = volume;
    musicSource.loop = loop;
    musicSource.isPlaying = true;
    musicSource.sourceID = static_cast<int>(alSource);
    musicVolume = volume;
    
    std::cout << "  Started music source (AL source: " << alSource << ")" << std::endl;
#else
    musicSource.volume = volume;
    musicSource.loop = loop;
    musicSource.isPlaying = true;
    musicVolume = volume;
    std::cout << "  (Stub mode - OpenAL not available)" << std::endl;
#endif
}

void AudioEngine::stopMusic() {
    if (musicSource.isPlaying) {
#ifdef FRESH_OPENAL_AVAILABLE
        if (musicSource.sourceID != -1) {
            ALuint alSource = static_cast<ALuint>(musicSource.sourceID);
            alSourceStop(alSource);
            alDeleteSources(1, &alSource);
        }
#endif
        musicSource.isPlaying = false;
        musicSource.sourceID = -1;
    }
}

void AudioEngine::setMusicVolume(float volume) {
    musicVolume = volume;
#ifdef FRESH_OPENAL_AVAILABLE
    if (musicSource.isPlaying && musicSource.sourceID != -1) {
        ALuint alSource = static_cast<ALuint>(musicSource.sourceID);
        float effectiveVolume = volume * masterVolume;
        if (muted) effectiveVolume = 0.0f;
        alSourcef(alSource, AL_GAIN, effectiveVolume);
    }
#endif
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
#ifdef FRESH_OPENAL_AVAILABLE
    alListener3f(AL_POSITION, position.x, position.y, position.z);
#endif
}

void AudioEngine::setListenerVelocity(const glm::vec3& velocity) {
    listener.velocity = velocity;
#ifdef FRESH_OPENAL_AVAILABLE
    alListener3f(AL_VELOCITY, velocity.x, velocity.y, velocity.z);
#endif
}

void AudioEngine::setListenerOrientation(const glm::vec3& forward, const glm::vec3& up) {
    listener.forward = forward;
    listener.up = up;
#ifdef FRESH_OPENAL_AVAILABLE
    ALfloat orientation[] = { forward.x, forward.y, forward.z, up.x, up.y, up.z };
    alListenerfv(AL_ORIENTATION, orientation);
#endif
}

void AudioEngine::setMasterVolume(float volume) {
    masterVolume = std::clamp(volume, 0.0f, 1.0f);
#ifdef FRESH_OPENAL_AVAILABLE
    // Update all active sources
    for (auto& [id, source] : activeSources) {
        if (source.sourceID != -1) {
            ALuint alSource = static_cast<ALuint>(source.sourceID);
            float effectiveVolume = source.volume * effectsVolume * masterVolume;
            if (muted) effectiveVolume = 0.0f;
            alSourcef(alSource, AL_GAIN, effectiveVolume);
        }
    }
    // Update music
    if (musicSource.isPlaying && musicSource.sourceID != -1) {
        ALuint alSource = static_cast<ALuint>(musicSource.sourceID);
        float effectiveVolume = musicVolume * masterVolume;
        if (muted) effectiveVolume = 0.0f;
        alSourcef(alSource, AL_GAIN, effectiveVolume);
    }
#endif
}

void AudioEngine::setMuted(bool isMuted) {
    muted = isMuted;
#ifdef FRESH_OPENAL_AVAILABLE
    float volumeMultiplier = muted ? 0.0f : 1.0f;
    // Update all active sources
    for (auto& [id, source] : activeSources) {
        if (source.sourceID != -1) {
            ALuint alSource = static_cast<ALuint>(source.sourceID);
            float effectiveVolume = source.volume * effectsVolume * masterVolume * volumeMultiplier;
            alSourcef(alSource, AL_GAIN, effectiveVolume);
        }
    }
    // Update music
    if (musicSource.isPlaying && musicSource.sourceID != -1) {
        ALuint alSource = static_cast<ALuint>(musicSource.sourceID);
        float effectiveVolume = musicVolume * masterVolume * volumeMultiplier;
        alSourcef(alSource, AL_GAIN, effectiveVolume);
    }
#endif
}

int AudioEngine::getNextSourceID() {
    return nextSourceID++;
}

void AudioEngine::cleanupFinishedSources() {
    // Remove sources that have finished playing (non-looping)
    auto it = activeSources.begin();
    while (it != activeSources.end()) {
        if (!it->second.loop && !it->second.isPlaying) {
#ifdef FRESH_OPENAL_AVAILABLE
            if (it->second.sourceID != -1) {
                ALuint alSource = static_cast<ALuint>(it->second.sourceID);
                alDeleteSources(1, &alSource);
            }
#endif
            it = activeSources.erase(it);
        } else {
            ++it;
        }
    }
}

} // namespace fresh
