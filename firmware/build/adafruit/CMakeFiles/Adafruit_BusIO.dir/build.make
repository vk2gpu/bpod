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
include adafruit/CMakeFiles/Adafruit_BusIO.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include adafruit/CMakeFiles/Adafruit_BusIO.dir/compiler_depend.make

# Include the progress variables for this target.
include adafruit/CMakeFiles/Adafruit_BusIO.dir/progress.make

# Include the compile flags for this target's objects.
include adafruit/CMakeFiles/Adafruit_BusIO.dir/flags.make

adafruit/CMakeFiles/Adafruit_BusIO.dir/Adafruit_BusIO/Adafruit_BusIO_Register.cpp.o: adafruit/CMakeFiles/Adafruit_BusIO.dir/flags.make
adafruit/CMakeFiles/Adafruit_BusIO.dir/Adafruit_BusIO/Adafruit_BusIO_Register.cpp.o: /Users/vk2gpu/Dev/bpod/firmware/adafruit/Adafruit_BusIO/Adafruit_BusIO_Register.cpp
adafruit/CMakeFiles/Adafruit_BusIO.dir/Adafruit_BusIO/Adafruit_BusIO_Register.cpp.o: adafruit/CMakeFiles/Adafruit_BusIO.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/vk2gpu/Dev/bpod/firmware/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object adafruit/CMakeFiles/Adafruit_BusIO.dir/Adafruit_BusIO/Adafruit_BusIO_Register.cpp.o"
	cd /Users/vk2gpu/Dev/bpod/firmware/build/adafruit && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT adafruit/CMakeFiles/Adafruit_BusIO.dir/Adafruit_BusIO/Adafruit_BusIO_Register.cpp.o -MF CMakeFiles/Adafruit_BusIO.dir/Adafruit_BusIO/Adafruit_BusIO_Register.cpp.o.d -o CMakeFiles/Adafruit_BusIO.dir/Adafruit_BusIO/Adafruit_BusIO_Register.cpp.o -c /Users/vk2gpu/Dev/bpod/firmware/adafruit/Adafruit_BusIO/Adafruit_BusIO_Register.cpp

adafruit/CMakeFiles/Adafruit_BusIO.dir/Adafruit_BusIO/Adafruit_BusIO_Register.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Adafruit_BusIO.dir/Adafruit_BusIO/Adafruit_BusIO_Register.cpp.i"
	cd /Users/vk2gpu/Dev/bpod/firmware/build/adafruit && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/vk2gpu/Dev/bpod/firmware/adafruit/Adafruit_BusIO/Adafruit_BusIO_Register.cpp > CMakeFiles/Adafruit_BusIO.dir/Adafruit_BusIO/Adafruit_BusIO_Register.cpp.i

adafruit/CMakeFiles/Adafruit_BusIO.dir/Adafruit_BusIO/Adafruit_BusIO_Register.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Adafruit_BusIO.dir/Adafruit_BusIO/Adafruit_BusIO_Register.cpp.s"
	cd /Users/vk2gpu/Dev/bpod/firmware/build/adafruit && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/vk2gpu/Dev/bpod/firmware/adafruit/Adafruit_BusIO/Adafruit_BusIO_Register.cpp -o CMakeFiles/Adafruit_BusIO.dir/Adafruit_BusIO/Adafruit_BusIO_Register.cpp.s

