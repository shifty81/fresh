@echo off
REM Fresh Voxel Engine - Visual Studio Project Generation Script
REM This script generates Visual Studio 2022 solution files using CMake

echo ================================================
echo Fresh Voxel Engine - Visual Studio Build Setup
echo ================================================
echo.

REM Check if CMake is installed
where cmake >nul 2>nul
if %ERRORLEVEL% NEQ 0 (
    echo ERROR: CMake is not found in PATH
    echo Please install CMake from https://cmake.org/download/
    echo.
    pause
    exit /b 1
)

REM Check for Visual Studio 2022
set VS2022_FOUND=0
if exist "C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\IDE\devenv.exe" set VS2022_FOUND=1
if exist "C:\Program Files\Microsoft Visual Studio\2022\Professional\Common7\IDE\devenv.exe" set VS2022_FOUND=1
if exist "C:\Program Files\Microsoft Visual Studio\2022\Enterprise\Common7\IDE\devenv.exe" set VS2022_FOUND=1

if %VS2022_FOUND%==0 (
    echo WARNING: Visual Studio 2022 not found in default location
    echo The project will be generated anyway, but you may need to adjust paths
    echo.
)

REM Create build directory
if not exist "build" mkdir build
cd build

echo.
echo Generating Visual Studio 2022 solution...
echo.

REM Generate Visual Studio 2022 project files
cmake -G "Visual Studio 17 2022" -A x64 ..

if %ERRORLEVEL% NEQ 0 (
    echo.
    echo ERROR: CMake generation failed
    echo.
    cd ..
    pause
    exit /b 1
)

echo.
echo ================================================
echo Visual Studio solution generated successfully!
echo ================================================
echo.
echo Solution file: build\FreshVoxelEngine.sln
echo.
echo To build the project:
echo   1. Open build\FreshVoxelEngine.sln in Visual Studio 2022
echo   2. Select your configuration (Debug/Release)
echo   3. Build the solution (F7 or Ctrl+Shift+B)
echo.
echo Or build from command line:
echo   cmake --build build --config Release
echo.
pause
cd ..
