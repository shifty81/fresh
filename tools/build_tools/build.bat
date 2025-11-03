@echo off
REM ============================================================================
REM Fresh Voxel Engine - Quick Build Script
REM ============================================================================
REM This script quickly builds the project without regenerating project files
REM Useful for incremental builds during development
REM ============================================================================

setlocal

REM Navigate to repository root
cd /d "%~dp0..\.."
set "REPO_ROOT=%CD%"

echo.
echo ============================================================================
echo        Fresh Voxel Engine - Quick Build
echo ============================================================================
echo.

REM Check if build directory exists
if not exist "build" (
    echo ERROR: Build directory does not exist
    echo.
    echo Please run install.bat first to generate project files
    echo Or run: tools\build_tools\install.bat
    echo.
    pause
    exit /b 1
)

REM Prompt for build configuration
echo Select build configuration:
echo   1. Debug
echo   2. Release
echo   3. RelWithDebInfo
echo.
set /p CONFIG_CHOICE="Enter choice (1-3, default is 2): "

if "%CONFIG_CHOICE%"=="" set CONFIG_CHOICE=2

if "%CONFIG_CHOICE%"=="1" set BUILD_CONFIG=Debug
if "%CONFIG_CHOICE%"=="2" set BUILD_CONFIG=Release
if "%CONFIG_CHOICE%"=="3" set BUILD_CONFIG=RelWithDebInfo

if not defined BUILD_CONFIG (
    echo Invalid choice. Using Release.
    set BUILD_CONFIG=Release
)

echo.
echo Building %BUILD_CONFIG% configuration...
echo.

REM Build the project
cmake --build build --config %BUILD_CONFIG%

if %ERRORLEVEL% NEQ 0 (
    echo.
    echo ERROR: Build failed
    echo.
    pause
    exit /b 1
)

echo.
echo ============================================================================
echo Build completed successfully!
echo ============================================================================
echo.
echo Configuration: %BUILD_CONFIG%
echo Executable: build\%BUILD_CONFIG%\FreshVoxelEngine.exe
echo.

REM Ask if user wants to run the engine
set /p RUN_ENGINE="Would you like to run the engine now? (Y/N): "
if /i "%RUN_ENGINE%"=="Y" (
    echo.
    echo Starting Fresh Voxel Engine...
    start "" "build\%BUILD_CONFIG%\FreshVoxelEngine.exe"
)

pause
