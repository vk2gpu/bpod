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
include apps/uartterm/CMakeFiles/AppUARTTerm.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include apps/uartterm/CMakeFiles/AppUARTTerm.dir/compiler_depend.make

# Include the progress variables for this target.
include apps/uartterm/CMakeFiles/AppUARTTerm.dir/progress.make

# Include the compile flags for this target's objects.
include apps/uartterm/CMakeFiles/AppUARTTerm.dir/flags.make

apps/uartterm/CMakeFiles/AppUARTTerm.dir/App/UARTTerm.cpp.o: apps/uartterm/CMakeFiles/AppUARTTerm.dir/flags.make
apps/uartterm/CMakeFiles/AppUARTTerm.dir/App/UARTTerm.cpp.o: /Users/vk2gpu/Dev/bpod/firmware/apps/uartterm/App/UARTTerm.cpp
apps/uartterm/CMakeFiles/AppUARTTerm.dir/App/UARTTerm.cpp.o: apps/uartterm/CMakeFiles/AppUARTTerm.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/vk2gpu/Dev/bpod/firmware/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object apps/uartterm/CMakeFiles/AppUARTTerm.dir/App/UARTTerm.cpp.o"
	cd /Users/vk2gpu/Dev/bpod/firmware/build/apps/uartterm && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT apps/uartterm/CMakeFiles/AppUARTTerm.dir/App/UARTTerm.cpp.o -MF CMakeFiles/AppUARTTerm.dir/App/UARTTerm.cpp.o.d -o CMakeFiles/AppUARTTerm.dir/App/UARTTerm.cpp.o -c /Users/vk2gpu/Dev/bpod/firmware/apps/uartterm/App/UARTTerm.cpp

apps/uartterm/CMakeFiles/AppUARTTerm.dir/App/UARTTerm.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/AppUARTTerm.dir/App/UARTTerm.cpp.i"
	cd /Users/vk2gpu/Dev/bpod/firmware/build/apps/uartterm && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/vk2gpu/Dev/bpod/firmware/apps/uartterm/App/UARTTerm.cpp > CMakeFiles/AppUARTTerm.dir/App/UARTTerm.cpp.i

apps/uartterm/CMakeFiles/AppUARTTerm.dir/App/UARTTerm.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/AppUARTTerm.dir/App/UARTTerm.cpp.s"
	cd /Users/vk2gpu/Dev/bpod/firmware/build/apps/uartterm && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/vk2gpu/Dev/bpod/firmware/apps/uartterm/App/UARTTerm.cpp -o CMakeFiles/AppUARTTerm.dir/App/UARTTerm.cpp.s

# Object files for target AppUARTTerm
AppUARTTerm_OBJECTS = \
"CMakeFiles/AppUARTTerm.dir/App/UARTTerm.cpp.o"

# External object files for target AppUARTTerm
AppUARTTerm_EXTERNAL_OBJECTS =

apps/uartterm/libAppUARTTerm.a: apps/uartterm/CMakeFiles/AppUARTTerm.dir/App/UARTTerm.cpp.o
apps/uartterm/libAppUARTTerm.a: apps/uartterm/CMakeFiles/AppUARTTerm.dir/build.make
apps/uartterm/libAppUARTTerm.a: apps/uartterm/CMakeFiles/AppUARTTerm.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/vk2gpu/Dev/bpod/firmware/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library libAppUARTTerm.a"
	cd /Users/vk2gpu/Dev/bpod/firmware/build/apps/uartterm && $(CMAKE_COMMAND) -P CMakeFiles/AppUARTTerm.dir/cmake_clean_target.cmake
	cd /Users/vk2gpu/Dev/bpod/firmware/build/apps/uartterm && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/AppUARTTerm.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
apps/uartterm/CMakeFiles/AppUARTTerm.dir/build: apps/uartterm/libAppUARTTerm.a
.PHONY : apps/uartterm/CMakeFiles/AppUARTTerm.dir/build

apps/uartterm/CMakeFiles/AppUARTTerm.dir/clean:
	cd /Users/vk2gpu/Dev/bpod/firmware/build/apps/uartterm && $(CMAKE_COMMAND) -P CMakeFiles/AppUARTTerm.dir/cmake_clean.cmake
.PHONY : apps/uartterm/CMakeFiles/AppUARTTerm.dir/clean

apps/uartterm/CMakeFiles/AppUARTTerm.dir/depend:
	cd /Users/vk2gpu/Dev/bpod/firmware/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/vk2gpu/Dev/bpod/firmware /Users/vk2gpu/Dev/bpod/firmware/apps/uartterm /Users/vk2gpu/Dev/bpod/firmware/build /Users/vk2gpu/Dev/bpod/firmware/build/apps/uartterm /Users/vk2gpu/Dev/bpod/firmware/build/apps/uartterm/CMakeFiles/AppUARTTerm.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : apps/uartterm/CMakeFiles/AppUARTTerm.dir/depend

