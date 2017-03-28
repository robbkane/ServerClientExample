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

.PHONY: all links server client clean howto it

all: clean server client links
	@echo "make all completed successfully"
	@echo 

links:
	@echo "Building links in \" "$(TEST_DIR) "\""
	@cd $(TEST_DIR) && make all
	@echo "completed successfully"
	@echo 

server:
	@echo Building $(SERVER_EXE) in $(SERVER_DIR)
	@cd $(SERVER_DIR) && make all
	@cd $(TEST_DIR)   && make server
	@echo "completed successfully"
	@echo 

client: 
	@echo Building $(CLIENT_EXE) in $(CLIENT_DIR)
	@cd $(CLIENT_DIR) && make all
	@cd $(TEST_DIR)   && make client
	@echo "completed successfully"
	@echo 

clean:
	@cd $(SERVER_DIR) && make clean
	@cd $(CLIENT_DIR) && make clean
	@cd $(TEST_DIR)   && make clean
	@echo "completed successfully"
	@echo 

it:
	@cd $(SERVER_DIR) && make clean
	@cd $(CLIENT_DIR) && make clean
	@cd $(TEST_DIR)   && make clean
	@cd $(SERVER_DIR) && make 
	@cd $(CLIENT_DIR) && make
	@cd $(TEST_DIR)   && make all
	@cd $(TEST_DIR)   && make server_start
	@cd $(TEST_DIR)   && make client_start
	@echo "${TEST_DIR}: completed it."
	@echo 

howto:
	@echo "Explanation goes here"
	@echo 
