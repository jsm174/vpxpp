#!/bin/bash

SHADERS=( "basic_without_texture_isNotMetal" \
          "basic_without_texture_isMetal" \
          "basic_with_texture_isNotMetal" \
          "basic_with_texture_isMetal" \
        )

for SHADER in ${SHADERS[@]}; do
   echo ""
   echo "Generating vs_${SHADER}.sc ..."
   echo ""

   shaderc \
      -f "./src/vs_${SHADER}.sc" \
      -o "./bin/vs_${SHADER}_mtl.bin" \
      --type vertex \
      --platform osx \
      -p metal \
      -i /usr/local/include/bgfx

   echo ""
   echo "Generating fs_${SHADER}.sc ..."
   echo ""

   shaderc \
      -f "./src/fs_${SHADER}.sc" \
      -o "./bin/fs_${SHADER}_mtl.bin" \
      --type fragment \
      --platform osx \
      -p metal \
      -i /usr/local/include/bgfx
done

cp ./bin/*.bin ../build
