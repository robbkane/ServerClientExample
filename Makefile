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

.PHONY: all clean howto it

all:
	@echo Building $(SERVER_EXE) in $(SERVER_DIR)
	@cd $(SERVER_DIR) && make 
	@echo Building $(CLIENT_EXE) in $(CLIENT_DIR)
	@cd $(CLIENT_DIR) && make 
	@echo "Building links in \" "$(TEST_DIR) "\""
	@cd $(TEST_DIR) && make 
	@echo "completed successfully"
	@echo 

clean:
	@cd $(SERVER_DIR) && make clean
	@cd $(CLIENT_DIR) && make clean
	@cd $(TEST_DIR) && make clean
	@echo "completed successfully"
	@echo 

it:
	@cd $(TEST_DIR) && make it
	@echo "completed successfully"
	@echo 

howto:
	@echo "Explanation goes here"
	@echo 
