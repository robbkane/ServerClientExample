# tab stops set to 4
#
# Required libraries:
#
#

TOP_DIR    = `pwd`
SERVER_DIR = $(TOP_DIR)/server
CLIENT_DIR = $(TOP_DIR)/async_client

SERVER_EXE = httpserver
CLIENT_EXE = httpclient

.PHONY: clean all

all:
	@echo 
	@echo Building $(SERVER_EXE) in $(SERVER_DIR)
	@echo 
	cd $(SERVER_DIR) && make 
	@echo 
	@echo Building $(CLIENT_EXE) in $(CLIENT_DIR)
	@echo 
	cd $(CLIENT_DIR) && make 
	@echo 
	@echo "completed successfully"
	@echo 

clean:
	@echo 
	cd $(SERVER_DIR) && make clean
	@echo 
	cd $(CLIENT_DIR) && make clean
	@echo 
	@echo "completed successfully"
	@echo 
