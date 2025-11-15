#!/bin/bash
# Automated build script for Fresh Voxel Engine (Linux/macOS)
# 
# This script automates the entire build process for Fresh Voxel Engine:
# 1. Checks prerequisites (GCC/Clang, CMake, Git)
# 2. Installs system dependencies via package manager
# 3. Generates Makefiles with CMake
# 4. Builds the project
# 5. Optionally runs tests
#
# Usage:
#   ./setup-and-build.sh [OPTIONS]
#
# Options:
#   --skip-build       Skip the actual build step and just configure
#   --config <type>    Build configuration (Debug or Release, default: Release)
#   --skip-deps        Skip dependency installation check
#   --run-tests        Run test suite after successful build
#   --help             Show this help message

set -e  # Exit on error

# Default values
SKIP_BUILD=false
BUILD_CONFIG="Release"
SKIP_DEPS=false
RUN_TESTS=false

# Parse command line arguments
while [[ $# -gt 0 ]]; do
    case $1 in
        --skip-build)
            SKIP_BUILD=true
            shift
            ;;
        --config)
            BUILD_CONFIG="$2"
            shift 2
            ;;
        --skip-deps)
            SKIP_DEPS=true
            shift
            ;;
        --run-tests)
            RUN_TESTS=true
            shift
            ;;
        --help)
            cat << EOF
Fresh Voxel Engine - Automated Build Script

Usage: $0 [OPTIONS]

Options:
  --skip-build       Skip the actual build step and just configure
  --config <type>    Build configuration (Debug or Release, default: Release)
  --skip-deps        Skip dependency installation check
  --run-tests        Run test suite after successful build
  --help             Show this help message

Examples:
  $0                             # Full automated setup and build
  $0 --skip-build                # Configure only, no build
  $0 --config Debug --run-tests  # Build Debug and run tests
EOF
            exit 0
            ;;
        *)
            echo "Error: Unknown option: $1"
            echo "Use --help for usage information"
            exit 1
            ;;
    esac
done

# Color output functions
print_header() {
    echo ""
    echo "============================================================"
    echo " $1"
    echo "============================================================"
    echo ""
}

print_success() {
    echo -e "\033[0;32m[OK]\033[0m $1"
}

print_info() {
    echo -e "\033[0;33m[INFO]\033[0m $1"
}

print_error() {
    echo -e "\033[0;31m[ERROR]\033[0m $1"
}

print_step() {
    echo ""
    echo -e "\033[0;35m>>\033[0m $1"
}

# Detect OS
detect_os() {
    if [[ "$OSTYPE" == "linux-gnu"* ]]; then
        OS="linux"
        if [ -f /etc/os-release ]; then
            . /etc/os-release
            DISTRO=$ID
        fi
    elif [[ "$OSTYPE" == "darwin"* ]]; then
        OS="macos"
        DISTRO="macos"
    else
        print_error "Unsupported OS: $OSTYPE"
        exit 1
    fi
    print_info "Detected OS: $OS ($DISTRO)"
}

