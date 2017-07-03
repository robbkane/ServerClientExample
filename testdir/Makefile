# Makefile for testing the client/server C++ SDK instrumentation logic.
#

.PHONY: all check_pids check_server_pid check_client_pid check_links check_server_link check_client_link clean clean_server_link clean_client_link links server_links client_links start server_start client_start stop server_stop client_stop status title_status server_status client_status

all: links
	@echo "make all completed."

links: clean server_link client_link
	@echo "make links completed."

server_link: check_server_link
	ln -s ${SERVER_EXE} ${SERVER_EXE_NAME}

client_link: check_client_link
	ln -s ${CLIENT_EXE} ${CLIENT_EXE_NAME}

clean: clean_server_link clean_client_link
	@echo "Completed clean."

clean_server_link: check_server_link check_server_pid
	@if [ -h ${SERVER_EXE_NAME} ]; then\
		echo "rm -f ${SERVER_EXE_NAME}";\
		rm -f ${SERVER_EXE_NAME};\
	fi;

clean_client_link: check_client_link check_client_pid
	@if [ -h ${CLIENT_EXE_NAME} ]; then\
		echo "rm -f ${CLIENT_EXE_NAME}";\
		rm -f ${CLIENT_EXE_NAME};\
	fi;

start: server_start client_start
	@echo "Completed program start for server & client."

server_start: check_server_link check_server_pid
	@if [ -n "`ps -ef | grep -v grep | grep proxy | grep "${PROXY_TYPE}"`" ]; then\
		if [ 1 = "`ps -ef | grep -v grep | grep proxy | grep "${PROXY_TYPE}" | wc -l`" ]; then\
			echo "${BOLD_GREEN}Existing proxy process running.${RESET_TERM}";\
			ps -ef | grep -v grep | grep proxy | grep "${PROXY_TYPE}";\
		fi;\
	else\
		echo "${BOLD_RED}No Proxy process running!  Try \"make proxy_start\" first.${RESET_TERM}";\
		exit 1;\
	fi;
	@echo
	@echo "About to start \"${SERVER_EXE_NAME}\" using host \"${HOST}\", port \"${PORT}\", and HTTP root directory \"${HTTP_DIR}\"..."
	@echo "\n\n********************************************" > ${SERVER_LOG}
	@echo "Starting ${SERVER_EXE_NAME} using host ${HOST}, port ${PORT}, and HTTP root directory \"${HTTP_DIR}\"" >> ${SERVER_LOG}
	@echo "********************************************\n\n" >> ${SERVER_LOG}
	@echo "Executed on "`date` >> ${SERVER_LOG}
	@${SDK_LOAD_PATH} ./${SERVER_EXE_NAME} ${HOST} ${PORT} ${HTTP_DIR} >> ${SERVER_LOG} 2>&1 &
	@sleep 1
	@ps -ef | grep -v grep | grep "${SERVER_EXE_NAME}" | grep "${HOST}" | grep "${PORT}" | awk ' { print $$2 } ' | head -1 > ${SERVER_PID_FILE}
	@echo "Server started with pid: ${BOLD_YELLOW}`cat ${SERVER_PID_FILE}`${RESET_TERM}"
	@echo "Sending output to: ${SERVER_LOG}"
	@echo "Use this command to tail the output: ${BOLD_GREEN} tail -100f ${SERVER_LOG}${RESET_TERM}"
	@echo

client_start: check_client_link check_client_pid
	@if [ -n "`ps -ef | grep -v grep | grep proxy | grep "${PROXY_TYPE}"`" ]; then\
		if [ 1 = "`ps -ef | grep -v grep | grep proxy | grep "${PROXY_TYPE}" | wc -l`" ]; then\
			echo "${BOLD_GREEN}Existing proxy process running.${RESET_TERM}";\
			ps -ef | grep -v grep | grep proxy | grep "${PROXY_TYPE}";\
		fi;\
	else\
		echo "${BOLD_RED}No Proxy process running!  Try \"make proxy_start\" first.${RESET_TERM}";\
		exit 1;\
	fi;
	@echo
	@echo "About to start \"${CLIENT_EXE_NAME}\" using host \"${HOST}\", port \"${PORT}\", and file \"${SAMPLE}\"..."
	@echo "\n\n********************************************" > ${CLIENT_LOG}
	@echo "Starting ${CLIENT_EXE_NAME} using host ${HOST}, port ${PORT}, and file ${SAMPLE}" >> ${CLIENT_LOG}
	@echo "********************************************\n\n" >> ${CLIENT_LOG}
	@echo "Executed on "`date` >> ${CLIENT_LOG}
	@${SDK_LOAD_PATH} ./${CLIENT_EXE_NAME} ${HOST} ${PORT} ${SAMPLE} >> ${CLIENT_LOG} 2>&1 &
	@ps -ef | grep -v grep | grep "${CLIENT_EXE_NAME}" | grep "${HOST}" | grep "${PORT}" | awk ' { print $$2 } ' | head -1 > ${CLIENT_PID_FILE}
	@echo "Client started with pid: ${BOLD_YELLOW}`cat ${CLIENT_PID_FILE}`${RESET_TERM}"
	@echo "Sending output to: ${CLIENT_LOG}"
	@echo "Use this command to tail the output: ${BOLD_GREEN} tail -100f ${CLIENT_LOG}${RESET_TERM}"
	@echo

