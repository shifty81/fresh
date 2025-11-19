# Day/Night Cycle System - TimeManager

## Overview

The `TimeManager` class provides a complete day/night cycle system for the Fresh Voxel Engine. It handles time progression, sun/moon positioning, dynamic lighting calculations, and time-based events.

## Features

- **Time Progression**: 24000 ticks = 1 full day (20 minutes real-time at 1x speed)
- **Configurable Speed**: Adjust time rate from 0.1x to 10x or more
- **Pause/Resume**: Full control over time flow
- **Sun/Moon Cycles**: Realistic celestial body positioning
- **Dynamic Lighting**: Automatic ambient light and color calculations
- **Time Presets**: Quick jump to Dawn, Noon, Dusk, or Midnight
- **Event Callbacks**: Get notified of sunrise, sunset, and day changes
- **Smooth Transitions**: Gradual color changes during sunrise/sunset periods

## Basic Usage

### Creating and Updating TimeManager

```cpp
#include "gameplay/TimeManager.h"

// Create TimeManager instance
fresh::TimeManager timeManager;

// In your game loop
void gameLoop(float deltaTime) {
    // Update time progression
    timeManager.update(deltaTime);
    
    // Use lighting data for rendering
    float ambientIntensity = timeManager.getAmbientLightIntensity();
    glm::vec3 skyColor = timeManager.getSkyColor();
    glm::vec3 sunDirection = timeManager.getSunDirection();
}
```

### Time Control

```cpp
// Set specific time (0-24000 ticks)
timeManager.setTime(12000);  // Noon

// Set time using presets
timeManager.setTimeOfDay(TimeManager::TimeOfDay::Dawn);     // 6:00 AM
timeManager.setTimeOfDay(TimeManager::TimeOfDay::Noon);     // 12:00 PM
timeManager.setTimeOfDay(TimeManager::TimeOfDay::Dusk);     // 6:00 PM
timeManager.setTimeOfDay(TimeManager::TimeOfDay::Midnight); // 12:00 AM

// Pause and resume time
timeManager.pause();
timeManager.resume();
timeManager.togglePause();

// Adjust time speed
timeManager.setTimeRate(2.0f);  // 2x speed
timeManager.setTimeRate(0.5f);  // Half speed
```

### Getting Time Information

```cpp
// Get current time in ticks
int currentTicks = timeManager.getTime();  // 0-24000

// Get time in hours
float hours = timeManager.getTimeInHours();  // 0.0-24.0

// Get formatted time string
std::string timeStr = timeManager.getFormattedTime();  // "03:45 PM"

// Check time of day
bool isDay = timeManager.isDaytime();
bool isNight = timeManager.isNighttime();
bool isSunrise = timeManager.isSunrise();
bool isSunset = timeManager.isSunset();

// Get current day number
int dayNumber = timeManager.getCurrentDay();
```

### Sun and Moon Positioning

```cpp
// Get sun direction for directional light
glm::vec3 sunDir = timeManager.getSunDirection();
float sunElevation = timeManager.getSunElevation();  // Degrees above horizon

// Get moon direction
glm::vec3 moonDir = timeManager.getMoonDirection();

// Use in shader
shader.setVec3("u_sunDirection", sunDir);
shader.setFloat("u_sunElevation", sunElevation);
```

### Dynamic Lighting

```cpp
// Get lighting parameters for rendering
float ambientIntensity = timeManager.getAmbientLightIntensity();  // 0.1-1.0
glm::vec3 skyColor = timeManager.getSkyColor();          // RGB color
glm::vec3 horizonColor = timeManager.getHorizonColor();  // RGB color
glm::vec3 sunColor = timeManager.getSunLightColor();     // RGB color
glm::vec3 moonColor = timeManager.getMoonLightColor();   // RGB color

// Apply to rendering
shader.setFloat("u_ambientIntensity", ambientIntensity);
shader.setVec3("u_skyColor", skyColor);
shader.setVec3("u_sunColor", sunColor);
```

### Event Callbacks

