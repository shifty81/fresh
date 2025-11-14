#include "ui/EditorMenuBar.h"

#include "core/Logger.h"
#include "editor/WorldEditor.h"
#include "voxel/VoxelWorld.h"

#include <imgui.h>

namespace fresh
{

EditorMenuBar::EditorMenuBar()
    : m_world(nullptr),
      m_editor(nullptr),
      m_showSceneHierarchy(nullptr),
      m_showInspector(nullptr),
      m_showContentBrowser(nullptr),
      m_showConsole(nullptr),
      m_showToolPalette(nullptr)
{
}

EditorMenuBar::~EditorMenuBar() {}

bool EditorMenuBar::initialize(VoxelWorld* world, WorldEditor* editor)
{
    m_world = world;
    m_editor = editor;

    LOG_INFO_C("Editor Menu Bar initialized", "EditorMenuBar");
    return true;
}

void EditorMenuBar::render()
{
    if (ImGui::BeginMainMenuBar()) {
        renderFileMenu();
        renderEditMenu();
        renderWindowMenu();
        renderBuildMenu();
        renderSettingsMenu();
        renderHelpMenu();

        // FPS display on the right
        ImGui::SetCursorPosX(ImGui::GetWindowWidth() - 150);
        ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);

        ImGui::EndMainMenuBar();
    }
}

void EditorMenuBar::renderFileMenu()
{
    if (ImGui::BeginMenu("File")) {
        if (ImGui::MenuItem("New World", "Ctrl+N")) {
            if (m_newWorldCallback) {
                m_newWorldCallback();
            }
        }

        if (ImGui::MenuItem("Open World...", "Ctrl+O")) {
            if (m_loadWorldCallback) {
                m_loadWorldCallback();
            }
        }

        if (ImGui::MenuItem("Save World", "Ctrl+S")) {
            if (m_saveWorldCallback) {
                m_saveWorldCallback();
            }
        }

        if (ImGui::MenuItem("Save World As...", "Ctrl+Shift+S")) {
            if (m_saveWorldAsCallback) {
                m_saveWorldAsCallback();
            } else {
                LOG_INFO_C("Save World As... (callback not set)", "EditorMenuBar");
            }
        }

        ImGui::Separator();

        if (ImGui::BeginMenu("Recent Worlds")) {
            ImGui::MenuItem("World_001.world");
            ImGui::MenuItem("TestWorld.world");
            ImGui::MenuItem("MyWorld.world");
            ImGui::EndMenu();
        }

        ImGui::Separator();

        if (ImGui::MenuItem("Import Assets...")) {
            if (m_importAssetsCallback) {
                m_importAssetsCallback();
            } else {
                LOG_INFO_C("Import Assets dialog would open here", "EditorMenuBar");
            }
        }

        if (ImGui::MenuItem("Export World...")) {
            LOG_INFO_C("Export World dialog would open here", "EditorMenuBar");
        }

        ImGui::Separator();

        if (ImGui::MenuItem("Exit", "Alt+F4")) {
            if (m_exitCallback) {
                m_exitCallback();
            }
        }

        ImGui::EndMenu();
    }
}

void EditorMenuBar::renderEditMenu()
{
    if (ImGui::BeginMenu("Edit")) {
        bool canUndo = m_editor && m_editor->getTerraformingSystem();
        bool canRedo = canUndo;

        if (ImGui::MenuItem("Undo", "Ctrl+Z", false, canUndo)) {
            if (m_undoCallback) {
                m_undoCallback();
            }
        }

        if (ImGui::MenuItem("Redo", "Ctrl+Y", false, canRedo)) {
            if (m_redoCallback) {
                m_redoCallback();
            }
        }

        ImGui::Separator();

        if (ImGui::MenuItem("Cut", "Ctrl+X")) {
            if (m_cutCallback) {
                m_cutCallback();
            } else {
                LOG_INFO_C("Cut (callback not set)", "EditorMenuBar");
            }
        }

        if (ImGui::MenuItem("Copy", "Ctrl+C")) {
            if (m_copyCallback) {
                m_copyCallback();
            } else {
                LOG_INFO_C("Copy (callback not set)", "EditorMenuBar");
            }
        }

        if (ImGui::MenuItem("Paste", "Ctrl+V")) {
            if (m_pasteCallback) {
                m_pasteCallback();
            } else {
                LOG_INFO_C("Paste (callback not set)", "EditorMenuBar");
            }
        }

        if (ImGui::MenuItem("Delete", "Delete")) {
            if (m_deleteCallback) {
                m_deleteCallback();
            } else {
                LOG_INFO_C("Delete (callback not set)", "EditorMenuBar");
            }
        }

        ImGui::Separator();

        if (ImGui::MenuItem("Select All", "Ctrl+A")) {
            LOG_INFO_C("Select All - would select all objects in scene", "EditorMenuBar");
        }

        if (ImGui::MenuItem("Deselect All", "Ctrl+D")) {
            LOG_INFO_C("Deselect All - would clear selection", "EditorMenuBar");
        }

        ImGui::Separator();

        if (ImGui::MenuItem("Preferences...")) {
            LOG_INFO_C("Preferences window would open here", "EditorMenuBar");
        }

        ImGui::EndMenu();
    }
}

