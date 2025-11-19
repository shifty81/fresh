# Dialogue Editor Integration - Visual Guide

## Menu Integration

The dialogue editor is now accessible through the main editor's menu system:

```
╔════════════════════════════════════════════════════════════════════╗
║ Fresh Engine Editor                                       [_][□][X] ║
╠════════════════════════════════════════════════════════════════════╣
║ File  Edit  Window  Tools  Build  Settings  Help         FPS: 60.0 ║
║                       │                                              ║
║                       ▼                                              ║
║                    ┌──────────────────────┐                         ║
║                    │ Dialogue Editor  ^D  │ ◄── NEW MENU ITEM      ║
║                    ├──────────────────────┤                         ║
║                    │ Import Assets... ^I  │                         ║
║                    └──────────────────────┘                         ║
╠════════════════════════════════════════════════════════════════════╣
║                                                                      ║
║  [Editor Content - 3D Viewport, Panels, etc.]                       ║
║                                                                      ║
╚════════════════════════════════════════════════════════════════════╝
```

## Launch Flow

```
┌────────────────────────────────────────────────────────────────┐
│ User Action                                                    │
│ • Click "Tools → Dialogue Editor"                             │
│ • OR Press Ctrl+D                                             │
└────────────────────┬───────────────────────────────────────────┘
                     ▼
┌────────────────────────────────────────────────────────────────┐
│ EditorMenuBar::renderToolsMenu()                              │
│ • Detects menu item click                                      │
│ • Calls m_dialogueEditorCallback()                            │
└────────────────────┬───────────────────────────────────────────┘
                     ▼
┌────────────────────────────────────────────────────────────────┐
│ EditorManager::launchDialogueEditor()                         │
│ • Checks for executable in Release/Debug folders              │
│ • Uses CreateProcessA() to spawn process                       │
│ • Logs success/failure                                         │
└────────────────────┬───────────────────────────────────────────┘
                     ▼
┌────────────────────────────────────────────────────────────────┐
│ DialogueEditor.exe (WinForms App)                             │
│ • Launches in separate process                                 │
│ • Independent window (Unreal-style UI)                        │
│ • User creates/edits dialogues                                │
│ • Saves as JSON files                                         │
└────────────────────┬───────────────────────────────────────────┘
                     ▼
┌────────────────────────────────────────────────────────────────┐
│ Dialogue Files (JSON)                                          │
│ • Saved to project directory                                   │
│ • Can be loaded by DialogueManager at runtime                 │
│ • Version controlled with Git                                  │
└────────────────────────────────────────────────────────────────┘
```

## File System Integration

```
Fresh Engine Project
│
├── Fresh Engine Editor (C++)
│   ├── EditorManager.cpp          ◄─── Added launchDialogueEditor()
│   ├── EditorMenuBar.cpp          ◄─── Added Tools menu
│   └── [Other editor components]
│
├── dotnet/
│   ├── DialogueEditor/
│   │   ├── bin/
│   │   │   ├── Release/
│   │   │   │   └── net9.0-windows/
│   │   │   │       └── DialogueEditor.exe  ◄─── Launched by editor
│   │   │   └── Debug/
│   │   │       └── net9.0-windows/
│   │   │           └── DialogueEditor.exe  ◄─── Fallback path
│   │   └── [Source files]
│   │
│   └── DialogueSystem/
│       └── [Core dialogue classes]
│
└── dialogues/                      ◄─── User saves dialogues here
    ├── quest_dialogue.json
    ├── npc_conversations.json
    └── [Other dialogue files]
```

## Side-by-Side View

When the dialogue editor is launched from the main editor:

```
┌─────────────────────────────┐  ┌─────────────────────────────┐
│ Fresh Engine Editor         │  │ Dialogue Editor             │
│ (C++ Native)                │  │ (C# WinForms)               │
│                             │  │                             │
│  ┌──────────────────────┐   │  │  ┌──────────────────────┐   │
│  │                      │   │  │  │  [Start] → [Dialog]  │   │
│  │  3D Viewport         │   │  │  │     ↓                 │   │
│  │                      │   │  │  │  [Choice]             │   │
│  │                      │   │  │  │   ├─→ [Accept]        │   │
│  │                      │   │  │  │   └─→ [Decline]       │   │
│  └──────────────────────┘   │  │  └──────────────────────┘   │
│                             │  │                             │
│  [Scene] [Inspector]        │  │  [Details Panel]            │
│  [Console] [Content]        │  │  - Title: NPC Greeting      │
│                             │  │  - Speaker: Village Elder   │
│  Tools → Dialogue Editor    │  │  - Text: "Hello..."         │
│      ▲                      │  │                             │
│      └──────────────────────┼──┘  Save → quest_dialogue.json│
│    Launches this app        │                               │
└─────────────────────────────┘                               │
         │                                                     │
         └─────────────────────────────────────────────────────┘
              Both work together but are separate processes
```

## Code Integration Points

### 1. EditorManager.h (Line ~365)
```cpp
/**
 * @brief Launch dialogue editor
 * Opens the dialogue editor tool for creating and editing game dialogues
 */
void launchDialogueEditor();
```