```cpp
// Get notified when day changes
timeManager.setOnDayChange([](int newDay) {
    std::cout << "Day " << newDay << " has begun!" << std::endl;
});

// Get notified at sunrise
timeManager.setOnSunrise([]() {
    std::cout << "The sun is rising!" << std::endl;
    // Play morning ambience sound
});

// Get notified at sunset
timeManager.setOnSunset([]() {
    std::cout << "The sun is setting!" << std::endl;
    // Play evening ambience sound
    // Spawn nighttime creatures
});
```

## Time System Details

### Time Scale

- **24000 ticks** = 1 full in-game day
- **1000 ticks** = 1 in-game hour
- **20 ticks/second** = default progression rate at 1x speed
- **20 minutes real-time** = 1 full day at 1x speed

### Time of Day Periods

| Period | Ticks | Real Time | Hours |
|--------|-------|-----------|-------|
| Midnight | 0 | 00:00 | 12:00 AM |
| Sunrise Start | 5000 | 05:00 | 5:00 AM |
| Dawn | 6000 | 06:00 | 6:00 AM |
| Sunrise End | 7000 | 07:00 | 7:00 AM |
| Noon | 12000 | 12:00 | 12:00 PM |
| Sunset Start | 17000 | 17:00 | 5:00 PM |
| Dusk | 18000 | 18:00 | 6:00 PM |
| Sunset End | 19000 | 19:00 | 7:00 PM |

### Lighting Behavior

**Daytime (Sun Elevation > 0°)**
- Ambient Light: 0.8 - 1.0 (bright)
- Sky Color: Blue (0.53, 0.81, 0.92)
- Sun Color: White-Yellow (1.0, 1.0, 0.95)

**Sunrise/Sunset (5000-7000 or 17000-19000 ticks)**
- Ambient Light: Transitioning (0.3 - 0.8)
- Sky Color: Gradient from Orange to Blue
- Horizon Color: Warm orange/yellow (1.0, 0.7, 0.4)
- Sun Color: Orange (1.0, 0.6, 0.3)

**Nighttime (Sun Elevation < 0°)**
- Ambient Light: 0.1 - 0.2 (dark)
- Sky Color: Dark Blue/Black (0.05, 0.05, 0.15)
- Moon Color: Blue-White (0.7, 0.7, 0.9)

## Integration with Renderer

### Shader Integration

Update your shaders to use time-based lighting:

```glsl
// Vertex/Fragment Shader
uniform vec3 u_sunDirection;
uniform float u_ambientIntensity;
uniform vec3 u_skyColor;
uniform vec3 u_sunColor;

void main() {
    // Calculate sun lighting
    float sunDot = max(dot(normal, u_sunDirection), 0.0);
    vec3 sunLight = u_sunColor * sunDot;
    
    // Add ambient lighting
    vec3 ambient = u_skyColor * u_ambientIntensity;
    
    // Final color
    vec3 finalColor = ambient + sunLight;
    fragColor = vec4(finalColor * albedo, 1.0);
}
```

### Rendering Sky

```cpp
void renderSky(const TimeManager& timeManager) {
    // Get sky colors
    glm::vec3 skyColor = timeManager.getSkyColor();
    glm::vec3 horizonColor = timeManager.getHorizonColor();
    
    // Render sky dome/box with gradient
    skyShader.use();
    skyShader.setVec3("u_skyTopColor", skyColor);
    skyShader.setVec3("u_skyHorizonColor", horizonColor);
    
    // Draw sky geometry
    skyMesh.draw();
}
```

### Complete Example

