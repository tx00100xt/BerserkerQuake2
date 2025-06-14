cmake_minimum_required(VERSION 3.10)
project(berserkerq2 LANGUAGES C)

##################################
###      Options section       ###
##################################

# Use system SDL2 is on by default
option(USE_SYSTEM_SDL2 "Use systems sdl2 development files" On)
option(USE_SYSTEM_ZLIB "Use systems zlib development files" On)
option(USE_CCACHE "Set to ON to use ccache if present in the system" ${USE_CCACHE})
option(USE_SYSTEM_INSTALL "Install to systems directories" Off)
option(USE_SYSTEM_OGGVORBIS "Use systems ogg vorbis vorbisfile development files" On)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")
include(CheckCCompilerFlag)


# fallback for cmake versions without add_compile_options # RAKE! Borrowed from dhewm3 project
if(NOT COMMAND add_compile_options)
	function(add_compile_options)
		foreach(arg ${ARGN})
			set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${arg}" PARENT_SCOPE)
		endforeach()
	endfunction()
endif()

if(USE_CCACHE)
    find_program(CCACHE_FOUND ccache)
    if(CCACHE_FOUND)
        set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
        set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache)
    endif()
endif()

# Set up some sanity stuff...
if(CMAKE_SYSTEM_NAME STREQUAL "Linux" OR CMAKE_SYSTEM_NAME MATCHES "GNU")
    set(LINUX TRUE)
endif()
if(CMAKE_SYSTEM_NAME STREQUAL "FreeBSD" OR CMAKE_SYSTEM_NAME STREQUAL "OpenBSD" OR CMAKE_SYSTEM_NAME STREQUAL "NetBSD")
    set(FREEBSD TRUE)
endif()

# Build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "None Debug Release RelWithDebInfo MinSizeRel" FORCE)
    message(STATUS "...CMAKE build type: ${CMAKE_BUILD_TYPE}")
endif()

set(DEBUG FALSE)
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(DEBUG TRUE)
endif()
##################################
###    End Options section     ###
##################################

##################################
###       Detect section       ###
##################################

if(LINUX)
    set(CMAKE_OS_NAME "GNU/Linux" CACHE STRING "Operating system name" FORCE)
    file(READ "/etc/os-release" OS_RELEASE)
    if(OS_RELEASE MATCHES "ID=alpine")
      message(STATUS "Alpine Linux detected")
      set(CMAKE_OS_NAME "Alpine" CACHE STRING "Operating system name" FORCE)
    endif()

    ## Check for Debian GNU/Linux
    find_file(DEBIAN_FOUND debian_version debconf.conf
        PATHS /etc
    )
    if(DEBIAN_FOUND)
        set(CMAKE_OS_NAME "Debian" CACHE STRING "Operating system name" FORCE)
    endif(DEBIAN_FOUND)
    ##  Check for Fedora
    find_file(FEDORA_FOUND fedora-release
        PATHS /etc
    )
    if(FEDORA_FOUND)
        set(CMAKE_OS_NAME "Fedora" CACHE STRING "Operating system name" FORCE)
    endif(FEDORA_FOUND)
    ##  Check for RedHat
    find_file(REDHAT_FOUND redhat-release inittab.RH
        PATHS /etc
    )
    if(REDHAT_FOUND)
        set(CMAKE_OS_NAME "RedHat" CACHE STRING "Operating system name" FORCE)
    endif(REDHAT_FOUND)
    ##  Check for Gentoo
    find_file(OS_Release_FOUND os-release
        PATHS /usr/lib
    )
    if (OS_Release_FOUND)
        ## Scan contents of file
        file(STRINGS ${OS_Release_FOUND} Gentoo_FOUND
            REGEX Gentoo
        )
        ## Check result of string search
        if(Gentoo_FOUND)
            set(CMAKE_OS_NAME "Gentoo" CACHE STRING "Operating system name" FORCE)
            set(DEBIAN_FOUND FALSE)
        endif(Gentoo_FOUND)
    endif(OS_Release_FOUND)
    ## Extra check for Ubuntu    
    if(DEBIAN_FOUND)
      ## At its core Ubuntu is a Debian system, with
      ## a slightly altered configuration; hence from
      ## a first superficial inspection a system will
      ## be considered as Debian, which signifies an
      ## extra check is required.
      find_file(UBUNTU_EXTRA legal issue
        PATHS /etc
      )
      if(UBUNTU_EXTRA)
        ## Scan contents of file
        file(STRINGS ${UBUNTU_EXTRA} UBUNTU_FOUND
          REGEX Ubuntu
        )
        ## Check result of string search
        if(UBUNTU_FOUND)
          set(CMAKE_OS_NAME "Ubuntu" CACHE STRING "Operating system name" FORCE)
          set(DEBIAN_FOUND FALSE)
        endif(UBUNTU_FOUND)
      endif(UBUNTU_EXTRA)      
    endif(DEBIAN_FOUND)
