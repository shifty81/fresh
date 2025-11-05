@echo off
REM Fresh Voxel Engine - Visual Studio 2022 Solution Generator
REM This script generates the Visual Studio 2022 solution using CMake
REM For detailed build instructions, see BUILD.md

echo ================================================
echo Fresh Voxel Engine - Visual Studio 2022 Setup
echo ================================================
echo/

REM Check if CMake is installed
where cmake >nul 2>nul
if %ERRORLEVEL% NEQ 0 (
    echo ERROR: CMake is not found in PATH
    echo/
    echo Please install CMake 3.20 or higher from:
    echo https://cmake.org/download/
    echo/
    echo Make sure to add CMake to system PATH during installation.
    pause
    exit /b 1
)

echo [OK] CMake found
echo/

REM Create build directory
if not exist "build" mkdir build
cd build

REM Check for vcpkg (project directory first, then parent directory)
set "VCPKG_ROOT="
set "VCPKG_LOCAL=%~dp0vcpkg"
set "VCPKG_PARENT=%~dp0..\vcpkg"

if exist "%VCPKG_LOCAL%\scripts\buildsystems\vcpkg.cmake" (
    set "VCPKG_ROOT=%VCPKG_LOCAL%"
    echo [OK] Found vcpkg in project directory
) else if exist "%VCPKG_PARENT%\scripts\buildsystems\vcpkg.cmake" (
    set "VCPKG_ROOT=%VCPKG_PARENT%"
    echo [OK] Found vcpkg in parent directory
) else (
    echo [WARNING] vcpkg not found
    echo/
    echo vcpkg is required for automatic dependency management.
    echo/
    echo To set up vcpkg, run these commands from the project root:
    echo   git clone https://github.com/microsoft/vcpkg.git
    echo   cd vcpkg
    echo   bootstrap-vcpkg.bat
    echo   cd ..
    echo   generate_vs2022.bat
    echo/
    echo Or see BUILD.md for detailed setup instructions.
    echo/
    pause
    cd ..
    exit /b 1
)

echo/
echo Generating Visual Studio 2022 solution...
echo Configuration: Windows x64 with DirectX 11/12
echo/
echo Dependencies will be automatically installed via vcpkg.
echo This may take 5-15 minutes on first run.
echo/

REM Generate Visual Studio 2022 solution with vcpkg
cmake -G "Visual Studio 17 2022" -A x64 -DCMAKE_TOOLCHAIN_FILE="%VCPKG_ROOT%\scripts\buildsystems\vcpkg.cmake" ..

if %ERRORLEVEL% NEQ 0 (
    echo/
    echo [ERROR] CMake generation failed
    echo/
    echo Please check the error messages above.
    echo See BUILD.md for troubleshooting help.
    echo/
    cd ..
    pause
    exit /b 1
)

echo/
echo ================================================
echo SUCCESS! Visual Studio solution generated
echo ================================================
echo/
echo Solution: build\FreshVoxelEngine.sln
echo/
echo Next steps:
echo   1. Open build\FreshVoxelEngine.sln in Visual Studio 2022
echo   2. Right-click "FreshVoxelEngine" project, select "Set as Startup Project"
echo   3. Press F7 to build or F5 to build and run
echo/
echo Or build from command line:
echo   cmake --build build --config Release
echo/
echo For detailed instructions, see BUILD.md
echo/
pause
cd ..
