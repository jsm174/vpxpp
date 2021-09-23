# Courtesy of: https://raw.githubusercontent.com/gwaldron/osgearth/master/CMakeModules/FindV8.cmake
#
# Locate V8
# This module defines
# V8_LIBRARY
# V8_FOUND, if false, do not try to link to V8
# V8_INCLUDE_DIR, where to find the headers

IF (NOT $ENV{V8_DIR} STREQUAL "")
  SET(V8_DIR $ENV{V8_DIR})
ENDIF()


SET(V8_LIBRARY_SEARCH_PATHS
  ${V8_DIR}/
  ${V8_DIR}/lib/
  ${V8_DIR}/build/Release/lib/
  ${V8_DIR}/build/Release/lib/third_party/icu/
  ${V8_DIR}/build/Release/obj/
  ${V8_DIR}/build/Release/obj/third_party/icu/
  ${V8_DIR}/out/ia32.release/lib.target/
  ${V8_DIR}/out/ia32.release/lib.target/third_party/icu/
  ${V8_DIR}/out/ia32.release/obj/
  ${V8_DIR}/out/ia32.release/obj/third_party/icu/
  ${V8_DIR}/out.gn/ia32.release/lib.target/
  ${V8_DIR}/out.gn/ia32.release/lib.target/third_party/icu/
  ${V8_DIR}/out.gn/ia32.release/obj/
  ${V8_DIR}/out.gn/ia32.release/obj/third_party/icu/
  ${V8_DIR}/out/x64.release/lib.target/
  ${V8_DIR}/out/x64.release/lib.target/third_party/icu/
  ${V8_DIR}/out/x64.release/obj/
  ${V8_DIR}/out/x64.release/obj/third_party/icu/
  ${V8_DIR}/out.gn/x64.release/lib.target/
  ${V8_DIR}/out.gn/x64.release/lib.target/third_party/icu/
  ${V8_DIR}/out.gn/x64.release/obj/
  ${V8_DIR}/out.gn/x64.release/obj/third_party/icu/
  ~/Library/Frameworks
  /Library/Frameworks
  /usr/local/lib
  /usr/lib
  /sw/lib
  /opt/local/lib
  /opt/csw/lib
  /opt/lib
  /usr/freeware/lib64
  ${V8_DIR}/windows/
)

FIND_PATH(V8_INCLUDE_DIR v8.h
  ${V8_DIR}
  ${V8_DIR}/include
  ~/Library/Frameworks
  /Library/Frameworks
  /usr/local/include
  /usr/include
  /sw/include # Fink
  /opt/local/include # DarwinPorts
  /opt/csw/include # Blastwave
  /opt/include
  /usr/freeware/include
  /devel
  ${V8_DIR}/headers/
)

FIND_LIBRARY(V8_BASE_LIBRARY
  NAMES libv8_base.a v8_base.a v8 v8_base libv8_base v8.dll
  PATHS ${V8_LIBRARY_SEARCH_PATHS}
)

FIND_LIBRARY(V8_LIBPLATFORM_LIBRARY
  NAMES v8_libplatform libv8_libplatform v8_libplatform.dll
  PATHS ${V8_LIBRARY_SEARCH_PATHS}
)

FIND_LIBRARY(V8_LIBBASE_LIBRARY
  NAMES v8_libbase libv8_libbase
  PATHS ${V8_LIBRARY_SEARCH_PATHS}
)

FIND_LIBRARY(V8_SNAPSHOT_LIBRARY
  NAMES v8_snapshot libv8_snapshot
  PATHS ${V8_LIBRARY_SEARCH_PATHS}
)

FIND_LIBRARY(V8_LIBSAMPLER_LIBRARY
  NAMES v8_libsampler libv8_libsampler
  PATHS ${V8_LIBRARY_SEARCH_PATHS}
)

SET(V8_FOUND "NO")

IF (NOT UNIX)
  IF (V8_BASE_LIBRARY AND V8_SNAPSHOT_LIBRARY AND V8_ICUUC_LIBRARY AND V8_ICUI18N_LIBRARY AND V8_INCLUDE_DIR)
    SET(V8_FOUND "YES")
  ENDIF()
ELSEIF(V8_LIBRARY AND V8_ICUUC_LIBRARY AND V8_ICUI18N_LIBRARY AND V8_INCLUDE_DIR)
  SET(V8_FOUND "YES")
ENDIF(NOT UNIX)

add_library(V8::BASE UNKNOWN IMPORTED) 
set_target_properties(V8::BASE PROPERTIES
  INTERFACE_INCLUDE_DIRECTORIES "${V8_INCLUDE_DIR}"
  IMPORTED_LOCATION "${V8_BASE_LIBRARY}"
)

add_library(V8::PLATFORM UNKNOWN IMPORTED) 
set_target_properties(V8::PLATFORM PROPERTIES
  INTERFACE_INCLUDE_DIRECTORIES "${V8_INCLUDE_DIR}"
  IMPORTED_LOCATION "${V8_LIBPLATFORM_LIBRARY}"
)

add_library(V8::V8 INTERFACE IMPORTED)
set_property(TARGET V8::V8 PROPERTY
  INTERFACE_LINK_LIBRARIES V8::BASE V8::PLATFORM)
