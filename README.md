# VPXPP

[![CI status (macOS)](https://github.com/jsm174/vpxpp/actions/workflows/vpxpp.yml/badge.svg?branch=main)](https://github.com/jsm174/vpxpp/actions)

What is VPXPP? I'm not sure yet myself. [Visual Pinball Engine](https://github.com/freezy/VisualPinball.Engine) is no doubt the future of Visual Pinball.

This is more of a side project to see if I can actually make a cross platform version of VPX. It will not use COM, ActiveX, VBScript, etc. It will not run existing tables without rewritting the table script. 

## Setup

MacOS:

```
brew install nlohmann-json sdl2 freeimage v8 cxxopts jsm174/bgfx/bgfx
git clone https://github.com/jsm174/vpxpp.git
cd vpxpp
cp cmake/CMakeLists_osx-x64.txt CMakeLists.txt
cmake -DCMAKE_BUILD_TYPE=Release -B build/Release
cmake --build build/Release
```

Windows:

```
git clone https://github.com/jsm174/vpxpp.git
cd vpxpp
copy cmake\CMakeLists_win-x64.txt CMakeLists.txt
mkdir deps
cd deps
git clone https://github.com/microsoft/vcpkg
.\vcpkg\bootstrap-vcpkg.bat
.\vcpkg\vcpkg install nlohmann-json sdl2 freeimage cxxopts --triplet=x64-windows
curl -sL https://github.com/tbossi/v8-builder/releases/download/9.2.230.22/v8_engine_9.2.230.22.zip -o v8.zip
tar -xf v8.zip
git clone https://github.com/bkaradzic/bgfx.cmake
cd bgfx.cmake
git submodule update --init --recursive
cmake -DCMAKE_INSTALL_PREFIX=./install -DBGFX_BUILD_EXAMPLES=OFF -DBGFX_BUILD_TOOLS=ON -DBGFX_INSTALL=ON -B build
cmake --build build --target install --config Release
cd ../..
cmake -DCMAKE_PREFIX_PATH=./deps/bgfx.cmake/install -DV8_DIR=./deps/v8 -DCMAKE_TOOLCHAIN_FILE=./deps/vcpkg/scripts/buildsystems/vcpkg.cmake -B build
cmake --build build --config Release
```

## Usage

MacOS:

```
build/Release/vpxpp -f tables/blankTable.vpx
```

Windows:

```
build\Release\vpxpp.exe -f tables/blankTable.vpx
```

## Done

- Table loading 

## To Do

- Everything

## Help

- So I'm to the point where I can start working on rendering the table. I'm not a 3D guy or a physics guy. I know a little OpenGL and not much DirectX. I was looking for a cross platform rendering library and settled on [bgfx](https://github.com/bkaradzic/bgfx). Visual Pinball uses Direct3D 9, so there is a ton of work that will be required to port it to bgfx. If you are interested in contributing, I would be extremely greatful! 

## Why work on this?

I've been thinking about this project for well over a decade, so realistically it will probably go nowhere. 

The past few years I've contributed to [VPX-JS](https://github.com/vpdb/vpx-js), [Visual Pinball Engine](https://github.com/VisualPinball), [NetMiniZ](https://github.com/jsm174/net-miniz), [WPCEmuCS](https://github.com/jsm174/wpc-emu-cs), [PinMAME for .NET](https://github.com/vpinball/pinmame-dotnet), [PinMAME Silk](https://github.com/jsm174/pinmame-silk), and the CIs for [PinMAME](https://github.com/vpinball/pinmame) and [Visual Pinball](https://github.com/vpinball/vpinball). 

So, I guess there's no time like the present!

## Misc

- Clang Format Style (Clang_format_style)

```
{ BasedOnStyle: LLVM, PointerAlignment: Left, UseTab: Always, IndentWidth: 4, TabWidth: 4, BreakBeforeBraces: Allman, AllowShortIfStatementsOnASingleLine: false, IndentCaseLabels: false, ColumnLimit: 0, AccessModifierOffset: -4, NamespaceIndentation: All, FixNamespaceComments: false, ColumnLimit: 120 }
```

## License
