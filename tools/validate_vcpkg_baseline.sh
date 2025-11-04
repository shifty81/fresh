#!/bin/bash
# Validation script to verify vcpkg.json baseline is valid
# This script checks if the baseline commit exists in the vcpkg repository
# and contains the required dependencies

set -e

echo "=========================================="
echo "vcpkg Baseline Validation Script"
echo "=========================================="
echo ""

# Get the repository root (parent of tools directory)
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
REPO_ROOT="$( cd "$SCRIPT_DIR/.." && pwd )"

# Extract baseline from vcpkg.json
BASELINE=$(grep -o '"builtin-baseline": "[^"]*"' "$REPO_ROOT/vcpkg.json" | cut -d'"' -f4)
echo "Found baseline commit: $BASELINE"
echo ""

# Create a temporary directory for vcpkg
TEMP_DIR=$(mktemp -d)
trap "rm -rf $TEMP_DIR" EXIT

echo "Cloning vcpkg repository to temporary directory..."
git clone --quiet https://github.com/Microsoft/vcpkg.git "$TEMP_DIR/vcpkg"

echo "Checking if baseline commit exists..."
cd "$TEMP_DIR/vcpkg"
if ! git cat-file -e "$BASELINE^{commit}" 2>/dev/null; then
    echo "ERROR: Baseline commit $BASELINE does not exist in vcpkg repository"
    exit 1
fi
echo "✓ Baseline commit exists"
echo ""

echo "Checking if baseline contains versions/baseline.json..."
if ! git show "$BASELINE:versions/baseline.json" > /dev/null 2>&1; then
    echo "ERROR: Baseline commit $BASELINE does not contain versions/baseline.json"
    exit 1
fi
echo "✓ versions/baseline.json found in baseline"
echo ""

echo "Checking if required dependencies are in baseline..."
BASELINE_JSON=$(git show "$BASELINE:versions/baseline.json")

REQUIRED_DEPS=("glfw3" "glm" "imgui")
for dep in "${REQUIRED_DEPS[@]}"; do
    if echo "$BASELINE_JSON" | grep -q "\"$dep\""; then
        VERSION=$(echo "$BASELINE_JSON" | grep -A2 "\"$dep\"" | grep "baseline" | cut -d'"' -f4)
        echo "✓ $dep found in baseline (version: $VERSION)"
    else
        echo "ERROR: Required dependency $dep not found in baseline"
        exit 1
    fi
done

echo ""
echo "=========================================="
echo "✓ All validation checks passed!"
echo "=========================================="
