#include "ui/EngineConfigPanel.h"

#ifdef FRESH_IMGUI_AVAILABLE
#include <imgui.h>
#endif

#include "core/Logger.h"
#include "renderer/GraphicsAPI.h"
#include "renderer/LightingSystem.h"
#include "renderer/Material.h"
#include "renderer/PostProcessing.h"
#include "renderer/RenderContext.h"

#ifdef _WIN32
    #include "input/Win32InputManager.h"
#else
    #include "input/InputManager.h"
#endif

#include <fstream>
#include <sstream>

namespace fresh
{

EngineConfigPanel::EngineConfigPanel()
    : renderContext(nullptr),
      lightingSystem(nullptr),
      postProcessing(nullptr),
      inputManager(nullptr),
      isVisible(false),
      selectedTab(0),
      selectedLightIndex(-1),
      fpsHistoryIndex(0),
      frameTime(0.0f),
      avgFPS(0.0f)
{
    // Initialize FPS history
    for (int i = 0; i < 100; ++i) {
        fpsHistory[i] = 0.0f;
    }
}

EngineConfigPanel::~EngineConfigPanel()
{
    shutdown();
}

bool EngineConfigPanel::initialize(IRenderContext* ctx)
{
    if (!ctx) {
        LOG_ERROR_C("Invalid render context for EngineConfigPanel", "EngineConfigPanel");
        return false;
    }

    this->renderContext = ctx;

    // Load configuration from file
    loadConfig("engine_config.cfg");

    LOG_INFO_C("Engine configuration panel initialized", "EngineConfigPanel");
    return true;
}

void EngineConfigPanel::shutdown()
{
    // Save configuration on shutdown
    if (isVisible) {
        saveConfig("engine_config.cfg");
    }
    LOG_INFO_C("Engine configuration panel shutdown", "EngineConfigPanel");
}

void EngineConfigPanel::render()
{
#ifdef FRESH_IMGUI_AVAILABLE
    if (!isVisible) {
        return;
    }

    ImGui::SetNextWindowSize(ImVec2(800, 600), ImGuiCond_FirstUseEver);

    if (ImGui::Begin("Engine Configuration", &isVisible, ImGuiWindowFlags_NoCollapse)) {
        // Tab bar for different configuration categories
        if (ImGui::BeginTabBar("ConfigTabs")) {
            if (ImGui::BeginTabItem("Graphics")) {
                selectedTab = 0;
                renderGraphicsPanel();
                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Lighting")) {
                selectedTab = 1;
                renderLightingPanel();
                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Post-Processing")) {
                selectedTab = 2;
                renderPostProcessingPanel();
                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Input")) {
                selectedTab = 3;
                renderInputPanel();
                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Performance")) {
                selectedTab = 4;
                renderPerformancePanel();
                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Materials")) {
                renderMaterialEditorPanel();
                ImGui::EndTabItem();
            }

            ImGui::EndTabBar();
        }

        ImGui::Separator();
        ImGui::Spacing();

        // Bottom buttons
        if (ImGui::Button("Save Configuration", ImVec2(150, 30))) {
            saveConfig("engine_config.cfg");
            LOG_INFO_C("Engine configuration saved", "EngineConfigPanel");
        }

        ImGui::SameLine();
        if (ImGui::Button("Load Configuration", ImVec2(150, 30))) {
            loadConfig("engine_config.cfg");
            LOG_INFO_C("Engine configuration loaded", "EngineConfigPanel");
        }

        ImGui::SameLine();
        if (ImGui::Button("Close", ImVec2(100, 30))) {
            isVisible = false;
        }
    }
    ImGui::End();
#endif
}

void EngineConfigPanel::renderGraphicsPanel()
{
#ifdef FRESH_IMGUI_AVAILABLE
    ImGui::Spacing();
    ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Graphics Configuration");
    ImGui::Separator();
    ImGui::Spacing();

    // API Selector
    if (ImGui::CollapsingHeader("Graphics API", ImGuiTreeNodeFlags_DefaultOpen)) {
        renderAPISelector();
    }

    // Render Quality
    if (ImGui::CollapsingHeader("Render Quality", ImGuiTreeNodeFlags_DefaultOpen)) {
        renderRenderQualitySettings();
    }

    // Display Settings
    if (ImGui::CollapsingHeader("Display Settings")) {
        renderDisplaySettings();
    }
#endif
}

void EngineConfigPanel::renderLightingPanel()
{
#ifdef FRESH_IMGUI_AVAILABLE
    ImGui::Spacing();
    ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Lighting Configuration");
    ImGui::Separator();
    ImGui::Spacing();

    if (!lightingSystem) {
        ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.7f, 1.0f), "Lighting system not available");
        return;
    }

    // Ambient Light
    if (ImGui::CollapsingHeader("Ambient Light", ImGuiTreeNodeFlags_DefaultOpen)) {
        renderAmbientLightSettings();
    }

    // Directional Lights
    if (ImGui::CollapsingHeader("Directional Lights")) {
        renderDirectionalLightSettings();
    }

    // Point Lights
    if (ImGui::CollapsingHeader("Point Lights")) {
        renderPointLightSettings();
    }

    // Spot Lights
    if (ImGui::CollapsingHeader("Spot Lights")) {
        renderSpotLightSettings();
    }

    // Shadow Settings
    if (ImGui::CollapsingHeader("Shadow Settings")) {
        renderShadowSettings();
    }

    // PBR Settings
    if (ImGui::CollapsingHeader("PBR Settings")) {
        renderPBRSettings();
    }
