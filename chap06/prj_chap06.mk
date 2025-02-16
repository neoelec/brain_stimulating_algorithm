PRJ_CHAP06_MK_FILE	:= $(realpath $(lastword $(MAKEFILE_LIST)))
PRJ_CHAP06_MK_DIR	:= $(shell dirname $(PRJ_CHAP06_MK_FILE))

EXTRAINCDIRS		+= $(PRJ_CHAP06_MK_DIR)
VPATH			+= $(PRJ_CHAP06_MK_DIR)

include $(PRJ_CHAP06_MK_DIR)/../prj_native.mk
