# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.20

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
CMAKE_COMMAND = /home/student/Clion/CLion-2021.2.3/clion-2021.2.3/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /home/student/Clion/CLion-2021.2.3/clion-2021.2.3/bin/cmake/linux/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/student/CLionProjects/Version8MK3

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/student/CLionProjects/Version8MK3/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/Version8MK3.dir/depend.make
# Include the progress variables for this target.
include CMakeFiles/Version8MK3.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Version8MK3.dir/flags.make

CMakeFiles/Version8MK3.dir/main.c.o: CMakeFiles/Version8MK3.dir/flags.make
CMakeFiles/Version8MK3.dir/main.c.o: ../main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/student/CLionProjects/Version8MK3/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/Version8MK3.dir/main.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/Version8MK3.dir/main.c.o -c /home/student/CLionProjects/Version8MK3/main.c

CMakeFiles/Version8MK3.dir/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Version8MK3.dir/main.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/student/CLionProjects/Version8MK3/main.c > CMakeFiles/Version8MK3.dir/main.c.i

CMakeFiles/Version8MK3.dir/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Version8MK3.dir/main.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/student/CLionProjects/Version8MK3/main.c -o CMakeFiles/Version8MK3.dir/main.c.s

CMakeFiles/Version8MK3.dir/debug.c.o: CMakeFiles/Version8MK3.dir/flags.make
CMakeFiles/Version8MK3.dir/debug.c.o: ../debug.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/student/CLionProjects/Version8MK3/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/Version8MK3.dir/debug.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/Version8MK3.dir/debug.c.o -c /home/student/CLionProjects/Version8MK3/debug.c

CMakeFiles/Version8MK3.dir/debug.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Version8MK3.dir/debug.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/student/CLionProjects/Version8MK3/debug.c > CMakeFiles/Version8MK3.dir/debug.c.i

CMakeFiles/Version8MK3.dir/debug.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Version8MK3.dir/debug.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/student/CLionProjects/Version8MK3/debug.c -o CMakeFiles/Version8MK3.dir/debug.c.s

CMakeFiles/Version8MK3.dir/bigint/src/bigint.c.o: CMakeFiles/Version8MK3.dir/flags.make
CMakeFiles/Version8MK3.dir/bigint/src/bigint.c.o: ../bigint/src/bigint.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/student/CLionProjects/Version8MK3/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object CMakeFiles/Version8MK3.dir/bigint/src/bigint.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/Version8MK3.dir/bigint/src/bigint.c.o -c /home/student/CLionProjects/Version8MK3/bigint/src/bigint.c

CMakeFiles/Version8MK3.dir/bigint/src/bigint.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Version8MK3.dir/bigint/src/bigint.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/student/CLionProjects/Version8MK3/bigint/src/bigint.c > CMakeFiles/Version8MK3.dir/bigint/src/bigint.c.i

CMakeFiles/Version8MK3.dir/bigint/src/bigint.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Version8MK3.dir/bigint/src/bigint.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/student/CLionProjects/Version8MK3/bigint/src/bigint.c -o CMakeFiles/Version8MK3.dir/bigint/src/bigint.c.s

CMakeFiles/Version8MK3.dir/bigint/tst/support.c.o: CMakeFiles/Version8MK3.dir/flags.make
CMakeFiles/Version8MK3.dir/bigint/tst/support.c.o: ../bigint/tst/support.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/student/CLionProjects/Version8MK3/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building C object CMakeFiles/Version8MK3.dir/bigint/tst/support.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/Version8MK3.dir/bigint/tst/support.c.o -c /home/student/CLionProjects/Version8MK3/bigint/tst/support.c

CMakeFiles/Version8MK3.dir/bigint/tst/support.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Version8MK3.dir/bigint/tst/support.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/student/CLionProjects/Version8MK3/bigint/tst/support.c > CMakeFiles/Version8MK3.dir/bigint/tst/support.c.i

CMakeFiles/Version8MK3.dir/bigint/tst/support.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Version8MK3.dir/bigint/tst/support.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/student/CLionProjects/Version8MK3/bigint/tst/support.c -o CMakeFiles/Version8MK3.dir/bigint/tst/support.c.s

# Object files for target Version8MK3
Version8MK3_OBJECTS = \
"CMakeFiles/Version8MK3.dir/main.c.o" \
"CMakeFiles/Version8MK3.dir/debug.c.o" \
"CMakeFiles/Version8MK3.dir/bigint/src/bigint.c.o" \
"CMakeFiles/Version8MK3.dir/bigint/tst/support.c.o"

# External object files for target Version8MK3
Version8MK3_EXTERNAL_OBJECTS =

Version8MK3: CMakeFiles/Version8MK3.dir/main.c.o
Version8MK3: CMakeFiles/Version8MK3.dir/debug.c.o
Version8MK3: CMakeFiles/Version8MK3.dir/bigint/src/bigint.c.o
Version8MK3: CMakeFiles/Version8MK3.dir/bigint/tst/support.c.o
Version8MK3: CMakeFiles/Version8MK3.dir/build.make
Version8MK3: CMakeFiles/Version8MK3.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/student/CLionProjects/Version8MK3/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Linking C executable Version8MK3"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Version8MK3.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Version8MK3.dir/build: Version8MK3
.PHONY : CMakeFiles/Version8MK3.dir/build

CMakeFiles/Version8MK3.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Version8MK3.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Version8MK3.dir/clean

CMakeFiles/Version8MK3.dir/depend:
	cd /home/student/CLionProjects/Version8MK3/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/student/CLionProjects/Version8MK3 /home/student/CLionProjects/Version8MK3 /home/student/CLionProjects/Version8MK3/cmake-build-debug /home/student/CLionProjects/Version8MK3/cmake-build-debug /home/student/CLionProjects/Version8MK3/cmake-build-debug/CMakeFiles/Version8MK3.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Version8MK3.dir/depend