#endif
}

void EngineConfigPanel::renderPostProcessingPanel()
{
#ifdef FRESH_IMGUI_AVAILABLE
    ImGui::Spacing();
    ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Post-Processing Effects");
    ImGui::Separator();
    ImGui::Spacing();

    if (!postProcessing) {
        ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.7f, 1.0f), "Post-processing system not available");
        return;
    }

    // FXAA
    if (ImGui::CollapsingHeader("FXAA (Anti-Aliasing)", ImGuiTreeNodeFlags_DefaultOpen)) {
        renderFXAASettings();
    }

    // Bloom
    if (ImGui::CollapsingHeader("Bloom")) {
        renderBloomSettings();
    }

    // Tone Mapping
    if (ImGui::CollapsingHeader("Tone Mapping")) {
        renderToneMappingSettings();
    }

    // Color Grading
    if (ImGui::CollapsingHeader("Color Grading")) {
        renderColorGradingSettings();
    }

    // Vignette
    if (ImGui::CollapsingHeader("Vignette")) {
        renderVignetteSettings();
    }

    // Depth of Field
    if (ImGui::CollapsingHeader("Depth of Field")) {
        renderDepthOfFieldSettings();
    }

    // SSAO
    if (ImGui::CollapsingHeader("SSAO (Ambient Occlusion)")) {
        renderSSAOSettings();
    }
#endif
}

void EngineConfigPanel::renderInputPanel()
{
#ifdef FRESH_IMGUI_AVAILABLE
    ImGui::Spacing();
    ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Input Configuration");
    ImGui::Separator();
    ImGui::Spacing();

    if (!inputManager) {
        ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.7f, 1.0f), "Input manager not available");
        return;
    }

    // Key Bindings
    if (ImGui::CollapsingHeader("Key Bindings", ImGuiTreeNodeFlags_DefaultOpen)) {
        renderKeyBindingsSettings();
    }

    // Gamepad Settings
    if (ImGui::CollapsingHeader("Gamepad")) {
        renderGamepadSettings();
    }

    // Mouse Settings
    if (ImGui::CollapsingHeader("Mouse", ImGuiTreeNodeFlags_DefaultOpen)) {
        renderMouseSettings();
    }
#endif
}

void EngineConfigPanel::renderPerformancePanel()
{
#ifdef FRESH_IMGUI_AVAILABLE
    ImGui::Spacing();
    ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Performance Monitoring");
    ImGui::Separator();
    ImGui::Spacing();

    // FPS Counter
    if (ImGui::CollapsingHeader("Frame Rate", ImGuiTreeNodeFlags_DefaultOpen)) {
        renderFPSCounter();
    }

    // Memory Usage
    if (ImGui::CollapsingHeader("Memory Usage")) {
        renderMemoryUsage();
    }

    // GPU Stats
    if (ImGui::CollapsingHeader("GPU Statistics")) {
        renderGPUStats();
    }
#endif
}

void EngineConfigPanel::renderMaterialEditorPanel()
{
#ifdef FRESH_IMGUI_AVAILABLE
    ImGui::Spacing();
    ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Material Editor");
    ImGui::Separator();
    ImGui::Spacing();

    ImGui::BeginChild("MaterialList", ImVec2(200, 0), true);
    renderMaterialList();
    ImGui::EndChild();

    ImGui::SameLine();

    ImGui::BeginChild("MaterialProperties", ImVec2(0, 0), true);
    if (selectedMaterial) {
        renderMaterialProperties(selectedMaterial.get());
    } else {
        ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.7f, 1.0f), "Select a material to edit");
    }
    ImGui::EndChild();
