# Installation Script Visual Preview

This document shows what users will see when running the new installation scripts.

## Root Launcher (install.bat)

When users double-click `install.bat` in the repository root:

```
============================================================================
       Fresh Voxel Engine - Installation Launcher
============================================================================

Choose your installation method:

  1. PowerShell (Recommended - Better progress display and error handling)
  2. Batch File (Classic - Works on older systems)
  3. Cancel

Enter your choice (1-3): _
```

## PowerShell Installation (install.ps1)

### Opening Banner Animation

The script starts with an animated ASCII art banner that appears line-by-line in cyan:

```
    ███████╗██████╗ ███████╗███████╗██╗  ██╗    ██╗   ██╗ ██████╗ ██╗  ██╗███████╗██╗         
    ██╔════╝██╔══██╗██╔════╝██╔════╝██║  ██║    ██║   ██║██╔═══██╗╚██╗██╔╝██╔════╝██║         
    █████╗  ██████╔╝█████╗  ███████╗███████║    ██║   ██║██║   ██║ ╚███╔╝ █████╗  ██║         
    ██╔══╝  ██╔══██╗██╔══╝  ╚════██║██╔══██║    ╚██╗ ██╔╝██║   ██║ ██╔██╗ ██╔══╝  ██║         
    ██║     ██║  ██║███████╗███████║██║  ██║     ╚████╔╝ ╚██████╔╝██╔╝ ██╗███████╗███████╗    
    ╚═╝     ╚═╝  ╚═╝╚══════╝╚══════╝╚═╝  ╚═╝      ╚═══╝   ╚═════╝ ╚═╝  ╚═╝╚══════╝╚══════╝    
                                                                                                 
    ███████╗███╗   ██╗ ██████╗ ███████╗███╗   ██╗███████╗                                      
    ██╔════╝████╗  ██║██╔════╝ ██╔════╝████╗  ██║██╔════╝                                      
    █████╗  ██╔██╗ ██║██║  ███╗█████╗  ██╔██╗ ██║█████╗                                        
    ██╔══╝  ██║╚██╗██║██║   ██║██╔══╝  ██║╚██╗██║██╔══╝                                        
    ███████╗██║ ╚████║╚██████╔╝███████╗██║ ╚████║███████╗                                      
    ╚══════╝╚═╝  ╚═══╝ ╚═════╝ ╚══════╝╚═╝  ╚═══╝╚══════╝                                      
```

Followed by an animated loading bar (appears character-by-character in green):

```
    ▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰
```

Then a typing effect tagline (yellow):

```
    [ A u t o m a t e d   I n s t a l l a t i o n   S y s t e m ]
```

And header information:

```
    ═══════════════════════════════════════════════════════════════════════════════
                         Automated Windows Installation Script                      
                              PowerShell Edition v2.0                               
    ═══════════════════════════════════════════════════════════════════════════════
```

### Installation Steps

Each step has a beautiful section banner:

```
    ╔═══════════════════════════════════════════════════════════════════════════╗
    ║  🔍 STEP 1/6: Checking Prerequisites                                      ║
    ╚═══════════════════════════════════════════════════════════════════════════╝

    Checking for CMake...
    ✓ CMake found: cmake version 3.20.0
    
    Checking for Visual Studio 2022...
    ✓ Visual Studio 2022 found at: C:\Program Files\Microsoft Visual Studio\2022\Community

    ✓ Prerequisites check completed!
```

```
    ╔═══════════════════════════════════════════════════════════════════════════╗
    ║  📦 STEP 2/6: Setting Up vcpkg Package Manager                            ║
    ╚═══════════════════════════════════════════════════════════════════════════╝

    ℹ️  vcpkg not found. Would you like to install it?
    vcpkg is a package manager for C++ that makes dependency management easier.

    Install vcpkg? (Y/N): y

    📥 Installing vcpkg...
    Cloning vcpkg repository...
    ⚠ This will download vcpkg. Progress will be shown below.

    [Git clone progress shown in real-time...]
```

```
    ╔═══════════════════════════════════════════════════════════════════════════╗
    ║  🔧 STEP 3/6: Configuring Dependencies                                    ║
    ╚═══════════════════════════════════════════════════════════════════════════╝

    This project uses vcpkg manifest mode (vcpkg.json).
    Dependencies will be installed automatically during CMake configuration.

    Dependencies defined in vcpkg.json:
      - GLFW3 (window management)
      - GLM (math library)
      - ImGui (editor UI)

    ✓ vcpkg is configured for manifest mode
      Dependencies will be installed in the next step during CMake configuration.
```

