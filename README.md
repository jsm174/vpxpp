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

## Why work on this?

I've been thinking about this project for well over a decade, so realistically it will probably go nowhere. 

The past few years I've contributed to [VPX-JS](https://github.com/vpdb/vpx-js), [Visual Pinball Engine](https://github.com/VisualPinball), [NetMiniZ](https://github.com/jsm174/net-miniz), [WPCEmuCS](https://github.com/jsm174/wpc-emu-cs), [PinMAME for .NET](https://github.com/vpinball/pinmame-dotnet), [PinMAME Silk](https://github.com/jsm174/pinmame-silk), and the CIs for [PinMAME](https://github.com/vpinball/pinmame) and [Visual Pinball](https://github.com/vpinball/vpinball). 

So, I guess there's no time like the present!

## License
