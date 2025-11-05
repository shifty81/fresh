@echo off
setlocal enabledelayedexpansion
REM Fresh Voxel Engine - Complete Build Script
REM Builds both .NET 9 managed wrapper (primary) and native C++ engine

echo ================================================
echo Fresh Voxel Engine - Complete Build
echo ================================================
echo/
echo This script builds the engine using the recommended
echo .NET-first approach:
echo   1. .NET 9 managed wrapper (C# bindings)
echo   2. Native C++ engine (backend)
echo/

echo Step 1: Checking prerequisites...
echo/

REM Check .NET SDK first (primary build method)
where dotnet >nul 2>nul
if %ERRORLEVEL% NEQ 0 (
    echo ERROR: .NET SDK is not found in PATH
    echo/
    echo .NET 9 is the primary build method for Fresh Voxel Engine.
    echo Install from: https://dotnet.microsoft.com/download/dotnet/9.0
    echo/
    pause
    exit /b 1
)
echo [OK] .NET SDK found

REM Check if .NET 9 is available
dotnet --list-sdks | findstr "^9\." >nul 2>nul
if %ERRORLEVEL% NEQ 0 (
    echo ERROR: .NET 9 SDK is not installed
    echo/
    echo Installed SDKs:
    dotnet --list-sdks
    echo/
    echo Install .NET 9 SDK from:
    echo https://dotnet.microsoft.com/download/dotnet/9.0
    echo/
    pause
    exit /b 1
)
echo [OK] .NET 9 SDK found

REM Check CMake
where cmake >nul 2>nul
if %ERRORLEVEL% NEQ 0 (
    echo ERROR: CMake is not found in PATH
    echo Please install CMake from https://cmake.org/download/
    pause
    exit /b 1
)
echo [OK] CMake found

REM Check Visual Studio 2022
set VS2022_FOUND=0
if exist "C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\IDE\devenv.exe" set VS2022_FOUND=1
if exist "C:\Program Files\Microsoft Visual Studio\2022\Professional\Common7\IDE\devenv.exe" set VS2022_FOUND=1
if exist "C:\Program Files\Microsoft Visual Studio\2022\Enterprise\Common7\IDE\devenv.exe" set VS2022_FOUND=1

if %VS2022_FOUND%==1 (
    echo [OK] Visual Studio 2022 found
) else (
    echo WARNING: Visual Studio 2022 not found in default location
)

echo/
echo Step 2: Building .NET 9 managed wrapper...
echo/

if not exist "dotnet" (
    echo ERROR: dotnet directory not found
    pause
    exit /b 1
)
cd /d dotnet
dotnet build -c Release
set BUILD_RESULT=!ERRORLEVEL!
cd /d ..
if !BUILD_RESULT! NEQ 0 (
    echo ERROR: Failed to build .NET wrapper
    pause
    exit /b 1
)

echo/
echo [OK] .NET managed wrapper built successfully!
echo      Output: dotnet\bin\Release\net9.0-windows\FreshEngine.Managed.dll
echo/

echo Step 3: Generating Visual Studio 2022 solution...
echo/

call generate_vs2022.bat
if !ERRORLEVEL! NEQ 0 (
    echo ERROR: Failed to generate Visual Studio solution
    pause
    exit /b 1
)

echo/
echo Step 4: Building native C++ engine...
echo/

cmake --build build --config Release
if !ERRORLEVEL! NEQ 0 (
    echo ERROR: Failed to build native engine
    pause
    exit /b 1
)

echo/
echo ================================================
echo Build completed successfully!
echo ================================================
echo/
echo .NET wrapper:    dotnet\bin\Release\net9.0-windows\FreshEngine.Managed.dll
echo Native engine:   build\Release\FreshVoxelEngine.exe
echo/
echo You can now:
echo   1. Use the managed wrapper in C# applications
echo   2. Run the native engine: build\Release\FreshVoxelEngine.exe
echo   3. Open Visual Studio: build\FreshVoxelEngine.sln
echo/
pause