#endif
}

// Graphics sub-panels
void EngineConfigPanel::renderAPISelector()
{
#ifdef FRESH_IMGUI_AVAILABLE
    ImGui::Text("Select Graphics API:");
    ImGui::Spacing();

    const char* apiNames[] = {"Auto", "OpenGL", "DirectX 11", "DirectX 12"};
    static int currentAPI = 0;

    if (ImGui::Combo("API", &currentAPI, apiNames, IM_ARRAYSIZE(apiNames))) {
        LOG_INFO_C("Graphics API selection changed", "EngineConfigPanel");
    }

    ImGui::Spacing();
    ImGui::TextWrapped("Note: Changing the graphics API requires restarting the application.");
#endif
}

void EngineConfigPanel::renderRenderQualitySettings()
{
#ifdef FRESH_IMGUI_AVAILABLE
    ImGui::Text("Quality Presets:");
    ImGui::Spacing();

    static int qualityPreset = 2; // 0=Low, 1=Medium, 2=High, 3=Ultra
    const char* presetNames[] = {"Low", "Medium", "High", "Ultra"};

    if (ImGui::Combo("Preset", &qualityPreset, presetNames, IM_ARRAYSIZE(presetNames))) {
        LOG_INFO_C("Quality preset changed", "EngineConfigPanel");
    }

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    ImGui::Text("Advanced Settings:");
    static int shadowQuality = 2;
    ImGui::SliderInt("Shadow Quality", &shadowQuality, 0, 3);

    static int textureQuality = 2;
    ImGui::SliderInt("Texture Quality", &textureQuality, 0, 3);

    static int anisotropicFiltering = 8;
    ImGui::SliderInt("Anisotropic Filtering", &anisotropicFiltering, 1, 16);

    static bool enableMSAA = false;
    ImGui::Checkbox("Enable MSAA", &enableMSAA);

    if (enableMSAA) {
        static int msaaSamples = 4;
        const char* msaaOptions[] = {"2x", "4x", "8x"};
        static int msaaIndex = 1;
        ImGui::Combo("MSAA Samples", &msaaIndex, msaaOptions, IM_ARRAYSIZE(msaaOptions));
    }
#endif
}

void EngineConfigPanel::renderDisplaySettings()
{
#ifdef FRESH_IMGUI_AVAILABLE
    static bool fullscreen = false;
    ImGui::Checkbox("Fullscreen", &fullscreen);

    static bool vsync = true;
    ImGui::Checkbox("V-Sync", &vsync);

    static int fpsLimit = 60;
    ImGui::SliderInt("FPS Limit", &fpsLimit, 30, 240);

    static float brightness = 1.0f;
    ImGui::SliderFloat("Brightness", &brightness, 0.5f, 2.0f);

    static float gamma = 2.2f;
    ImGui::SliderFloat("Gamma", &gamma, 1.0f, 3.0f);
#endif
}

// Lighting sub-panels
void EngineConfigPanel::renderAmbientLightSettings()
{
#ifdef FRESH_IMGUI_AVAILABLE
    static float ambientColor[3] = {0.2f, 0.2f, 0.3f};
    ImGui::ColorEdit3("Ambient Color", ambientColor);

    static float ambientIntensity = 0.3f;
    ImGui::SliderFloat("Intensity", &ambientIntensity, 0.0f, 1.0f);
#endif
}

void EngineConfigPanel::renderDirectionalLightSettings()
{
#ifdef FRESH_IMGUI_AVAILABLE
    ImGui::Text("Directional lights (e.g., sun, moon)");
    ImGui::Spacing();

    static float sunDirection[3] = {0.0f, -1.0f, 0.0f};
    ImGui::DragFloat3("Sun Direction", sunDirection, 0.01f);

    static float sunColor[3] = {1.0f, 0.95f, 0.8f};
    ImGui::ColorEdit3("Sun Color", sunColor);

    static float sunIntensity = 1.0f;
    ImGui::SliderFloat("Sun Intensity", &sunIntensity, 0.0f, 2.0f);

    static bool castShadows = true;
    ImGui::Checkbox("Cast Shadows", &castShadows);
#endif
}

