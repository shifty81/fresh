@echo off
REM Fresh Voxel Engine - Complete Build Script
REM Builds both native C++ engine and .NET 9 managed wrapper

echo ================================================
echo Fresh Voxel Engine - Complete Build
echo ================================================
echo/

echo Step 1: Checking prerequisites...
echo/

REM Check CMake
where cmake >nul 2>nul
if %ERRORLEVEL% NEQ 0 (
    echo ERROR: CMake is not found in PATH
    echo Please install CMake from https://cmake.org/download/
    pause
    exit /b 1
)
echo [OK] CMake found

REM Check .NET SDK
where dotnet >nul 2>nul
if %ERRORLEVEL% NEQ 0 (
    echo WARNING: .NET SDK is not found in PATH
    echo .NET bindings will not be built
    echo Install from: https://dotnet.microsoft.com/download/dotnet/9.0
    set BUILD_DOTNET=0
) else (
    echo [OK] .NET SDK found
    set BUILD_DOTNET=1
)

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
echo Step 2: Generating Visual Studio 2022 solution...
echo/

call generate_vs2022.bat
if %ERRORLEVEL% NEQ 0 (
    echo ERROR: Failed to generate Visual Studio solution
    pause
    exit /b 1
)

echo/
echo Step 3: Building native C++ engine...
echo/

cmake --build build --config Release
if %ERRORLEVEL% NEQ 0 (
    echo ERROR: Failed to build native engine
    pause
    exit /b 1
)

if %BUILD_DOTNET%==1 (
    echo/
    echo Step 4: Building .NET managed wrapper...
    echo/
    
    cd dotnet
    dotnet build -c Release
    if %ERRORLEVEL% NEQ 0 (
        echo ERROR: Failed to build .NET wrapper
        cd ..
        pause
        exit /b 1
    )
    cd ..
    
    echo/
    echo ================================================
    echo Build completed successfully!
    echo ================================================
    echo/
    echo Native engine: build\Release\FreshVoxelEngine.exe
    echo .NET wrapper: dotnet\bin\Release\net9.0-windows\FreshEngine.Managed.dll
) else (
    echo/
    echo ================================================
    echo Build completed successfully!
    echo ================================================
    echo/
    echo Native engine: build\Release\FreshVoxelEngine.exe
    echo/
    echo Note: .NET wrapper was not built (SDK not found)
)

echo/
pause
