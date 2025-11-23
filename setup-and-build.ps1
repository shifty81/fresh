<#
.SYNOPSIS
    Automated build script for Fresh Voxel Engine
    
.DESCRIPTION
    This script automates the entire build process for Fresh Voxel Engine:
    1. Checks prerequisites (Visual Studio 2022, CMake, Git)
    2. Sets up vcpkg if not present
    3. Generates Visual Studio 2022 solution
    4. Builds the project
    5. Opens Visual Studio solution for development
    
.PARAMETER SkipBuild
    Skip the actual build step and just generate the solution
    
.PARAMETER BuildConfig
    Build configuration (Debug or Release). Default is Release.
    
.PARAMETER SkipVcpkgBootstrap
    Skip vcpkg bootstrap if it already exists
    
.PARAMETER OpenVS
    Open Visual Studio after successful build. Default is true.
    
.EXAMPLE
    .\setup-and-build.ps1
    Runs full automated setup and build process
    
.EXAMPLE
    .\setup-and-build.ps1 -SkipBuild
    Sets up and generates solution without building
    
.EXAMPLE
    .\setup-and-build.ps1 -BuildConfig Debug
    Builds Debug configuration instead of Release
#>

param(
    [switch]$SkipBuild = $false,
    [ValidateSet("Debug", "Release")]
    [string]$BuildConfig = "Release",
    [switch]$SkipVcpkgBootstrap = $false,
    [bool]$OpenVS = $true
)

# Set error action preference
$ErrorActionPreference = "Stop"

# CMake version configuration
$CMAKE_MIN_MAJOR = 3
$CMAKE_MIN_MINOR = 20
$CMAKE_RECOMMENDED_VERSION = "3.30.x or 3.31.x"
$CMAKE_TESTED_RANGE = "3.20 through 3.31"

# Get script directory (project root) early for absolute paths
$ScriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path

# Setup logging with absolute paths
$LogDir = Join-Path $ScriptDir "logs"
if (-not (Test-Path $LogDir)) {
    New-Item -ItemType Directory -Path $LogDir | Out-Null
}
$Timestamp = Get-Date -Format "yyyyMMdd_HHmmss"
$LogFile = Join-Path $LogDir "setup-and-build_$Timestamp.log"

# Logging function
function Write-Log {
    param([string]$Message, [string]$Level = "INFO")
    $timestamp = Get-Date -Format "yyyy-MM-dd HH:mm:ss"
    $logEntry = "[$timestamp] [$Level] $Message"
    Add-Content -Path $LogFile -Value $logEntry
}

# Color output functions with logging
function Write-Header {
    param([string]$Message)
    $separator = "============================================================"
    Write-Host ""
    Write-Host $separator -ForegroundColor Cyan
    Write-Host " $Message" -ForegroundColor Cyan
    Write-Host $separator -ForegroundColor Cyan
    Write-Host ""
    Write-Log "" "HEADER"
    Write-Log $separator "HEADER"
    Write-Log " $Message" "HEADER"
    Write-Log $separator "HEADER"
    Write-Log "" "HEADER"
}

function Write-Success {
    param([string]$Message)
    Write-Host "[OK] $Message" -ForegroundColor Green
    Write-Log $Message "SUCCESS"
}

function Write-Info {
    param([string]$Message)
    Write-Host "[INFO] $Message" -ForegroundColor Yellow
    Write-Log $Message "INFO"
}

function Write-ErrorMsg {
    param([string]$Message)
    Write-Host "[ERROR] $Message" -ForegroundColor Red
    Write-Log $Message "ERROR"
}

function Write-Step {
    param([string]$Message)
    Write-Host ""
    Write-Host ">> $Message" -ForegroundColor Magenta
    Write-Log "" "STEP"
    Write-Log ">> $Message" "STEP"
}

