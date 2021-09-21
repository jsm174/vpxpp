# VPXPP

[![CI status (macOS)](https://github.com/jsm174/vpxpp/actions/workflows/vpxpp.yml/badge.svg?branch=main)](https://github.com/jsm174/vpxpp/actions)

What is VPXPP? I'm not sure yet myself. [Visual Pinball Engine](https://github.com/freezy/VisualPinball.Engine) is no doubt the future of Visual Pinball.

This is more of a side project to see if I can actually make a cross platform version of VPX. It will not use COM, ActiveX, VBScript, etc. It will not run existing tables without rewritting the table script. 

## Setup

```
cp cmake/CMakeLists_osx-x64.txt CMakeLists.txt
cmake -DCMAKE_BUILD_TYPE=Release -B build/Release
cmake --build build/Release
```

## Usage

```
build/Release/vpxpp tables/blankTable.vpx
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

## License
