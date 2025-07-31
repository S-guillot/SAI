# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

SAI is a Qt-based desktop application designed as a task management solution. The project uses CMake as its build system and follows a standard Qt application architecture with:

- **Language**: C++ (C++17 standard)
- **Framework**: Qt 5.15+ or Qt 6.x
- **Build System**: CMake 3.16+
- **Database**: SQLite/MySQL (planned)
- **Architecture**: MVC pattern using Qt Model/View framework

## Project Structure

```
SAI/
├── src/                    # Source code
│   ├── main.cpp           # Application entry point
│   ├── mainwindow.h/.cpp  # Main window class
│   └── mainwindow.ui      # UI designer file
├── database/              # Database schemas and migrations
├── docs/                  # Documentation
├── resources/             # Assets and resources
└── build/                 # Build artifacts (generated)
```

## Build Commands

### Building the Application
```bash
# Create build directory if it doesn't exist
mkdir -p build/Desktop-Debug
cd build/Desktop-Debug

# Configure with CMake
cmake ../..

# Build the application
make

# Run the application
./SAI
```

### Clean Build
```bash
# From project root
rm -rf build/Desktop-Debug
mkdir -p build/Desktop-Debug
cd build/Desktop-Debug
cmake ../..
make
```

## Architecture Notes

- The application follows Qt's standard main window pattern with `MainWindow` as the primary UI container
- CMake configuration supports both Qt5 and Qt6, with automatic detection
- The build system uses Qt's MOC (Meta-Object Compiler) for signal/slot processing
- UI files are processed automatically through Qt's UIC (User Interface Compiler)
- The project is set up for cross-platform deployment (Windows, macOS, Linux, Android)

## Development Environment

- Qt Creator is the recommended IDE (build configuration already includes Qt Creator settings)
- The project uses CMake's automatic Qt features (AUTOUIC, AUTOMOC, AUTORCC)
- Build artifacts are organized in the `build/Desktop-Debug` directory
- The executable is named `SAI` and built directly in the build directory