```
    ╔═══════════════════════════════════════════════════════════════════════════╗
    ║  ⚙️ STEP 4/6: Generating Visual Studio Project Files                      ║
    ╚═══════════════════════════════════════════════════════════════════════════╝

    NOTE: This step will automatically install dependencies from vcpkg.json
          if vcpkg is available. This may take several minutes on first run.

    Running CMake...
    Using vcpkg toolchain file (manifest mode)...
    Dependencies will be installed automatically from vcpkg.json...

    ⚠ NOTE: vcpkg will now install dependencies. This may take 10-15 minutes on first run.
    ⚠       You will see download and build progress below. Please be patient.

    [CMake output shown in real-time with all vcpkg dependency downloads and builds...]
```

```
    ╔═══════════════════════════════════════════════════════════════════════════╗
    ║  🔨 STEP 5/6: Building the Project                                        ║
    ╚═══════════════════════════════════════════════════════════════════════════╝

    This may take several minutes depending on your system...

    Building Release configuration...
    ⚠ Building... This may take several minutes. Progress will be shown below.

    [Build output shown in real-time...]

    ✓ Build completed successfully!
```

```
    ╔═══════════════════════════════════════════════════════════════════════════╗
    ║  🔗 STEP 6/6: Creating Shortcuts                                          ║
    ╚═══════════════════════════════════════════════════════════════════════════╝

    Would you like to create desktop shortcuts? (Y/N): y
    Creating shortcuts...
    ✓ Created Open_Solution.bat
    ✓ Created Run_Engine.bat
```

### Completion Banner

```
    ╔═══════════════════════════════════════════════════════════════════════════╗
    ║                                                                           ║
    ║                  ✅ INSTALLATION COMPLETED SUCCESSFULLY! ✅                  ║
    ║                                                                           ║
    ╚═══════════════════════════════════════════════════════════════════════════╝

    🎉 Installation finished: 2025-11-03 22:35:42

    ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━ NEXT STEPS ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

    📂 1. Open the solution:
       • Double-click: Open_Solution.bat
       • Or manually: build\FreshVoxelEngine.sln

    🎮 2. Run the engine:
       • Double-click: Run_Engine.bat
       • Or manually: build\Release\FreshVoxelEngine.exe

    📚 3. Learn more:
       • README.md - Project overview
       • GETTING_STARTED.md - Usage guide
       • DEVELOPER_SETUP.md - Development setup
       • docs/EDITOR_INTEGRATION.md - Editor guide

    ═══════════════════════════════════════════════════════════════════════════

    🚀 Thank you for installing Fresh Voxel Engine! 🚀

    📝 Complete installation log saved to:
       C:\Users\...\fresh\logs\install_log_2025-11-03_22-20-15.txt

    ═══════════════════════════════════════════════════════════════════════════

    Press any key to exit...
```

## Batch File Installation (tools/build_tools/install.bat)

The batch file version now also shows real-time output for all operations:

```
============================================================================
       Fresh Voxel Engine - Automated Installation
============================================================================

This script will:
  1. Check for prerequisites (CMake, Visual Studio)
  2. Install vcpkg package manager (optional)
  3. Verify vcpkg manifest mode configuration
  4. Generate Visual Studio project files
  5. Build the engine (dependencies installed during CMake)

Press any key to continue or Ctrl+C to cancel...

[Step 1/6] Checking prerequisites...
...

[Step 4/6] Generating Visual Studio project files...

NOTE: This step will automatically install dependencies from vcpkg.json
      if vcpkg is available. This may take several minutes on first run.

Running CMake...
Using vcpkg toolchain file (manifest mode)...
Dependencies will be installed automatically from vcpkg.json...

NOTE: vcpkg will now install dependencies. This may take 10-15 minutes on first run.
      You will see download and build progress below. Please be patient.

[All CMake output is now visible in real-time - no more silent operations!]
```

## Key Benefits

### PowerShell Version
- ✅ Beautiful animated ASCII art banner
- ✅ Professional section headers with emojis
- ✅ Color-coded messages (success in green, errors in red, warnings in yellow)
- ✅ Real-time progress for all operations
- ✅ Clear visual separation between steps
- ✅ Engaging user experience

### Batch Version (Improved)
- ✅ Real-time output for all operations (no more silent running)
- ✅ Clear warnings before long operations
- ✅ Immediate error visibility
- ✅ Compatible with older Windows systems
- ✅ Familiar interface for classic batch users

### Both Versions
- ✅ Comprehensive logging to timestamped files
- ✅ Clear error messages when something fails
- ✅ Interactive prompts for user choices
- ✅ Progress visibility during 10-15 minute dependency installations
- ✅ Professional completion message with next steps