void EditorMenuBar::renderWindowMenu()
{
    if (ImGui::BeginMenu("Window")) {
        if (m_showSceneHierarchy) {
            ImGui::MenuItem("Scene Hierarchy", nullptr, m_showSceneHierarchy);
        }

        if (m_showInspector) {
            ImGui::MenuItem("Inspector", nullptr, m_showInspector);
        }

        if (m_showContentBrowser) {
            ImGui::MenuItem("Content Browser", nullptr, m_showContentBrowser);
        }

        if (m_showConsole) {
            ImGui::MenuItem("Console", nullptr, m_showConsole);
        }

        if (m_showToolPalette) {
            ImGui::MenuItem("Tool Palette", nullptr, m_showToolPalette);
        }

        ImGui::Separator();

        if (ImGui::BeginMenu("Layout")) {
            if (ImGui::MenuItem("Default")) {
                LOG_INFO_C("Loading default layout", "EditorMenuBar");
            }
            if (ImGui::MenuItem("Minimal")) {
                LOG_INFO_C("Loading minimal layout", "EditorMenuBar");
            }
            if (ImGui::MenuItem("Debugging")) {
                LOG_INFO_C("Loading debugging layout", "EditorMenuBar");
            }
            ImGui::Separator();
            if (ImGui::MenuItem("Save Layout")) {
                LOG_INFO_C("Saving current layout", "EditorMenuBar");
            }
            ImGui::EndMenu();
        }

        ImGui::Separator();

        if (ImGui::MenuItem("Reset Layout")) {
            LOG_INFO_C("Resetting to default layout", "EditorMenuBar");
        }

        ImGui::EndMenu();
    }
}

void EditorMenuBar::renderBuildMenu()
{
    if (ImGui::BeginMenu("Build")) {
        if (ImGui::MenuItem("Build World", "F7")) {
            LOG_INFO_C("Building/optimizing world...", "EditorMenuBar");
        }

        if (ImGui::MenuItem("Build and Run", "F5")) {
            LOG_INFO_C("Building and entering play mode...", "EditorMenuBar");
        }

        ImGui::Separator();

        if (ImGui::MenuItem("Generate Lightmaps")) {
            LOG_INFO_C("Generating lightmaps...", "EditorMenuBar");
        }

        if (ImGui::MenuItem("Optimize Meshes")) {
            LOG_INFO_C("Optimizing meshes...", "EditorMenuBar");
        }

        if (ImGui::MenuItem("Compress Textures")) {
            LOG_INFO_C("Compressing textures...", "EditorMenuBar");
        }

        ImGui::Separator();

        if (ImGui::MenuItem("Build Settings...")) {
            LOG_INFO_C("Opening build settings...", "EditorMenuBar");
        }

        ImGui::EndMenu();
    }
}

void EditorMenuBar::renderSettingsMenu()
{
    if (ImGui::BeginMenu("Settings")) {
        if (ImGui::MenuItem("Game Settings...", "Ctrl+,")) {
            if (m_settingsCallback) {
                m_settingsCallback();
            }
        }

        ImGui::Separator();

        if (ImGui::BeginMenu("Graphics API")) {
            bool isOpenGL = true; // Would query from render context
            bool isDX11 = false;
            bool isDX12 = false;

            ImGui::MenuItem("OpenGL", nullptr, &isOpenGL);
            ImGui::MenuItem("DirectX 11", nullptr, &isDX11);
            ImGui::MenuItem("DirectX 12", nullptr, &isDX12);
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Quality")) {
            static int quality = 2;
            ImGui::RadioButton("Low", &quality, 0);
            ImGui::RadioButton("Medium", &quality, 1);
            ImGui::RadioButton("High", &quality, 2);
            ImGui::RadioButton("Ultra", &quality, 3);
            ImGui::EndMenu();
        }

        ImGui::Separator();

        if (ImGui::MenuItem("Input Settings...")) {
            // TODO: Open input settings
        }

        if (ImGui::MenuItem("Audio Settings...")) {
            // TODO: Open audio settings
        }

        if (ImGui::MenuItem("Editor Settings...")) {
            // TODO: Open editor settings
        }

        ImGui::EndMenu();
    }
}

void EditorMenuBar::renderHelpMenu()
{
    if (ImGui::BeginMenu("Help")) {
        if (ImGui::MenuItem("Documentation", "F1")) {
            // TODO: Open documentation
        }

        if (ImGui::MenuItem("API Reference")) {
            // TODO: Open API reference
        }

        if (ImGui::MenuItem("Tutorials")) {
            // TODO: Open tutorials
        }

        ImGui::Separator();

        if (ImGui::MenuItem("Report Bug...")) {
            // TODO: Open bug report form
        }

        if (ImGui::MenuItem("Feature Request...")) {
            // TODO: Open feature request form
        }

        ImGui::Separator();

        if (ImGui::MenuItem("About Fresh Engine")) {
            ImGui::OpenPopup("About");
        }

        ImGui::EndMenu();
    }

    // About popup
    if (ImGui::BeginPopupModal("About", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::Text("Fresh Voxel Engine");
        ImGui::Text("Version 0.3.0");
        ImGui::Separator();
        ImGui::Text("A modern voxel-based game engine");
        ImGui::Text("Built with C++17 for Windows");
        ImGui::Spacing();
        ImGui::Text("Graphics APIs:");
        ImGui::BulletText("OpenGL 4.5+");
        ImGui::BulletText("DirectX 11");
        ImGui::BulletText("DirectX 12");
        ImGui::Spacing();
        if (ImGui::Button("Close", ImVec2(120, 0))) {
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }
}

} // namespace fresh
