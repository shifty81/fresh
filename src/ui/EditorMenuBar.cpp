#include "ui/EditorMenuBar.h"
#include "voxel/VoxelWorld.h"
#include "editor/WorldEditor.h"
#include "core/Logger.h"
#include <imgui.h>

namespace fresh {

EditorMenuBar::EditorMenuBar()
    : m_world(nullptr)
    , m_editor(nullptr)
    , m_showSceneHierarchy(nullptr)
    , m_showInspector(nullptr)
    , m_showContentBrowser(nullptr)
    , m_showConsole(nullptr)
    , m_showToolPalette(nullptr)
{
}

EditorMenuBar::~EditorMenuBar() {
}

bool EditorMenuBar::initialize(VoxelWorld* world, WorldEditor* editor) {
    m_world = world;
    m_editor = editor;
    
    LOG_INFO_C("Editor Menu Bar initialized", "EditorMenuBar");
    return true;
}

void EditorMenuBar::render() {
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

void EditorMenuBar::renderFileMenu() {
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
            // TODO: Implement save as
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
            // TODO: Implement asset import
        }
        
        if (ImGui::MenuItem("Export World...")) {
            // TODO: Implement world export
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

void EditorMenuBar::renderEditMenu() {
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
            // TODO: Implement cut
        }
        
        if (ImGui::MenuItem("Copy", "Ctrl+C")) {
            // TODO: Implement copy
        }
        
        if (ImGui::MenuItem("Paste", "Ctrl+V")) {
            // TODO: Implement paste
        }
        
        if (ImGui::MenuItem("Delete", "Delete")) {
            // TODO: Implement delete
        }
        
        ImGui::Separator();
        
        if (ImGui::MenuItem("Select All", "Ctrl+A")) {
            // TODO: Implement select all
        }
        
        if (ImGui::MenuItem("Deselect All", "Ctrl+D")) {
            // TODO: Implement deselect all
        }
        
        ImGui::Separator();
        
        if (ImGui::MenuItem("Preferences...")) {
            // TODO: Open preferences window
        }
        
        ImGui::EndMenu();
    }
}

void EditorMenuBar::renderWindowMenu() {
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
                // TODO: Load default layout
            }
            if (ImGui::MenuItem("Minimal")) {
                // TODO: Load minimal layout
            }
            if (ImGui::MenuItem("Debugging")) {
                // TODO: Load debugging layout
            }
            ImGui::Separator();
            if (ImGui::MenuItem("Save Layout")) {
                // TODO: Save current layout
            }
            ImGui::EndMenu();
        }
        
        ImGui::Separator();
        
        if (ImGui::MenuItem("Reset Layout")) {
            // TODO: Reset to default layout
        }
        
        ImGui::EndMenu();
    }
}

void EditorMenuBar::renderBuildMenu() {
    if (ImGui::BeginMenu("Build")) {
        if (ImGui::MenuItem("Build World", "F7")) {
            // TODO: Trigger world build/optimization
        }
        
        if (ImGui::MenuItem("Build and Run", "F5")) {
            // TODO: Build and enter play mode
        }
        
        ImGui::Separator();
        
        if (ImGui::MenuItem("Generate Lightmaps")) {
            // TODO: Generate lightmaps
        }
        
        if (ImGui::MenuItem("Optimize Meshes")) {
            // TODO: Optimize meshes
        }
        
        if (ImGui::MenuItem("Compress Textures")) {
            // TODO: Compress textures
        }
        
        ImGui::Separator();
        
        if (ImGui::MenuItem("Build Settings...")) {
            // TODO: Open build settings
        }
        
        ImGui::EndMenu();
    }
}

void EditorMenuBar::renderSettingsMenu() {
    if (ImGui::BeginMenu("Settings")) {
        if (ImGui::BeginMenu("Graphics API")) {
            bool isOpenGL = true;  // TODO: Get from render context
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

void EditorMenuBar::renderHelpMenu() {
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