void EngineConfigPanel::renderPointLightSettings()
{
#ifdef FRESH_IMGUI_AVAILABLE
    ImGui::Text("Point lights (e.g., torches, lamps)");
    ImGui::Spacing();

    ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.7f, 1.0f),
                       "Point light configuration is done per-light in the scene.");
    ImGui::Spacing();

    ImGui::Text("Default Attenuation:");
    static float constant = 1.0f;
    ImGui::SliderFloat("Constant", &constant, 0.1f, 2.0f);

    static float linear = 0.09f;
    ImGui::SliderFloat("Linear", &linear, 0.01f, 0.5f);

    static float quadratic = 0.032f;
    ImGui::SliderFloat("Quadratic", &quadratic, 0.001f, 0.1f);
#endif
}

void EngineConfigPanel::renderSpotLightSettings()
{
#ifdef FRESH_IMGUI_AVAILABLE
    ImGui::Text("Spot lights (e.g., flashlights, spotlights)");
    ImGui::Spacing();

    ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.7f, 1.0f),
                       "Spot light configuration is done per-light in the scene.");
    ImGui::Spacing();

    ImGui::Text("Default Cone Angles:");
    static float innerCone = 12.5f;
    ImGui::SliderFloat("Inner Cone", &innerCone, 5.0f, 45.0f);

    static float outerCone = 17.5f;
    ImGui::SliderFloat("Outer Cone", &outerCone, 10.0f, 60.0f);
#endif
}

void EngineConfigPanel::renderShadowSettings()
{
#ifdef FRESH_IMGUI_AVAILABLE
    static bool enableShadows = true;
    ImGui::Checkbox("Enable Shadows", &enableShadows);

    if (enableShadows) {
        static int shadowMapSize = 2;
        const char* shadowMapSizes[] = {"512", "1024", "2048", "4096"};
        ImGui::Combo("Shadow Map Size", &shadowMapSize, shadowMapSizes, IM_ARRAYSIZE(shadowMapSizes));

        static float shadowBias = 0.005f;
        ImGui::SliderFloat("Shadow Bias", &shadowBias, 0.0001f, 0.01f, "%.4f");

        static bool enablePCF = true;
        ImGui::Checkbox("Enable PCF (Soft Shadows)", &enablePCF);

        if (enablePCF) {
            static int pcfSamples = 4;
            ImGui::SliderInt("PCF Samples", &pcfSamples, 1, 16);
        }
    }
#endif
}

void EngineConfigPanel::renderPBRSettings()
{
#ifdef FRESH_IMGUI_AVAILABLE
    static bool enablePBR = true;
    ImGui::Checkbox("Enable PBR Rendering", &enablePBR);

    if (enablePBR) {
        ImGui::Spacing();
        ImGui::Text("Image-Based Lighting (IBL):");

        static bool enableIBL = true;
        ImGui::Checkbox("Enable IBL", &enableIBL);

        if (enableIBL) {
            static float iblIntensity = 1.0f;
            ImGui::SliderFloat("IBL Intensity", &iblIntensity, 0.0f, 2.0f);
        }

        ImGui::Spacing();
        ImGui::Text("Specular Settings:");
        static float specularIntensity = 1.0f;
        ImGui::SliderFloat("Specular Intensity", &specularIntensity, 0.0f, 2.0f);
    }
#endif
}

// Post-processing sub-panels
void EngineConfigPanel::renderFXAASettings()
{
#ifdef FRESH_IMGUI_AVAILABLE
    static bool enableFXAA = true;
    ImGui::Checkbox("Enable FXAA", &enableFXAA);

    if (enableFXAA) {
        static float fxaaQuality = 0.75f;
        ImGui::SliderFloat("Quality", &fxaaQuality, 0.0f, 1.0f);

        ImGui::Spacing();
        ImGui::TextWrapped(
            "FXAA (Fast Approximate Anti-Aliasing) reduces jagged edges with minimal performance cost.");
    }
#endif
}

void EngineConfigPanel::renderBloomSettings()
{
#ifdef FRESH_IMGUI_AVAILABLE
    static bool enableBloom = true;
    ImGui::Checkbox("Enable Bloom", &enableBloom);

    if (enableBloom) {
        static float bloomThreshold = 1.0f;
        ImGui::SliderFloat("Threshold", &bloomThreshold, 0.5f, 3.0f);

        static float bloomIntensity = 0.5f;
        ImGui::SliderFloat("Intensity", &bloomIntensity, 0.0f, 2.0f);

        static float bloomRadius = 1.0f;
        ImGui::SliderFloat("Radius", &bloomRadius, 0.1f, 5.0f);

        static int bloomIterations = 5;
        ImGui::SliderInt("Iterations", &bloomIterations, 1, 10);
    }
#endif
}

