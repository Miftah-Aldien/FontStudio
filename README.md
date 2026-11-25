# FontStudio 15 (working name)

A native Win32 C++ font-editor project inspired by the workflow and layout of professional font editors.
It is an independent implementation and does not contain Hi-Logic FontCreator source code or assets.

## Target
- Windows 7 SP1, x86 (32-bit)
- Visual Studio 2019 toolset (v142), Win32
- CMake
- No Qt or runtime framework dependency

## Build locally
Open an x86 Native Tools Command Prompt for VS 2019:

```bat
cmake -S . -B build -A Win32
cmake --build build --config Release
```

The executable will be in `build\\Release\\FontStudio15.exe`.

## Build with GitHub Actions
Push this repository to GitHub. The workflow in `.github/workflows/build-windows.yml`
builds the Win32 Release executable and uploads it as an artifact.

## Current milestone: 1
Implemented:
- Main window with English menu bar and toolbar
- Font Overview character grid
- Glyph Editor workspace with baseline / x-height / ascender / descender guides
- Properties panel placeholder
- Basic window layout and resizing

Not implemented yet:
- Bézier editing and node manipulation
- Real font parsing or TTF/OTF export
- Unicode mapping, kerning, import/export
These are planned milestones, not working features in this starter.

## Next milestones
1. Complete glyph selection and editor interaction
2. Bézier outlines and glyph metrics
3. Unicode mapping and font tables
4. OpenType/TrueType import and export
5. Preview, validation, and packaging
