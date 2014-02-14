# This file is a makefile included from the top level makefile which
# defines the sources built for the target.

# Define the prefix to this directory. 
# Note: The name must be unique within this build and should be
#       based on the root of the project
TARGET_LIB_PATH = lib

# Add include to all objects built for this target
# INCLUDE_DIRS += $(TARGET_LIB_PATH)/aJson
# INCLUDE_DIRS += $(TARGET_LIB_PATH)/aJson/utility
INCLUDE_DIRS += $(TARGET_LIB_PATH)/cJSON

# C source files included in this build.
CSRC += $(TARGET_LIB_PATH)/cJSON/cJSON.c
# CSRC += $(TARGET_LIB_PATH)/aJson/utility/streamhelper.c
# CSRC += $(TARGET_LIB_PATH)/aJson/utility/stringbuffer.c

# C++ source files included in this build.
# CPPSRC += $(TARGET_LIB_PATH)/aJson/aJSON.cpp

# ASM source files included in this build.
ASRC +=

