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

CLIENT_SRC  = client.cpp 
CLIENT_OBJ  = ${CLIENT_SRC:.cpp=.o}
CLIENT_INC  = 

# LINUX specific library locations.
# Ubuntu/Debian                 LIB_DIR = /usr/lib/x86_64-linux-gnu
# Centos/Redhat/Fedora          LIB_DIR = /usr/lib64
LIB_DIR         = /usr/lib/x86_64-linux-gnu

# POSIX compliant abstraction layer.
PTHREAD_LIB_DIR = ${LIB_DIR}
PTHREAD_LIBS    = ${PTHREAD_LIB_DIR}/libpthread.so

# BOOST files.
BOOST_ROOT      = /usr/local/boost
BOOST_INC_DIR   = ${BOOST_ROOT}/include
BOOST_LIB_DIR   = ${BOOST_ROOT}/lib

BOOST_LIBS      =  ${BOOST_LIB_DIR}/libboost_thread.a
BOOST_LIBS      += ${BOOST_LIB_DIR}/libboost_system.a
BOOST_LIBS      += ${BOOST_LIB_DIR}/libboost_program_options.a
BOOST_LIBS      += ${BOOST_LIB_DIR}/libboost_date_time.a
BOOST_LIBS      += ${BOOST_LIB_DIR}/libboost_timer.a
BOOST_LIBS      += ${BOOST_LIB_DIR}/libboost_chrono.a


# GNU
CCP         = g++
CPPFLAGS    = -g -std=c++11

# includes
BOOST_INC   = -I${BOOST_INC_DIR}
SDK_INC     = -I${SDK_INC_DIR}
INC         = -I.

# libs
BOOST_SYS_LIB   = -lboost_system
LDFLAGS         = -lz -lcrypt -lrt ${BOOST_SYS_LIB}

.PHONY: clean all

all: ${CLIENT_EXE}

%.o: %.cpp
	    @echo building $@
		    ${CCP} ${CPPFLAGS} ${INC} ${SDK_INC} -c $^

${CLIENT_EXE}: ${CLIENT_SRC} ${CLIENT_INC} ${CLIENT_OBJ} Makefile
	@echo building ${CLIENT_EXE} 
	rm -f $@
	${CCP} ${CPPFLAGS} ${INC} ${SDK_INC} ${BOOST_INC} ${CLIENT_OBJ} ${SDK_LIB} ${BOOST_LIBS} ${PTHREAD_LIBS} ${LDFLAGS} -o $@

clean:
	rm -f ${CLIENT_EXE}

junk:
	@echo ROOT_DIR is ${ROOT_DIR}
	@echo JUNK is ${JUNK}
