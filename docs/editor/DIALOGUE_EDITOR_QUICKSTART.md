# Dialogue Editor Quick Start Guide

## Installation

1. **Prerequisites**
   - .NET 9 SDK installed
   - Windows 10/11 (x64)

2. **Build the Editor**
   ```batch
   cd dotnet/DialogueEditor
   dotnet build -c Release
   ```

3. **Run the Editor**
   ```batch
   cd bin/Release/net9.0-windows
   DialogueEditor.exe
   ```

## Your First Dialogue

### Step 1: Create Nodes

1. Launch the Dialogue Editor
2. You'll see a **Start** node (green) already on the canvas
3. Click **"+ Dialogue"** in the toolbar to add a dialogue node
4. Click **"+ Choice"** to add player choices
5. Click **"+ End"** to add an ending point

### Step 2: Position Nodes

- **Left Click + Drag** any node to move it
- Arrange nodes left-to-right to show dialogue flow
- Use the grid to align nodes neatly

### Step 3: Connect Nodes

1. Hold **Ctrl** and **Left Click** on the Start node
2. **Left Click** on your dialogue node
3. A curved line (Bezier curve) will connect them
4. Repeat to connect all nodes in sequence

### Step 4: Edit Node Content

1. **Double-click** a dialogue node or select it
2. Use the **Details** panel on the right to edit:
   - **Title**: Node display name
   - **Speaker**: Character name
   - **DialogueText**: What they say
3. For choice nodes, add choice texts in **ChoiceOptions**

### Step 5: Add Lua Logic (Optional)

1. Add a **"+ Lua Script"** node (purple)
2. Select it and edit **LuaScript** in Details panel
3. Example:
   ```lua
   print("Quest started!")
   questFlag = true
   ```

### Step 6: Save Your Work

1. Click **File → Save** or press **Ctrl+S**
2. Choose a location (e.g., `dialogues/my_quest.json`)
3. Give it a descriptive name

## Example: Simple NPC Conversation

```
[Start] → [NPC Says Hello] → [Player Choice]
                                ├→ [Accept Quest] → [Start Quest Script] → [End]
                                ├→ [Ask Questions] → [More Info] → [Player Choice] (loops back)
                                └→ [Decline] → [End]
```

### Creating This:

1. **Nodes needed**:
   - Start (already exists)
   - 1 Dialogue node: "NPC Says Hello"
   - 1 Choice node: "Player Choice" with 3 options
   - 3 Dialogue nodes: "Accept Quest", "More Info", "Decline"
   - 1 Lua Script node: "Start Quest Script"
   - 1 End node

2. **Content**:
   - NPC Says Hello: "Greetings, traveler! I need your help!"
   - Player Choice options:
     - "I'll help you"
     - "Tell me more"
     - "Not interested"
   
3. **Connect** with Ctrl+Click between nodes

## Using in Your Game

### C# Code Example

```csharp
using FreshEngine.DialogueSystem;

// Load the dialogue
var dialogue = DialogueGraph.LoadFromFile("dialogues/my_quest.json");

// Create manager
var manager = new DialogueManager();

// Handle events
manager.OnDialogueDisplay += (sender, args) => {
    // Show dialogue in your game UI
    ShowDialogue(args.Speaker, args.DialogueText);
};

manager.OnChoicePresented += (sender, args) => {
    // Show choice buttons
    ShowChoices(args.Choices);
};

manager.OnDialogueEnded += (sender, args) => {
    // Close dialogue UI
    CloseDialogue();
};

// Start the dialogue
manager.StartDialogue(dialogue);

// When player clicks continue
manager.Continue();

// When player makes a choice
manager.MakeChoice(choiceIndex);
```

## Tips for Beginners

1. **Start Simple**: Create a linear conversation first
2. **Use Colors**: Node colors help you identify types at a glance
3. **Zoom Out**: Use mouse wheel to see the whole dialogue tree
4. **Test Often**: Load your dialogue in-game frequently to test
5. **Save Versions**: Save different versions as you iterate
6. **Use Lua Later**: Get comfortable with basic dialogue first
7. **Right-Click**: Context menus provide quick access to functions

## Common Mistakes

❌ **Don't**: Create links from End nodes
✅ **Do**: End nodes should have no outgoing connections

❌ **Don't**: Forget to connect all nodes
✅ **Do**: Every node (except Start) needs an incoming link

❌ **Don't**: Create overly complex trees at first
✅ **Do**: Start with 3-5 nodes, then expand

❌ **Don't**: Put game logic in DialogueText
✅ **Do**: Use Lua Script nodes for game logic

## Keyboard Shortcuts

| Shortcut | Action |
|----------|--------|
| Ctrl + Left Click | Create link |
| Ctrl + S | Save |
| Double Click | Edit node |
| Delete | Delete selected (after confirming) |
| Mouse Wheel | Zoom in/out |

## Next Steps

1. ✅ Create your first dialogue
2. 📚 Read the full README in `dotnet/DialogueEditor/README.md`
3. 🎮 Integrate with your game
4. 📜 Learn Lua scripting for advanced features
5. 🎨 Customize node appearance (advanced)

## Getting Help

- Check `examples/dialogue/sample_quest.json` for a complete example
- See `examples/dialogue/dialogue_logic.lua` for Lua examples
- Read the comprehensive README for detailed documentation

## Unreal Engine Users

If you're familiar with Unreal Engine:
- The interface is designed to feel like Blueprint editor
- Nodes = Blueprint nodes
- Links = Execution pins
- Details panel = Details panel (same name!)
- Dark theme = Unreal's dark theme

Welcome to Fresh Engine dialogue editing! 🎮✨
