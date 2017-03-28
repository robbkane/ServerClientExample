# Makefile for testing the client/server C++ SDK instrumentation logic.
#

HOST       = 127.0.0.1
PORT       = 10517
SAMPLE     = /sample.html

SERVER_NAME = httpserver
CLIENT_NAME = httpclient
TEST_NAME   = testdir

BASE_DIR   = ..

SERVER_DIR = ${BASE_DIR}/server
CLIENT_DIR = ${BASE_DIR}/client
TEST_DIR   = ${BASE_DIR}/testdir
HTTP_DIR   = /

SERVER_EXE = ${SERVER_DIR}/${SERVER_NAME}
CLIENT_EXE = ${CLIENT_DIR}/${CLIENT_NAME}

SERVER_OUT = "${TEST_DIR}/${SERVER_NAME}.out"
CLIENT_OUT = "${TEST_DIR}/${CLIENT_NAME}.out"

# The current SDK.
SDK_ROOT	= /opt/appdynamics-sdk-native
SDK_INC_DIR     = ${SDK_ROOT}/sdk_lib
SDK_LIB_DIR     = ${SDK_ROOT}/sdk_lib/lib
SDK_LIB	 = ${SDK_LIB_DIR}/libappdynamics_native_sdk.so
SDK_LOAD_PATH   = LD_LIBRARY_PATH=${SDK_LIB_DIR}


.PHONY: all check_links check_server_link check_client_link clean server client it

all: check_links
	@echo "Building links..."
	@if [ -e ${SERVER_NAME} ]; then\
		if [ ! -h ${SERVER_NAME} ]; then\
			echo "ERROR: ${SERVER_NAME} exists but is not a symbolic link.";\
			exit 1;\
		fi;\
		rm -f ${SERVER_NAME};\
	fi
	ln -s ${SERVER_EXE} ${SERVER_NAME}
	@if [ -h ${CLIENT_NAME} ]; then\
		if [ ! -h ${CLIENT_NAME} ]; then\
			echo "ERROR: ${CLIENT_NAME} exists but is not a symbolic link.";\
			exit 1;\
		fi;\
		rm -f ${CLIENT_NAME};\
	fi
	ln -s ${CLIENT_EXE} ${CLIENT_NAME}

check_links: check_server_link check_client_link
	@echo "Completed check_links"

clean: check_links
	@echo "Removing links..."
	@if [ -h ${SERVER_NAME} ]; then\
		echo "rm -f ${SERVER_NAME}";\
		rm -f ${SERVER_NAME};\
	fi;
	@if [ -h ${CLIENT_NAME} ]; then\
		echo "rm -f ${CLIENT_NAME}";\
		rm -f ${CLIENT_NAME};\
	fi;

server_start: check_server_link
	@SERVER_PID=0
	@echo "Starting ${SERVER_NAME} using host ${HOST}, port ${PORT}, and HTTP root directory \"${HTTP_DIR}\"" > ${SERVER_OUT}
	@echo "Executed on "`date` >> ${SERVER_OUT}
	${SDK_LOAD_PATH} ./${SERVER_NAME} ${HOST} ${PORT} ${HTTP_DIR} 2>&1 >> ${SERVER_OUT} &
	@echo "Server started with pid "`ps -ef | grep -v grep | grep "${SERVER_EXE}" | grep "${HOST}" | grep "${PORT}" | awk ' { print $$2 }'`
	@echo "Sending output to \"${SERVER_OUT}\"..."
	@echo "Use the following command to tail the output file:"
	@echo "tail -100f ${SERVER_OUT} &"


client_start: check_client_link
	@CLIENT_PID=0
	@echo "Starting ${CLIENT_NAME} using host ${HOST}, port ${PORT}, and file ${SAMPLE}" > ${CLIENT_OUT}
	@echo "Executed on "`date` >> ${CLIENT_OUT}
	${SDK_LOAD_PATH} ./${CLIENT_NAME} ${HOST} ${PORT} ${SAMPLE} 2>&1 >> ${CLIENT_OUT} &
	@echo "Client started with pid "`ps -ef | grep -v grep | grep "${CLIENT_EXE}" | grep "${HOST}" | grep "${PORT}" | grep "${SAMPLE}" | awk ' { print $$2 }'`
	@echo "Sending output to \"${CLIENT_OUT}\"..."
	@echo "Use the following command to tail the output file:"
	@echo "tail -100f ${CLIENT_OUT} &"

check_server_link:
	@if [ -e ${SERVER_NAME} ] && [ ! -h ${SERVER_NAME} ]; then\
		echo "ERROR: ${SERVER_NAME} exists but is not a symbolic link.";\
		exit 1;\
	fi;

check_client_link:
	@if [ -e ${CLIENT_NAME} ] && [ ! -h ${CLIENT_NAME} ]; then\
		echo "ERROR: ${CLIENT_NAME} exists but is not a symbolic link.";\
		exit 1;\
	fi;

it: server_start client_start
	@echo "Completed it."
