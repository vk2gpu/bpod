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
include schedulec/CMakeFiles/schedulec.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include schedulec/CMakeFiles/schedulec.dir/compiler_depend.make

# Include the progress variables for this target.
include schedulec/CMakeFiles/schedulec.dir/progress.make

# Include the compile flags for this target's objects.
include schedulec/CMakeFiles/schedulec.dir/flags.make

schedulec/schedule_data.h: /Users/vk2gpu/Dev/bpod/schedule/README.md
schedulec/schedule_data.h: /Users/vk2gpu/Dev/bpod/firmware/schedulec/schedule_encoder.py
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/Users/vk2gpu/Dev/bpod/firmware/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Generating schedule_data.h"
	cd /Users/vk2gpu/Dev/bpod/firmware/build/schedulec && python3 /Users/vk2gpu/Dev/bpod/firmware/schedulec/schedule_encoder.py --in-schedule-md /Users/vk2gpu/Dev/bpod/firmware/schedulec/../../schedule/README.md --out-h /Users/vk2gpu/Dev/bpod/firmware/build/schedulec/schedule_data.h

schedulec/CMakeFiles/schedulec.dir/src/schedulec.c.o: schedulec/CMakeFiles/schedulec.dir/flags.make
schedulec/CMakeFiles/schedulec.dir/src/schedulec.c.o: /Users/vk2gpu/Dev/bpod/firmware/schedulec/src/schedulec.c
schedulec/CMakeFiles/schedulec.dir/src/schedulec.c.o: schedulec/CMakeFiles/schedulec.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/vk2gpu/Dev/bpod/firmware/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object schedulec/CMakeFiles/schedulec.dir/src/schedulec.c.o"
	cd /Users/vk2gpu/Dev/bpod/firmware/build/schedulec && /Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT schedulec/CMakeFiles/schedulec.dir/src/schedulec.c.o -MF CMakeFiles/schedulec.dir/src/schedulec.c.o.d -o CMakeFiles/schedulec.dir/src/schedulec.c.o -c /Users/vk2gpu/Dev/bpod/firmware/schedulec/src/schedulec.c

schedulec/CMakeFiles/schedulec.dir/src/schedulec.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/schedulec.dir/src/schedulec.c.i"
	cd /Users/vk2gpu/Dev/bpod/firmware/build/schedulec && /Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/vk2gpu/Dev/bpod/firmware/schedulec/src/schedulec.c > CMakeFiles/schedulec.dir/src/schedulec.c.i

schedulec/CMakeFiles/schedulec.dir/src/schedulec.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/schedulec.dir/src/schedulec.c.s"
	cd /Users/vk2gpu/Dev/bpod/firmware/build/schedulec && /Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/vk2gpu/Dev/bpod/firmware/schedulec/src/schedulec.c -o CMakeFiles/schedulec.dir/src/schedulec.c.s

# Object files for target schedulec
schedulec_OBJECTS = \
"CMakeFiles/schedulec.dir/src/schedulec.c.o"

# External object files for target schedulec
schedulec_EXTERNAL_OBJECTS =

schedulec/libschedulec.a: schedulec/CMakeFiles/schedulec.dir/src/schedulec.c.o
schedulec/libschedulec.a: schedulec/CMakeFiles/schedulec.dir/build.make
schedulec/libschedulec.a: schedulec/CMakeFiles/schedulec.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/vk2gpu/Dev/bpod/firmware/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking C static library libschedulec.a"
	cd /Users/vk2gpu/Dev/bpod/firmware/build/schedulec && $(CMAKE_COMMAND) -P CMakeFiles/schedulec.dir/cmake_clean_target.cmake
	cd /Users/vk2gpu/Dev/bpod/firmware/build/schedulec && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/schedulec.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
schedulec/CMakeFiles/schedulec.dir/build: schedulec/libschedulec.a
.PHONY : schedulec/CMakeFiles/schedulec.dir/build

schedulec/CMakeFiles/schedulec.dir/clean:
	cd /Users/vk2gpu/Dev/bpod/firmware/build/schedulec && $(CMAKE_COMMAND) -P CMakeFiles/schedulec.dir/cmake_clean.cmake
.PHONY : schedulec/CMakeFiles/schedulec.dir/clean

schedulec/CMakeFiles/schedulec.dir/depend: schedulec/schedule_data.h
	cd /Users/vk2gpu/Dev/bpod/firmware/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/vk2gpu/Dev/bpod/firmware /Users/vk2gpu/Dev/bpod/firmware/schedulec /Users/vk2gpu/Dev/bpod/firmware/build /Users/vk2gpu/Dev/bpod/firmware/build/schedulec /Users/vk2gpu/Dev/bpod/firmware/build/schedulec/CMakeFiles/schedulec.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : schedulec/CMakeFiles/schedulec.dir/depend