void EngineConfigPanel::renderToneMappingSettings()
{
#ifdef FRESH_IMGUI_AVAILABLE
    static bool enableToneMapping = true;
    ImGui::Checkbox("Enable Tone Mapping", &enableToneMapping);

    if (enableToneMapping) {
        static int toneMappingOp = 4; // ACES Approx
        const char* operators[] = {"None", "Reinhard", "Reinhard Extended", "Uncharted 2", "ACES", "ACES Approx"};
        ImGui::Combo("Operator", &toneMappingOp, operators, IM_ARRAYSIZE(operators));

        static float exposure = 1.0f;
        ImGui::SliderFloat("Exposure", &exposure, 0.1f, 5.0f);

        static float gamma = 2.2f;
        ImGui::SliderFloat("Gamma", &gamma, 1.0f, 3.0f);
    }
#endif
}

void EngineConfigPanel::renderColorGradingSettings()
{
#ifdef FRESH_IMGUI_AVAILABLE
    static bool enableColorGrading = false;
    ImGui::Checkbox("Enable Color Grading", &enableColorGrading);

    if (enableColorGrading) {
        static float saturation = 1.0f;
        ImGui::SliderFloat("Saturation", &saturation, 0.0f, 2.0f);

        static float contrast = 1.0f;
        ImGui::SliderFloat("Contrast", &contrast, 0.0f, 2.0f);

        static float brightness = 1.0f;
        ImGui::SliderFloat("Brightness", &brightness, 0.0f, 2.0f);

        static float colorFilter[3] = {1.0f, 1.0f, 1.0f};
        ImGui::ColorEdit3("Color Filter", colorFilter);
    }
#endif
}

void EngineConfigPanel::renderVignetteSettings()
{
#ifdef FRESH_IMGUI_AVAILABLE
    static bool enableVignette = false;
    ImGui::Checkbox("Enable Vignette", &enableVignette);

    if (enableVignette) {
        static float vignetteIntensity = 0.3f;
        ImGui::SliderFloat("Intensity", &vignetteIntensity, 0.0f, 1.0f);

        static float vignetteSmoothness = 0.5f;
        ImGui::SliderFloat("Smoothness", &vignetteSmoothness, 0.0f, 1.0f);
    }
#endif
}

void EngineConfigPanel::renderDepthOfFieldSettings()
{
#ifdef FRESH_IMGUI_AVAILABLE
    static bool enableDOF = false;
    ImGui::Checkbox("Enable Depth of Field", &enableDOF);

    if (enableDOF) {
        static float focusDistance = 10.0f;
        ImGui::SliderFloat("Focus Distance", &focusDistance, 1.0f, 100.0f);

        static float focusRange = 5.0f;
        ImGui::SliderFloat("Focus Range", &focusRange, 1.0f, 50.0f);

        static float bokehRadius = 4.0f;
        ImGui::SliderFloat("Bokeh Radius", &bokehRadius, 1.0f, 10.0f);
    }
#endif
}

void EngineConfigPanel::renderSSAOSettings()
{
#ifdef FRESH_IMGUI_AVAILABLE
    static bool enableSSAO = false;
    ImGui::Checkbox("Enable SSAO", &enableSSAO);

    if (enableSSAO) {
        static float ssaoRadius = 0.5f;
        ImGui::SliderFloat("Radius", &ssaoRadius, 0.1f, 2.0f);

        static float ssaoBias = 0.025f;
        ImGui::SliderFloat("Bias", &ssaoBias, 0.001f, 0.1f);

        static int ssaoSamples = 16;
        ImGui::SliderInt("Samples", &ssaoSamples, 4, 64);

        ImGui::Spacing();
        ImGui::TextWrapped("SSAO (Screen-Space Ambient Occlusion) adds realistic shadowing in crevices.");
    }
#endif
}