```cpp
class Game {
    TimeManager m_timeManager;
    Renderer m_renderer;
    
public:
    Game() {
        // Set up time callbacks
        m_timeManager.setOnSunrise([this]() {
            m_renderer.playAmbientSound("morning_birds.ogg");
        });
        
        m_timeManager.setOnSunset([this]() {
            m_renderer.playAmbientSound("evening_crickets.ogg");
        });
        
        // Start at dawn
        m_timeManager.setTimeOfDay(TimeManager::TimeOfDay::Dawn);
    }
    
    void update(float deltaTime) {
        // Update time
        m_timeManager.update(deltaTime);
        
        // Update renderer with time data
        m_renderer.setAmbientLight(m_timeManager.getAmbientLightIntensity());
        m_renderer.setSkyColor(m_timeManager.getSkyColor());
        m_renderer.setSunDirection(m_timeManager.getSunDirection());
        m_renderer.setSunColor(m_timeManager.getSunLightColor());
    }
    
    void renderUI() {
        // Display time on HUD
        ImGui::Begin("Time Control");
        ImGui::Text("Time: %s", m_timeManager.getFormattedTime().c_str());
        ImGui::Text("Day: %d", m_timeManager.getCurrentDay());
        
        // Time control buttons
        if (ImGui::Button("Dawn")) {
            m_timeManager.setTimeOfDay(TimeManager::TimeOfDay::Dawn);
        }
        ImGui::SameLine();
        if (ImGui::Button("Noon")) {
            m_timeManager.setTimeOfDay(TimeManager::TimeOfDay::Noon);
        }
        ImGui::SameLine();
        if (ImGui::Button("Dusk")) {
            m_timeManager.setTimeOfDay(TimeManager::TimeOfDay::Dusk);
        }
        ImGui::SameLine();
        if (ImGui::Button("Midnight")) {
            m_timeManager.setTimeOfDay(TimeManager::TimeOfDay::Midnight);
        }
        
        // Time rate control
        float timeRate = m_timeManager.getTimeRate();
        if (ImGui::SliderFloat("Time Speed", &timeRate, 0.1f, 10.0f)) {
            m_timeManager.setTimeRate(timeRate);
        }
        
        // Pause button
        if (ImGui::Button(m_timeManager.isPaused() ? "Resume" : "Pause")) {
            m_timeManager.togglePause();
        }
        
        ImGui::End();
    }
};
```

## Performance Considerations

- `update()` is very lightweight - only simple math operations
- Lighting calculations are cached and only updated each frame
- No heap allocations during normal operation
- Sun/moon calculations use fast trigonometric functions
- Event callbacks are only triggered when state changes

## Testing

The TimeManager includes comprehensive unit tests covering:

- Time progression and normalization
- Pause/resume functionality
- Time rate adjustments
- Time conversions and formatting
- Day/night detection
- Sun/moon positioning
- Lighting calculations
- Event callbacks

Run tests:
```bash
cd build
ctest -R TimeManagerTests -V
```

## Advanced Usage

### Custom Transition Periods

You can modify the transition periods by adjusting the constants in `TimeManager.cpp`:

```cpp
static constexpr int SUNRISE_START = 5000;  // ~5 AM
static constexpr int SUNRISE_END = 7000;    // ~7 AM
static constexpr int SUNSET_START = 17000;  // ~5 PM
static constexpr int SUNSET_END = 19000;    // ~7 PM
```

### Custom Lighting Colors

Override the default colors by modifying the `updateLighting()` method to use your preferred color palette.

### Syncing with Weather System

```cpp
void updateWeather(const TimeManager& timeManager) {
    if (timeManager.isDaytime()) {
        // Day weather (sun, clouds)
    } else {
        // Night weather (moon, stars, fog)
    }
    
    if (timeManager.isSunset()) {
        // Transition effects
    }
}
```

## Future Enhancements

Potential additions to the system:

- [ ] Seasons (winter, spring, summer, fall)
- [ ] Moon phases
- [ ] Eclipse events
- [ ] Custom celestial bodies
- [ ] Time zones for multiplayer
- [ ] Save/load time state
- [ ] Time-based spawn rates
- [ ] Weather integration
- [ ] Star field rendering

## See Also

- [IMMEDIATE_ACTION_PLAN.md](IMMEDIATE_ACTION_PLAN.md) - Week 13 implementation details
- [GAMEPLAY_SYSTEMS.md](GAMEPLAY_SYSTEMS.md) - Overview of gameplay systems
- API Documentation - Full Doxygen documentation in `TimeManager.h`

---

**Status**: ✅ Complete  
**Version**: 1.0  
**Last Updated**: 2025-11-18  
**Author**: Fresh Voxel Engine Team