endif(LINUX)

if(FREEBSD)
  set(OpenGL_GL_PREFERENCE LEGACY)
else()
  set(OpenGL_GL_PREFERENCE GLVND)
endif()
##################################
###    End Detect section      ###
##################################

##################################
###  Set install path section  ###
##################################
# Where to install the binaries.
if(CMAKE_INSTALL_PREFIX STREQUAL "/usr/local" OR CMAKE_INSTALL_PREFIX STREQUAL "")
    set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/../")                      
    if(NOT USE_SYSTEM_INSTALL)
        set(LOCAL_INSTALL TRUE)
        message(STATUS "...Using LOCAL Install")
    endif() 
endif()
if(CMAKE_INSTALL_PREFIX STREQUAL "/usr" OR USE_SYSTEM_INSTALL)
    set(LOCAL_INSTALL FALSE)
    message(STATUS "...Using SYSTEM Install")
    if(CMAKE_SYSTEM_NAME STREQUAL "FreeBSD" OR CMAKE_SYSTEM_NAME STREQUAL "OpenBSD")
        set(CMAKE_INSTALL_PREFIX "/usr/local")
    elseif(CMAKE_SYSTEM_NAME STREQUAL "NetBSD")
        set(CMAKE_INSTALL_PREFIX "/usr/pkg")
    else()
        set(CMAKE_INSTALL_PREFIX "/usr")  
    endif()
endif()
##################################
## End Set install path section ##
##################################

##################################
###       Find section         ###
##################################

# Find math and pthread library
find_library(MATH m REQUIRED)

# Use systemwide SDL2 or custom build
if(NOT USE_SYSTEM_SDL2)
    include_directories(${CMAKE_SOURCE_DIR}/External/SDL2)
else()
    find_package(SDL2 REQUIRED)
    if(SDL2_FOUND)
	    include_directories(${SDL2_INCLUDE_DIR})
        message(STATUS "...SDL2 library found")
    else()
        message(FATAL_ERROR "Error USE_SYSTEM_SDL2 is set but neccessary developer files are missing")
    endif()
endif()

# Use systemwide ZLIB or custom build
if(USE_SYSTEM_ZLIB)
    find_package(ZLIB REQUIRED)
    if(ZLIB_FOUND)
        message(STATUS "...ZLIB library found")
        include_directories(${ZLIB_INCLUDE_DIRS})
    else()
        message(FATAL_ERROR "Error! USE_SYSTEM_ZLIB is set but neccessary developer files are missing")
    endif()
endif()

find_package(OpenGL)
if(OpenGL_FOUND)
  message(STATUS "...OpenGL library found")
  include_directories(${OpenGL_INCLUDE_DIRS})
  if(NOT FREEBSD AND NOT CMAKE_OS_NAME STREQUAL "Alpine" AND NOT RPIOS)
    set(OpenGL_LIBS OpenGL::OpenGL)
  else()
    set(OpenGL_LIBS OpenGL::GL)
  endif()
else()
  message(FATAL_ERROR "OpenGL library not found")
endif()