// Input sub-panels
void EngineConfigPanel::renderKeyBindingsSettings()
{
#ifdef FRESH_IMGUI_AVAILABLE
    ImGui::Text("Key Bindings:");
    ImGui::Spacing();

    ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.7f, 1.0f), "Key binding customization coming soon...");
    ImGui::Spacing();

    // Example key bindings display
    ImGui::Columns(2);
    ImGui::Text("Action");
    ImGui::NextColumn();
    ImGui::Text("Key");
    ImGui::NextColumn();
    ImGui::Separator();

    ImGui::Text("Move Forward");
    ImGui::NextColumn();
    ImGui::Text("W");
    ImGui::NextColumn();

    ImGui::Text("Move Backward");
    ImGui::NextColumn();
    ImGui::Text("S");
    ImGui::NextColumn();

    ImGui::Text("Move Left");
    ImGui::NextColumn();
    ImGui::Text("A");
    ImGui::NextColumn();

    ImGui::Text("Move Right");
    ImGui::NextColumn();
    ImGui::Text("D");
    ImGui::NextColumn();

    ImGui::Text("Jump");
    ImGui::NextColumn();
    ImGui::Text("Space");
    ImGui::NextColumn();

    ImGui::Text("Toggle Editor");
    ImGui::NextColumn();
    ImGui::Text("T");
    ImGui::NextColumn();

    ImGui::Columns(1);
#endif
}

void EngineConfigPanel::renderGamepadSettings()
{
#ifdef FRESH_IMGUI_AVAILABLE
    ImGui::Text("Gamepad Settings:");
    ImGui::Spacing();

    static bool enableGamepad = true;
    ImGui::Checkbox("Enable Gamepad", &enableGamepad);

    if (enableGamepad) {
        static float gamepadSensitivity = 1.0f;
        ImGui::SliderFloat("Sensitivity", &gamepadSensitivity, 0.1f, 3.0f);

        static float deadzone = 0.15f;
        ImGui::SliderFloat("Deadzone", &deadzone, 0.0f, 0.5f);

        static bool invertYAxis = false;
        ImGui::Checkbox("Invert Y-Axis", &invertYAxis);

        static bool enableVibration = true;
        ImGui::Checkbox("Enable Vibration", &enableVibration);
    }
#endif
}

void EngineConfigPanel::renderMouseSettings()
{
#ifdef FRESH_IMGUI_AVAILABLE
    static float mouseSensitivity = 1.0f;
    ImGui::SliderFloat("Mouse Sensitivity", &mouseSensitivity, 0.1f, 5.0f);

    static bool invertYAxis = false;
    ImGui::Checkbox("Invert Mouse Y-Axis", &invertYAxis);

    static bool rawInput = true;
    ImGui::Checkbox("Use Raw Input", &rawInput);

    ImGui::Spacing();
    ImGui::TextWrapped("Raw input provides more precise mouse tracking but may feel different.");
#endif
}

// Performance monitoring
void EngineConfigPanel::renderFPSCounter()
{
#ifdef FRESH_IMGUI_AVAILABLE
    // Update FPS history
    static float lastFrameTime = 0.0f;
    frameTime = ImGui::GetIO().DeltaTime;
    float currentFPS = frameTime > 0.0f ? 1.0f / frameTime : 0.0f;

    fpsHistory[fpsHistoryIndex] = currentFPS;
    fpsHistoryIndex = (fpsHistoryIndex + 1) % 100;

    // Calculate average FPS
    float fpsSum = 0.0f;
    for (int i = 0; i < 100; ++i) {
        fpsSum += fpsHistory[i];
    }
    avgFPS = fpsSum / 100.0f;

    ImGui::Text("Current FPS: %.1f", currentFPS);
    ImGui::Text("Average FPS: %.1f", avgFPS);
    ImGui::Text("Frame Time: %.2f ms", frameTime * 1000.0f);

    ImGui::Spacing();
    ImGui::PlotLines("FPS History", fpsHistory, 100, fpsHistoryIndex, nullptr, 0.0f, 200.0f, ImVec2(0, 80));
#endif
}

void EngineConfigPanel::renderMemoryUsage()
{
#ifdef FRESH_IMGUI_AVAILABLE
    ImGui::Text("Memory Usage:");
    ImGui::Spacing();

    // These would be real values from MemoryManager in actual implementation
    ImGui::Text("Total Allocated: ~256 MB");
    ImGui::Text("Chunk Memory: ~128 MB");
    ImGui::Text("Texture Memory: ~64 MB");
    ImGui::Text("Audio Memory: ~32 MB");
    ImGui::Text("Other: ~32 MB");

    ImGui::Spacing();
    ImGui::ProgressBar(0.65f, ImVec2(-1, 0), "65%% of available memory");
#endif
}

