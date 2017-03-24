# Makefile for testing the client/server C++ SDK instrumentation logic.
#

HOST       = 127.0.0.1
PORT       = 10517
SAMPLE     = sample.html

SERVER_DIR = ../server
CLIENT_DIR = ../client

SERVER_EXE = httpserver
CLIENT_EXE = httpclient

CLIENT_OUT = "$(CLIENT_EXE).out"
SERVER_OUT = "$(SERVER_EXE).out"

# The current SDK.
SDK_ROOT        = /opt/appdynamics-sdk-native
SDK_INC_DIR     = $(SDK_ROOT)/sdk_lib
SDK_LIB_DIR     = $(SDK_ROOT)/sdk_lib/lib

.PHONY: all clean

all:
	@echo "Building links..."
	@if [ -e $(SERVER_EXE) ]; then\
		if [ ! -h $(SERVER_EXE) ]; then\
			echo "ERROR: $(SERVER_EXE) exists but is not a symbolic link.";\
			exit 1;\
		fi;\
		rm -f $(SERVER_EXE);\
	fi
	ln -s $(SERVER_DIR)/$(SERVER_EXE) $(SERVER_EXE)
	@if [ -h $(CLIENT_EXE) ]; then\
		if [ ! -h $(CLIENT_EXE) ]; then\
			echo "ERROR: $(CLIENT_EXE) exists but is not a symbolic link.";\
			exit 1;\
		fi;\
		rm -f $(CLIENT_EXE);\
	fi
	ln -s $(CLIENT_DIR)/$(CLIENT_EXE) $(CLIENT_EXE)

clean:
	@echo "Removing links..."
	@if [ -h $(SERVER_EXE) ]; then\
		rm -f $(SERVER_EXE);\
	else if [ -e $(SERVER_EXE) ]; then\
		echo "ERROR: $(SERVER_EXE) exists but is not a symbolic link.";\
		exit 1;\
	fi; fi;
	@if [ -h $(CLIENT_EXE) ]; then\
		rm -f $(CLIENT_EXE);\
	else if [ -e $(CLIENT_EXE) ]; then\
		echo "ERROR: $(CLIENT_EXE) exists but is not a symbolic link.";\
		exit 1;\
	fi; fi;

it: $(SERVER_EXE) $(CLIENT_EXE)
	LD_LIBRARY_PATH=$(SDK_LIB_DIR) ./$(CLIENT_EXE) $(HOST) $(PORT) $(SAMPLE) > $(CLIENT_OUT) &
	tail -100 $(CLIENT_OUT)
