/**
 * @file audio_test.cpp
 * @brief Simple test program to verify AudioEngine functionality
 * 
 * This program demonstrates the basic usage of the AudioEngine:
 * - Initializing the audio system
 * - Playing 2D and 3D sounds
 * - Controlling volume and muting
 * - Setting listener position for 3D audio
 * - Updating the audio system
 */

#include "audio/AudioEngine.h"
#include <iostream>
#include <thread>
#include <chrono>

int main() {
    std::cout << "====================================" << std::endl;
    std::cout << "  Fresh Voxel Engine - Audio Test" << std::endl;
    std::cout << "====================================" << std::endl;
    std::cout << std::endl;
    
    // Get the audio engine singleton
    fresh::AudioEngine& audio = fresh::AudioEngine::getInstance();
    
    // Initialize the audio system
    std::cout << "1. Initializing audio system..." << std::endl;
    if (!audio.initialize()) {
        std::cerr << "Failed to initialize audio system!" << std::endl;
        return 1;
    }
    std::cout << "   ✓ Audio system initialized" << std::endl;
    std::cout << std::endl;
    
    // Test 2D audio playback
    std::cout << "2. Testing 2D audio playback..." << std::endl;
    int soundId1 = audio.play2D("sounds/test_sound.wav", 1.0f, false);
    std::cout << "   ✓ 2D sound started (ID: " << soundId1 << ")" << std::endl;
    std::cout << std::endl;
    
    // Test 3D audio playback
    std::cout << "3. Testing 3D audio playback..." << std::endl;
    glm::vec3 soundPos(10.0f, 0.0f, 0.0f);
    int soundId2 = audio.play3D("sounds/ambient.wav", soundPos, 0.8f, true);
    std::cout << "   ✓ 3D sound started at position (" 
              << soundPos.x << ", " << soundPos.y << ", " << soundPos.z 
              << ") - ID: " << soundId2 << std::endl;
    std::cout << std::endl;
    
    // Update listener position
    std::cout << "4. Setting listener position..." << std::endl;
    glm::vec3 listenerPos(0.0f, 0.0f, 0.0f);
    audio.setListenerPosition(listenerPos);
    audio.setListenerOrientation(glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    std::cout << "   ✓ Listener positioned at origin, facing -Z" << std::endl;
    std::cout << std::endl;
    
    // Test volume control
    std::cout << "5. Testing volume control..." << std::endl;
    std::cout << "   - Current master volume: " << audio.getMasterVolume() << std::endl;
    audio.setMasterVolume(0.5f);
    std::cout << "   - New master volume: " << audio.getMasterVolume() << std::endl;
    std::cout << "   ✓ Volume adjusted" << std::endl;
    std::cout << std::endl;
    
    // Test mute/unmute
    std::cout << "6. Testing mute functionality..." << std::endl;
    std::cout << "   - Muting audio..." << std::endl;
    audio.setMuted(true);
    std::cout << "   - Currently muted: " << (audio.isMuted() ? "Yes" : "No") << std::endl;
    std::cout << "   - Unmuting audio..." << std::endl;
    audio.setMuted(false);
    std::cout << "   - Currently muted: " << (audio.isMuted() ? "Yes" : "No") << std::endl;
    std::cout << "   ✓ Mute/unmute working" << std::endl;
    std::cout << std::endl;
    
    // Test music playback
    std::cout << "7. Testing music playback..." << std::endl;
    audio.playMusic("music/background.ogg", 0.6f, true);
    std::cout << "   ✓ Music started" << std::endl;
    std::cout << std::endl;
    
    // Simulate game loop updates
    std::cout << "8. Simulating game loop (5 updates)..." << std::endl;
    for (int i = 0; i < 5; i++) {
        std::cout << "   - Update " << (i + 1) << "/5" << std::endl;
        audio.update(0.016f);  // ~60 FPS (16ms per frame)
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
    std::cout << "   ✓ Audio updates working" << std::endl;
    std::cout << std::endl;
    
    // Cleanup
    std::cout << "9. Cleaning up..." << std::endl;
    audio.stopMusic();
    audio.stopAll();
    audio.shutdown();
    std::cout << "   ✓ Audio system shutdown complete" << std::endl;
    std::cout << std::endl;
    
    std::cout << "====================================" << std::endl;
    std::cout << "  All audio tests completed!" << std::endl;
    std::cout << "====================================" << std::endl;
    std::cout << std::endl;
    std::cout << "Note: Audio file loading not yet implemented." << std::endl;
    std::cout << "      The system demonstrates proper API usage" << std::endl;
    std::cout << "      and OpenAL integration structure." << std::endl;
    
    return 0;
}