stop: client_stop server_stop status
	@echo "Completed program stop for client and server."

server_stop: check_server_pid
	@echo -n

client_stop: check_client_pid
	@echo -n

check_links: check_server_link check_client_link
	@echo "Completed check_links"

check_server_link:
	@if [ -e ${SERVER_EXE_NAME} ] && [ ! -h ${SERVER_EXE_NAME} ]; then\
		echo;\
		echo "${BOLD_RED}ERROR: ${SERVER_EXE_NAME} exists but is not a symbolic link.${RESET_TERM}";\
		echo;\
		exit 1;\
	fi

check_client_link:
	@if [ -e ${CLIENT_EXE_NAME} ] && [ ! -h ${CLIENT_EXE_NAME} ]; then\
		echo;\
		echo "${BOLD_RED}ERROR: ${CLIENT_EXE_NAME} exists but is not a symbolic link.${RESET_TERM}";\
		echo;\
		exit 1;\
	fi

check_pids: check_server_pid check_client_pid
	@echo -n

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
		echo "${BOLD_RED}Found a pre-existing server pid file \"${SERVER_PID_FILE}\" with pid `${SERVER_PID_FILE}`.${RESET_TERM}";\
		ps -f --pid `cat ${SERVER_PID_FILE}` ;\
		echo;\
		read -p "Would you like to kill the process before proceeding (yes/no)?" ANSWER;\
		echo;\
		if [ "$${ANSWER}" != "yes" ]; then\
		echo "${BOLD_RED}Terminating make at user request.${RESET_TERM}";\
		echo;\
		exit 1;\
		fi;\
		echo "${BOLD_RED}Aborting process `cat ${SERVER_PID_FILE}`, and removing file \"${SERVER_PID_FILE}\".${RESET_TERM}";\
		kill `cat ${SERVER_PID_FILE}`;\
		rm -f ${SERVER_PID_FILE};\
		echo "Process and/or pid file eliminated.";\
		echo;\
	fi
	@if [ -n "`ps -ef | grep -v grep | grep "${SERVER_EXE_NAME}" | grep "${HOST}" | grep "${PORT}"`" ]; then\
		echo;\
		echo "${BOLD_RED}ERROR: Found one or more matching server processes with no corresponding \"${SERVER_PID_FILE}\".${RESET_TERM}";\
		ps -ef | grep -v grep | grep "${SERVER_EXE_NAME}" | grep "${HOST}";\
		echo;\
		while [ -n "`ps -ef | grep -v grep | grep "${SERVER_EXE_NAME}" | grep "${HOST}" | grep "${PORT}"`" ]; do\
			echo;\
			read -p "Would you like to kill process `ps -ef | grep -v grep | grep "${SERVER_EXE_NAME}" | grep "${HOST}" | grep "${PORT}" | awk ' { print $$2 } '` (yes/no/abort)?" ANSWER;\
			if [ "$${ANSWER}" = "yes" ]; then\
				echo;\
				echo "${BOLD_RED}Terminating process at user request.${RESET_TERM}";\
				kill -9 `ps -ef | grep -v grep | grep "${SERVER_EXE_NAME}" | grep "${HOST}" | grep "${PORT}" | awk ' { print $$2 } '`;\
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
	fi
	@echo "Completed check_server_pid";\

