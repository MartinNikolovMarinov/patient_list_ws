CC            = g++
CFLAGS        = -ggdb -std=c++17 -Wno-unknown-pragmas -O0 -g -Wall -Wno-unused
DEBUG_DEFINES = -DDEBUG=1
SRC           = main.cpp $(shell find ./src/ -name "*.cpp")
OUT_DIR       = build
BIN_NAME      = main

.PHONY: help
help:
	@echo Supported targets:
	@cat $(MAKEFILE_LIST) | grep -e "^[\.a-zA-Z0-9_-]*: *.*## *" | awk 'BEGIN {FS = ":.*?## "}; {printf "\033[36m%-35s\033[0m %s\n", $$1, $$2}'

.DEFAULT_GOAL := help

.PHONY: build
build: ## Build the project in the build folder. Creates ./build folder if it does not exist.
	mkdir -p $(OUT_DIR)
	$(CC) $(CFLAGS) $(DEBUG_DEFINES) $(HEADERS) $(SRC) -o $(OUT_DIR)/$(BIN_NAME)

.PHONY: patient_list
patient_list: ## List brief patients information.
	$(OUT_DIR)/$(BIN_NAME) patient_list

.PHONY: patient_list_with_details
patient_list_with_details: ## List detailed patients information.
	$(OUT_DIR)/$(BIN_NAME) patient_list_with_details

.PHONY: clean
clean: ## Deletes the build folder.
	rm -rf $(OUT_DIR)