# tab stops set to 4
#
# Required libraries:
#
#
SDK_ROOT        = /opt/appdynamics-sdk-native
SDK_INC_DIR     = ${SDK_ROOT}/sdk_lib
SDK_LIB_DIR     = ${SDK_ROOT}/sdk_lib/lib
SDK_LIB         = ${SDK_LIB_DIR}/libappdynamics_native_sdk.so
SDK_LOAD_PATH   = LD_LIBRARY_PATH=${SDK_LIB_DIR}

PROXY_DIR       = ${SDK_ROOT}
PROXY_TYPE      = NATIVE_SDK
PROXY_NICKNAME  = proxy

PROXY_EXE_NAME  = runSDKProxy.sh
SERVER_EXE_NAME = httpserver
CLIENT_EXE_NAME = httpclient

SERVER_DIR_NAME = server
CLIENT_DIR_NAME = client
TEST_DIR_NAME   = testdir

ROOT_DIR        = ${PWD}
TEST_DIR        = ${ROOT_DIR}/${TEST_DIR_NAME}
SERVER_DIR      = ${ROOT_DIR}/${SERVER_DIR_NAME}
CLIENT_DIR      = ${ROOT_DIR}/${CLIENT_DIR_NAME}

PROXY_EXE       = ${PROXY_DIR}/${PROXY_EXE_NAME}
SERVER_EXE      = ${SERVER_DIR}/${SERVER_EXE_NAME}
CLIENT_EXE      = ${CLIENT_DIR}/${CLIENT_EXE_NAME}

PROXY_PID_FILE  = ${TEST_DIR}/${PROXY_NICKNAME}_pid
SERVER_PID_FILE = ${TEST_DIR}/${SERVER_EXE_NAME}_pid
CLIENT_PID_FILE = ${TEST_DIR}/${CLIENT_EXE_NAME}_pid

PROXY_PID       = `cat ${PROXY_PID_FILE}`
SERVER_PID      = `cat ${SERVER_PID_FILE}`
CLIENT_PID      = `cat ${CLIENT_PID_FILE}`

PROXY_LOG       = "${TEST_DIR}/${PROXY_NICKNAME}.log"
SERVER_LOG      = "${TEST_DIR}/${SERVER_EXE_NAME}.log"
CLIENT_LOG      = "${TEST_DIR}/${CLIENT_EXE_NAME}.log"

BOLD_RED        = \033[1m\033[31m
BOLD_GREEN      = \033[1m\033[32m
BOLD_YELLOW     = \033[1m\033[33m
RESET_TERM      = \033[0m

HOST            = 127.0.0.1
PORT            = 10517
SAMPLE          = /sample.html
HTTP_DIR        = /home/user1/SW/ServerClientExample/testdir

JUNK            = ${ROOT_DIR}/junk

export

.PHONY: all server client testdir clean it list help proxy proxy_start server_start client_start proxy_stop server_stop client_stop proxy_status proxy_check_pid

all: server client testdir
	@echo "make all completed successfully"

proxy: proxy_start
	@echo 

proxy_start:
	@cd ${TEST_DIR} && make proxy_start
	@echo "Make proxy_start completed."
	@echo 

server_start:
	@cd ${TEST_DIR} && make server_start
	@echo "Make server_start completed."
	@echo 

client_start:
	@cd ${TEST_DIR} && make client_start
	@echo "Make client_start completed."
	@echo 

proxy_stop:
	@cd ${TEST_DIR} && make proxy_stop
	@echo "Make proxy_stop completed."
	@echo 

server_stop:
	@cd ${TEST_DIR} && make server_stop
	@echo "Make server_stop completed."
	@echo 

client_stop:
	@cd ${TEST_DIR} && make client_stop
	@echo "Make client_stop completed."
	@echo 

proxy_status:
	@cd ${TEST_DIR} && make proxy_status
	@echo "Make proxy_status completed."
	@echo 

proxy_check_pid:
	@cd ${TEST_DIR} && make proxy_check_pid
	@echo "Make proxy_check_pid completed."
	@echo 

server:
	@cd ${SERVER_DIR} && make
	@echo "Make server completed."
	@echo 

client: 
	@cd ${CLIENT_DIR} && make 
	@echo "Make client completed."
	@echo 

testdir:
	@cd ${TEST_DIR} && make
	@echo "Make testdir completed."
	@echo 

clean:
	@cd ${SERVER_DIR} && make clean
	@cd ${CLIENT_DIR} && make clean
	@cd ${TEST_DIR}   && make clean
	@echo "Make clean completed."
	@echo 

it:
	@cd ${SERVER_DIR} && make 
	@cd ${CLIENT_DIR} && make
	@cd ${TEST_DIR}   && make
	@cd ${TEST_DIR}   && make start
	@cd ${TEST_DIR}   && make status
	@echo "Make it completed."
	@echo 

start:
	@cd ${TEST_DIR} && make start
	@echo "Make start completed."
	@echo 

stop:
	@cd ${TEST_DIR} && make stop
	@echo "Make stop completed."
	@echo 

status:
	@cd ${TEST_DIR} && make status
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
	@echo "links  - create \[in ${TEST_DIR}\] the symlinks to executes."
	@echo "start  - start the server and client. \[also: server_start, client_start\]"
	@echo "stop   - stop the server and client \[also: server_stop, client_stop\]"
	@echo "proxy_start   - start the proxy."
	@echo "proxy_stop    - stop the proxy."
	@echo "proxy_status  - show proxy status."

help:
	@echo 
	@echo "This Makefile script is used to start the proxy, server and client, the last two of which areinstrumented  
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
	@echo "Remember to always start the proxy only once for a given host, and to make sure that the appdynamics_agent.conf file"
	@echo "does not contain the AppDynamicsLauncProxy directive, or you will spawn multiple proxies unintentionally."
	@echo 

junk:
	@cd ${SERVER_DIR} && make junk
	@cd ${CLIENT_DIR} && make junk
	@cd ${TEST_DIR} && make junk
	@echo "Make junk completed."
	@echo 

