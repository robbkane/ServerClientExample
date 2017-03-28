# tab stops set to 4
#
# Required libraries:
#
# Steps involved:
#
# zlib                  [ sudo apt-get install zlib1g-dev                                     ]
# python                [ sudo apt-get install python-dev                                     ]
# boost libs            [ get download at www.boost.org, tar -xvf boost.tar.bz2 -C /usr/local ]
#                       [ cd /usr/local/boost_1_63_0 (for example)                            ]
#                       [ sudo ./bootstrap.sh --prefix=/usr/local                             ]
#                       [ sudo ./b2 install                                                   ]
#
# appdynamics SDK       [ sudo tar xvf                                                        ]
#

# Name we'll use for the client.
CLIENT_NAME		= httpclient

SDK_ROOT        = /opt/appdynamics-sdk-native
SDK_INC_DIR     = $(SDK_ROOT)/sdk_lib
SDK_LIB_DIR     = $(SDK_ROOT)/sdk_lib/lib
SDK_LIB         = $(SDK_LIB_DIR)/libappdynamics_native_sdk.so

SDK_LOAD_PATH   = LD_LIBRARY_PATH=$(SDK_LIB_DIR)

# LINUX specific library locations.
# Ubuntu/Debian                 LIB_DIR = /usr/lib/x86_64-linux-gnu
# Centos/Redhat/Fedora          LIB_DIR = /usr/lib64
LIB_DIR         = /usr/lib/x86_64-linux-gnu

# POSIX compliant abstraction layer.
PTHREAD_LIB_DIR = $(LIB_DIR)
PTHREAD_LIBS    = $(PTHREAD_LIB_DIR)/libpthread.so

# BOOST files.
BOOST_ROOT      = /usr/local/boost
BOOST_INC_DIR   = $(BOOST_ROOT)/include
BOOST_LIB_DIR   = $(BOOST_ROOT)/lib

BOOST_LIBS      =  $(BOOST_LIB_DIR)/libboost_thread.a
BOOST_LIBS      += $(BOOST_LIB_DIR)/libboost_system.a
BOOST_LIBS      += $(BOOST_LIB_DIR)/libboost_program_options.a
BOOST_LIBS      += $(BOOST_LIB_DIR)/libboost_date_time.a
BOOST_LIBS      += $(BOOST_LIB_DIR)/libboost_timer.a
BOOST_LIBS      += $(BOOST_LIB_DIR)/libboost_chrono.a

# GNU
CCP         = g++
CPPFLAGS    = -g -std=c++11

# includes
BOOST_INC   = -I$(BOOST_INC_DIR)
SDK_INC     = -I$(SDK_INC_DIR)

# libs
LDFLAGS     = -lz -lcrypt -lrt

all: $(CLIENT_NAME) 

.PHONY: clean

$(CLIENT_NAME): client.cpp Makefile
	@echo building $(CLIENT_NAME) 
	rm -f $@
	$(CCP) $(CPPFLAGS) $(SDK_INC) $(BOOST_INC) $< $(SDK_LIB) $(BOOST_LIBS) $(PTHREAD_LIBS) $(LDFLAGS) -o $@

clean:
	rm -f $(CLIENT_NAME)