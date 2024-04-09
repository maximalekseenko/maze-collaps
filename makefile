EXECUTABLE  = maze-collapse

CODE_DIR 	= ./src/code/
DATA_DIR 	= ./src/data/

BIN_DIR     = ./bin/
OBJ_DIR 	= ./bin/obj/
OUT_PREF	?= build
OUT_DIR		= $(BIN_DIR)/$(OUT_PREF)

SOURCES 	= $(wildcard $(CODE_DIR)/*.cpp) $(wildcard $(CODE_DIR)/**/*.cpp)
LIBS	    = -lncursesw

OBJ_PATTERN = $(OBJ_DIR)%.o
SRC_PATTERN = $(CODE_DIR)%.cpp
OBJECTS		= $(SOURCES:$(SRC_PATTERN)=$(OBJ_PATTERN))

CC          = g++
CFLAGS      = -std=c++20 -Wall -lncursesw -I$(CODE_DIR)
DEBUG_CFLAGS= -fsanitize=address -fdiagnostics-color=always 

# ------------------------- commands -------------------------
all: build-debug-full

run:
	(cd $(OUT_DIR); ./$(EXECUTABLE))

build-debug: create_directories create_executable create_data
	(cd $(OUT_DIR); ./$(EXECUTABLE))


build-debug-full: clean-full build-debug
	(cd $(OUT_DIR); ./$(EXECUTABLE))


build-prod: build-debug-full clean


.PHONY: all run build-debug build-debug-full build-prod


# ------------------------- create directories -------------------------
create_directories:
#	+++ create binary directory +++
	mkdir -p $(BIN_DIR)
# 	+++ create object directory +++
	mkdir -p $(OBJ_DIR)
# 	+++ create out directory +++
	mkdir -p $(OUT_DIR)


.PHONY: create_directories


# ------------------------- create executable -------------------------
create_executable: $(SOURCES) $(EXECUTABLE)


$(OBJECTS): $(OBJ_PATTERN) : $(SRC_PATTERN) # +++ compile +++
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@


$(EXECUTABLE): $(OBJECTS) # +++ link +++
	# ifeq ($(UNAME), x86_64)
	$(CC) $(CFLAGS) $(LIBS) $(OBJECTS) -o $(OUT_DIR)/$(EXECUTABLE)


.PHONY: create_executable $(EXECUTABLE)


# ------------------------- create data -------------------------
create_data:
	rsync -a --delete $(DATA_DIR) $(OUT_DIR)/data


.PHONY: create_data


# ------------------------- extra -------------------------
clean: # +++ cleanup +++
	$(RM) -rf $(OBJ_DIR)


clean-full: # +++ cleanup +++
	$(RM) -rf $(BIN_DIR)


.PHONY: clean clean-full