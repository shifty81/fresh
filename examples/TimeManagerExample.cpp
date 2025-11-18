/**
 * @file TimeManagerExample.cpp
 * @brief Example demonstrating how to integrate TimeManager into the Fresh Voxel Engine
 * 
 * This example shows:
 * - Basic TimeManager setup
 * - Integration with rendering system
 * - UI controls for time manipulation
 * - Event handling for time-based gameplay
 */

#include "gameplay/TimeManager.h"
#include "core/Engine.h"
#include "renderer/Renderer.h"
#include "ui/ImGuiPanel.h"
#include <iostream>

namespace fresh
{

/**
 * @brief Example game class with TimeManager integration
 */
class TimeManagerExample
{
public:
    TimeManagerExample()
    {
        setupTimeCallbacks();
        m_timeManager.setTimeOfDay(TimeManager::TimeOfDay::Dawn);
        std::cout << "Time Manager Example Started\n";
        std::cout << "Time: " << m_timeManager.getFormattedTime() << "\n";
    }

    /**
     * @brief Main update loop - call every frame
     * @param deltaTime Time elapsed since last frame in seconds
     */
    void update(float deltaTime)
    {
        // Update time progression
        m_timeManager.update(deltaTime);
        
        // Update rendering based on time
        updateLighting();
        
        // Update gameplay based on time of day
        updateGameplay();
    }

    /**
     * @brief Render UI controls for time management
     */
    void renderUI()
    {
        ImGui::Begin("Time Manager Control");
        
        // Time display
        ImGui::Text("Current Time: %s", m_timeManager.getFormattedTime().c_str());
        ImGui::Text("Day: %d", m_timeManager.getCurrentDay());
        ImGui::Text("Status: %s", m_timeManager.isDaytime() ? "Day" : "Night");
        
        ImGui::Separator();
        
        // Time preset buttons
        ImGui::Text("Quick Time Presets:");
        if (ImGui::Button("Dawn (6 AM)")) {
            m_timeManager.setTimeOfDay(TimeManager::TimeOfDay::Dawn);
        }
        ImGui::SameLine();
        if (ImGui::Button("Noon (12 PM)")) {
            m_timeManager.setTimeOfDay(TimeManager::TimeOfDay::Noon);
        }
        if (ImGui::Button("Dusk (6 PM)")) {
            m_timeManager.setTimeOfDay(TimeManager::TimeOfDay::Dusk);
        }
        ImGui::SameLine();
        if (ImGui::Button("Midnight (12 AM)")) {
            m_timeManager.setTimeOfDay(TimeManager::TimeOfDay::Midnight);
        }
        
        ImGui::Separator();
        
        // Time rate control
        ImGui::Text("Time Speed Control:");
        float timeRate = m_timeManager.getTimeRate();
        if (ImGui::SliderFloat("Speed Multiplier", &timeRate, 0.1f, 10.0f, "%.1fx")) {
            m_timeManager.setTimeRate(timeRate);
        }
        
        // Quick speed buttons
        if (ImGui::Button("0.5x")) m_timeManager.setTimeRate(0.5f);
        ImGui::SameLine();
        if (ImGui::Button("1x")) m_timeManager.setTimeRate(1.0f);
        ImGui::SameLine();
        if (ImGui::Button("2x")) m_timeManager.setTimeRate(2.0f);
        ImGui::SameLine();
        if (ImGui::Button("5x")) m_timeManager.setTimeRate(5.0f);
        ImGui::SameLine();
        if (ImGui::Button("10x")) m_timeManager.setTimeRate(10.0f);
        
        ImGui::Separator();
        
        // Pause control
        if (ImGui::Button(m_timeManager.isPaused() ? "Resume Time" : "Pause Time")) {
            m_timeManager.togglePause();
        }
        
        ImGui::Separator();
        
        // Manual time control
        ImGui::Text("Manual Time Control:");
        int currentTime = m_timeManager.getTime();
        if (ImGui::SliderInt("Time (ticks)", &currentTime, 0, 24000)) {
            m_timeManager.setTime(currentTime);
        }
        
        ImGui::Separator();
        
        // Lighting info (read-only)
        ImGui::Text("Lighting Information:");
        ImGui::Text("Ambient Intensity: %.2f", m_timeManager.getAmbientLightIntensity());
        ImGui::Text("Sun Elevation: %.1f°", m_timeManager.getSunElevation());
        
        glm::vec3 skyColor = m_timeManager.getSkyColor();
        ImGui::ColorEdit3("Sky Color", &skyColor.x, ImGuiColorEditFlags_NoInputs);
        
        glm::vec3 sunColor = m_timeManager.getSunLightColor();
        ImGui::ColorEdit3("Sun Color", &sunColor.x, ImGuiColorEditFlags_NoInputs);
        
        ImGui::End();
        
        // Optional: Visual representation of sun/moon position
        renderSunMoonIndicator();
    }

private:
    /**
     * @brief Set up time-based event callbacks
     */
    void setupTimeCallbacks()
    {
        // Day change callback
        m_timeManager.setOnDayChange([this](int day) {
            std::cout << "=== Day " << day << " has begun! ===\n";
            onNewDay(day);
        });
        
        // Sunrise callback
        m_timeManager.setOnSunrise([this]() {
            std::cout << "The sun is rising... Time: " 
                     << m_timeManager.getFormattedTime() << "\n";
            onSunrise();
        });
        
        // Sunset callback
        m_timeManager.setOnSunset([this]() {
            std::cout << "The sun is setting... Time: " 
                     << m_timeManager.getFormattedTime() << "\n";
            onSunset();
        });
    }

