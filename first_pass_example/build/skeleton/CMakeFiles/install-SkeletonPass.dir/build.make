# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.29

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
CMAKE_COMMAND = /usr/local/Cellar/cmake/3.29.2/bin/cmake

# The command to remove a file.
RM = /usr/local/Cellar/cmake/3.29.2/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/lorenzo/Developer/cyfes-project-cfi/first_pass_example

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/lorenzo/Developer/cyfes-project-cfi/first_pass_example/build

# Utility rule file for install-SkeletonPass.

# Include any custom commands dependencies for this target.
include skeleton/CMakeFiles/install-SkeletonPass.dir/compiler_depend.make

# Include the progress variables for this target.
include skeleton/CMakeFiles/install-SkeletonPass.dir/progress.make

skeleton/CMakeFiles/install-SkeletonPass:
	cd /Users/lorenzo/Developer/cyfes-project-cfi/first_pass_example/build/skeleton && /usr/local/Cellar/cmake/3.29.2/bin/cmake -DCMAKE_INSTALL_COMPONENT="SkeletonPass" -P /Users/lorenzo/Developer/cyfes-project-cfi/first_pass_example/build/cmake_install.cmake

install-SkeletonPass: skeleton/CMakeFiles/install-SkeletonPass
install-SkeletonPass: skeleton/CMakeFiles/install-SkeletonPass.dir/build.make
.PHONY : install-SkeletonPass

# Rule to build all files generated by this target.
skeleton/CMakeFiles/install-SkeletonPass.dir/build: install-SkeletonPass
.PHONY : skeleton/CMakeFiles/install-SkeletonPass.dir/build

skeleton/CMakeFiles/install-SkeletonPass.dir/clean:
	cd /Users/lorenzo/Developer/cyfes-project-cfi/first_pass_example/build/skeleton && $(CMAKE_COMMAND) -P CMakeFiles/install-SkeletonPass.dir/cmake_clean.cmake
.PHONY : skeleton/CMakeFiles/install-SkeletonPass.dir/clean

skeleton/CMakeFiles/install-SkeletonPass.dir/depend:
	cd /Users/lorenzo/Developer/cyfes-project-cfi/first_pass_example/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/lorenzo/Developer/cyfes-project-cfi/first_pass_example /Users/lorenzo/Developer/cyfes-project-cfi/first_pass_example/skeleton /Users/lorenzo/Developer/cyfes-project-cfi/first_pass_example/build /Users/lorenzo/Developer/cyfes-project-cfi/first_pass_example/build/skeleton /Users/lorenzo/Developer/cyfes-project-cfi/first_pass_example/build/skeleton/CMakeFiles/install-SkeletonPass.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : skeleton/CMakeFiles/install-SkeletonPass.dir/depend