void EngineConfigPanel::renderGPUStats()
{
#ifdef FRESH_IMGUI_AVAILABLE
    ImGui::Text("GPU Statistics:");
    ImGui::Spacing();

    // These would be real values from RenderContext in actual implementation
    ImGui::Text("Draw Calls: ~2,500 per frame");
    ImGui::Text("Triangles: ~1.2M per frame");
    ImGui::Text("Texture Switches: ~150 per frame");
    ImGui::Text("Shader Switches: ~25 per frame");

    ImGui::Spacing();
    ImGui::Text("VRAM Usage: ~512 MB");
    ImGui::ProgressBar(0.45f, ImVec2(-1, 0), "45%% of VRAM");
#endif
}

// Material editor
void EngineConfigPanel::renderMaterialList()
{
#ifdef FRESH_IMGUI_AVAILABLE
    ImGui::Text("Materials:");
    ImGui::Separator();
    ImGui::Spacing();

    // Example material list
    if (ImGui::Selectable("Default Material", false)) {
        selectedMaterial = std::make_shared<Material>("Default Material");
    }
    if (ImGui::Selectable("Stone Material", false)) {
        selectedMaterial = std::make_shared<Material>("Stone Material");
    }
    if (ImGui::Selectable("Metal Material", false)) {
        selectedMaterial = std::make_shared<Material>("Metal Material");
    }
    if (ImGui::Selectable("Wood Material", false)) {
        selectedMaterial = std::make_shared<Material>("Wood Material");
    }

    ImGui::Spacing();
    if (ImGui::Button("Create New", ImVec2(-1, 0))) {
        selectedMaterial = std::make_shared<Material>("New Material");
        LOG_INFO_C("Created new material", "EngineConfigPanel");
    }
#endif
}

void EngineConfigPanel::renderMaterialProperties(Material* material)
{
#ifdef FRESH_IMGUI_AVAILABLE
    if (!material) {
        return;
    }

    ImGui::Text("Material: %s", material->name.c_str());
    ImGui::Separator();
    ImGui::Spacing();

    // Basic properties
    if (ImGui::CollapsingHeader("Basic Properties", ImGuiTreeNodeFlags_DefaultOpen)) {
        float albedo[3] = {material->albedo.r, material->albedo.g, material->albedo.b};
        if (ImGui::ColorEdit3("Albedo Color", albedo)) {
            material->albedo = glm::vec3(albedo[0], albedo[1], albedo[2]);
        }

        ImGui::SliderFloat("Metallic", &material->metallic, 0.0f, 1.0f);
        ImGui::SliderFloat("Roughness", &material->roughness, 0.0f, 1.0f);
        ImGui::SliderFloat("AO", &material->ao, 0.0f, 1.0f);
    }

    // Emissive properties
    if (ImGui::CollapsingHeader("Emissive")) {
        float emissive[3] = {material->emissive.r, material->emissive.g, material->emissive.b};
        if (ImGui::ColorEdit3("Emissive Color", emissive)) {
            material->emissive = glm::vec3(emissive[0], emissive[1], emissive[2]);
        }
        ImGui::SliderFloat("Emissive Strength", &material->emissiveStrength, 0.0f, 10.0f);
    }

    // Transparency
    if (ImGui::CollapsingHeader("Transparency")) {
        ImGui::SliderFloat("Alpha", &material->alpha, 0.0f, 1.0f);
        ImGui::Checkbox("Alpha Blend", &material->alphaBlend);
        ImGui::Checkbox("Alpha Cutoff", &material->alphaCutoff);
        if (material->alphaCutoff) {
            ImGui::SliderFloat("Cutoff Value", &material->alphaCutoffValue, 0.0f, 1.0f);
        }
    }

    // Cell shading
    if (ImGui::CollapsingHeader("Cell Shading (Toon)")) {
        ImGui::Checkbox("Enable Cell Shading", &material->enableCellShading);
        if (material->enableCellShading) {
            ImGui::SliderInt("Shading Levels", &material->cellShadingLevels, 2, 10);
            ImGui::SliderFloat("Edge Width", &material->cellShadingEdgeWidth, 0.0f, 0.5f);
            float edgeColor[3] = {material->cellShadingEdgeColor.r, material->cellShadingEdgeColor.g,
                                  material->cellShadingEdgeColor.b};
            if (ImGui::ColorEdit3("Edge Color", edgeColor)) {
                material->cellShadingEdgeColor = glm::vec3(edgeColor[0], edgeColor[1], edgeColor[2]);
            }
        }
    }

    // Render state
    if (ImGui::CollapsingHeader("Render State")) {
        ImGui::Checkbox("Double Sided", &material->doubleSided);
        ImGui::Checkbox("Receive Shadows", &material->receiveShadows);
        ImGui::Checkbox("Cast Shadows", &material->castShadows);
    }

    // Texture maps
    if (ImGui::CollapsingHeader("Texture Maps")) {
        ImGui::Text("Albedo Map:");
        renderTextureSelector(material->albedoMap, "Albedo");

        ImGui::Text("Normal Map:");
        renderTextureSelector(material->normalMap, "Normal");

        ImGui::Text("Metallic/Roughness Map:");
        renderTextureSelector(material->metallicRoughnessMap, "MetallicRoughness");

        ImGui::Text("AO Map:");
        renderTextureSelector(material->aoMap, "AO");

        ImGui::Text("Emissive Map:");
        renderTextureSelector(material->emissiveMap, "Emissive");
    }
#else
    (void)material;  // Suppress unreferenced parameter warning
#endif
}

