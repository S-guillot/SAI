# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

SAI ("Solution pour une gestion fluide des tâches") is a Qt-based desktop application designed as a comprehensive productivity and task management solution. The application consists of two main parts:

1. **Task Management System** - Advanced task and project management with categorization, priority indices, and productivity tracking
2. **Work Method Management** - Integration with productivity methods like Pomodoro technique with interactive work mode

**Technical Stack:**
- **Language**: C++ (C++17 standard)
- **Framework**: Qt 5.15+ or Qt 6.x
- **Build System**: CMake 3.16+
- **Database**: PostgreSQL with complete schema for tasks, projects, categories, users, and productivity tracking
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

## Application Features

### Task Management Features
- **Tasks and Projects**: Tasks can be grouped into projects, both must be associated with categories
- **Advanced Task Properties**:
  - Step dates (due dates)
  - Estimated duration and time tracking
  - Status management (todo, in progress, etc.)
  - Priority indices (lower = more urgent)
  - Likability indices (user interest tracking)
  - Redundancy flags

### Productivity Indices System
- **Likability Index**: Tracks user interest in tasks/projects/categories based on completion patterns
- **Productivity Index**: Tracks productivity across 14 time periods (morning/afternoon × 7 days)
- **Intelligent Task Recommendation**: Algorithm suggests optimal tasks based on current time period and user preferences

### Work Methods Integration
- **Pomodoro Technique**: Built-in timer with customizable work/pause durations
- **Interactive Work Mode**: Notification system requiring user interaction for synchronization
- **Statistics Dashboard**: Tracks work patterns and method effectiveness

## Database Schema

The application uses a PostgreSQL database with the following main entities:
- **Users**: User management and productivity indices
- **Tasks**: Complete task management with relationships
- **Projects**: Project organization and tracking
- **Categories**: Task/project categorization system
- **Work Methods**: Productivity method configurations
- **Notifications**: User notification system
- **Productivity Periods**: Time-based productivity tracking

## Architecture Notes

- The application follows Qt's standard main window pattern with `MainWindow` as the primary UI container
- CMake configuration supports both Qt5 and Qt6, with automatic detection
- The build system uses Qt's MOC (Meta-Object Compiler) for signal/slot processing
- UI files are processed automatically through Qt's UIC (User Interface Compiler)
- The project is set up for cross-platform deployment (Windows, macOS, Linux, Android)
- Database scripts are available in `/database/` directory with setup automation

## Development Environment

- Qt Creator is the recommended IDE (build configuration already includes Qt Creator settings)
- The project uses CMake's automatic Qt features (AUTOUIC, AUTOMOC, AUTORCC)
- Build artifacts are organized in the `build/Desktop-Debug` directory
- The executable is named `SAI` and built directly in the build directory