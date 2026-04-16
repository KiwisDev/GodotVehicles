# Godot Vehicles

**Godot Vehicles** is a realistic vehicles framework for Godot 4.x. built into a **C++** GDExtension.

## Quick Start

### Prerequisites
* **C++20 Compiler**
* **CMake 3.1+**
* **Python 3 & SCons**

### Initial Setup
Clone the repository (including submodules) and run the setup to download the Godot Engine binary:

```bash
# Configure the project
cmake -S . -B build

# Download the Godot Engine binary
cmake --build build --target setup
```
### Development Workflow

The build system handles header generation via cbindgen, and C++ linking automatically.
- Build everything: `cmake --build build`
- Launch Godot Editor: `cmake --build build --target editor`
- Run the Game: `cmake --build build --target play`

## Packaging for Release
```bash
cmake --build build --target package
```

The exported artifacts (e.g., .exe for Windows, .zip for macOS, or .x86_64 for Linux) will be located in `build/`.

## License

This is free and unencumbered software released into the public domain.

Anyone is free to copy, modify, publish, use, compile, sell, or distribute this software, either in source code form or as a compiled binary, for any purpose, commercial or non-commercial, and by any means.

For more information, please refer to the `LICENSE` file or <http://unlicense.org/>.
