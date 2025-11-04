@echo off
REM Fresh Voxel Engine - .NET 9 SDK Check Script

echo ================================================
echo Fresh Voxel Engine - .NET SDK Check
echo ================================================
echo.

REM Check if dotnet is installed
where dotnet >nul 2>nul
if %ERRORLEVEL% NEQ 0 (
    echo ERROR: .NET SDK is not found in PATH
    echo.
    echo Please install .NET 9 SDK from:
    echo https://dotnet.microsoft.com/download/dotnet/9.0
    echo.
    pause
    exit /b 1
)

echo .NET SDK found!
echo.

REM Check .NET version
echo Checking .NET SDK version...
dotnet --version
echo.

REM Check if .NET 9 is available
dotnet --list-sdks | findstr "^9\." >nul 2>nul
if %ERRORLEVEL% NEQ 0 (
    echo WARNING: .NET 9 SDK is not installed
    echo.
    echo Installed SDKs:
    dotnet --list-sdks
    echo.
    echo Please install .NET 9 SDK from:
    echo https://dotnet.microsoft.com/download/dotnet/9.0
    echo.
    pause
    exit /b 1
)

echo .NET 9 SDK is installed!
echo.
echo Building .NET managed wrapper...
cd dotnet
dotnet build -c Release

if %ERRORLEVEL% NEQ 0 (
    echo.
    echo ERROR: .NET build failed
    echo.
    pause
    exit /b 1
)

echo.
echo ================================================
echo .NET 9 SDK check completed successfully!
echo ================================================
echo.
echo The managed wrapper has been built successfully.
echo You can now build the main project with Visual Studio 2022.
echo.
pause
