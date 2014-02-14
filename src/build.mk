# This file is a makefile included from the top level makefile which
# defines the sources built for the target.

# Define the prefix to this directory. 
# Note: The name must be unique within this build and should be
#       based on the root of the project
TARGET_SRC_PATH = src

# Add include to all objects built for this target
INCLUDE_DIRS += inc
INCLUDE_DIRS += inc/lemmalib

# C source files included in this build.
CSRC +=
#CSRC += $(TARGET_SRC_PATH)/dump.c
#CSRC += $(TARGET_SRC_PATH)/error.c
#CSRC += $(TARGET_SRC_PATH)/hashtable.c
#CSRC += $(TARGET_SRC_PATH)/LemmaList.c
#CSRC += $(TARGET_SRC_PATH)/LemmaListDeserializer.c
#CSRC += $(TARGET_SRC_PATH)/LemmaListSerializer.c
#CSRC += $(TARGET_SRC_PATH)/LemmaUtil.c
#CSRC += $(TARGET_SRC_PATH)/load.c
#CSRC += $(TARGET_SRC_PATH)/memory.c
#CSRC += $(TARGET_SRC_PATH)/pack_unpack.c
#CSRC += $(TARGET_SRC_PATH)/strbuffer.c
#CSRC += $(TARGET_SRC_PATH)/strconv.c
#CSRC += $(TARGET_SRC_PATH)/utf.c
#CSRC += $(TARGET_SRC_PATH)/value.c

# C++ source files included in this build.
CPPSRC += $(TARGET_SRC_PATH)/application.cpp
CPPSRC += $(TARGET_SRC_PATH)/main.cpp
CPPSRC += $(TARGET_SRC_PATH)/newlib_stubs.cpp
CPPSRC += $(TARGET_SRC_PATH)/spark_utilities.cpp
CPPSRC += $(TARGET_SRC_PATH)/spark_wiring.cpp
CPPSRC += $(TARGET_SRC_PATH)/spark_wiring_i2c.cpp
CPPSRC += $(TARGET_SRC_PATH)/spark_wiring_interrupts.cpp
CPPSRC += $(TARGET_SRC_PATH)/spark_wiring_ipaddress.cpp
CPPSRC += $(TARGET_SRC_PATH)/spark_wiring_network.cpp
CPPSRC += $(TARGET_SRC_PATH)/spark_wiring_print.cpp
CPPSRC += $(TARGET_SRC_PATH)/spark_wiring_servo.cpp
CPPSRC += $(TARGET_SRC_PATH)/spark_wiring_spi.cpp
CPPSRC += $(TARGET_SRC_PATH)/spark_wiring_stream.cpp
CPPSRC += $(TARGET_SRC_PATH)/spark_wiring_string.cpp
CPPSRC += $(TARGET_SRC_PATH)/spark_wiring_tcpclient.cpp
CPPSRC += $(TARGET_SRC_PATH)/spark_wiring_tcpserver.cpp
CPPSRC += $(TARGET_SRC_PATH)/spark_wiring_udp.cpp
CPPSRC += $(TARGET_SRC_PATH)/spark_wiring_usartserial.cpp
CPPSRC += $(TARGET_SRC_PATH)/spark_wiring_usbserial.cpp
CPPSRC += $(TARGET_SRC_PATH)/spark_wlan.cpp
CPPSRC += $(TARGET_SRC_PATH)/stm32_it.cpp
CPPSRC += $(TARGET_SRC_PATH)/usb_desc.cpp
CPPSRC += $(TARGET_SRC_PATH)/usb_endp.cpp
CPPSRC += $(TARGET_SRC_PATH)/usb_istr.cpp
CPPSRC += $(TARGET_SRC_PATH)/usb_prop.cpp
CPPSRC += $(TARGET_SRC_PATH)/wifi_credentials_reader.cpp

CPPSRC += $(TARGET_SRC_PATH)/lemmalib/MaestroLocater.cpp
CPPSRC += $(TARGET_SRC_PATH)/lemmalib/MessageParser.cpp
CPPSRC += $(TARGET_SRC_PATH)/lemmalib/TcpReader.cpp
CPPSRC += $(TARGET_SRC_PATH)/lemmalib/EventFilter.cpp
CPPSRC += $(TARGET_SRC_PATH)/lemmalib/MemoryFree.cpp
CPPSRC += $(TARGET_SRC_PATH)/lemmalib/MessageSender.cpp
CPPSRC += $(TARGET_SRC_PATH)/lemmalib/nJSON.cpp
CPPSRC += $(TARGET_SRC_PATH)/lemmalib/Lemma.cpp
CPPSRC += $(TARGET_SRC_PATH)/lemmalib/MessageBuilder.cpp
CPPSRC += $(TARGET_SRC_PATH)/lemmalib/TcpProtocol.cpp
CPPSRC += $(TARGET_SRC_PATH)/lemmalib/Event.cpp
CPPSRC += $(TARGET_SRC_PATH)/Bounce.cpp

# ASM source files included in this build.
ASRC +=