    /**
     * @brief Update rendering system with current time-based lighting
     */
    void updateLighting()
    {
        // Get lighting parameters from TimeManager
        float ambientIntensity = m_timeManager.getAmbientLightIntensity();
        glm::vec3 skyColor = m_timeManager.getSkyColor();
        glm::vec3 sunDirection = m_timeManager.getSunDirection();
        glm::vec3 sunColor = m_timeManager.getSunLightColor();
        glm::vec3 moonDirection = m_timeManager.getMoonDirection();
        glm::vec3 moonColor = m_timeManager.getMoonLightColor();
        
        // Apply to renderer (pseudo-code - adapt to your renderer)
        // m_renderer.setAmbientLight(ambientIntensity, skyColor);
        // m_renderer.setDirectionalLight(sunDirection, sunColor);
        // m_renderer.setMoonLight(moonDirection, moonColor);
        // m_renderer.setSkyColor(skyColor);
        
        // Update sky rendering
        // if (m_skyRenderer) {
        //     m_skyRenderer->setSkyColor(skyColor);
        //     m_skyRenderer->setHorizonColor(m_timeManager.getHorizonColor());
        // }
    }

    /**
     * @brief Update gameplay mechanics based on time of day
     */
    void updateGameplay()
    {
        if (m_timeManager.isDaytime()) {
            // Daytime gameplay
            // - Peaceful mobs spawn
            // - Increase visibility
            // - Enable certain activities
        } else {
            // Nighttime gameplay
            // - Hostile mobs spawn
            // - Decrease visibility
            // - Different ambient sounds
        }
        
        if (m_timeManager.isSunrise()) {
            // Sunrise effects
            // - Morning ambient sounds
            // - Special events
        }
        
        if (m_timeManager.isSunset()) {
            // Sunset effects
            // - Evening ambient sounds
            // - Warning for night approaching
        }
    }

    /**
     * @brief Handle new day event
     */
    void onNewDay(int day)
    {
        // Example: Save game progress
        std::cout << "Auto-saving progress for Day " << day << "...\n";
        
        // Example: Reset daily mechanics
        // - Respawn resources
        // - Reset NPC inventories
        // - Update quest states
    }