# Main script
try {
    Write-Header "Fresh Voxel Engine - Automated Build Script"
    
    # Change to script directory (project root) - $ScriptDir already set at top
    Set-Location $ScriptDir
    Write-Info "Project directory: $ScriptDir"
    Write-Info "Log file: $LogFile"
    Write-Log "Script started from directory: $ScriptDir" "INFO"
    Write-Log "Build configuration: $BuildConfig" "INFO"
    Write-Log "Skip build: $SkipBuild" "INFO"
    Write-Log "Open Visual Studio: $OpenVS" "INFO"
    
    # ============================================================================
    # Step 1: Check Prerequisites
    # ============================================================================
    Write-Header "Step 1: Checking Prerequisites"
    
    # Check Git
    Write-Step "Checking for Git..."
    try {
        $gitVersion = & git --version 2>&1
        Write-Success "Git found: $gitVersion"
    }
    catch {
        Write-ErrorMsg "Git is not installed or not in PATH"
        Write-Host "Please install Git from: https://git-scm.com/download/win"
        exit 1
    }
    
    # Check CMake
    Write-Step "Checking for CMake..."
    try {
        $cmakeVersion = & cmake --version 2>&1 | Select-Object -First 1
        Write-Success "CMake found: $cmakeVersion"
        
        # Verify CMake version is 3.20 or higher
        # Full version match to capture major, minor, patch, and any suffix (rc, alpha, beta)
        $versionMatch = $cmakeVersion -match "cmake version (\d+)\.(\d+)\.(\d+)(-rc\d*|-alpha\d*|-beta\d*|-pre|-dev)?"
        if ($versionMatch) {
            $major = [int]$matches[1]
            $minor = [int]$matches[2]
            $patch = [int]$matches[3]
            $suffix = $matches[4]
            
            # Check for minimum version
            if ($major -lt $CMAKE_MIN_MAJOR -or ($major -eq $CMAKE_MIN_MAJOR -and $minor -lt $CMAKE_MIN_MINOR)) {
                Write-ErrorMsg "CMake version $CMAKE_MIN_MAJOR.$CMAKE_MIN_MINOR or higher is required"
                Write-Host "Current version: $major.$minor.$patch"
                Write-Host "Please update CMake from: https://cmake.org/download/"
                exit 1
            }
            
            # Warn about unstable versions (rc, alpha, beta, pre, dev)
            if (-not [string]::IsNullOrEmpty($suffix)) {
                Write-Host ""
                Write-Host "WARNING: You are using an unstable CMake version!" -ForegroundColor Red
                Write-Host "  Current version: $major.$minor.$patch$suffix" -ForegroundColor Yellow
                Write-Host "  This is a pre-release version and may have bugs or breaking changes." -ForegroundColor Yellow
                Write-Host ""
                Write-Host "RECOMMENDATION: Use the latest stable CMake 3.x release" -ForegroundColor Cyan
                Write-Host "  Download from: https://cmake.org/download/" -ForegroundColor White
                Write-Host "  Recommended: CMake $CMAKE_RECOMMENDED_VERSION" -ForegroundColor White
                Write-Host ""
                Write-Log "WARNING: Unstable CMake version detected: $major.$minor.$patch$suffix" "WARNING"
                
                # Ask user if they want to continue
                Write-Host "Do you want to continue anyway? (y/n): " -NoNewline -ForegroundColor Yellow
                $response = Read-Host
                if ($response -ne "y" -and $response -ne "Y") {
                    Write-Host ""
                    Write-Host "Build cancelled. Please install a stable CMake version and try again." -ForegroundColor Yellow
                    Write-Log "Build cancelled due to unstable CMake version" "INFO"
                    exit 0
                }
                Write-Host ""
                Write-Info "Continuing with unstable CMake version at user's request..."
                Write-Log "User chose to continue with unstable CMake version" "INFO"
            }
            
            # Warn about CMake 4.x and higher (major version changes)
            if ($major -ge 4) {
                Write-Host ""
                Write-Host "WARNING: CMake 4.x or higher detected!" -ForegroundColor Red
                Write-Host "  Current version: $major.$minor.$patch$suffix" -ForegroundColor Yellow
                Write-Host "  CMake 4.x introduces major changes and may not be compatible." -ForegroundColor Yellow
                Write-Host ""
                Write-Host "STRONG RECOMMENDATION: Use CMake $CMAKE_RECOMMENDED_VERSION instead" -ForegroundColor Cyan
                Write-Host "  This project has been tested with CMake $CMAKE_TESTED_RANGE" -ForegroundColor White
                Write-Host "  Download stable version from: https://cmake.org/download/" -ForegroundColor White
                Write-Host ""
                Write-Log "WARNING: CMake $major.x detected - may have compatibility issues" "WARNING"
                
                # Ask user if they want to continue
                Write-Host "Do you want to continue anyway? (y/n): " -NoNewline -ForegroundColor Yellow
                $response = Read-Host
                if ($response -ne "y" -and $response -ne "Y") {
                    Write-Host ""
                    Write-Host "Build cancelled. Please install CMake $CMAKE_RECOMMENDED_VERSION and try again." -ForegroundColor Yellow
                    Write-Log "Build cancelled due to CMake $major.x version" "INFO"
                    exit 0
                }
                Write-Host ""
                Write-Info "Continuing with CMake $major.x at user's request..."
                Write-Log "User chose to continue with CMake $major.x" "INFO"
            }
        }
    }
    catch {
        Write-ErrorMsg "CMake is not installed or not in PATH"
        Write-Host "Please install CMake 3.20+ from: https://cmake.org/download/"
        Write-Host "Make sure to add CMake to system PATH during installation"
        exit 1
    }
    
    # Check Visual Studio 2022
    Write-Step "Checking for Visual Studio 2022..."
    $vsWhere = "${env:ProgramFiles(x86)}\Microsoft Visual Studio\Installer\vswhere.exe"
    if (Test-Path $vsWhere) {
        $vsPath = & $vsWhere -version "[17.0,18.0)" -property installationPath -latest 2>&1
        if ($vsPath -and (Test-Path $vsPath)) {
            Write-Success "Visual Studio 2022 found at: $vsPath"
            
            # Check for required workload
            $workloads = & $vsWhere -version "[17.0,18.0)" -requires Microsoft.VisualStudio.Workload.NativeDesktop -property displayName 2>&1
            if ($workloads) {
                Write-Success "Desktop development with C++ workload is installed"
            }
            else {
                Write-Info "Warning: 'Desktop development with C++' workload may not be installed"
                Write-Info "If build fails, install it via Visual Studio Installer"
            }
        }
        else {
            Write-ErrorMsg "Visual Studio 2022 not found"
            Write-Host "Please install Visual Studio 2022 from: https://visualstudio.microsoft.com/downloads/"
            Write-Host "Required workload: Desktop development with C++"
            exit 1
        }
    }
    else {
        Write-Info "Warning: Could not find vswhere.exe to verify Visual Studio installation"
        Write-Info "Continuing anyway - CMake will verify Visual Studio is available"
    }
    
    # Check .NET 9 SDK (optional)
    Write-Step "Checking for .NET 9 SDK (optional)..."
    try {
        $dotnetVersion = & dotnet --version 2>&1
        if ($dotnetVersion -match "^9\.") {
            Write-Success ".NET 9 SDK found: $dotnetVersion"
        }
        else {
            Write-Info ".NET SDK found but not version 9: $dotnetVersion"
            Write-Info ".NET 9 bindings will not be available"
        }
    }
    catch {
        Write-Info ".NET SDK not found - .NET bindings will not be available"
        Write-Info "To enable C# bindings, install .NET 9 SDK from: https://dotnet.microsoft.com/download/dotnet/9.0"
    }
    
    # ============================================================================
    # Step 2: Setup vcpkg
    # ============================================================================
    Write-Header "Step 2: Setting up vcpkg for dependency management"
    
    # Check for vcpkg in project directory first
    $vcpkgProjectPath = Join-Path $ScriptDir "vcpkg"
    $vcpkgParentPath = Join-Path (Split-Path $ScriptDir) "vcpkg"
    $vcpkgPath = $null
    $vcpkgToolchainFile = $null
    
    if (Test-Path (Join-Path $vcpkgProjectPath "vcpkg.exe")) {
        $vcpkgPath = $vcpkgProjectPath
        Write-Success "Found vcpkg in project directory: $vcpkgPath"
    }
    elseif (Test-Path (Join-Path $vcpkgParentPath "vcpkg.exe")) {
        $vcpkgPath = $vcpkgParentPath
        Write-Success "Found vcpkg in parent directory: $vcpkgPath"
    }
    else {
        Write-Info "vcpkg not found - setting up in project directory..."
        
        # Clone vcpkg
        Write-Step "Cloning vcpkg repository..."
        try {
            & git clone https://github.com/microsoft/vcpkg.git $vcpkgProjectPath
            if ($LASTEXITCODE -ne 0) {
                throw "Git clone failed"
            }
            Write-Success "vcpkg cloned successfully"
            $vcpkgPath = $vcpkgProjectPath
        }
        catch {
            Write-ErrorMsg "Failed to clone vcpkg repository"
            Write-Host "Error: $_"
            exit 1
        }
        
        # Bootstrap vcpkg
        Write-Step "Bootstrapping vcpkg..."
        try {
            Push-Location $vcpkgPath
            $bootstrapScript = Join-Path $vcpkgPath "bootstrap-vcpkg.bat"
            & cmd /c $bootstrapScript
            if ($LASTEXITCODE -ne 0) {
                throw "Bootstrap failed"
            }
            Pop-Location
            Write-Success "vcpkg bootstrapped successfully"
        }
        catch {
            Pop-Location
            Write-ErrorMsg "Failed to bootstrap vcpkg"
            Write-Host "Error: $_"
            exit 1
        }
    }
    
    # Verify vcpkg executable exists
    $vcpkgExe = Join-Path $vcpkgPath "vcpkg.exe"
    if (-not (Test-Path $vcpkgExe)) {
        if ($SkipVcpkgBootstrap) {
            Write-ErrorMsg "vcpkg.exe not found at $vcpkgExe"
            exit 1
        }
        
        # Try to bootstrap
        Write-Info "vcpkg.exe not found, attempting to bootstrap..."
        try {
            Push-Location $vcpkgPath
            $bootstrapScript = Join-Path $vcpkgPath "bootstrap-vcpkg.bat"
            & cmd /c $bootstrapScript
            if ($LASTEXITCODE -ne 0) {
                throw "Bootstrap failed"
            }
            Pop-Location
            Write-Success "vcpkg bootstrapped successfully"
        }
        catch {
            Pop-Location
            Write-ErrorMsg "Failed to bootstrap vcpkg"
            Write-Host "Error: $_"
            exit 1
        }
    }
    
    # Set vcpkg toolchain file path
    $vcpkgToolchainFile = Join-Path $vcpkgPath "scripts\buildsystems\vcpkg.cmake"
    if (-not (Test-Path $vcpkgToolchainFile)) {
        Write-ErrorMsg "vcpkg toolchain file not found at: $vcpkgToolchainFile"
        exit 1
    }
    Write-Success "Using vcpkg toolchain: $vcpkgToolchainFile"
    
    # ============================================================================
    # Step 3: Generate Visual Studio Solution
    # ============================================================================
    Write-Header "Step 3: Generating Visual Studio 2022 Solution"
    
    # Create build directory
    $buildDir = Join-Path $ScriptDir "build"
    if (-not (Test-Path $buildDir)) {
        New-Item -ItemType Directory -Path $buildDir | Out-Null
        Write-Info "Created build directory: $buildDir"
    }
    else {
        Write-Info "Using existing build directory: $buildDir"
    }
    
    # Generate solution with CMake
    Write-Step "Running CMake to generate Visual Studio solution..."
    Write-Info "Configuration: Windows x64 with DirectX 11/12"
    Write-Info "This may take 5-15 minutes on first run (vcpkg will download and build dependencies)"
    Write-Host ""
    Write-Log "Starting CMake generation..." "INFO"
    Write-Log "Build directory: $buildDir" "INFO"
    Write-Log "vcpkg toolchain: $vcpkgToolchainFile" "INFO"
    
    try {
        Push-Location $buildDir
        
        $cmakeArgs = @(
            "-G", "Visual Studio 17 2022",
            "-A", "x64",
            "-DCMAKE_TOOLCHAIN_FILE=$vcpkgToolchainFile",
            ".."
        )
        
        Write-Host "Executing: cmake $($cmakeArgs -join ' ')" -ForegroundColor Gray
        Write-Log "Executing: cmake $($cmakeArgs -join ' ')" "INFO"
        Write-Host ""
        
        # Stream cmake output in real-time to both console and log file
        # Use & operator instead of Start-Process to properly handle arguments with spaces
        # Capture output to a variable first to preserve exit code
        $cmakeOutput = @()
        $cmakeExitCode = $null
        
        try {
            $cmakeOutput = & cmake $cmakeArgs 2>&1
            $cmakeExitCode = $LASTEXITCODE
            
            # Display and log the output
            foreach ($line in $cmakeOutput) {
                Write-Host $line
                $cmakeTimestamp = Get-Date -Format "yyyy-MM-dd HH:mm:ss"
                Add-Content -Path $LogFile -Value "[$cmakeTimestamp] [CMAKE] $line"
            }
            
            if ($cmakeExitCode -ne 0) {
                throw "CMake generation failed with exit code $cmakeExitCode"
            }
        }
        catch {
            # Re-throw to outer catch block
            throw
        }
        
        Pop-Location
        Write-Success "Visual Studio solution generated successfully"
    }
    catch {
        Pop-Location
        Write-ErrorMsg "Failed to generate Visual Studio solution"
        Write-Host "Error: $_"
        Write-Host ""
        if ($null -ne $cmakeExitCode) {
            Write-Host "CMake exit code: $cmakeExitCode"
            Write-Host ""
        }
        if ($cmakeOutput -and $cmakeOutput.Count -gt 0) {
            Write-Host "Last 20 lines of CMake output:"
            $cmakeOutput | Select-Object -Last 20 | ForEach-Object {
                Write-Host "  $_" -ForegroundColor Yellow
            }
            Write-Host ""
        }
        Write-Host "Troubleshooting:"
        Write-Host "  1. Verify all prerequisites are installed"
        Write-Host "  2. Check that vcpkg is properly set up"
        Write-Host "  3. See BUILD.md for detailed instructions"
        Write-Host "  4. Check the full log file for more details: $LogFile"
        exit 1
    }
    
    # Verify solution file was created
    $solutionFile = Join-Path $buildDir "FreshVoxelEngine.sln"
    if (-not (Test-Path $solutionFile)) {
        Write-ErrorMsg "Solution file was not created: $solutionFile"
        exit 1
    }
    Write-Success "Solution file created: $solutionFile"
    
    # ============================================================================
    # Step 4: Build the Project
    # ============================================================================
    if (-not $SkipBuild) {
        Write-Header "Step 4: Building Fresh Voxel Engine ($BuildConfig)"
        
        Write-Info "This may take 2-5 minutes depending on your system"
        Write-Host ""
        Write-Log "Starting build process..." "INFO"
        Write-Log "Configuration: $BuildConfig" "INFO"
        
        try {
            $cmakeBuildArgs = @(
                "--build", $buildDir,
                "--config", $BuildConfig,
                "--", "/m"  # Use multiple processors
            )
            
            Write-Host "Executing: cmake $($cmakeBuildArgs -join ' ')" -ForegroundColor Gray
            Write-Log "Executing: cmake $($cmakeBuildArgs -join ' ')" "INFO"
            Write-Host ""
            
            # Stream build output in real-time to both console and log file
            # Use & operator instead of Start-Process to properly handle arguments with spaces
            # Capture output to a variable first to preserve exit code
            $buildOutput = @()
            $buildExitCode = $null
            
            try {
                $buildOutput = & cmake $cmakeBuildArgs 2>&1
                $buildExitCode = $LASTEXITCODE
                
                # Display and log the output
                foreach ($line in $buildOutput) {
                    Write-Host $line
                    $buildTimestamp = Get-Date -Format "yyyy-MM-dd HH:mm:ss"
                    Add-Content -Path $LogFile -Value "[$buildTimestamp] [BUILD] $line"
                }
                
                if ($buildExitCode -ne 0) {
                    throw "Build failed with exit code $buildExitCode"
                }
            }
            catch {
                # Re-throw to outer catch block
                throw
            }
            
            Write-Success "Build completed successfully!"
            Write-Log "Build completed successfully" "SUCCESS"
        }
        catch {
            Write-ErrorMsg "Build failed"
            Write-Log "Build failed: $_" "ERROR"
            Write-Host "Error: $_"
            Write-Host ""
            if ($null -ne $buildExitCode) {
                Write-Host "Build exit code: $buildExitCode"
                Write-Host ""
            }
            if ($buildOutput -and $buildOutput.Count -gt 0) {
                Write-Host "Last 30 lines of build output:"
                $buildOutput | Select-Object -Last 30 | ForEach-Object {
                    Write-Host "  $_" -ForegroundColor Yellow
                }
                Write-Host ""
            }
            Write-Host "The Visual Studio solution has been generated."
            Write-Host "You can open it manually to investigate build errors:"
            Write-Host "  $solutionFile"
            Write-Host ""
            Write-Host "Check the full log file for more details: $LogFile"
            exit 1
        }
        
        # Verify executable was created
        $exePath = Join-Path $buildDir "$BuildConfig\FreshVoxelEngine.exe"
        if (Test-Path $exePath) {
            Write-Success "Executable created: $exePath"
        }
        else {
            Write-Info "Warning: Executable not found at expected location: $exePath"
        }
    }
    else {
        Write-Header "Step 4: Build Skipped"
        Write-Info "Build step was skipped as requested"
        Write-Info "You can build from Visual Studio or run:"
        Write-Info "  cmake --build build --config $BuildConfig"
    }
    
    # ============================================================================
    # Step 5: Summary and Open Visual Studio
    # ============================================================================
    Write-Header "Setup Complete!"
    
    Write-Host ""
    Write-Success "Fresh Voxel Engine setup completed successfully!"
    Write-Host ""
    Write-Info "Full log saved to: $LogFile"
    Write-Host ""
    Write-Log "Setup completed successfully" "SUCCESS"
    
    if (-not $SkipBuild) {
        Write-Host "Next steps:" -ForegroundColor Cyan
        Write-Host "  1. The Visual Studio solution will open automatically" -ForegroundColor White
        Write-Host "  2. In Solution Explorer, right-click 'FreshVoxelEngine' project" -ForegroundColor White
        Write-Host "  3. Select 'Set as Startup Project' (project name will appear bold)" -ForegroundColor White
        Write-Host "  4. Press F5 to run the engine with debugging" -ForegroundColor White
        Write-Host "     Or press Ctrl+F5 to run without debugging" -ForegroundColor White
        Write-Host ""
        Write-Host "Executable location:" -ForegroundColor Cyan
        Write-Host "  $buildDir\$BuildConfig\FreshVoxelEngine.exe" -ForegroundColor White
    }
    else {
        Write-Host "Next steps:" -ForegroundColor Cyan
        Write-Host "  1. The Visual Studio solution will open automatically" -ForegroundColor White
        Write-Host "  2. In Solution Explorer, right-click 'FreshVoxelEngine' project" -ForegroundColor White
        Write-Host "  3. Select 'Set as Startup Project'" -ForegroundColor White
        Write-Host "  4. Press F7 to build the solution" -ForegroundColor White
        Write-Host "  5. Press F5 to run the engine" -ForegroundColor White
    }
    
    Write-Host ""
    Write-Host "Documentation:" -ForegroundColor Cyan
    Write-Host "  - Build instructions: BUILD.md" -ForegroundColor White
    Write-Host "  - Getting started: GETTING_STARTED.md" -ForegroundColor White
    Write-Host "  - Controls: CONTROLS.md" -ForegroundColor White
    Write-Host "  - Documentation index: DOCUMENTATION_INDEX.md" -ForegroundColor White
    Write-Host ""
    
    # Open Visual Studio
    if ($OpenVS) {
        Write-Step "Opening Visual Studio 2022..."
        try {
            Start-Process $solutionFile
            Write-Success "Visual Studio is opening with the solution"
            Write-Info "The build automation is now complete. You're in control!"
        }
        catch {
            Write-ErrorMsg "Failed to open Visual Studio automatically"
            Write-Host "Please open the solution manually:"
            Write-Host "  $solutionFile"
        }
    }
    else {
        Write-Info "Visual Studio not opened automatically (OpenVS = false)"
        Write-Host "To open the solution manually:"
        Write-Host "  $solutionFile"
    }
    
    Write-Host ""
    Write-Host "============================================================" -ForegroundColor Green
    Write-Host " Setup and build completed successfully!" -ForegroundColor Green
    Write-Host "============================================================" -ForegroundColor Green
    Write-Host ""
    
}
catch {
    Write-Host ""
    Write-ErrorMsg "An unexpected error occurred"
    Write-Host "Error: $_"
    Write-Host "Stack Trace: $($_.ScriptStackTrace)"
    Write-Host ""
    Write-Log "Unexpected error occurred: $_" "ERROR"
    Write-Log "Stack trace: $($_.ScriptStackTrace)" "ERROR"
    Write-Host "Full log saved to: $LogFile"
    Write-Host ""
    Write-Host "For help, see BUILD.md or open an issue at:"
    Write-Host "https://github.com/shifty81/fresh/issues"
    exit 1
}