adafruit/CMakeFiles/Adafruit_BusIO.dir/Adafruit_BusIO/Adafruit_I2CDevice.cpp.o: adafruit/CMakeFiles/Adafruit_BusIO.dir/flags.make
adafruit/CMakeFiles/Adafruit_BusIO.dir/Adafruit_BusIO/Adafruit_I2CDevice.cpp.o: /Users/vk2gpu/Dev/bpod/firmware/adafruit/Adafruit_BusIO/Adafruit_I2CDevice.cpp
adafruit/CMakeFiles/Adafruit_BusIO.dir/Adafruit_BusIO/Adafruit_I2CDevice.cpp.o: adafruit/CMakeFiles/Adafruit_BusIO.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/vk2gpu/Dev/bpod/firmware/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object adafruit/CMakeFiles/Adafruit_BusIO.dir/Adafruit_BusIO/Adafruit_I2CDevice.cpp.o"
	cd /Users/vk2gpu/Dev/bpod/firmware/build/adafruit && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT adafruit/CMakeFiles/Adafruit_BusIO.dir/Adafruit_BusIO/Adafruit_I2CDevice.cpp.o -MF CMakeFiles/Adafruit_BusIO.dir/Adafruit_BusIO/Adafruit_I2CDevice.cpp.o.d -o CMakeFiles/Adafruit_BusIO.dir/Adafruit_BusIO/Adafruit_I2CDevice.cpp.o -c /Users/vk2gpu/Dev/bpod/firmware/adafruit/Adafruit_BusIO/Adafruit_I2CDevice.cpp

adafruit/CMakeFiles/Adafruit_BusIO.dir/Adafruit_BusIO/Adafruit_I2CDevice.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Adafruit_BusIO.dir/Adafruit_BusIO/Adafruit_I2CDevice.cpp.i"
	cd /Users/vk2gpu/Dev/bpod/firmware/build/adafruit && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/vk2gpu/Dev/bpod/firmware/adafruit/Adafruit_BusIO/Adafruit_I2CDevice.cpp > CMakeFiles/Adafruit_BusIO.dir/Adafruit_BusIO/Adafruit_I2CDevice.cpp.i

adafruit/CMakeFiles/Adafruit_BusIO.dir/Adafruit_BusIO/Adafruit_I2CDevice.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Adafruit_BusIO.dir/Adafruit_BusIO/Adafruit_I2CDevice.cpp.s"
	cd /Users/vk2gpu/Dev/bpod/firmware/build/adafruit && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/vk2gpu/Dev/bpod/firmware/adafruit/Adafruit_BusIO/Adafruit_I2CDevice.cpp -o CMakeFiles/Adafruit_BusIO.dir/Adafruit_BusIO/Adafruit_I2CDevice.cpp.s

adafruit/CMakeFiles/Adafruit_BusIO.dir/Adafruit_BusIO/Adafruit_SPIDevice.cpp.o: adafruit/CMakeFiles/Adafruit_BusIO.dir/flags.make
adafruit/CMakeFiles/Adafruit_BusIO.dir/Adafruit_BusIO/Adafruit_SPIDevice.cpp.o: /Users/vk2gpu/Dev/bpod/firmware/adafruit/Adafruit_BusIO/Adafruit_SPIDevice.cpp
adafruit/CMakeFiles/Adafruit_BusIO.dir/Adafruit_BusIO/Adafruit_SPIDevice.cpp.o: adafruit/CMakeFiles/Adafruit_BusIO.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/vk2gpu/Dev/bpod/firmware/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object adafruit/CMakeFiles/Adafruit_BusIO.dir/Adafruit_BusIO/Adafruit_SPIDevice.cpp.o"
	cd /Users/vk2gpu/Dev/bpod/firmware/build/adafruit && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT adafruit/CMakeFiles/Adafruit_BusIO.dir/Adafruit_BusIO/Adafruit_SPIDevice.cpp.o -MF CMakeFiles/Adafruit_BusIO.dir/Adafruit_BusIO/Adafruit_SPIDevice.cpp.o.d -o CMakeFiles/Adafruit_BusIO.dir/Adafruit_BusIO/Adafruit_SPIDevice.cpp.o -c /Users/vk2gpu/Dev/bpod/firmware/adafruit/Adafruit_BusIO/Adafruit_SPIDevice.cpp