if(USE_SYSTEM_OGGVORBIS)
		find_package(Vorbis REQUIRED)
		if(Vorbis_FOUND)
			find_path (Vorbis_Vorbis_INCLUDE_DIR vorbis/codec.h)
			find_path (Vorbis_Enc_INCLUDE_DIR vorbis/vorbisenc.h)
			find_path (Vorbis_File_INCLUDE_DIR vorbis/vorbisfile.h)
			set(Vorbis_Vorbis_LIBRARY Vorbis::vorbis)
			set(Vorbis_Enc_LIBRARY Vorbis::vorbisenc)
			set(Vorbis_File_LIBRARY Vorbis::vorbisfile)
			set(oggvorbis_inc ${OGG_INCLUDE_DIR} ${Vorbis_Vorbis_INCLUDE_DIR} ${Vorbis_Enc_INCLUDE_DIR} ${Vorbis_File_INCLUDE_DIR})
			set(oggvorbis_lib ${OGG_LIBRARY} ${Vorbis_Vorbis_LIBRARY} ${Vorbis_Enc_LIBRARY} ${Vorbis_File_LIBRARY})
			set(COMPILE_OGGVORBIS FALSE)
			message(STATUS "...Using system ogg/vorbis")
		else()
			set(oggvorbis_inc "externlibs/libogg/include" "externlibs/libvorbis/include")
			set(oggvorbis_lib "static_oggvorbis")
			set(COMPILE_OGGVORBIS TRUE)
			message(STATUS "...Using internal ogg/vorbis")
		endif()
else()
		set(oggvorbis_inc "externlibs/libogg/include" "externlibs/libvorbis/include")
		set(oggvorbis_lib "static_oggvorbis")
		set(COMPILE_OGGVORBIS TRUE)
		message(STATUS "...Using internal ogg/vorbis")
endif()

find_package(Minizip REQUIRED)
if(Minizip_FOUND)
  message(STATUS "...Minizip library found")
  include_directories(${Minizip_INCLUDE_DIRS})
else()
  message(FATAL_ERROR "Minizip library not found")
endif()

find_package(PNG  REQUIRED)
if(PNG_FOUND)
  message(STATUS "...PNG library found")
  include_directories(${PNG_INCLUDE_DIRS})
else()
  message(FATAL_ERROR "PNG library not found")
endif()

find_package(JPEG REQUIRED) # Find the JPEG library, error if not found
if(JPEG_FOUND)
  message(STATUS "...JPEG library found")
  include_directories(${JPEG_INCLUDE_DIRS})
else()
  message(FATAL_ERROR "JPEG library not found")
endif()

##################################
###     End Find section       ###
##################################


##################################
###    Setup stuff section     ###
##################################
# Set up some sanity stuff...
# Note:
# "-rpath,$ORIGIN" allows you not to use LD_LIBRARY_PATH and is well suited for local installation
# "-rpath,$ORIGIN" don't like new versions of clang + ninja and crashes during linking of the Engine library
# "-rpath=/tmp" is suitable for linking clang + ninja and installing the game into the system
# "-rpath=/var/tmp/portage" is suitable for Gentoo and installing the game into the system
if(CMAKE_SYSTEM_NAME STREQUAL "Linux" OR CMAKE_SYSTEM_NAME MATCHES "GNU")
    set(LINUX TRUE)
    message(STATUS "Set GNU Linux TRUE")
    if(CMAKE_OS_NAME STREQUAL "Gentoo" AND USE_SYSTEM_INSTALL)
      set(RPATH_SETTINGS "-rpath=/var/tmp/portage")
      message(STATUS "Build for Gentoo with '-rpath=/var/tmp/portage'")
    else()
      set(RPATH_SETTINGS "-rpath,$ORIGIN")
    endif()
endif()

if(FREEBSD)
    if(CMAKE_GENERATOR STREQUAL "Unix Makefiles")                                  # for local instal
      if(CMAKE_SYSTEM_PROCESSOR MATCHES "i386|i686|x86|amd64|AMD64" AND CMAKE_SIZEOF_VOID_P EQUAL 4)
        if(USE_ASM)
          set(RPATH_SETTINGS "-z,notext,-rpath,$ORIGIN")
          message(STATUS "Arch i386 using -Wl,-z,notext LD flags")
        else()
          set(RPATH_SETTINGS "-rpath,$ORIGIN")
          message(STATUS "Arch i386 portable C instead of all ASM")
        endif()
      else()
        set(RPATH_SETTINGS "-rpath,$ORIGIN")
      endif()
      message(STATUS "Using cmake generator 'Unix Makefiles'")
    elseif(CMAKE_GENERATOR STREQUAL "Ninja")                                       # defalt for install to the system
      if(CMAKE_SYSTEM_PROCESSOR MATCHES "i386|i686|x86|amd64|AMD64" AND CMAKE_SIZEOF_VOID_P EQUAL 4)
        if(USE_ASM)
          set(RPATH_SETTINGS "-z,notext,-rpath=/tmp")
          message(STATUS "Arch i386 using -Wl,-z,notext LD flags")
        else()
          set(RPATH_SETTINGS "-rpath=/tmp")
          message(STATUS "Arch i386 portable C instead of all ASM")
        endif()
      else()
        set(RPATH_SETTINGS "-rpath=/tmp")
      endif()  
      message(STATUS "Using cmake generator 'Ninja'")
    else()
        message(FATAL_ERROR "Unknown cmake generator")
    endif()
