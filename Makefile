# tab stops set to 4
#
# Required libraries:
#
#
SERVER_DIR_NAME = server
CLIENT_DIR_NAME = client
TEST_DIR_NAME   = testdir

SERVER_EXE = httpserver
CLIENT_EXE = httpclient

TOP_DIR    = `pwd`
SERVER_DIR = $(TOP_DIR)/${SERVER_DIR_NAME}
CLIENT_DIR = $(TOP_DIR)/${CLIENT_DIR_NAME}
TEST_DIR   = $(TOP_DIR)/${TEST_DIR_NAME}

.PHONY: all server client testdir clean it list help 

all: server client testdir
	@echo "make all completed successfully"

server:
	@cd $(SERVER_DIR) && make
	@echo "Make server completed."
	@echo 

client: 
	@cd $(CLIENT_DIR) && make 
	@echo "Make client completed."
	@echo 

testdir:
	@cd $(TEST_DIR) && make
	@echo "Make testdir completed."
	@echo 

clean:
	@cd $(SERVER_DIR) && make clean
	@cd $(CLIENT_DIR) && make clean
	@cd $(TEST_DIR)   && make clean
	@echo "Make clean completed."
	@echo 

it:
	@cd $(SERVER_DIR) && make 
	@cd $(CLIENT_DIR) && make
	@cd $(TEST_DIR)   && make
	@cd $(TEST_DIR)   && make start
	@echo "Make it completed."
	@echo 

start:
	@cd $(TEST_DIR) && make start
	@echo "Make start completed."
	@echo 

stop:
	@cd $(TEST_DIR) && make stop
	@echo "Make stop completed."
	@echo 

status:
	@cd $(TEST_DIR) && make status
	@echo 

list:
	@echo
	@echo "Target           Description"
	@echo "======================================================================="
	@echo "help   - describes how to use build/invoke the client & server."
	@echo "all    - build everything."
	@echo "status - show status of all running programs."
	@echo "it     - build everything if needed, start everything."
	@echo "clean  - removed all derived objects."
	@echo "links  - create (in ${TEST_DIR} the symlinks to executes."
	@echo "start  - start the client & server (also: server_start, client_start)"
	@echo "stop   - stop the client & server (also: server_stop, client_stop)"

help:
	@echo 
	@echo "This Makefile script is used to start the server and client, both of which are instrumented"
	@echo "using the AppDynamics C++/C SDK.   Run make list for a list of which commands are available."
	@echo 
	@echo "The make process first checks to see if there is a client or server process already running, and"
	@echo "if so, prompts the user to abort the prior version before continuing.   The code is kept in two"
	@echo "subdirectories, a \"${SERVER_DIR_NAME}\" directory and a \"${CLIENT_DIR_NAME}\" directory, including the dervied objects and"
	@echo "executables.  A \"${TEST_DIR_NAME} subdirectory creates symbolic links to the executables. The programs execute"
	@echo "out of the \"${TEST_DIR_NAME}\" directory, witht the client making HTTP requests to the server."
	@echo
	@echo "The Server process will poll on \"${HOST}:${PORT}\", satisfying the client requests." 
	@echo 
