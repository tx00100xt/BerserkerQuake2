cmake_minimum_required(VERSION 3.5)

project(AppImage-Runner VERSION 1.0.0)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(LOCAL_INSTALL TRUE)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")
include(CheckCXXCompilerFlag)

set(INTERNAL_NAME "AppImage-Runner")

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "None Debug Release RelWithDebInfo MinSizeRel" FORCE)
endif()

set(DEBUG FALSE)
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(DEBUG TRUE)
endif()

# RAKE! Where to install the binaries.
if(CMAKE_INSTALL_PREFIX STREQUAL "/usr/local" OR CMAKE_INSTALL_PREFIX STREQUAL "") # Only works for linux since I don't
                                                                                   # know what default is for windows/macos/freebsd.

    set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/")                               # set install path to project root directory since
                                                                                   # since one wasn't set during config
    set(LOCAL_INSTALL TRUE)
    message(STATUS "Using LOCAL Install")
endif()

if(CMAKE_INSTALL_PREFIX STREQUAL "/usr" ) # Only works for linux since I don't know what default is for windows/macos/freebsd.
    set(LOCAL_INSTALL FALSE)
    message(STATUS "Using SYSTEM Install")
endif()

if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_C_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID MATCHES "AppleClang")
    # This section and the like are for flags/defines that can be shared between 
    # c and c++ compile options
    add_compile_options(-Wall)
    add_compile_options(-pipe)
    add_compile_options(-fPIC)
    add_compile_options(-fms-extensions)
    if(LOCAL_INSTALL)
       add_compile_options(-mtune=generic)
    elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "i386|i586|i686|x86|AMD64|x86_64")
       add_compile_options(-mtune=generic) # set mtune=generic if nedded
    endif()
    add_compile_options(-fno-strict-aliasing)
    add_definitions(-D_REENTRANT=1)
    add_definitions(-D_MT=1)

    ## Add your custom C and CXX flags on the command line aka -DCMAKE_C_FLAGS=-std=c98 or -DCMAKE_CXX_FLAGS=-std=c++11
    ## For C flags
        set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS} -g -D_DEBUG=1 -DDEBUG=1 -O0")
        if(MACOSX AND CMAKE_SYSTEM_PROCESSOR STREQUAL "arm64")
            set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS} -DNDEBUG=1 -D_NDEBUG=1 -g -O0 -fno-unsafe-math-optimizations")
            set(CMAKE_C_FLAGS_RELWITHDEBINFO "${CMAKE_C_FLAGS} -DNDEBUG=1 -D_NDEBUG=1 -g -O0 -fno-unsafe-math-optimizations")
            set(CMAKE_C_FLAGS_MINSIZEREL "${CMAKE_C_FLAGS} -DNDEBUG=1 -D_NDEBUG=1 -O0 -fno-unsafe-math-optimizations")
        else()
            set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS} -DNDEBUG=1 -D_NDEBUG=1 -g -O2 -fno-delete-null-pointer-checks -fno-unsafe-math-optimizations")
            set(CMAKE_C_FLAGS_RELWITHDEBINFO "${CMAKE_C_FLAGS} -DNDEBUG=1 -D_NDEBUG=1 -g -O2 -fno-delete-null-pointer-checks -fno-unsafe-math-optimizations")
            set(CMAKE_C_FLAGS_MINSIZEREL "${CMAKE_C_FLAGS} -DNDEBUG=1 -D_NDEBUG=1 -Os -fno-delete-null-pointer-checks -fno-unsafe-math-optimizations")
        endif()

    ## For C++ flags
        set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -g -D_DEBUG=1 -DDEBUG=1 -O0")
        if(MACOSX AND CMAKE_SYSTEM_PROCESSOR STREQUAL "arm64")
            set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -DNDEBUG=1 -D_NDEBUG=1 -O0 -fno-unsafe-math-optimizations")
            set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS} -DNDEBUG=1 -D_NDEBUG=1 -g -O0 -fno-unsafe-math-optimizations") ## RAKE! Does -DNDEBUG=1 and -D_NDEBUG=1 mess with RelWithDebInfo?
            set(CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_CXX_FLAGS} -DNDEBUG=1 -D_NDEBUG=1 -O0 -fno-unsafe-math-optimizations")
        else()
            set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -DNDEBUG=1 -D_NDEBUG=1 -O2 -fno-delete-null-pointer-checks -fno-unsafe-math-optimizations")
            set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS} -DNDEBUG=1 -D_NDEBUG=1 -g -O2 -fno-delete-null-pointer-checks -fno-unsafe-math-optimizations") ## RAKE! Does -DNDEBUG=1 and -D_NDEBUG=1 mess with RelWithDebInfo?
            set(CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_CXX_FLAGS} -DNDEBUG=1 -D_NDEBUG=1 -Os -fno-delete-null-pointer-checks -fno-unsafe-math-optimizations")
        endif()

    # TODO fix these warnings
    add_compile_options(-Wno-format-security)
    add_compile_options(-Wno-switch)
    add_compile_options(-Wno-char-subscripts)
    add_compile_options(-Wno-unknown-pragmas)
    add_compile_options(-Wno-unused-variable) # TODO: maybe only enable this for Entities
    add_compile_options(-Wno-unused-value)    # same here (the Scripts generate tons of unused variables and values)
    add_compile_options(-Wno-missing-braces)
    #MESSAGE(WARNING, "re-enable some of the warnings some day!")

    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID MATCHES "AppleClang")
        # !!! FIXME: turn a bunch of these off, this is just for now. I hope.  --ryan.
        add_compile_options(-Wno-c++11-compat-deprecated-writable-strings)
    endif()

    if(LINUX)
        set(CMAKE_SKIP_RPATH ON CACHE BOOL "Skip RPATH" FORCE)
        add_definitions(-DPLATFORM_UNIX=1)
        add_definitions(-D_FILE_OFFSET_BITS=64)
        add_definitions(-D_LARGEFILE_SOURCE=1)
        add_definitions(-DPRAGMA_ONCE=1)
    endif()

    if(MACOSX OR LINUX OR FREEBSD)
        add_compile_options(-pthread)
        add_compile_options(-fsigned-char)
    endif()
endif()

set(PROJECT_SOURCES
        Sources/mainapp.c
        Sources/appimage.c
)
add_executable(AppImage-Runner ${PROJECT_SOURCES})

# Link libraries.
target_link_libraries(AppImage-Runner PRIVATE  -lGL -lSDL2 -lSDL2_image -lGLEW)

set_target_properties(AppImage-Runner PROPERTIES OUTPUT_NAME "AppImage-Runner")

# Install Section.
if(LOCAL_INSTALL)
    install(TARGETS AppImage-Runner
            RUNTIME DESTINATION "${CMAKE_INSTALL_PREFIX}/Img"
            PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_WRITE GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
else()
    install(TARGETS AppImage-Runner
            RUNTIME DESTINATION "${CMAKE_INSTALL_PREFIX}/Img"
            PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_WRITE GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
endif()

# end of CMakeLists.txt ...
