@echo off
setlocal enabledelayedexpansion
REM ============================================================================
REM Fresh Voxel Engine - Installation Launcher
REM ============================================================================
REM This script runs the complete automated installation process
REM ============================================================================

echo/
echo ============================================================================
echo        Fresh Voxel Engine - Installation Launcher
echo ============================================================================
echo/
echo This installer will:
echo   1. Check prerequisites (.NET 9 SDK, CMake, Visual Studio 2022)
echo   2. Build .NET 9 managed wrapper (primary method)
echo   3. Install vcpkg package manager in parent directory (shared, if needed)
echo   4. Install dependencies (GLFW, GLM, ImGui) automatically via vcpkg.json
echo   5. Generate Visual Studio 2022 solution
echo   6. Build the native engine
echo   7. Create convenient shortcuts
echo/
echo NOTE: vcpkg will be installed in the parent directory to allow
echo       sharing across multiple projects, saving disk space and time.
echo/
echo Estimated time: 10-20 minutes (depending on your system and internet speed)
echo/
echo Starting installation...
echo/
pause

REM Check for .NET 9 SDK first as it's the primary build method
echo Checking for .NET 9 SDK...
echo/
call "%~dp0check_dotnet.bat"
if %ERRORLEVEL% NEQ 0 (
    set DOTNET_CHECK_RESULT=1
) else (
    set DOTNET_CHECK_RESULT=0
)

if !DOTNET_CHECK_RESULT! NEQ 0 (
    echo/
    echo ============================================================================
    echo                    .NET 9 SDK Not Found
    echo ============================================================================
    echo/
    echo .NET 9 is the recommended way to build Fresh Voxel Engine.
    echo/
    echo Please install .NET 9 SDK from:
    echo   https://dotnet.microsoft.com/download/dotnet/9.0
    echo/
    echo After installing .NET 9, run this installer again.
    echo/
    echo Alternatively, you can continue with the CMake-only installation,
    echo but you won't have access to C# scripting and managed features.
    echo/
    set /p CONTINUE_WITHOUT_DOTNET="Continue without .NET 9? (Y/N): "
    if /i "!CONTINUE_WITHOUT_DOTNET!" NEQ "Y" (
        echo Installation cancelled.
        pause
        exit /b 1
    )
)

echo/
echo Proceeding with complete installation (native C++ engine)...
echo/

call "%~dp0tools\build_tools\install.bat"

if !ERRORLEVEL! EQU 0 (
    echo/
    echo ============================================================================
    echo                    Installation Complete!
    echo ============================================================================
    echo/
    echo NEXT STEPS:
    echo/
    echo   1. Open Visual Studio solution:
    if exist "%~dp0Open_Solution.bat" (
        echo      - Double-click: Open_Solution.bat
        echo      - Or run this installer with option below
    ) else (
        echo      - Open: build\FreshVoxelEngine.sln
    )
    echo/
    echo   2. Set FreshVoxelEngine as startup project:
    echo      - Right-click "FreshVoxelEngine" in Solution Explorer
    echo      - Select "Set as Startup Project"
    echo/
    echo   3. Build and run:
    echo      - Press F5 in Visual Studio
    if exist "%~dp0Run_Engine.bat" (
        echo      - Or double-click: Run_Engine.bat
    )
    echo/
    echo DOCUMENTATION:
    echo   - Quick Start: QUICKSTART_VISUAL.md
    echo   - Find Docs: DOCUMENTATION_INDEX.md
    echo   - Troubleshooting: FIXING_ALL_BUILD_ERROR.md
    echo/
    echo LOGS:
    echo   - Check logs\ directory for detailed installation output
    echo/
    
    if exist "%~dp0Open_Solution.bat" (
        echo/
        set /p OPEN_VS="Would you like to open Visual Studio now? (Y/N): "
        if /i "!OPEN_VS!"=="Y" (
            echo Opening Visual Studio...
            start "" "%~dp0build\FreshVoxelEngine.sln"
        )
    )
) else (
    echo/
    echo ============================================================================
    echo                    Installation Failed
    echo ============================================================================
    echo/
    echo The installation encountered errors.
    echo/
    echo TROUBLESHOOTING:
    echo   1. Check the log file in the logs\ directory
    echo   2. Verify all prerequisites are installed:
    echo      - CMake 3.20+
    echo      - Visual Studio 2022 with C++ workload
    echo      - Git (for vcpkg)
    echo   3. See BATCH_INSTALLER_GUIDE.md for common issues
    echo   4. See DEVELOPER_SETUP.md for manual installation
    echo/
)

echo/
pause
