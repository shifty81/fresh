#include "ui/EditorMenuBar.h"

#include "core/Logger.h"
#include "editor/WorldEditor.h"
#include "voxel/VoxelWorld.h"

#include <imgui.h>
#include <filesystem>

#ifdef _WIN32
#include <windows.h>
#include <shellapi.h>
#endif

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
        renderToolsMenu();
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
                LOG_INFO_C("Cut - TODO: Requires voxel selection system. Would cut selected voxels to clipboard.", "EditorMenuBar");
            }
        }

        if (ImGui::MenuItem("Copy", "Ctrl+C")) {
            if (m_copyCallback) {
                m_copyCallback();
            } else {
                LOG_INFO_C("Copy - TODO: Requires voxel selection system. Would copy selected voxels to clipboard.", "EditorMenuBar");
            }
        }

        if (ImGui::MenuItem("Paste", "Ctrl+V")) {
            if (m_pasteCallback) {
                m_pasteCallback();
            } else {
                LOG_INFO_C("Paste - TODO: Requires voxel selection system. Would paste voxels from clipboard.", "EditorMenuBar");
            }
        }

        if (ImGui::MenuItem("Delete", "Delete")) {
            if (m_deleteCallback) {
                m_deleteCallback();
            } else {
                LOG_INFO_C("Delete - TODO: Would delete currently selected object(s) or voxels.", "EditorMenuBar");
            }
        }

        ImGui::Separator();

        if (ImGui::MenuItem("Select All", "Ctrl+A")) {
            if (m_selectAllCallback) {
                m_selectAllCallback();
            } else {
                LOG_INFO_C("Select All - would select all objects in scene", "EditorMenuBar");
            }
        }

        if (ImGui::MenuItem("Deselect All", "Ctrl+D")) {
            if (m_deselectAllCallback) {
                m_deselectAllCallback();
            } else {
                LOG_INFO_C("Deselect All - would clear selection", "EditorMenuBar");
            }
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
                if (m_loadLayoutCallback) {
                    m_loadLayoutCallback("Default");
                } else {
                    LOG_INFO_C("Loading default layout", "EditorMenuBar");
                }
            }
            if (ImGui::MenuItem("Minimal")) {
                if (m_loadLayoutCallback) {
                    m_loadLayoutCallback("Minimal");
                } else {
                    LOG_INFO_C("Loading minimal layout", "EditorMenuBar");
                }
            }
            if (ImGui::MenuItem("Debugging")) {
                if (m_loadLayoutCallback) {
                    m_loadLayoutCallback("Debugging");
                } else {
                    LOG_INFO_C("Loading debugging layout", "EditorMenuBar");
                }
            }
            ImGui::Separator();
            if (ImGui::MenuItem("Save Layout")) {
                if (m_saveLayoutCallback) {
                    m_saveLayoutCallback();
                } else {
                    LOG_INFO_C("Saving current layout", "EditorMenuBar");
                }
            }
            ImGui::EndMenu();
        }

        ImGui::Separator();

        if (ImGui::MenuItem("Reset Layout")) {
            if (m_resetLayoutCallback) {
                m_resetLayoutCallback();
            } else {
                LOG_INFO_C("Resetting to default layout", "EditorMenuBar");
            }
        }

        ImGui::EndMenu();
    }
}

