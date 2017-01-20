#------------------------------------------------------------------------------
# $ATH_LICENSE_HOSTSDK0_C$
#------------------------------------------------------------------------------
#==============================================================================
# Author(s): ="Atheros"
#==============================================================================

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_C_INCLUDES := \
    $(LOCAL_PATH)/../../../	\
    $(LOCAL_PATH)/../include	\
    $(LOCAL_PATH)/../../../include 
	

LOCAL_SRC_FILES:= wlanconfig.c
LOCAL_MODULE := wlanconfig
LOCAL_MODULE_TAGS := debug eng optional

include $(BUILD_EXECUTABLE)

