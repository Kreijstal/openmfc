cmake_minimum_required(VERSION 3.10)
project(OpenMFC C CXX)

# Option to build for Windows using MinGW
option(BUILD_MINGW "Build Windows DLL using MinGW cross-compiler" OFF)

# Find Python for tools
find_package(Python3 REQUIRED COMPONENTS Interpreter)

# Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Include directories
include_directories(include)

# Generated files
set(GENERATED_DIR ${CMAKE_BINARY_DIR}/generated)
file(MAKE_DIRECTORY ${GENERATED_DIR})
file(MAKE_DIRECTORY ${GENERATED_DIR}/src)
file(MAKE_DIRECTORY ${GENERATED_DIR}/include/openmfc)

set(GEN_DEF ${GENERATED_DIR}/openmfc.def)
set(GEN_STUBS ${GENERATED_DIR}/src/stubs.cpp)
set(GEN_RTTI_C ${GENERATED_DIR}/src/generated_rtti.c)
set(GEN_RTTI_H ${GENERATED_DIR}/include/openmfc/eh_rtti.h)

# Custom command for stubs
add_custom_command(
    OUTPUT ${GEN_DEF} ${GEN_STUBS}
    COMMAND ${Python3_EXECUTABLE} ${CMAKE_SOURCE_DIR}/tools/gen_stubs.py
            --db ${CMAKE_SOURCE_DIR}/mfc_db_correct.json
            --out-def ${GEN_DEF}
            --out-stubs ${GEN_STUBS}
    DEPENDS ${CMAKE_SOURCE_DIR}/tools/gen_stubs.py ${CMAKE_SOURCE_DIR}/mfc_db_correct.json
    COMMENT "Generating stubs and def file"
)

# Custom command for RTTI
add_custom_command(
    OUTPUT ${GEN_RTTI_C} ${GEN_RTTI_H}
    COMMAND ${Python3_EXECUTABLE} ${CMAKE_SOURCE_DIR}/tools/gen_rtti.py
            --exceptions ${CMAKE_SOURCE_DIR}/exceptions.json
            --out-c ${GEN_RTTI_C}
            --out-h ${GEN_RTTI_H}
    DEPENDS ${CMAKE_SOURCE_DIR}/tools/gen_rtti.py ${CMAKE_SOURCE_DIR}/exceptions.json
    COMMENT "Generating RTTI metadata"
)

# Add generated include dir
include_directories(${GENERATED_DIR}/include)

# Sources
set(SOURCES
    ${GEN_STUBS}
    ${GEN_RTTI_C}
    src/mfc/exceptions.cpp
    src/mfc/appcore.cpp
    src/mfc/strcore.cpp
    src/mfc/afxmem.cpp
    src/mfc/dlgcore.cpp
    src/mfc/commondlgs.cpp
    src/mfc/winfrm.cpp
    src/mfc/controls.cpp
    src/mfc/docview.cpp
    src/mfc/archive.cpp
    src/mfc/file.cpp
    src/mfc/collections.cpp
    src/mfc/sync.cpp
    src/mfc/lock.cpp
    src/mfc/thread.cpp
)

# Target
add_library(openmfc SHARED ${SOURCES})

# Compiler options
if(MSVC)
    # MSVC specific options
elseif(BUILD_MINGW OR MINGW)
    # MinGW options - build actual Windows DLL
    target_compile_options(openmfc PRIVATE $<$<COMPILE_LANGUAGE:CXX>:-fpermissive> -Wno-attributes -DUNICODE -D_UNICODE)
    # Use the .def file for exports - MinGW uses different syntax
    set_target_properties(openmfc PROPERTIES
        LINK_FLAGS "-Wl,${GEN_DEF}"
        PREFIX ""
        SUFFIX ".dll"
    )
else()
    # Native Linux build
    target_compile_options(openmfc PRIVATE $<$<COMPILE_LANGUAGE:CXX>:-fpermissive> -Wno-attributes)
endif()

# Ensure generators run
add_custom_target(openmfc-stubs DEPENDS ${GEN_DEF} ${GEN_RTTI_C})
add_dependencies(openmfc openmfc-stubs)

# Generate import library for MinGW
if(BUILD_MINGW OR MINGW)
    find_program(DLLTOOL dlltool)
    if(DLLTOOL)
        add_custom_command(TARGET openmfc POST_BUILD
            COMMAND ${DLLTOOL} -d ${GEN_DEF} -l ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/libopenmfc.a
            COMMENT "Generating MinGW import library"
        )
    endif()
endif()