void EditorMenuBar::renderToolsMenu()
{
    if (ImGui::BeginMenu("Tools")) {
        if (ImGui::MenuItem("Dialogue Editor", "Ctrl+D")) {
            if (m_dialogueEditorCallback) {
                m_dialogueEditorCallback();
            } else {
                LOG_INFO_C("Dialogue Editor (callback not set)", "EditorMenuBar");
            }
        }

        ImGui::Separator();

        if (ImGui::MenuItem("Import Assets...", "Ctrl+I")) {
            if (m_importAssetsCallback) {
                m_importAssetsCallback();
            } else {
                LOG_INFO_C("Import Assets... (callback not set)", "EditorMenuBar");
            }
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

        if (ImGui::MenuItem("Engine Configuration...", "Ctrl+E")) {
            if (m_engineConfigCallback) {
                m_engineConfigCallback();
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

#ifdef _WIN32
        if (ImGui::MenuItem("Windows Customization...", "Ctrl+W")) {
            if (m_windowsCustomizationCallback) {
                m_windowsCustomizationCallback();
            }
        }

        ImGui::Separator();
#endif

        if (ImGui::MenuItem("Input Settings...")) {
            ImGui::OpenPopup("Input Settings");
        }

        if (ImGui::MenuItem("Audio Settings...")) {
            ImGui::OpenPopup("Audio Settings");
        }

        if (ImGui::MenuItem("Editor Settings...")) {
            if (m_editorSettingsCallback) {
                m_editorSettingsCallback();
            } else {
                ImGui::OpenPopup("Editor Settings");
            }
        }

        ImGui::EndMenu();
    }
}

void EditorMenuBar::renderHelpMenu()
{
    if (ImGui::BeginMenu("Help")) {
        if (ImGui::MenuItem("Documentation", "F1")) {
#ifdef _WIN32
            // Open README.md or DOCUMENTATION.md with default application
            ShellExecuteA(nullptr, "open", "README.md", nullptr, nullptr, SW_SHOW);
            LOG_INFO_C("Opening documentation (README.md)", "EditorMenuBar");
#else
            LOG_INFO_C("Documentation: See README.md in the project root", "EditorMenuBar");
#endif
        }

        if (ImGui::MenuItem("API Reference")) {
#ifdef _WIN32
            // Try to open Doxygen HTML documentation if it exists
            if (std::filesystem::exists("docs/html/index.html")) {
                ShellExecuteA(nullptr, "open", "docs/html/index.html", nullptr, nullptr, SW_SHOW);
                LOG_INFO_C("Opening API reference (docs/html/index.html)", "EditorMenuBar");
            } else {
                LOG_INFO_C("API Reference not found. Run 'doxygen' to generate API docs.", "EditorMenuBar");
            }
#else
            LOG_INFO_C("API Reference: See docs/html/index.html (run doxygen to generate)", "EditorMenuBar");
#endif
        }

        if (ImGui::MenuItem("Tutorials")) {
#ifdef _WIN32
            // Open getting started guide
            if (std::filesystem::exists("docs/getting-started/GETTING_STARTED.md")) {
                ShellExecuteA(nullptr, "open", "docs/getting-started/GETTING_STARTED.md", nullptr, nullptr, SW_SHOW);
                LOG_INFO_C("Opening tutorials", "EditorMenuBar");
            } else {
                LOG_INFO_C("Tutorials: See docs/getting-started/ folder", "EditorMenuBar");
            }
#else
            LOG_INFO_C("Tutorials: See docs/getting-started/GETTING_STARTED.md", "EditorMenuBar");
#endif
        }

        ImGui::Separator();

        if (ImGui::MenuItem("Report Bug...")) {
#ifdef _WIN32
            ShellExecuteA(nullptr, "open", "https://github.com/shifty81/fresh/issues/new", nullptr, nullptr, SW_SHOW);
            LOG_INFO_C("Opening GitHub issues page in browser", "EditorMenuBar");
#else
            LOG_INFO_C("Report bugs at: https://github.com/shifty81/fresh/issues/new", "EditorMenuBar");
#endif
        }

        if (ImGui::MenuItem("Feature Request...")) {
#ifdef _WIN32
            ShellExecuteA(nullptr, "open", "https://github.com/shifty81/fresh/discussions", nullptr, nullptr, SW_SHOW);
            LOG_INFO_C("Opening GitHub discussions page in browser", "EditorMenuBar");
#else
            LOG_INFO_C("Feature requests at: https://github.com/shifty81/fresh/discussions", "EditorMenuBar");
#endif
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