# Main script
main() {
    print_header "Fresh Voxel Engine - Automated Build Script"
    
    # Get script directory (project root)
    SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
    cd "$SCRIPT_DIR"
    print_info "Project directory: $SCRIPT_DIR"
    
    # Detect OS
    detect_os
    
    # ============================================================================
    # Step 1: Check Prerequisites
    # ============================================================================
    print_header "Step 1: Checking Prerequisites"
    
    # Check Git
    print_step "Checking for Git..."
    if command -v git &> /dev/null; then
        GIT_VERSION=$(git --version)
        print_success "Git found: $GIT_VERSION"
    else
        print_error "Git is not installed"
        if [[ "$OS" == "linux" ]]; then
            echo "Install with: sudo apt-get install git  (Ubuntu/Debian)"
            echo "          or: sudo dnf install git      (Fedora/RHEL)"
        else
            echo "Install with: brew install git"
        fi
        exit 1
    fi
    
    # Check CMake
    print_step "Checking for CMake..."
    if command -v cmake &> /dev/null; then
        CMAKE_VERSION=$(cmake --version | head -n1)
        print_success "CMake found: $CMAKE_VERSION"
        
        # Verify CMake version is 3.20 or higher
        CMAKE_MAJOR=$(echo "$CMAKE_VERSION" | grep -oP '(?<=cmake version )\d+' || echo "0")
        CMAKE_MINOR=$(echo "$CMAKE_VERSION" | grep -oP '(?<=cmake version \d\.)\d+' || echo "0")
        
        if [[ "$CMAKE_MAJOR" -lt 3 ]] || [[ "$CMAKE_MAJOR" -eq 3 && "$CMAKE_MINOR" -lt 20 ]]; then
            print_error "CMake version 3.20 or higher is required"
            echo "Current version: $CMAKE_MAJOR.$CMAKE_MINOR"
            echo "Please update CMake from: https://cmake.org/download/"
            exit 1
        fi
    else
        print_error "CMake is not installed"
        if [[ "$OS" == "linux" ]]; then
            echo "Install with: sudo apt-get install cmake  (Ubuntu/Debian)"
            echo "          or: sudo dnf install cmake      (Fedora/RHEL)"
        else
            echo "Install with: brew install cmake"
        fi
        exit 1
    fi
    
    # Check C++ compiler
    print_step "Checking for C++ compiler..."
    if command -v g++ &> /dev/null; then
        GCC_VERSION=$(g++ --version | head -n1)
        print_success "GCC found: $GCC_VERSION"
    elif command -v clang++ &> /dev/null; then
        CLANG_VERSION=$(clang++ --version | head -n1)
        print_success "Clang found: $CLANG_VERSION"
    else
        print_error "No C++ compiler found (g++ or clang++)"
        if [[ "$OS" == "linux" ]]; then
            echo "Install with: sudo apt-get install build-essential  (Ubuntu/Debian)"
            echo "          or: sudo dnf install gcc-c++              (Fedora/RHEL)"
        else
            echo "Install Xcode Command Line Tools: xcode-select --install"
        fi
        exit 1
    fi
    
    # ============================================================================
    # Step 2: Check/Install Dependencies
    # ============================================================================
    if [[ "$SKIP_DEPS" == false ]]; then
        print_header "Step 2: Checking System Dependencies"
        
        if [[ "$OS" == "linux" ]]; then
            print_step "Checking for required libraries..."
            
            MISSING_DEPS=()
            
            # Check for GLFW
            if ! pkg-config --exists glfw3 2>/dev/null; then
                MISSING_DEPS+=("libglfw3-dev")
            fi
            
            # Check for GLM
            if [ ! -d "/usr/include/glm" ] && [ ! -d "/usr/local/include/glm" ]; then
                MISSING_DEPS+=("libglm-dev")
            fi
            
            # Check for GLEW
            if ! pkg-config --exists glew 2>/dev/null; then
                MISSING_DEPS+=("libglew-dev")
            fi
            
            # Check for OpenAL
            if ! pkg-config --exists openal 2>/dev/null; then
                MISSING_DEPS+=("libopenal-dev")
            fi
            
            # Check for GTest
            if [ ! -f "/usr/include/gtest/gtest.h" ] && [ ! -f "/usr/local/include/gtest/gtest.h" ]; then
                MISSING_DEPS+=("libgtest-dev")
            fi
            
            if [ ${#MISSING_DEPS[@]} -gt 0 ]; then
                print_info "Missing dependencies detected: ${MISSING_DEPS[*]}"
                print_step "Installing missing dependencies..."
                
                if [[ "$DISTRO" == "ubuntu" ]] || [[ "$DISTRO" == "debian" ]]; then
                    echo "Running: sudo apt-get update && sudo apt-get install -y ${MISSING_DEPS[*]}"
                    sudo apt-get update
                    sudo apt-get install -y "${MISSING_DEPS[@]}"
                    print_success "Dependencies installed successfully"
                elif [[ "$DISTRO" == "fedora" ]] || [[ "$DISTRO" == "rhel" ]] || [[ "$DISTRO" == "centos" ]]; then
                    # Convert Debian package names to Fedora equivalents
                    FEDORA_DEPS=()
                    for dep in "${MISSING_DEPS[@]}"; do
                        case $dep in
                            libglfw3-dev) FEDORA_DEPS+=("glfw-devel") ;;
                            libglm-dev) FEDORA_DEPS+=("glm-devel") ;;
                            libglew-dev) FEDORA_DEPS+=("glew-devel") ;;
                            libopenal-dev) FEDORA_DEPS+=("openal-soft-devel") ;;
                            libgtest-dev) FEDORA_DEPS+=("gtest-devel") ;;
                        esac
                    done
                    echo "Running: sudo dnf install -y ${FEDORA_DEPS[*]}"
                    sudo dnf install -y "${FEDORA_DEPS[@]}"
                    print_success "Dependencies installed successfully"
                else
                    print_error "Automatic dependency installation not supported for $DISTRO"
                    echo "Please install manually:"
                    echo "  ${MISSING_DEPS[*]}"
                    exit 1
                fi
            else
                print_success "All required dependencies are already installed"
            fi
            
        elif [[ "$OS" == "macos" ]]; then
            print_step "Checking for Homebrew..."
            if ! command -v brew &> /dev/null; then
                print_error "Homebrew is not installed"
                echo "Install Homebrew from: https://brew.sh"
                exit 1
            fi
            print_success "Homebrew found"
            
            print_step "Checking for required libraries..."
            MISSING_DEPS=()
            
            if ! brew list glfw &> /dev/null; then
                MISSING_DEPS+=("glfw")
            fi
            
            if ! brew list glm &> /dev/null; then
                MISSING_DEPS+=("glm")
            fi
            
            if ! brew list openal-soft &> /dev/null; then
                MISSING_DEPS+=("openal-soft")
            fi
            
            if ! brew list googletest &> /dev/null; then
                MISSING_DEPS+=("googletest")
            fi
            
            if [ ${#MISSING_DEPS[@]} -gt 0 ]; then
                print_info "Installing missing dependencies: ${MISSING_DEPS[*]}"
                brew install "${MISSING_DEPS[@]}"
                print_success "Dependencies installed successfully"
            else
                print_success "All required dependencies are already installed"
            fi
        fi
    else
        print_header "Step 2: Dependency Check Skipped"
        print_info "Dependency installation check was skipped as requested"
    fi
    
    # ============================================================================
    # Step 3: Generate Makefiles
    # ============================================================================
    print_header "Step 3: Generating Build Files with CMake"
    
    # Create build directory
    BUILD_DIR="$SCRIPT_DIR/build"
    if [ ! -d "$BUILD_DIR" ]; then
        mkdir -p "$BUILD_DIR"
        print_info "Created build directory: $BUILD_DIR"
    else
        print_info "Using existing build directory: $BUILD_DIR"
    fi
    
    # Generate Makefiles with CMake
    print_step "Running CMake to generate Makefiles..."
    print_info "Configuration: $BUILD_CONFIG"
    print_info "Platform: $OS with OpenGL rendering"
    echo ""
    
    cd "$BUILD_DIR"
    cmake -DCMAKE_BUILD_TYPE="$BUILD_CONFIG" ..
    
    if [ $? -eq 0 ]; then
        print_success "CMake configuration completed successfully"
    else
        print_error "CMake configuration failed"
        echo ""
        echo "Troubleshooting:"
        echo "  1. Verify all prerequisites are installed"
        echo "  2. Check that all dependencies are available"
        echo "  3. See BUILD.md for detailed instructions"
        exit 1
    fi
    
    # ============================================================================
    # Step 4: Build the Project
    # ============================================================================
    if [[ "$SKIP_BUILD" == false ]]; then
        print_header "Step 4: Building Fresh Voxel Engine ($BUILD_CONFIG)"
        
        print_info "This may take 2-5 minutes depending on your system"
        print_info "Using $(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo "1") CPU cores for parallel build"
        echo ""
        
        # Determine number of CPU cores
        if [[ "$OS" == "linux" ]]; then
            NCORES=$(nproc)
        else
            NCORES=$(sysctl -n hw.ncpu)
        fi
        
        cmake --build . -j"$NCORES"
        
        if [ $? -eq 0 ]; then
            print_success "Build completed successfully!"
        else
            print_error "Build failed"
            echo ""
            echo "Check the output above for error details"
            exit 1
        fi
        
        # Verify executable was created
        if [ -f "$BUILD_DIR/FreshVoxelEngine" ]; then
            print_success "Executable created: $BUILD_DIR/FreshVoxelEngine"
        else
            print_info "Warning: Executable not found at expected location"
        fi
        
        # Run tests if requested
        if [[ "$RUN_TESTS" == true ]]; then
            print_header "Step 5: Running Test Suite"
            
            if [ -f "$BUILD_DIR/FreshVoxelEngineTests" ]; then
                print_step "Executing tests..."
                echo ""
                "$BUILD_DIR/FreshVoxelEngineTests"
                
                if [ $? -eq 0 ]; then
                    print_success "All tests passed!"
                else
                    print_error "Some tests failed"
                    exit 1
                fi
            else
                print_info "Test executable not found, skipping tests"
            fi
        fi
    else
        print_header "Step 4: Build Skipped"
        print_info "Build step was skipped as requested"
        print_info "You can build manually by running:"
        print_info "  cd build && cmake --build . -j$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo "4")"
    fi
    
    # ============================================================================
    # Summary
    # ============================================================================
    print_header "Setup Complete!"
    
    echo ""
    print_success "Fresh Voxel Engine setup completed successfully!"
    echo ""
    
    if [[ "$SKIP_BUILD" == false ]]; then
        echo -e "\033[0;36mNext steps:\033[0m"
        echo "  1. Run the engine:"
        echo "     cd build && ./FreshVoxelEngine"
        echo ""
        echo "  2. Run tests:"
        echo "     cd build && ./FreshVoxelEngineTests"
        echo ""
        echo -e "\033[0;36mExecutable location:\033[0m"
        echo "  $BUILD_DIR/FreshVoxelEngine"
    else
        echo -e "\033[0;36mNext steps:\033[0m"
        echo "  1. Build the project:"
        echo "     cd build && cmake --build . -j$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo "4")"
        echo ""
        echo "  2. Run the engine:"
        echo "     ./FreshVoxelEngine"
    fi
    
    echo ""
    echo -e "\033[0;36mDocumentation:\033[0m"
    echo "  - Build instructions: BUILD.md"
    echo "  - Getting started: GETTING_STARTED.md"
    echo "  - Controls: CONTROLS.md"
    echo "  - Documentation index: DOCUMENTATION_INDEX.md"
    echo ""
    
    echo "============================================================"
    echo -e "\033[0;32m Setup and build completed successfully!\033[0m"
    echo "============================================================"
    echo ""
}

# Run main function
main