endif()

message(STATUS "Operating system name: ${CMAKE_OS_NAME}")

set(CMAKE_ARCH_BITS 32)
set(CMAKE_INSTALL_LIBDIR "/usr/lib")
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(CMAKE_ARCH_BITS 64)
    set(CMAKE_INSTALL_LIBDIR "/usr/lib64")
endif()

if(FREEBSD)
    set(CMAKE_INSTALL_LIBDIR "/usr/local/lib")
endif()
if(CMAKE_SYSTEM_NAME STREQUAL "NetBSD")
  include_directories("/usr/pkg/include")
  set(CMAKE_INSTALL_LIBDIR "/usr/pkg/lib")
endif()

message(STATUS "Arch bits: ${CMAKE_ARCH_BITS}")

if(DEBIAN_FOUND OR UBUNTU_FOUND)
    #set libdir for debian based OS
    if(${CMAKE_SYSTEM_PROCESSOR} MATCHES "riscv64")
        set(CMAKE_INSTALL_LIBDIR "/usr/lib/riscv64-linux-gnu")
    elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "^(arm.*|ARM.*)")  # 32bit)
        set(CMAKE_INSTALL_LIBDIR "/usr/lib/arm-linux-gnueabihf")
    elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "^(aarch64.*|AARCH64.*)")
        set(CMAKE_INSTALL_LIBDIR "/usr/lib/aarch64-linux-gnu")
        if(NOT "${CMAKE_SIZEOF_VOID_P}" EQUAL "8")
            message(FATAL_ERROR "invalid pointer size")
        endif()
    elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "^(powerpc|ppc)64")
        set(CMAKE_INSTALL_LIBDIR "/usr/lib/powerpc64-linux-gnu")
    elseif(CMAKE_SYSTEM_PROCESSOR STREQUAL s390x)
        set(CMAKE_INSTALL_LIBDIR "/usr/lib/s390x-linux-gnu")
    elseif((CMAKE_SYSTEM_PROCESSOR MATCHES "i386|i686|x86|amd64|AMD64") AND (CMAKE_SIZEOF_VOID_P EQUAL 4))
        set(CMAKE_INSTALL_LIBDIR "/usr/lib/i386-linux-gnu")
    elseif((CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64|amd64|AMD64") AND (CMAKE_SIZEOF_VOID_P EQUAL 8))
        set(CMAKE_INSTALL_LIBDIR "/usr/lib/x86_64-linux-gnu")
    elseif((CMAKE_SYSTEM_PROCESSOR MATCHES "i386") AND (CMAKE_SIZEOF_VOID_P EQUAL 8) AND (APPLE))
        set(CMAKE_INSTALL_LIBDIR "/usr/lib/i386-linux-gnu")
    else()
        message(FATAL_ERROR "Unknown architecture")
    endif()
endif()

message(STATUS "Installed libraries dir: ${CMAKE_INSTALL_LIBDIR}")

set(INTERNAL_NAME "berserkerq2")
set(CMAKE_ADD_TARGET_DIR "${CMAKE_SOURCE_DIR}/../")

##################################
###  End Setup stuff section   ###
##################################

##################################
### End Compiler flags section ###
##################################

if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_C_COMPILER_ID STREQUAL "Clang" OR CMAKE_C_COMPILER_ID MATCHES "AppleClang")
	# This section and the like are for flags/defines that can be shared between 
	# c and c++ compile options
	add_compile_options(-Wall)
	add_compile_options(-pipe)
	add_compile_options(-fPIC)
	if(NOT FREEBSD)
		add_compile_options(-fms-extensions)
	endif()
	if(NOT RPI4 AND NOT (MACOSX AND CMAKE_SYSTEM_PROCESSOR STREQUAL "arm64"))
		if(${CMAKE_SYSTEM_PROCESSOR} MATCHES "riscv64")
			add_compile_options(-march=rv64gc)
		    add_compile_options(-ftree-vectorize)
		    add_compile_options(-fomit-frame-pointer)
			message(STATUS "Architecture RISCV64, -march=rv64gc")
        elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "^(powerpc|ppc)64")
			add_compile_options(-mpowerpc64)
        elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "loongarch64")
			add_compile_options(-march=loongarch64)
        elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "mips|mips64|mipsel|mips64el")
			add_compile_options(-march=mips32r2)
		else()
            if(LOCAL_INSTALL)
			    add_compile_options(-march=native)
            elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "i386|i586|i686|x86|amd64|AMD64|x86_64")
			    add_compile_options(-march=native) # set mtune=generic if nedded
		    endif()
		endif()
	endif()

    if(${CMAKE_HOST_SYSTEM_PROCESSOR} MATCHES "arm*")
        if(CMAKE_SYSTEM_PROCESSOR MATCHES "^(arm.*|ARM.*)")  # 32bit
            add_compile_options(-march=armv8-a+crc)
		    add_compile_options(-mfpu=neon-fp-armv8)
            if(RPI4) 
		        add_compile_options(-mcpu=cortex-a72)
            endif()
		    add_compile_options(-mfloat-abi=hard)
		    add_compile_options(-ftree-vectorize)
		    add_compile_options(-fomit-frame-pointer)
            message(STATUS "ARM 32bit, -march=armv8-a+crc, adding -mfpu=neon-fp-armv8")
        elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "^(aarch64.*|AARCH64.*)")  # NEON is implied on aarch64
            if(NOT "${CMAKE_SIZEOF_VOID_P}" EQUAL "8")
                message(FATAL_ERROR "invalid pointer size")
            endif()
            add_compile_options(-march=armv8-a+crc+fp+simd)
            if(RPI4) 
		        add_compile_options(-mcpu=cortex-a72)
            endif()
		    add_compile_options(-ftree-vectorize)
		    add_compile_options(-fomit-frame-pointer)
            message(STATUS "AARCH64, -march=armv8-a+crc+fp+simd")
        elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "loongarch64")
            message(STATUS "Loongsoon 3 5000 series processor")
        else()
            message(FATAL_ERROR "Unknown ARM processor '${CMAKE_SYSTEM_PROCESSOR}'")
        endif()
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
            set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS} -DNDEBUG=1 -D_NDEBUG=1 -g -O3 -fno-delete-null-pointer-checks -fno-unsafe-math-optimizations")
            set(CMAKE_C_FLAGS_RELWITHDEBINFO "${CMAKE_C_FLAGS} -DNDEBUG=1 -D_NDEBUG=1 -g -O2 -fno-delete-null-pointer-checks -fno-unsafe-math-optimizations")
            set(CMAKE_C_FLAGS_MINSIZEREL "${CMAKE_C_FLAGS} -DNDEBUG=1 -D_NDEBUG=1 -Os -fno-delete-null-pointer-checks -fno-unsafe-math-optimizations")
        endif()

	# TODO fix these warnings

	if(NOT FREEBSD)
		add_compile_options(-Wno-stringop-truncation)
		add_compile_options(-Wno-format-overflow)
		add_compile_options(-Wno-restrict)
		add_compile_options(-Wno-maybe-uninitialized)
		add_compile_options(-Wno-aggressive-loop-optimizations)
		add_compile_options(-Wno-stringop-overflow)
	endif()
	add_compile_options(-Wno-format-security)
	add_compile_options(-Wno-switch)
	add_compile_options(-Wno-char-subscripts)
	add_compile_options(-Wno-unused-result)
	add_compile_options(-Wno-unknown-pragmas)
	add_compile_options(-Wno-unused-variable) # TODO: maybe only enable this for Entities
	add_compile_options(-Wno-unused-value)    # same here (the Scripts generate tons of unused variables and values)
	add_compile_options(-Wno-missing-braces)
	add_compile_options(-Wno-unused-but-set-variable)
	# MESSAGE(WARNING, "re-enable some of the warnings some day!")

	if(MACOSX)
		    add_definitions(-DPLATFORM_UNIX=1)
    		add_definitions(-DPLATFORM_MACOSX=1)
    		add_definitions(-DPRAGMA_ONCE=1)
    		include_directories("/usr/local/include")
		    include_directories("/usr/X11/include/")
	elseif(LINUX)
	    	set(CMAKE_SKIP_RPATH ON CACHE BOOL "Skip RPATH" FORCE)
    		add_definitions(-DPLATFORM_UNIX=1)
    		add_definitions(-D_FILE_OFFSET_BITS=64)
    		add_definitions(-D_LARGEFILE_SOURCE=1)
    		add_definitions(-DPRAGMA_ONCE=1)
	elseif(FREEBSD)
    		set(CMAKE_SKIP_RPATH ON CACHE BOOL "Skip RPATH" FORCE)
    		add_definitions(-DPLATFORM_UNIX=1)
    		add_definitions(-DPLATFORM_FREEBSD=1)
    		add_definitions(-D_FILE_OFFSET_BITS=64)
    		add_definitions(-D_LARGEFILE_SOURCE=1)
    		add_definitions(-DPRAGMA_ONCE=1)
    		include_directories("/usr/local/include")
	endif()
	if(CMAKE_SYSTEM_NAME STREQUAL "OpenBSD")
    		include_directories("/usr/X11R6/include")	
	endif()

    if(MACOSX OR LINUX OR FREEBSD)
		add_compile_options(-pthread)
        add_compile_options(-fsigned-char)
	endif()

