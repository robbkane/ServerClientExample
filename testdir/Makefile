# tab stops set to 4
#

SERVER_DIR = ../server
CLIENT_DIR = ../client

SERVER_EXE = httpserver
CLIENT_EXE = httpclient

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
