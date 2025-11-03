# Fresh Voxel Engine - Build Tools

This directory contains automated build and installation tools for the Fresh Voxel Engine.

## Installation Script

### `install.bat` - Automated Windows Installation

A comprehensive, one-click installation script that automates the entire setup process for Windows users.

#### Features

- ✅ **Prerequisites Check** - Verifies CMake and Visual Studio 2022 installation
- ✅ **Package Manager Setup** - Optionally installs vcpkg for dependency management
- ✅ **Dependency Installation** - Automatically installs GLFW and GLM
- ✅ **Project Generation** - Creates Visual Studio 2022 solution files
- ✅ **Automated Build** - Compiles the engine in Release mode
- ✅ **Shortcut Creation** - Creates convenient shortcuts for running the engine
- ✅ **Interactive** - Provides clear prompts and feedback throughout the process

#### How to Use

1. **Double-click** `install.bat` to start the installation
2. Follow the on-screen prompts
3. Wait for the installation to complete
4. Use the generated shortcuts to run the engine

#### Requirements

Before running the installation script, ensure you have:

- **Windows 10 or later**
- **Internet connection** (for downloading dependencies)
- **Git** (for cloning vcpkg, if needed)
- **CMake 3.20+** - Download from [cmake.org](https://cmake.org/download/)
- **Visual Studio 2022** (Community, Professional, or Enterprise)
  - With "Desktop development with C++" workload
  - Download from [visualstudio.microsoft.com](https://visualstudio.microsoft.com/downloads/)

#### What the Script Does

##### Step 1: Prerequisites Check
- Verifies CMake is installed and accessible
- Checks for Visual Studio 2022 installation
- Reports versions and paths

##### Step 2: vcpkg Setup (Optional)
- Offers to install vcpkg package manager
- Clones vcpkg repository if not present
- Bootstraps vcpkg and integrates with Visual Studio
- Can be skipped if you prefer manual dependency management

##### Step 3: Install Dependencies
- Installs GLFW (window management library)
- Installs GLM (mathematics library)
- Uses vcpkg if available, otherwise expects manual installation

##### Step 4: Generate Project Files
- Creates `build` directory
- Runs CMake to generate Visual Studio 2022 solution
- Configures project with appropriate settings

##### Step 5: Build the Project
- Builds the engine in Release configuration
- May take several minutes depending on system performance
- Outputs to `build/Release/FreshVoxelEngine.exe`

##### Step 6: Create Shortcuts
- Optionally creates convenient batch file shortcuts:
  - `Open_Solution.bat` - Opens the Visual Studio solution
  - `Run_Engine.bat` - Launches the compiled engine

#### Installation Time

Expected installation time varies by system:
- **Fast System (SSD, 8+ cores)**: 10-15 minutes
- **Average System**: 15-25 minutes
- **Slower System**: 25-40 minutes

Most time is spent downloading and compiling dependencies.

#### Troubleshooting

##### CMake Not Found
**Error**: "CMake is not installed or not in PATH"

**Solution**:
1. Download and install CMake from [cmake.org](https://cmake.org/download/)
2. During installation, select "Add CMake to system PATH"
3. Restart the command prompt and try again

##### Visual Studio Not Found
**Error**: "Visual Studio 2022 not found"

**Solution**:
1. Download Visual Studio 2022 from [visualstudio.microsoft.com](https://visualstudio.microsoft.com/downloads/)
2. Install with "Desktop development with C++" workload
3. Restart the installation script

##### Git Not Found
**Error**: Failed to clone vcpkg repository

**Solution**:
1. Install Git from [git-scm.com](https://git-scm.com/downloads)
2. Add Git to system PATH
3. Restart the installation script

##### Build Fails
**Error**: "Build failed"

**Solution**:
1. Check the error messages for specific issues
2. Ensure all prerequisites are properly installed
3. Try opening `build\FreshVoxelEngine.sln` in Visual Studio
4. Build manually to see detailed error messages
5. Check [DEVELOPER_SETUP.md](../../DEVELOPER_SETUP.md) for manual build instructions

##### Network Issues
**Error**: Cannot download vcpkg or dependencies

**Solution**:
1. Check your internet connection
2. Verify firewall/proxy settings
3. Try again after network issues are resolved
4. Consider manual dependency installation (see below)

#### Manual Installation Alternative

If the automated script doesn't work for you, follow the manual installation guide:

1. **Manual Setup**: See [DEVELOPER_SETUP.md](../../DEVELOPER_SETUP.md)
2. **Quick Setup**: See [GETTING_STARTED.md](../../GETTING_STARTED.md)
3. **Visual Studio Specific**: See [VISUAL_STUDIO_SETUP.md](../../VISUAL_STUDIO_SETUP.md)

#### Advanced Options

##### Custom vcpkg Location
If you already have vcpkg installed elsewhere:
1. Edit `install.bat`
2. Change the `VCPKG_ROOT` variable to your vcpkg path
3. Run the script

##### Custom Build Configuration
To build in Debug mode instead of Release:
1. Edit `install.bat`
2. Change `--config Release` to `--config Debug` in the build step
3. Run the script

##### Skip Steps
You can modify the script to skip certain steps:
- Comment out Step 2 to skip vcpkg installation
- Comment out Step 5 to skip building (generate project only)
- Comment out Step 6 to skip shortcut creation

#### Output

After successful installation, you'll have:
- `build/` - Build directory with Visual Studio solution
- `build/FreshVoxelEngine.sln` - Visual Studio solution file
- `build/Release/FreshVoxelEngine.exe` - Compiled engine executable
- `vcpkg/` - vcpkg package manager (if installed)
- `Open_Solution.bat` - Shortcut to open VS solution (if created)
- `Run_Engine.bat` - Shortcut to run engine (if created)

#### Script Maintenance

This script is maintained as part of the Fresh Voxel Engine project. 

If you encounter issues:
1. Check the [Issues](https://github.com/shifty81/fresh/issues) page
2. Create a new issue with the error details
3. Include the output from the installation script

#### Contributing

Improvements to the installation script are welcome! Please submit pull requests with:
- Clear description of the change
- Justification for the improvement
- Testing on different Windows configurations

## Additional Tools

Future tools planned for this directory:
- `package.bat` - Create distribution package
- `update_deps.bat` - Update dependencies

## Support

For help and support:
- 📖 [Documentation](../../docs/)
- 💬 [GitHub Discussions](https://github.com/shifty81/fresh/discussions)
- 🐛 [Report Issues](https://github.com/shifty81/fresh/issues)

---

**Fresh Voxel Engine** - Build tools to simplify your development experience.