    /**
     * @brief Handle sunrise event
     */
    void onSunrise()
    {
        // Example: Play morning ambience
        // audioEngine.playSound("morning_birds.ogg");
        
        // Example: Update mob spawning
        // mobSpawner.setDaytimeMode(true);
        
        // Example: Show notification
        std::cout << "Good morning! A new day begins.\n";
    }

    /**
     * @brief Handle sunset event
     */
    void onSunset()
    {
        // Example: Play evening ambience
        // audioEngine.playSound("evening_crickets.ogg");
        
        // Example: Update mob spawning
        // mobSpawner.setNighttimeMode(true);
        
        // Example: Show warning
        std::cout << "Night is falling. Be careful!\n";
    }

    /**
     * @brief Render visual indicator of sun/moon position
     */
    void renderSunMoonIndicator()
    {
        ImGui::Begin("Day/Night Cycle", nullptr, ImGuiWindowFlags_NoResize);
        ImGui::SetWindowSize(ImVec2(300, 200));
        
        ImDrawList* drawList = ImGui::GetWindowDrawList();
        ImVec2 windowPos = ImGui::GetWindowPos();
        ImVec2 windowSize = ImGui::GetWindowSize();
        
        // Draw horizon line
        ImVec2 horizonStart(windowPos.x + 10, windowPos.y + windowSize.y / 2);
        ImVec2 horizonEnd(windowPos.x + windowSize.x - 10, windowPos.y + windowSize.y / 2);
        drawList->AddLine(horizonStart, horizonEnd, IM_COL32(100, 100, 100, 255), 2.0f);
        
        // Calculate sun position for visualization
        float sunElevation = m_timeManager.getSunElevation();
        float angle = glm::radians(sunElevation);
        float radius = 80.0f;
        
        ImVec2 center(windowPos.x + windowSize.x / 2, windowPos.y + windowSize.y / 2);
        ImVec2 sunPos(
            center.x + radius * std::cos(angle + glm::radians(90.0f)),
            center.y - radius * std::sin(angle + glm::radians(90.0f))
        );
        
        // Draw sun/moon
        if (m_timeManager.isDaytime()) {
            // Draw sun
            drawList->AddCircleFilled(sunPos, 15.0f, IM_COL32(255, 255, 0, 255));
            drawList->AddCircle(sunPos, 15.0f, IM_COL32(255, 200, 0, 255), 0, 2.0f);
        } else {
            // Draw moon
            ImVec2 moonPos(center.x - (sunPos.x - center.x), 
                          center.y - (sunPos.y - center.y));
            drawList->AddCircleFilled(moonPos, 12.0f, IM_COL32(200, 200, 255, 255));
            drawList->AddCircle(moonPos, 12.0f, IM_COL32(150, 150, 200, 255), 0, 2.0f);
        }
        
        // Draw time info
        ImGui::SetCursorPosY(windowSize.y - 30);
        ImGui::Text("%s", m_timeManager.getFormattedTime().c_str());
        
        ImGui::End();
    }

private:
    TimeManager m_timeManager;
    // Other game systems would go here
};

} // namespace fresh

// Example main function
int main()
{
    using namespace fresh;
    
    std::cout << "=== TimeManager Integration Example ===\n\n";
    
    TimeManagerExample example;
    
    // Simulate game loop
    float deltaTime = 1.0f / 60.0f;  // 60 FPS
    
    for (int frame = 0; frame < 60 * 30; ++frame) {  // Simulate 30 seconds
        example.update(deltaTime);
        
        // In a real game, you would also:
        // - Process input
        // - Update physics
        // - Render scene
        // - example.renderUI() (within ImGui frame)
        
        // Print status every 60 frames (1 second)
        if (frame % 60 == 0) {
            std::cout << "Frame " << frame 
                     << " - Time: " << example.getFormattedTime() 
                     << "\n";
        }
    }
    
    std::cout << "\n=== Example Complete ===\n";
    return 0;
}
