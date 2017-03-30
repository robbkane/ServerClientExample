# tab stops set to 4
#
# Required libraries:
#
#

TOP_DIR    = `pwd`
SERVER_DIR = $(TOP_DIR)/server
CLIENT_DIR = $(TOP_DIR)/client
TEST_DIR   = $(TOP_DIR)/testdir

SERVER_EXE = httpserver
CLIENT_EXE = httpclient

.PHONY: all server client testdir clean it howto 

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

howto:
	@echo "Explanation goes here"
	@echo 