void EngineConfigPanel::renderTextureSelector(std::shared_ptr<class Texture>& texture, const char* label)
{
#ifdef FRESH_IMGUI_AVAILABLE
    ImGui::PushID(label);
    if (texture) {
        ImGui::Text("Loaded: %s", "texture.png"); // Would show actual path
        if (ImGui::Button("Clear")) {
            texture = nullptr;
        }
    } else {
        ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.7f, 1.0f), "No texture");
        if (ImGui::Button("Load...")) {
            // Would open file dialog
            LOG_INFO_C("Load texture dialog (not implemented)", "EngineConfigPanel");
        }
    }
    ImGui::PopID();
    ImGui::Spacing();
#else
    (void)texture;  // Suppress unreferenced parameter warning
    (void)label;    // Suppress unreferenced parameter warning
#endif
}

bool EngineConfigPanel::saveConfig(const std::string& path)
{
    try {
        std::ofstream file(path);
        if (!file.is_open()) {
            LOG_ERROR_C("Failed to open config file for writing: " + path, "EngineConfigPanel");
            return false;
        }

        // Write configuration values
        file << "# Engine Configuration File\n";
        file << "# Auto-generated by EngineConfigPanel\n\n";

        // Graphics settings
        file << "[Graphics]\n";
        file << "api=auto\n";
        file << "quality_preset=high\n";
        file << "shadow_quality=2\n";
        file << "texture_quality=2\n";
        file << "anisotropic_filtering=8\n";
        file << "\n";

        // Display settings
        file << "[Display]\n";
        file << "fullscreen=0\n";
        file << "vsync=1\n";
        file << "fps_limit=60\n";
        file << "brightness=1.0\n";
        file << "gamma=2.2\n";
        file << "\n";

        // Post-processing
        file << "[PostProcessing]\n";
        file << "fxaa=1\n";
        file << "bloom=1\n";
        file << "tone_mapping=1\n";
        file << "\n";

        // Input
        file << "[Input]\n";
        file << "mouse_sensitivity=1.0\n";
        file << "invert_mouse_y=0\n";
        file << "\n";

        file.close();
        LOG_INFO_C("Configuration saved to: " + path, "EngineConfigPanel");
        return true;

    } catch (const std::exception& e) {
        LOG_ERROR_C("Exception while saving config: " + std::string(e.what()), "EngineConfigPanel");
        return false;
    }
}

bool EngineConfigPanel::loadConfig(const std::string& path)
{
    try {
        std::ifstream file(path);
        if (!file.is_open()) {
            LOG_WARNING_C("Config file not found, using defaults: " + path, "EngineConfigPanel");
            return false;
        }

        std::string line;
        while (std::getline(file, line)) {
            // Skip comments and empty lines
            if (line.empty() || line[0] == '#' || line[0] == '[') {
                continue;
            }

            // Parse key=value pairs
            size_t pos = line.find('=');
            if (pos != std::string::npos) {
                std::string key = line.substr(0, pos);
                std::string value = line.substr(pos + 1);
                // Would apply configuration values here
            }
        }

        file.close();
        LOG_INFO_C("Configuration loaded from: " + path, "EngineConfigPanel");
        return true;

    } catch (const std::exception& e) {
        LOG_ERROR_C("Exception while loading config: " + std::string(e.what()), "EngineConfigPanel");
        return false;
    }
}

} // namespace fresh
