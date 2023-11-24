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
include tinyaes/CMakeFiles/tinyaes.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include tinyaes/CMakeFiles/tinyaes.dir/compiler_depend.make

# Include the progress variables for this target.
include tinyaes/CMakeFiles/tinyaes.dir/progress.make

# Include the compile flags for this target's objects.
include tinyaes/CMakeFiles/tinyaes.dir/flags.make

tinyaes/CMakeFiles/tinyaes.dir/src/tinyaes.c.o: tinyaes/CMakeFiles/tinyaes.dir/flags.make
tinyaes/CMakeFiles/tinyaes.dir/src/tinyaes.c.o: /Users/vk2gpu/Dev/bpod/firmware/tinyaes/src/tinyaes.c
tinyaes/CMakeFiles/tinyaes.dir/src/tinyaes.c.o: tinyaes/CMakeFiles/tinyaes.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/vk2gpu/Dev/bpod/firmware/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object tinyaes/CMakeFiles/tinyaes.dir/src/tinyaes.c.o"
	cd /Users/vk2gpu/Dev/bpod/firmware/build/tinyaes && /Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT tinyaes/CMakeFiles/tinyaes.dir/src/tinyaes.c.o -MF CMakeFiles/tinyaes.dir/src/tinyaes.c.o.d -o CMakeFiles/tinyaes.dir/src/tinyaes.c.o -c /Users/vk2gpu/Dev/bpod/firmware/tinyaes/src/tinyaes.c

tinyaes/CMakeFiles/tinyaes.dir/src/tinyaes.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/tinyaes.dir/src/tinyaes.c.i"
	cd /Users/vk2gpu/Dev/bpod/firmware/build/tinyaes && /Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/vk2gpu/Dev/bpod/firmware/tinyaes/src/tinyaes.c > CMakeFiles/tinyaes.dir/src/tinyaes.c.i

tinyaes/CMakeFiles/tinyaes.dir/src/tinyaes.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/tinyaes.dir/src/tinyaes.c.s"
	cd /Users/vk2gpu/Dev/bpod/firmware/build/tinyaes && /Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/vk2gpu/Dev/bpod/firmware/tinyaes/src/tinyaes.c -o CMakeFiles/tinyaes.dir/src/tinyaes.c.s

# Object files for target tinyaes
tinyaes_OBJECTS = \
"CMakeFiles/tinyaes.dir/src/tinyaes.c.o"

# External object files for target tinyaes
tinyaes_EXTERNAL_OBJECTS =

tinyaes/libtinyaes.a: tinyaes/CMakeFiles/tinyaes.dir/src/tinyaes.c.o
tinyaes/libtinyaes.a: tinyaes/CMakeFiles/tinyaes.dir/build.make
tinyaes/libtinyaes.a: tinyaes/CMakeFiles/tinyaes.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/vk2gpu/Dev/bpod/firmware/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C static library libtinyaes.a"
	cd /Users/vk2gpu/Dev/bpod/firmware/build/tinyaes && $(CMAKE_COMMAND) -P CMakeFiles/tinyaes.dir/cmake_clean_target.cmake
	cd /Users/vk2gpu/Dev/bpod/firmware/build/tinyaes && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/tinyaes.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
tinyaes/CMakeFiles/tinyaes.dir/build: tinyaes/libtinyaes.a
.PHONY : tinyaes/CMakeFiles/tinyaes.dir/build

tinyaes/CMakeFiles/tinyaes.dir/clean:
	cd /Users/vk2gpu/Dev/bpod/firmware/build/tinyaes && $(CMAKE_COMMAND) -P CMakeFiles/tinyaes.dir/cmake_clean.cmake
.PHONY : tinyaes/CMakeFiles/tinyaes.dir/clean

tinyaes/CMakeFiles/tinyaes.dir/depend:
	cd /Users/vk2gpu/Dev/bpod/firmware/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/vk2gpu/Dev/bpod/firmware /Users/vk2gpu/Dev/bpod/firmware/tinyaes /Users/vk2gpu/Dev/bpod/firmware/build /Users/vk2gpu/Dev/bpod/firmware/build/tinyaes /Users/vk2gpu/Dev/bpod/firmware/build/tinyaes/CMakeFiles/tinyaes.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : tinyaes/CMakeFiles/tinyaes.dir/depend

