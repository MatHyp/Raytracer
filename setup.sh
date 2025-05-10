#!/bin/bash

# Step 1: Install OpenCV (via vcpkg or your package manager of choice)
echo "Installing OpenCV..."

# Check if vcpkg is installed, and install it if not
if [ ! -d "vcpkg" ]; then
    echo "Installing vcpkg..."
    git clone https://github.com/microsoft/vcpkg.git
    cd vcpkg
    ./bootstrap-vcpkg.sh
    cd ..
fi

# Install OpenCV using vcpkg
./vcpkg install opencv4

# Step 2: Set up the CMake build system
echo "Configuring the build system..."
mkdir -p cmake-build-debug
cd cmake-build-debug
cmake .. -DCMAKE_TOOLCHAIN_FILE=../vcpkg/scripts/buildsystems/vcpkg.cmake

# Step 3: Build the project
echo "Building the project..."
make  # or ninja if you're using Ninja as the generator

# Step 4: Check if the 'bin' directory exists and run the executable
echo "Running the raytracer..."

# Show current directory to understand where we are
echo "Current directory: $(pwd)"

# Ensure the 'bin' directory exists before trying to navigate
if [ -d "../bin" ]; then
    echo "'bin' directory found!"
    cd ../bin
else
    echo "'bin' directory not found!"
    exit 1
fi

# Debug: Show the contents of the 'bin' directory
echo "Contents of 'bin' directory:"
ls -al

# Run the executable if it exists
if [ -f "rt" ]; then
    echo "Running 'rt' executable..."
    ./rt
else
    echo "'rt' executable not found in the 'bin' directory!"
    exit 1
fi

# Done
echo "Setup and execution complete!"