else()
	message(FATAL_ERROR "Unsupported compiler")
endif()
##################################
### End Compiler flags section ###
##################################

##################################
###      Sources section       ###
##################################

set(GAMELIB "game")
    add_library(${GAMELIB} SHARED
        game.c
)

add_executable(berserkerq2
    main.c
    unpak.c
)
##################################
###    End Sources section     ###
##################################

##################################
###       Link section         ###
##################################

target_link_libraries(berserkerq2 ${GAMELIB})
set_target_properties(berserkerq2 PROPERTIES LINK_FLAGS "-Wl,${RPATH_SETTINGS}")
target_link_libraries(berserkerq2 "m")
if(LINUX)
    target_link_libraries(berserkerq2 "dl")
endif()
target_link_libraries(berserkerq2 "pthread")

target_link_libraries(berserkerq2 ${SDL2_LIBRARY})
target_link_libraries(berserkerq2 ${ZLIB_LIBRARIES})
target_link_libraries(berserkerq2 ${PNG_LIBRARIES})
target_link_libraries(berserkerq2 ${JPEG_LIBRARIES})
target_link_libraries(berserkerq2 ${MINIZIP_LIBRARIES})
target_link_libraries(berserkerq2 ${oggvorbis_lib})
target_link_libraries(berserkerq2 ${OpenGL_LIBS})

##################################
###     End Link section       ###
##################################

##################################
###      Install section       ###
##################################

if(LOCAL_INSTALL)
    install(TARGETS berserkerq2 ${GAMELIB}
            RUNTIME DESTINATION "${CMAKE_INSTALL_PREFIX}"
            LIBRARY DESTINATION "${CMAKE_INSTALL_PREFIX}/baseq2"
            PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_WRITE GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
# SYSTEM INSTALL BINARIES
elseif(NOT LOCAL_INSTALL)
    install(TARGETS berserkerq2 ${GAMELIB} 
            RUNTIME DESTINATION "${CMAKE_INSTALL_PREFIX}/bin"
            LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}/${INTERNAL_NAME}"
            PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_WRITE GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
endif()
# SYSTEM INSTALL README LICENSE
if(NOT LOCAL_INSTALL)
    install(FILES ${CMAKE_ADD_TARGET_DIR}/README.md ${CMAKE_ADD_TARGET_DIR}/LICENSE.md
            DESTINATION "${CMAKE_INSTALL_PREFIX}/share/${INTERNAL_NAME}"
            PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ)
endif()
##################################
###    End install section     ###
##################################

# end of CMakeLists.txt ...
