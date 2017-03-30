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
# appdynamics SDK       [ sudo tar xvf  xxxx                                                  ]
#

SERVER_NAME     =  httpserver

SERVER_SRC      =  connection.cpp  
SERVER_SRC      += connection_manager.cpp  
SERVER_SRC      += mime_types.cpp  
SERVER_SRC      += reply.cpp  
SERVER_SRC      += request_handler.cpp  
SERVER_SRC      += request_parser.cpp
SERVER_SRC      += server.cpp
SERVER_SRC      += main.cpp

SERVER_OBJ      = $(SERVER_SRC:.cpp=.o)

SERVER_INC      =  connection.hpp  
SERVER_INC      += connection_manager.hpp
SERVER_INC      += header.hpp
SERVER_INC      += mime_types.hpp
SERVER_INC      += reply.hpp
SERVER_INC      += request_handler.hpp
SERVER_INC      += request.hpp
SERVER_INC      += request_parser.hpp
SERVER_INC      += server.hpp

SERVER_GCH      =  $(SERVER_INC:.hpp=.hpp.gch)

# The current SDK.
SDK_ROOT        = /opt/appdynamics-sdk-native
SDK_INC_DIR     = $(SDK_ROOT)/sdk_lib
SDK_INC         = -I$(SDK_INC_DIR)
SDK_LIB_DIR     = $(SDK_ROOT)/sdk_lib/lib
SDK_LIB         = 
#SDK_LIB        = $(SDK_LIB_DIR)/libappdynamics_native_sdk.so
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
BOOST_INC       = -I$(BOOST_INC_DIR) -I$(BOOST_INC_DIR)/boost/system

BOOST_LIB_DIR   =  $(BOOST_ROOT)/lib
BOOST_LIBS      =  $(BOOST_LIB_DIR)/libboost_thread.a
BOOST_LIBS      += $(BOOST_LIB_DIR)/libboost_system.a
BOOST_LIBS      += $(BOOST_LIB_DIR)/libboost_program_options.a
BOOST_LIBS      += $(BOOST_LIB_DIR)/libboost_date_time.a
BOOST_LIBS      += $(BOOST_LIB_DIR)/libboost_timer.a
BOOST_LIBS      += $(BOOST_LIB_DIR)/libboost_chrono.a

BOOST_SYS_LIB   =  -lboost_system 

# GNU
CCP         = g++
CPPFLAGS    = -g -std=c++11
INC         = -I.

# libs
LDFLAGS     = -lz -lcrypt -lrt

.PHONY: clean

all: $(SERVER_NAME)

%.o: %.cpp
	@echo building $@
	$(CCP) $(CPPFLAGS) $(INC) $(SDK_INC) -c $^

$(SERVER_NAME): $(SERVER_SRC) $(SERVER_INC) $(SERVER_OBJ) Makefile
	@echo building $@
	rm -f $@
	$(CCP) $(CPPFLAGS) $(INC) $(SDK_INC) $(BOOST_INC) $(LDFLAGS) $(SDK_LIB) $(SERVER_OBJ) $(BOOST_LIBS) $(PTHREAD_LIBS) $(BOOST_SYS_LIB) -o $@

clean:
	rm -f $(SERVER_NAME) $(SERVER_OBJ) $(SERVER_GCH)
