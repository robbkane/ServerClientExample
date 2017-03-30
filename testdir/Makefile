# Makefile for testing the client/server C++ SDK instrumentation logic.
#
BOLD_RED        = \033[1m\033[31m
BOLD_GREEN      = \033[1m\033[32m
BOLD_YELLOW     = \033[1m\033[33m
RESET_TERM      = \033[0m

HOST            = 127.0.0.1
PORT            = 10517
SAMPLE          = sample.html

SERVER_NAME     = httpserver
CLIENT_NAME     = httpclient
TEST_NAME       = testdir

BASE_DIR        = ..
SERVER_DIR      = ${BASE_DIR}/server
CLIENT_DIR      = ${BASE_DIR}/client
TEST_DIR        = ${BASE_DIR}/testdir
HTTP_DIR        = /

SERVER_EXE      = ${SERVER_DIR}/${SERVER_NAME}
CLIENT_EXE      = ${CLIENT_DIR}/${CLIENT_NAME}

SERVER_PID_FILE = ${TEST_DIR}/${SERVER_NAME}_pid
CLIENT_PID_FILE = ${TEST_DIR}/${CLIENT_NAME}_pid

SERVER_PID      = `cat ${SERVER_PID_FILE}`
CLIENT_PID      = `cat ${CLIENT_PID_FILE}`

SERVER_OUT      = "${TEST_DIR}/${SERVER_NAME}.out"
CLIENT_OUT      = "${TEST_DIR}/${CLIENT_NAME}.out"

# The current SDK.
SDK_ROOT	= /opt/appdynamics-sdk-native
SDK_INC_DIR     = ${SDK_ROOT}/sdk_lib
SDK_LIB_DIR     = ${SDK_ROOT}/sdk_lib/lib
SDK_LIB	        = ${SDK_LIB_DIR}/libappdynamics_native_sdk.so
SDK_LOAD_PATH   = LD_LIBRARY_PATH=${SDK_LIB_DIR}

.PHONY: all check_pids check_server_pid check_client_pid check_links check_server_link check_client_link clean_links clean_server_link clean_client_link clean links server_links client_links start server_start client_start stop server_stop client_stop status server_status client_status

all: links
	@echo "make all completed."

links: clean server_link client_link
	@echo "make links completed."

server_link: check_server_link
	ln -s ${SERVER_EXE} ${SERVER_NAME}

client_link: check_client_link
	ln -s ${CLIENT_EXE} ${CLIENT_NAME}

clean: clean_server_link clean_client_link
	@echo "Completed clean."

clean_server_link: check_server_link check_server_pid
	@if [ -h ${SERVER_NAME} ]; then\
		echo "rm -f ${SERVER_NAME}";\
		rm -f ${SERVER_NAME};\
	fi;

clean_client_link: check_client_link check_client_pid
	@if [ -h ${CLIENT_NAME} ]; then\
		echo "rm -f ${CLIENT_NAME}";\
		rm -f ${CLIENT_NAME};\
	fi;

start: server_start client_start
	@echo "Completed program start for server & client."

server_start: check_server_link check_server_pid
	@echo
	@echo "Starting ${SERVER_NAME} using host ${HOST}, port ${PORT}, and HTTP root directory \"${HTTP_DIR}\"" > ${SERVER_OUT}
	@echo "Executed on "`date` >> ${SERVER_OUT}
	${SDK_LOAD_PATH} ./${SERVER_NAME} ${HOST} ${PORT} ${HTTP_DIR} 2>&1 >> ${SERVER_OUT} &
	@ps -ef | grep -v grep | grep "${SERVER_NAME}" | grep "${HOST}" | grep "${PORT}" | awk ' { print $$2 } ' | head -1 > ${SERVER_PID_FILE}
	@echo "Server started with pid: `cat ${SERVER_PID_FILE}`"
	@echo "Sending output to: ${SERVER_OUT}"
	@echo "Use this command to tail the output: ${BOLD_GREEN} tail -100f ${SERVER_OUT} &${RESET_TERM}"
	@echo


client_start: check_client_link check_client_pid
	@echo
	@echo "Starting ${CLIENT_NAME} using host ${HOST}, port ${PORT}, and file ${SAMPLE}" > ${CLIENT_OUT}
	@echo "Executed on "`date` >> ${CLIENT_OUT}
	${SDK_LOAD_PATH} ./${CLIENT_NAME} ${HOST} ${PORT} ${SAMPLE} 2>&1 >> ${CLIENT_OUT} &
	@ps -ef | grep -v grep | grep "${CLIENT_NAME}" | grep "${HOST}" | grep "${PORT}" | awk ' { print $$2 } ' | head -1 > ${CLIENT_PID_FILE}
	@echo "Client started with pid: `cat ${CLIENT_PID_FILE}`"
	@echo "Sending output to: ${CLIENT_OUT}"
	@echo "Use this command to tail the output: ${BOLD_GREEN} tail -100f ${CLIENT_OUT} &${RESET_TERM}"
	@echo

stop: server_stop client_stop
	@echo "Completed program stop for server & client."

server_stop: check_server_pid

client_stop: check_client_pid

check_links: check_server_link check_client_link
	@echo "Completed check_links"

check_server_link:
	@if [ -e ${SERVER_NAME} ] && [ ! -h ${SERVER_NAME} ]; then\
		echo;\
		echo "${BOLD_RED}ERROR: ${SERVER_NAME} exists but is not a symbolic link.${RESET_TERM}";\
		echo;\
		exit 1;\
	fi

check_client_link:
	@if [ -e ${CLIENT_NAME} ] && [ ! -h ${CLIENT_NAME} ]; then\
		echo;\
		echo "${BOLD_RED}ERROR: ${CLIENT_NAME} exists but is not a symbolic link.${RESET_TERM}";\
		echo;\
		exit 1;\
	fi

check_pids: check_server_pid check_client_pid

check_server_pid: 
	@if [ -f ${SERVER_PID_FILE} ] && [ -s ${SERVER_PID_FILE} ]; then\
		if [ "`cat ${SERVER_PID_FILE} | wc -l`" -ne "1" ]; then\
		echo;\
		echo "${BOLD_RED}ERROR: Found a malformed (line count != 1), pre-existing server pid file \"${SERVER_PID_FILE}\".${RESET_TERM}";\
		echo "Make sure no tasks are already running, and clean up the \"${SERVER_PID_FILE}\" pid file.";\
		echo;\
		exit 1;\
		fi;\
		if [ "`cat ${SERVER_PID_FILE} | wc -w`" -ne "1" ]; then\
		echo;\
		echo "${BOLD_RED}ERROR: Found a malformed (word count != 1), pre-existing server pid file \"${SERVER_PID_FILE}\".${RESET_TERM}";\
		echo "Make sure no tasks are already running, and clean up the \"${SERVER_PID_FILE}\" pid file.";\
		echo;\
		exit 1;\
		fi;\
		echo;\
		echo "${BOLD_RED}Found a pre-existing server pid file \"${SERVER_PID_FILE}\" with pid ${SERVER_PID}${RESET_TERM}";\
		ps -f --pid ${SERVER_PID} ;\
		echo;\
		read -p "Would you like to kill the process before proceeding (yes/no)?" ANSWER;\
		echo;\
		if [ "$${ANSWER}" != "yes" ]; then\
		echo "${BOLD_RED}Terminating make at user request.${RESET_TERM}";\
		echo;\
		exit 1;\
		fi;\
		echo "${BOLD_RED}Aborting process ${SERVER_PID}, and removing file \"${SERVER_PID_FILE}\".${RESET_TERM}";\
		kill ${SERVER_PID};\
		rm -f ${SERVER_PID_FILE};\
		echo "Process and/or pid file eliminated.";\
		echo;\
	fi;\
	if [ -n "`ps -ef | grep -v grep | grep "${SERVER_NAME}" | grep "${HOST}" | grep "${PORT}"`" ]; then\
		echo;\
		echo "${BOLD_RED}ERROR: Found one or more matching server processes with no corresponding \"${SERVER_PID_FILE}\".${RESET_TERM}";\
		ps -ef | grep -v grep | grep "${SERVER_NAME}" | grep "${HOST}";\
		echo;\
		while [ -n "`ps -ef | grep -v grep | grep "${SERVER_NAME}" | grep "${HOST}" | grep "${PORT}"`" ]; do\
			echo;\
			read -p "Would you like to kill process `ps -ef | grep -v grep | grep ${SERVER_NAME} | grep ${HOST} | grep ${PORT} | awk ' { print $$2 } '` (yes/no/abort)?" ANSWER;\
			if [ "$${ANSWER}" = "yes" ]; then\
				echo;\
				echo "${BOLD_RED}Terminating process at user request.${RESET_TERM}";\
				kill -9 `ps -ef | grep -v grep | grep ${SERVER_NAME} | grep ${HOST} | grep ${PORT} | awk ' { print $$2 } '`;\
				echo;\
			fi;\
			if [ "$${ANSWER}" = "abort" ]; then\
				echo;\
				echo "${BOLD_RED}Aborting make at user request.${RESET_TERM}";\
				echo;\
				exit 1;\
			fi;\
		done;\
		echo;\
		echo "Completed check_server_pid";\
	fi;

check_client_pid:
	@if [ -f ${CLIENT_PID_FILE} ] && [ -s ${CLIENT_PID_FILE} ]; then\
		if [ "`cat ${CLIENT_PID_FILE} | wc -l`" -ne "1" ]; then\
		echo;\
		echo "${BOLD_RED}ERROR: Found a malformed (line count != 1), pre-existing client pid file \"${CLIENT_PID_FILE}\".${RESET_TERM}";\
		echo "Make sure no tasks are already running, and clean up the \"${CLIENT_PID_FILE}\" pid file.";\
		echo;\
		exit 1;\
		fi;\
		if [ "`cat ${CLIENT_PID_FILE} | wc -w`" -ne "1" ]; then\
		echo;\
		echo "${BOLD_RED}ERROR: Found a malformed (word count != 1), pre-existing client pid file \"${CLIENT_PID_FILE}\".${RESET_TERM}";\
		echo "Make sure no tasks are already running, and clean up the \"${CLIENT_PID_FILE}\" pid file.";\
		echo;\
		exit 1;\
		fi;\
		echo;\
		echo "${BOLD_RED}Found a pre-existing client pid file \"${CLIENT_PID_FILE}\" with pid ${CLIENT_PID}${RESET_TERM}";\
		ps -f --pid ${CLIENT_PID} ;\
		echo;\
		read -p "Would you like to kill the process before proceeding (yes/no)?" ANSWER;\
		echo;\
		if [ "$${ANSWER}" != "yes" ]; then\
		echo "${BOLD_RED}Terminating make at user request.${RESET_TERM}";\
		echo;\
		exit 1;\
		fi;\
		echo "${BOLD_RED}Aborting process ${CLIENT_PID}, and removing file \"${CLIENT_PID_FILE}\".${RESET_TERM}";\
		kill ${CLIENT_PID};\
		rm -f ${CLIENT_PID_FILE};\
		echo "Process and/or pid file eliminated.";\
		echo;\
	fi;\
	if [ -n "`ps -ef | grep -v grep | grep "${CLIENT_NAME}" | grep "${HOST}" | grep "${PORT}"`" ]; then\
		echo;\
		echo "${BOLD_RED}ERROR: Found one or more matching client processes with no corresponding \"${CLIENT_PID_FILE}\".${RESET_TERM}";\
		ps -ef | grep -v grep | grep "${CLIENT_NAME}" | grep "${HOST}";\
		echo;\
		while [ -n "`ps -ef | grep -v grep | grep "${CLIENT_NAME}" | grep "${HOST}" | grep "${PORT}"`" ]; do\
			echo;\
			read -p "Would you like to kill process `ps -ef | grep -v grep | grep ${CLIENT_NAME} | grep ${HOST} | grep ${PORT} | awk ' { print $$2 } '` (yes/no/abort)?" ANSWER;\
			if [ "$${ANSWER}" = "yes" ]; then\
				echo;\
				echo "${BOLD_RED}Terminating process at user request.${RESET_TERM}";\
				kill -9 `ps -ef | grep -v grep | grep ${CLIENT_NAME} | grep ${HOST} | grep ${PORT} | awk ' { print $$2 } '`;\
				echo;\
			fi;\
			if [ "$${ANSWER}" = "abort" ]; then\
				echo;\
				echo "${BOLD_RED}Aborting make at user request.${RESET_TERM}";\
				echo;\
				exit 1;\
			fi;\
		done;\
		echo;\
		echo "Completed check_client_pid";\
	fi;

status:
	@if [ -n "`ps -ef | grep -v grep | grep ${SERVER_NAME} | grep ${HOST} | grep ${PORT}`" ] ||\
	    [ -n "`ps -ef | grep -v grep | grep ${CLIENT_NAME} | grep ${HOST} | grep ${PORT}`" ]; then\
		echo;\
		ps -ef | head -1;\
		if [ -n "`ps -ef | grep -v grep | grep ${SERVER_NAME} | grep ${HOST} | grep ${PORT}`" ]; then\
			ps -ef | grep -v grep | grep "${SERVER_NAME}" | grep "${HOST}" | grep "${PORT}";\
		fi;\
		if [ -n "`ps -ef | grep -v grep | grep ${CLIENT_NAME} | grep ${HOST} | grep ${PORT}`" ]; then\
			ps -ef | grep -v grep | grep "${CLIENT_NAME}" | grep "${HOST}" | grep "${PORT}";\
		fi;\
	else\
		echo;\
		echo "No Client or Server processes were running.";\
		echo;\
	fi;
	@echo

junk:
		if [ -n "`ps -ef | grep -v grep | grep ${CLIENT_NAME} | grep ${HOST} | grep ${PORT}`" ]; then\ 
			ps -ef | grep -v grep | grep "${CLIENT_NAME}" | grep "${HOST}" | grep "${PORT}";\
		fi;\
