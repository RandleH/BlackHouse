# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.18

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
CMAKE_COMMAND = /usr/local/Cellar/cmake/3.18.2/bin/cmake

# The command to remove a file.
RM = /usr/local/Cellar/cmake/3.18.2/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/randle_h/desktop/BlackHouse/CMake

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/randle_h/desktop/BlackHouse/CMake

# Include any dependencies generated for this target.
include CMakeFiles/RH_time.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/RH_time.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/RH_time.dir/flags.make

CMakeFiles/RH_time.dir/Users/randle_h/desktop/BlackHouse/RH_time.c.o: CMakeFiles/RH_time.dir/flags.make
CMakeFiles/RH_time.dir/Users/randle_h/desktop/BlackHouse/RH_time.c.o: /Users/randle_h/desktop/BlackHouse/RH_time.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/randle_h/desktop/BlackHouse/CMake/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/RH_time.dir/Users/randle_h/desktop/BlackHouse/RH_time.c.o"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/RH_time.dir/Users/randle_h/desktop/BlackHouse/RH_time.c.o -c /Users/randle_h/desktop/BlackHouse/RH_time.c

CMakeFiles/RH_time.dir/Users/randle_h/desktop/BlackHouse/RH_time.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/RH_time.dir/Users/randle_h/desktop/BlackHouse/RH_time.c.i"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/randle_h/desktop/BlackHouse/RH_time.c > CMakeFiles/RH_time.dir/Users/randle_h/desktop/BlackHouse/RH_time.c.i

CMakeFiles/RH_time.dir/Users/randle_h/desktop/BlackHouse/RH_time.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/RH_time.dir/Users/randle_h/desktop/BlackHouse/RH_time.c.s"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/randle_h/desktop/BlackHouse/RH_time.c -o CMakeFiles/RH_time.dir/Users/randle_h/desktop/BlackHouse/RH_time.c.s

# Object files for target RH_time
RH_time_OBJECTS = \
"CMakeFiles/RH_time.dir/Users/randle_h/desktop/BlackHouse/RH_time.c.o"

# External object files for target RH_time
RH_time_EXTERNAL_OBJECTS =

libRH_time.a: CMakeFiles/RH_time.dir/Users/randle_h/desktop/BlackHouse/RH_time.c.o
libRH_time.a: CMakeFiles/RH_time.dir/build.make
libRH_time.a: CMakeFiles/RH_time.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/randle_h/desktop/BlackHouse/CMake/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C static library libRH_time.a"
	$(CMAKE_COMMAND) -P CMakeFiles/RH_time.dir/cmake_clean_target.cmake
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/RH_time.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/RH_time.dir/build: libRH_time.a

.PHONY : CMakeFiles/RH_time.dir/build

CMakeFiles/RH_time.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/RH_time.dir/cmake_clean.cmake
.PHONY : CMakeFiles/RH_time.dir/clean

CMakeFiles/RH_time.dir/depend:
	cd /Users/randle_h/desktop/BlackHouse/CMake && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/randle_h/desktop/BlackHouse/CMake /Users/randle_h/desktop/BlackHouse/CMake /Users/randle_h/desktop/BlackHouse/CMake /Users/randle_h/desktop/BlackHouse/CMake /Users/randle_h/desktop/BlackHouse/CMake/CMakeFiles/RH_time.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/RH_time.dir/depend

