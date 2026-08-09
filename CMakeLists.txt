# 3.12 is the floor for file(GLOB ... CONFIGURE_DEPENDS), used for the source list below.
cmake_minimum_required(VERSION 3.12)
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
#
# gen_stubs.py dropped its --db option in favour of --mapping, so the old
# invocation (--db mfc_db_correct.json) failed outright with "the following
# arguments are required: --mapping". scripts/build_openmfc_mingw.sh prefers
# the complete mapping over the real one, since only the complete file carries
# the full ~14k ordinals needed for ABI compatibility.
set(ORDINAL_MAPPING ${CMAKE_CURRENT_SOURCE_DIR}/mfc_complete_ordinal_mapping.json)
if(NOT EXISTS ${ORDINAL_MAPPING})
    set(ORDINAL_MAPPING ${CMAKE_CURRENT_SOURCE_DIR}/mfc_real_ordinal_mapping.json)
endif()
if(NOT EXISTS ${ORDINAL_MAPPING})
    message(FATAL_ERROR "No ordinal mapping found; expected mfc_complete_ordinal_mapping.json or mfc_real_ordinal_mapping.json")
endif()
message(STATUS "Ordinal mapping: ${ORDINAL_MAPPING}")

add_custom_command(
    OUTPUT ${GEN_DEF} ${GEN_STUBS}
    COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/tools/gen_stubs.py
            --mapping ${ORDINAL_MAPPING}
            --out-def ${GEN_DEF}
            --out-stubs ${GEN_STUBS}
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/tools/gen_stubs.py ${ORDINAL_MAPPING}
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
#
# Discovered rather than listed by hand. The previous hand-written list named
# 11 files under src/mfc/ that were never committed (dlgcore, commondlgs,
# winfrm, controls, docview, archive, file, collections, sync, lock, thread),
# so add_library() failed at generate time and this build was unusable from a
# clean checkout. CONFIGURE_DEPENDS re-runs the glob when the directory
# changes, and matches how phase4/scripts/build_phase4.sh collects its own
# sources.
file(GLOB MFC_SOURCES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/mfc/*.cpp)
if(NOT MFC_SOURCES)
    message(FATAL_ERROR "No sources found in ${CMAKE_CURRENT_SOURCE_DIR}/src/mfc/")
endif()

set(SOURCES
    ${GEN_STUBS}
    ${GEN_RTTI_C}
    ${MFC_SOURCES}
)

# Target
add_library(openmfc SHARED ${SOURCES})

# Compiler options
if(MSVC)
    # MSVC specific options
elseif(BUILD_MINGW OR MINGW)
    # MinGW options - build actual Windows DLL
    target_compile_options(openmfc PRIVATE $<$<COMPILE_LANGUAGE:CXX>:-fpermissive> -Wno-attributes -DUNICODE -D_UNICODE)
    # Use the .def file for exports - MinGW uses different syntax.
    # -static matches phase4/scripts/build_phase4.sh. Without it the DLL imports
    # libgcc_s_seh-1.dll, libstdc++-6.dll and libwinpthread-1.dll, and LoadLibrary
    # fails with ERROR_MOD_NOT_FOUND (126) wherever those aren't on the search
    # path; the reference DLL depends on Windows system libraries only.
    set_target_properties(openmfc PROPERTIES
        LINK_FLAGS "-Wl,${GEN_DEF} -static"
        PREFIX ""
        SUFFIX ".dll"
    )
else()
    # There is no native Linux build: include/openmfc/afx.h:20 hard-errors with
    # "OpenMFC is Windows-only" and then fails on #include <windows.h>. Say so at
    # configure time rather than letting it surface as a wall of compiler errors.
    message(FATAL_ERROR
        "OpenMFC is Windows-only and cannot be built natively on this platform.\n"
        "Cross-compile with MinGW:\n"
        "  cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=cmake/mingw-w64-x86_64.cmake -DBUILD_MINGW=ON")
endif()

# Ensure generators run
add_custom_target(openmfc-stubs DEPENDS ${GEN_DEF} ${GEN_RTTI_C})
add_dependencies(openmfc openmfc-stubs)

# Generate import library for MinGW
if(BUILD_MINGW OR MINGW)
    # The cross toolchain only ships the target-prefixed dlltool, and it sets
    # CMAKE_FIND_ROOT_PATH_MODE_PROGRAM to NEVER, so find_program(DLLTOOL dlltool)
    # searched the host for a bare "dlltool", came back NOTFOUND, and this step
    # was skipped without a word. CMake already resolves the right binary itself.
    if(CMAKE_DLLTOOL)
        set(DLLTOOL ${CMAKE_DLLTOOL})
    else()
        find_program(DLLTOOL NAMES ${CMAKE_C_COMPILER_TARGET}-dlltool x86_64-w64-mingw32-dlltool dlltool)
    endif()
    if(DLLTOOL)
        add_custom_command(TARGET openmfc POST_BUILD
            COMMAND ${DLLTOOL} -d ${GEN_DEF} -l ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/libopenmfc.a
            COMMENT "Generating MinGW import library"
        )
    else()
        message(WARNING "dlltool not found; skipping .def-derived import library")
    endif()
endif()
