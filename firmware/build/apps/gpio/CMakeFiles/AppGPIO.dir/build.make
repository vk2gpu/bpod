# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.26

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /opt/homebrew/Cellar/cmake/3.26.4/bin/cmake

# The command to remove a file.
RM = /opt/homebrew/Cellar/cmake/3.26.4/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/vk2gpu/Dev/bpod/firmware

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/vk2gpu/Dev/bpod/firmware/build

# Include any dependencies generated for this target.
include apps/gpio/CMakeFiles/AppGPIO.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include apps/gpio/CMakeFiles/AppGPIO.dir/compiler_depend.make

# Include the progress variables for this target.
include apps/gpio/CMakeFiles/AppGPIO.dir/progress.make

# Include the compile flags for this target's objects.
include apps/gpio/CMakeFiles/AppGPIO.dir/flags.make

apps/gpio/CMakeFiles/AppGPIO.dir/App/GPIO.cpp.o: apps/gpio/CMakeFiles/AppGPIO.dir/flags.make
apps/gpio/CMakeFiles/AppGPIO.dir/App/GPIO.cpp.o: /Users/vk2gpu/Dev/bpod/firmware/apps/gpio/App/GPIO.cpp
apps/gpio/CMakeFiles/AppGPIO.dir/App/GPIO.cpp.o: apps/gpio/CMakeFiles/AppGPIO.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/vk2gpu/Dev/bpod/firmware/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object apps/gpio/CMakeFiles/AppGPIO.dir/App/GPIO.cpp.o"
	cd /Users/vk2gpu/Dev/bpod/firmware/build/apps/gpio && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT apps/gpio/CMakeFiles/AppGPIO.dir/App/GPIO.cpp.o -MF CMakeFiles/AppGPIO.dir/App/GPIO.cpp.o.d -o CMakeFiles/AppGPIO.dir/App/GPIO.cpp.o -c /Users/vk2gpu/Dev/bpod/firmware/apps/gpio/App/GPIO.cpp

apps/gpio/CMakeFiles/AppGPIO.dir/App/GPIO.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/AppGPIO.dir/App/GPIO.cpp.i"
	cd /Users/vk2gpu/Dev/bpod/firmware/build/apps/gpio && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/vk2gpu/Dev/bpod/firmware/apps/gpio/App/GPIO.cpp > CMakeFiles/AppGPIO.dir/App/GPIO.cpp.i

apps/gpio/CMakeFiles/AppGPIO.dir/App/GPIO.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/AppGPIO.dir/App/GPIO.cpp.s"
	cd /Users/vk2gpu/Dev/bpod/firmware/build/apps/gpio && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/vk2gpu/Dev/bpod/firmware/apps/gpio/App/GPIO.cpp -o CMakeFiles/AppGPIO.dir/App/GPIO.cpp.s

# Object files for target AppGPIO
AppGPIO_OBJECTS = \
"CMakeFiles/AppGPIO.dir/App/GPIO.cpp.o"

# External object files for target AppGPIO
AppGPIO_EXTERNAL_OBJECTS =

apps/gpio/libAppGPIO.a: apps/gpio/CMakeFiles/AppGPIO.dir/App/GPIO.cpp.o
apps/gpio/libAppGPIO.a: apps/gpio/CMakeFiles/AppGPIO.dir/build.make
apps/gpio/libAppGPIO.a: apps/gpio/CMakeFiles/AppGPIO.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/vk2gpu/Dev/bpod/firmware/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library libAppGPIO.a"
	cd /Users/vk2gpu/Dev/bpod/firmware/build/apps/gpio && $(CMAKE_COMMAND) -P CMakeFiles/AppGPIO.dir/cmake_clean_target.cmake
	cd /Users/vk2gpu/Dev/bpod/firmware/build/apps/gpio && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/AppGPIO.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
apps/gpio/CMakeFiles/AppGPIO.dir/build: apps/gpio/libAppGPIO.a
.PHONY : apps/gpio/CMakeFiles/AppGPIO.dir/build

apps/gpio/CMakeFiles/AppGPIO.dir/clean:
	cd /Users/vk2gpu/Dev/bpod/firmware/build/apps/gpio && $(CMAKE_COMMAND) -P CMakeFiles/AppGPIO.dir/cmake_clean.cmake
.PHONY : apps/gpio/CMakeFiles/AppGPIO.dir/clean

apps/gpio/CMakeFiles/AppGPIO.dir/depend:
	cd /Users/vk2gpu/Dev/bpod/firmware/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/vk2gpu/Dev/bpod/firmware /Users/vk2gpu/Dev/bpod/firmware/apps/gpio /Users/vk2gpu/Dev/bpod/firmware/build /Users/vk2gpu/Dev/bpod/firmware/build/apps/gpio /Users/vk2gpu/Dev/bpod/firmware/build/apps/gpio/CMakeFiles/AppGPIO.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : apps/gpio/CMakeFiles/AppGPIO.dir/depend

