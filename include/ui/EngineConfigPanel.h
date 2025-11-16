#pragma once

#include <memory>
#include <string>

#include <glm/glm.hpp>

namespace fresh
{

class LightingSystem;
class PostProcessing;
class IRenderContext;
enum class GraphicsAPI;

#ifdef _WIN32
class Win32InputManager;
using InputManagerType = Win32InputManager;
#else
class InputManager;
using InputManagerType = InputManager;
#endif

class Material;

/**
 * @brief In-game engine configuration panel
 *
 * Provides runtime configuration UI for graphics, input, and engine settings.
 * Uses ImGui for rendering UI elements.
 */
class EngineConfigPanel
{
public:
    EngineConfigPanel();
    ~EngineConfigPanel();

    /**
     * @brief Initialize configuration panel
     * @param renderContext Render context for ImGui
     * @return true if successful
     */
    bool initialize(IRenderContext* renderContext);

    /**
     * @brief Shutdown and cleanup
     */
    void shutdown();

    /**
     * @brief Render configuration UI
     */
    void render();

    /**
     * @brief Show/hide configuration panel
     */
    void setVisible(bool visible)
    {
        isVisible = visible;
    }
    bool getVisible() const
    {
        return isVisible;
    }
    void toggleVisible()
    {
        isVisible = !isVisible;
    }

    /**
     * @brief Set systems to configure
     */
    void setRenderContext(IRenderContext* ctx)
    {
        renderContext = ctx;
    }
    void setLightingSystem(LightingSystem* sys)
    {
        lightingSystem = sys;
    }
    void setPostProcessing(PostProcessing* pp)
    {
        postProcessing = pp;
    }
    void setInputManager(InputManagerType* im)
    {
        inputManager = im;
    }

    /**
     * @brief Save/Load configuration
     */
    bool saveConfig(const std::string& path);
    bool loadConfig(const std::string& path);

private:
    // UI Panels
    void renderGraphicsPanel();
    void renderLightingPanel();
    void renderPostProcessingPanel();
    void renderInputPanel();
    void renderPerformancePanel();
    void renderMaterialEditorPanel();

    // Graphics sub-panels
    void renderAPISelector();
    void renderRenderQualitySettings();
    void renderDisplaySettings();

    // Lighting sub-panels
    void renderAmbientLightSettings();
    void renderDirectionalLightSettings();
    void renderPointLightSettings();
    void renderSpotLightSettings();
    void renderShadowSettings();
    void renderPBRSettings();

    // Post-processing sub-panels
    void renderFXAASettings();
    void renderBloomSettings();
    void renderToneMappingSettings();
    void renderColorGradingSettings();
    void renderVignetteSettings();
    void renderDepthOfFieldSettings();
    void renderSSAOSettings();

    // Input sub-panels
    void renderKeyBindingsSettings();
    void renderGamepadSettings();
    void renderMouseSettings();

    // Performance monitoring
    void renderFPSCounter();
    void renderMemoryUsage();
    void renderGPUStats();

    // Material editor
    void renderMaterialList();
    void renderMaterialProperties(Material* material);
    void renderTextureSelector(std::shared_ptr<class Texture>& texture, const char* label);

    IRenderContext* renderContext = nullptr;
    LightingSystem* lightingSystem = nullptr;
    PostProcessing* postProcessing = nullptr;
    InputManagerType* inputManager = nullptr;

    bool isVisible = false;
    int selectedTab = 0; // 0=Graphics, 1=Lighting, 2=Post-Processing, 3=Input, 4=Performance

    // Temporary UI state
    int selectedLightIndex = -1;
    std::shared_ptr<Material> selectedMaterial;

    // Performance metrics
    float fpsHistory[100] = {0};
    int fpsHistoryIndex = 0;
    float frameTime = 0.0f;
    float avgFPS = 0.0f;
};

} // namespace fresh
