# Project Build and Execution Guide

This document provides instructions on how to build and run the project.

## Build the Project

Before running the program, you need to build the project using the following steps:

```sh
# Create a build directory
mkdir build
cd build

# Generate project files using CMake for Visual Studio 2022
cmake .. -G "Visual Studio 17 2022"
cd ..
```

## Run the Program

After successfully building the project, you can run the program using:

```sh
cmake --build build --config Release
```

## Execute Specific Operations

Depending on your needs, you can execute different operations using the following commands:

```sh
./build/Release/22127384.exe data\input\lena.jpg 
./build/Release/22127384.exe data\input\photo.jpg
./build/Release/22127384.exe data\input\tiger.jpg
./build/Release/22127384.exe data\input\lake.jpg 
./build/Release/22127384.exe data\input\flower.png
```
---

**Note:** Ensure that the required dependencies (such as OpenCV) are properly installed and configured before running the program.