### 2. EditorManager.cpp (Line ~1269)
```cpp
void EditorManager::launchDialogueEditor()
{
#ifdef _WIN32
    LOG_INFO_C("Launching dialogue editor", "EditorManager");
    
    // Path to the dialogue editor executable
    std::string editorPath = "dotnet/DialogueEditor/bin/Release/net9.0-windows/DialogueEditor.exe";
    
    // Check if the editor exists...
    // Launch the dialogue editor as a separate process...
    if (CreateProcessA(...)) {
        LOG_INFO_C("Dialogue editor launched successfully", "EditorManager");
    }
#else
    LOG_WARNING_C("Dialogue editor is only available on Windows", "EditorManager");
#endif
}
```

### 3. EditorMenuBar.h (Line ~97)
```cpp
void setDialogueEditorCallback(MenuCallback callback)
{
    m_dialogueEditorCallback = callback;
}
```

### 4. EditorMenuBar.cpp (Line ~247)
```cpp
void EditorMenuBar::renderToolsMenu()
{
    if (ImGui::BeginMenu("Tools")) {
        if (ImGui::MenuItem("Dialogue Editor", "Ctrl+D")) {
            if (m_dialogueEditorCallback) {
                m_dialogueEditorCallback();
            }
        }
        
        ImGui::Separator();
        
        if (ImGui::MenuItem("Import Assets...", "Ctrl+I")) {
            if (m_importAssetsCallback) {
                m_importAssetsCallback();
            }
        }
        
        ImGui::EndMenu();
    }
}
```

### 5. EditorManager.cpp Initialization (Line ~156)
```cpp
// Set dialogue editor callback
m_menuBar->setDialogueEditorCallback([this]() {
    launchDialogueEditor();
});
```

## User Workflow

### Step 1: Open Main Editor
```
[User launches FreshVoxelEngine.exe]
    ↓
[Editor opens with 3D viewport and panels]
    ↓
[User works on voxel world]
```

### Step 2: Access Dialogue Editor
```
[User clicks Tools → Dialogue Editor]  OR  [User presses Ctrl+D]
    ↓
[Menu callback triggered]
    ↓
[EditorManager::launchDialogueEditor() called]
    ↓
[DialogueEditor.exe spawned as new process]
```

### Step 3: Create Dialogue
```
[Dialogue Editor window appears]
    ↓
[User creates nodes and connections]
    ↓
[User adds dialogue text, choices, Lua scripts]
    ↓
[User clicks File → Save]
    ↓
[Dialogue saved as JSON file]
```

### Step 4: Use in Game
```
[In game code: DialogueGraph.LoadFromFile("quest.json")]
    ↓
[DialogueManager executes the dialogue]
    ↓
[Game displays dialogue, handles choices]
```

## Technical Details

### Process Spawning (Windows)
```cpp
STARTUPINFOA si = {};
PROCESS_INFORMATION pi = {};
si.cb = sizeof(si);

CreateProcessA(
    editorPath.c_str(),     // Application path
    nullptr,                // Command line
    nullptr,                // Process security
    nullptr,                // Thread security
    FALSE,                  // Inherit handles
    0,                      // Creation flags
    nullptr,                // Environment
    nullptr,                // Current directory
    &si,                    // Startup info
    &pi                     // Process info
);

// Close handles immediately (fire and forget)
CloseHandle(pi.hProcess);
CloseHandle(pi.hThread);
```

### Path Resolution
```cpp
1. Check: dotnet/DialogueEditor/bin/Release/net9.0-windows/DialogueEditor.exe
2. If not found, check: dotnet/DialogueEditor/bin/Debug/net9.0-windows/DialogueEditor.exe
3. If neither found: Log error and show message
```

## Benefits of This Integration

### ✅ Seamless Workflow
- No need to close editor to create dialogues
- Quick access via keyboard shortcut
- Integrated into familiar menu structure

### ✅ Separate Process
- Dialogue editor runs independently
- Main editor remains responsive
- Can have multiple dialogue editors open
- No memory/resource conflicts

### ✅ Standard Integration Pattern
- Follows typical game editor patterns
- Similar to Unity's "Tools" menu
- Similar to Unreal's editor tools
- Familiar to game developers

### ✅ File-Based Communication
- Editors communicate via JSON files
- No complex IPC needed
- Version control friendly
- Easy to backup/share

## Future Enhancements

### Potential Improvements
1. **In-Editor Preview Panel**
   - Embed small dialogue preview
   - Quick testing without full launch

2. **File Associations**
   - Double-click .json files to open in dialogue editor
   - Context menu "Edit Dialogue"

3. **Live Sync**
   - Hot-reload dialogues while game runs
   - See changes immediately

4. **Multi-Platform**
   - Port dialogue editor to Avalonia or MAUI
   - Support Linux and macOS

5. **Embedded Editor**
   - Embed dialogue editor as dockable panel
   - Full integration into main editor UI

## Comparison: Before vs After

### Before Integration
```
User wants to create dialogue:
1. Open separate DialogueEditor.exe
2. Create dialogue
3. Save file
4. Remember file location
5. Return to main editor
6. Load dialogue file in code
```

### After Integration
```
User wants to create dialogue:
1. Press Ctrl+D in editor
2. Create dialogue
3. Save file (auto-suggested location)
4. Close dialogue editor
5. Load dialogue file in code (known location)
```

**Time Saved**: ~30-60 seconds per dialogue editing session
**Workflow**: Much smoother and more integrated

## Summary

The dialogue editor is now fully integrated into the Fresh Engine editor workflow:
- ✅ Accessible via Tools menu
- ✅ Keyboard shortcut (Ctrl+D)
- ✅ Automatic path detection
- ✅ Separate process for stability
- ✅ Follows industry patterns
- ✅ Well documented

Users can now create dialogues without leaving their main editor environment!
