PRJ_NATIVE_MK_FILE	:= $(realpath $(lastword $(MAKEFILE_LIST)))
PRJ_NATIVE_MK_DIR	:= $(shell dirname $(PRJ_NATIVE_MK_FILE))

I_HDD00			:= $(HOME)/iHDD00
MK_RACCOON_DIR		:= $(I_HDD00)/08.PROJECT/mk-raccoon
DSA_RACCOON_DIR		:= $(I_HDD00)/08.PROJECT/dsa-raccoon

EXTRAINCDIRS		+= $(DSA_RACCOON_DIR)/inc
VPATH			+= $(DSA_RACCOON_DIR)/src

CFLAGS			+= -Wall -Werror -Wextra -Wno-unused-parameter

OPT			:= g

# include $(MK_RACCOON_DIR)/gcc_native.mk
include $(MK_RACCOON_DIR)/llvm_native.mk
