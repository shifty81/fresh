@echo off
REM ============================================================================
REM Fresh Voxel Engine - Installation Launcher
REM ============================================================================
REM This script runs the complete automated installation process
REM ============================================================================

echo.
echo ============================================================================
echo        Fresh Voxel Engine - Installation Launcher
echo ============================================================================
echo.
echo This installer will:
echo   1. Check prerequisites (CMake, Visual Studio 2022)
echo   2. Install vcpkg package manager (if needed)
echo   3. Install dependencies (GLFW, GLM, ImGui)
echo   4. Generate Visual Studio 2022 solution
echo   5. Build the engine
echo   6. Create convenient shortcuts
echo.
echo Estimated time: 10-20 minutes (depending on your system and internet speed)
echo.
echo Starting installation...
echo.
pause

call "%~dp0tools\build_tools\install.bat"

if %ERRORLEVEL% EQU 0 (
    echo.
    echo ============================================================================
    echo                    Installation Complete!
    echo ============================================================================
    echo.
    echo NEXT STEPS:
    echo.
    echo   1. Open Visual Studio solution:
    if exist "%~dp0Open_Solution.bat" (
        echo      - Double-click: Open_Solution.bat
        echo      - Or run this installer with option below
    ) else (
        echo      - Open: build\FreshVoxelEngine.sln
    )
    echo.
    echo   2. Set FreshVoxelEngine as startup project:
    echo      - Right-click "FreshVoxelEngine" in Solution Explorer
    echo      - Select "Set as Startup Project"
    echo.
    echo   3. Build and run:
    echo      - Press F5 in Visual Studio
    if exist "%~dp0Run_Engine.bat" (
        echo      - Or double-click: Run_Engine.bat
    )
    echo.
    echo DOCUMENTATION:
    echo   - Quick Start: QUICKSTART_VISUAL.md
    echo   - Find Docs: DOCUMENTATION_INDEX.md
    echo   - Troubleshooting: FIXING_ALL_BUILD_ERROR.md
    echo.
    echo LOGS:
    echo   - Check logs\ directory for detailed installation output
    echo.
    
    if exist "%~dp0Open_Solution.bat" (
        echo.
        set /p OPEN_VS="Would you like to open Visual Studio now? (Y/N): "
        if /i "!OPEN_VS!"=="Y" (
            echo Opening Visual Studio...
            start "" "%~dp0build\FreshVoxelEngine.sln"
        )
    )
) else (
    echo.
    echo ============================================================================
    echo                    Installation Failed
    echo ============================================================================
    echo.
    echo The installation encountered errors.
    echo.
    echo TROUBLESHOOTING:
    echo   1. Check the log file in the logs\ directory
    echo   2. Verify all prerequisites are installed:
    echo      - CMake 3.20+
    echo      - Visual Studio 2022 with C++ workload
    echo      - Git (for vcpkg)
    echo   3. See BATCH_INSTALLER_GUIDE.md for common issues
    echo   4. See DEVELOPER_SETUP.md for manual installation
    echo.
)

echo.
pause