adafruit/CMakeFiles/Adafruit_BusIO.dir/Adafruit_BusIO/Adafruit_SPIDevice.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Adafruit_BusIO.dir/Adafruit_BusIO/Adafruit_SPIDevice.cpp.i"
	cd /Users/vk2gpu/Dev/bpod/firmware/build/adafruit && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/vk2gpu/Dev/bpod/firmware/adafruit/Adafruit_BusIO/Adafruit_SPIDevice.cpp > CMakeFiles/Adafruit_BusIO.dir/Adafruit_BusIO/Adafruit_SPIDevice.cpp.i

adafruit/CMakeFiles/Adafruit_BusIO.dir/Adafruit_BusIO/Adafruit_SPIDevice.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Adafruit_BusIO.dir/Adafruit_BusIO/Adafruit_SPIDevice.cpp.s"
	cd /Users/vk2gpu/Dev/bpod/firmware/build/adafruit && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/vk2gpu/Dev/bpod/firmware/adafruit/Adafruit_BusIO/Adafruit_SPIDevice.cpp -o CMakeFiles/Adafruit_BusIO.dir/Adafruit_BusIO/Adafruit_SPIDevice.cpp.s

# Object files for target Adafruit_BusIO
Adafruit_BusIO_OBJECTS = \
"CMakeFiles/Adafruit_BusIO.dir/Adafruit_BusIO/Adafruit_BusIO_Register.cpp.o" \
"CMakeFiles/Adafruit_BusIO.dir/Adafruit_BusIO/Adafruit_I2CDevice.cpp.o" \
"CMakeFiles/Adafruit_BusIO.dir/Adafruit_BusIO/Adafruit_SPIDevice.cpp.o"

# External object files for target Adafruit_BusIO
Adafruit_BusIO_EXTERNAL_OBJECTS =

adafruit/libAdafruit_BusIO.a: adafruit/CMakeFiles/Adafruit_BusIO.dir/Adafruit_BusIO/Adafruit_BusIO_Register.cpp.o
adafruit/libAdafruit_BusIO.a: adafruit/CMakeFiles/Adafruit_BusIO.dir/Adafruit_BusIO/Adafruit_I2CDevice.cpp.o
adafruit/libAdafruit_BusIO.a: adafruit/CMakeFiles/Adafruit_BusIO.dir/Adafruit_BusIO/Adafruit_SPIDevice.cpp.o
adafruit/libAdafruit_BusIO.a: adafruit/CMakeFiles/Adafruit_BusIO.dir/build.make
adafruit/libAdafruit_BusIO.a: adafruit/CMakeFiles/Adafruit_BusIO.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/vk2gpu/Dev/bpod/firmware/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX static library libAdafruit_BusIO.a"
	cd /Users/vk2gpu/Dev/bpod/firmware/build/adafruit && $(CMAKE_COMMAND) -P CMakeFiles/Adafruit_BusIO.dir/cmake_clean_target.cmake
	cd /Users/vk2gpu/Dev/bpod/firmware/build/adafruit && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Adafruit_BusIO.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
adafruit/CMakeFiles/Adafruit_BusIO.dir/build: adafruit/libAdafruit_BusIO.a
.PHONY : adafruit/CMakeFiles/Adafruit_BusIO.dir/build

adafruit/CMakeFiles/Adafruit_BusIO.dir/clean:
	cd /Users/vk2gpu/Dev/bpod/firmware/build/adafruit && $(CMAKE_COMMAND) -P CMakeFiles/Adafruit_BusIO.dir/cmake_clean.cmake
.PHONY : adafruit/CMakeFiles/Adafruit_BusIO.dir/clean

adafruit/CMakeFiles/Adafruit_BusIO.dir/depend:
	cd /Users/vk2gpu/Dev/bpod/firmware/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/vk2gpu/Dev/bpod/firmware /Users/vk2gpu/Dev/bpod/firmware/adafruit /Users/vk2gpu/Dev/bpod/firmware/build /Users/vk2gpu/Dev/bpod/firmware/build/adafruit /Users/vk2gpu/Dev/bpod/firmware/build/adafruit/CMakeFiles/Adafruit_BusIO.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : adafruit/CMakeFiles/Adafruit_BusIO.dir/depend