check_client_pid:
	@if [ -f "${CLIENT_PID_FILE}" ] && [ -s "${CLIENT_PID_FILE}" ]; then\
		if [ "`cat "${CLIENT_PID_FILE}" | wc -l`" -ne "1" ]; then\
		echo;\
		echo "${BOLD_RED}ERROR: Found a malformed (line count != 1), pre-existing client pid file \"${CLIENT_PID_FILE}\".${RESET_TERM}";\
		echo "Make sure no tasks are already running, and clean up the \"${CLIENT_PID_FILE}\" pid file.";\
		echo;\
		exit 1;\
		fi;\
		if [ "`cat "${CLIENT_PID_FILE}" | wc -w`" -ne "1" ]; then\
		echo;\
		echo "${BOLD_RED}ERROR: Found a malformed (word count != 1), pre-existing client pid file \"${CLIENT_PID_FILE}\".${RESET_TERM}";\
		echo "Make sure no tasks are already running, and clean up the \"${CLIENT_PID_FILE}\" pid file.";\
		echo;\
		exit 1;\
		fi;\
		echo;\
		echo "${BOLD_RED}Found a pre-existing client pid file \"${CLIENT_PID_FILE}\" with pid `cat ${CLIENT_PID_FILE}`.${RESET_TERM}";\
		ps -f --pid "`cat ${CLIENT_PID_FILE}`" ;\
		echo;\
		read -p "Would you like to kill the process before proceeding (yes/no)?" ANSWER;\
		echo;\
		if [ "$${ANSWER}" != "yes" ]; then\
		echo "${BOLD_RED}Terminating make at user request.${RESET_TERM}";\
		echo;\
		exit 1;\
		fi;\
		echo "${BOLD_RED}Aborting process `cat ${CLIENT_PID_FILE}`, and removing file \"${CLIENT_PID_FILE}\".${RESET_TERM}";\
		kill "`cat ${CLIENT_PID_FILE}`";\
		rm -f "${CLIENT_PID_FILE}";\
		echo "Process and/or pid file eliminated.";\
		echo;\
	fi
	@if [ -n "`ps -ef | grep -v grep | grep "${CLIENT_EXE_NAME}" | grep "${HOST}" | grep "${PORT}"`" ]; then\
		echo;\
		echo "${BOLD_RED}ERROR: Found one or more matching client processes with no corresponding \"${CLIENT_PID_FILE}\".${RESET_TERM}";\
		ps -ef | grep -v grep | grep "${CLIENT_EXE_NAME}" | grep "${HOST}";\
		echo;\
		while [ -n "`ps -ef | grep -v grep | grep "${CLIENT_EXE_NAME}" | grep "${HOST}" | grep "${PORT}"`" ]; do\
			echo;\
			read -p "Would you like to kill process `ps -ef | grep -v grep | grep "${CLIENT_EXE_NAME}" | grep "${HOST}" | grep "${PORT}" | awk ' { print $$2 } '` (yes/no/abort)?" ANSWER;\
			if [ "$${ANSWER}" = "yes" ]; then\
				echo;\
				echo "${BOLD_RED}Terminating process at user request.${RESET_TERM}";\
				kill -9 `ps -ef | grep -v grep | grep "${CLIENT_EXE_NAME}" | grep "${HOST}" | grep "${PORT}" | awk ' { print $$2 } '`;\
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
	fi

# 		echo "Completed check_client_pid";\

status: title_status server_status client_status 

title_status:
	@echo;
	@if [ -n "`ps -ef | grep -v grep | grep "${SERVER_EXE_NAME}" | grep "${HOST}" | grep "${PORT}"`" ] ||\
	    [ -n "`ps -ef | grep -v grep | grep "${CLIENT_EXE_NAME}" | grep "${HOST}" | grep "${PORT}"`" ]; then\
		echo;\
		ps -ef | head -1;\
	fi

server_status:
	@if [ -n "`ps -ef | grep -v grep | grep "${SERVER_EXE_NAME}" | grep "${HOST}" | grep "${PORT}"`" ]; then\
		if [ -n "`ps -ef | grep -v grep | grep "${SERVER_EXE_NAME}" | grep "${HOST}" | grep "${PORT}"`" ]; then\
			echo;\
			echo "${BOLD_GREEN}Found Server process running.${RESET_TERM}";\
			ps -ef | grep -v grep | grep "${SERVER_EXE_NAME}" | grep "${HOST}" | grep "${PORT}";\
			echo;\
		fi;\
	else\
		echo "${BOLD_GREEN}No Server processes were running.${RESET_TERM}";\
	fi

client_status:
	@if [ -n "`ps -ef | grep -v grep | grep "${CLIENT_EXE_NAME}" | grep "${HOST}" | grep "${PORT}"`" ]; then\
		if [ -n "`ps -ef | grep -v grep | grep "${CLIENT_EXE_NAME}" | grep "${HOST}" | grep "${PORT}"`" ]; then\
			echo;\
			echo "${BOLD_GREEN}Found Client process running.${RESET_TERM}";\
			ps -ef | grep -v grep | grep "${CLIENT_EXE_NAME}" | grep "${HOST}" | grep "${PORT}";\
			echo;\
		fi;\
	else\
		echo "${BOLD_GREEN}No Client processes were running.${RESET_TERM}";\
	fi
