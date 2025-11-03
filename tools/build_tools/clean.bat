@echo off
REM ============================================================================
REM Fresh Voxel Engine - Clean Build Script
REM ============================================================================
REM This script cleans all build artifacts and generated files
REM ============================================================================

setlocal

REM Navigate to repository root
cd /d "%~dp0..\.."
set "REPO_ROOT=%CD%"

echo.
echo ============================================================================
echo        Fresh Voxel Engine - Clean Build Artifacts
echo ============================================================================
echo.
echo This will delete:
echo   - build/ directory (Visual Studio project files and binaries)
echo   - Generated shortcuts (Open_Solution.bat, Run_Engine.bat)
echo.
set /p CONFIRM="Are you sure you want to continue? (Y/N): "

if /i "%CONFIRM%" NEQ "Y" (
    echo Cancelled.
    pause
    exit /b 0
)

echo.
echo Cleaning build artifacts...
echo.

REM Remove build directory
if exist "build" (
    echo Deleting build directory...
    rmdir /s /q "build"
    echo   [OK] build/ removed
) else (
    echo   [SKIP] build/ does not exist
)

REM Remove generated shortcuts
if exist "Open_Solution.bat" (
    echo Deleting Open_Solution.bat...
    del /q "Open_Solution.bat"
    echo   [OK] Open_Solution.bat removed
)

if exist "Run_Engine.bat" (
    echo Deleting Run_Engine.bat...
    del /q "Run_Engine.bat"
    echo   [OK] Run_Engine.bat removed
)

echo.
echo ============================================================================
echo Clean completed successfully!
echo ============================================================================
echo.
echo To rebuild the project, run: tools\build_tools\install.bat
echo.
pause
