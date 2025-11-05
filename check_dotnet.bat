@echo off
setlocal enabledelayedexpansion
REM Fresh Voxel Engine - .NET 9 SDK Check and Build Script

echo ================================================
echo Fresh Voxel Engine - .NET 9 SDK Check
echo ================================================
echo/

REM Check if dotnet is installed
where dotnet >nul 2>nul
if %ERRORLEVEL% NEQ 0 (
    echo ERROR: .NET SDK is not found in PATH
    echo/
    echo .NET 9 is the primary build method for Fresh Voxel Engine.
    echo/
    echo Please install .NET 9 SDK from:
    echo https://dotnet.microsoft.com/download/dotnet/9.0
    echo/
    echo After installation:
    echo   1. Restart your command prompt
    echo   2. Run this installer again
    echo/
    exit /b 1
)

echo [OK] .NET SDK found!
echo/

REM Check .NET version
echo Checking .NET SDK version...
dotnet --version
echo/

REM Check if .NET 9 is available
dotnet --list-sdks | findstr "^9\." >nul 2>nul
if %ERRORLEVEL% NEQ 0 (
    echo WARNING: .NET 9 SDK is not installed
    echo/
    echo Installed SDKs:
    dotnet --list-sdks
    echo/
    echo .NET 9 is required for the primary build method.
    echo/
    echo Please install .NET 9 SDK from:
    echo https://dotnet.microsoft.com/download/dotnet/9.0
    echo/
    exit /b 1
)

echo [OK] .NET 9 SDK is installed!
echo/
echo Building .NET managed wrapper...
echo/
cd /d "%~dp0dotnet"
if !ERRORLEVEL! NEQ 0 (
    echo ERROR: Failed to navigate to dotnet directory
    exit /b 1
)

dotnet build -c Release

if !ERRORLEVEL! NEQ 0 (
    echo/
    echo ERROR: .NET build failed
    echo/
    echo Please check:
    echo   1. .NET 9 SDK is properly installed
    echo   2. FreshEngine.Managed.csproj is valid
    echo   3. All dependencies are available
    echo/
    exit /b 1
)

cd /d "%~dp0"

echo/
echo ================================================
echo .NET 9 SDK check completed successfully!
echo ================================================
echo/
echo [OK] Managed wrapper built: dotnet\bin\Release\net9.0-windows\FreshEngine.Managed.dll
echo/
echo The .NET bindings are ready. You can now:
echo   1. Use the managed wrapper in C# applications
echo   2. Continue with native engine build
echo/
exit /b 0
