EXECUTABLE  = maze-collapse

CC          = g++
LDFLAGS     =
CFLAGS      = -fsanitize=address -fdiagnostics-color=always -std=c++20 -Wall

CODE_DIR 	= ./src/code/
DATA_DIR 	= ./src/data/

BIN_DIR     = ./bin/
OBJ_DIR 	= ./bin/obj/
OUT_PREF	?= build
OUT_DIR		= $(BIN_DIR)/$(OUT_PREF)

SOURCES 	= $(wildcard $(CODE_DIR)/*.cpp)
LIBS	    = -lncurses

OBJ_PATTERN = $(OBJ_DIR)%.o
SRC_PATTERN = $(CODE_DIR)%.cpp
OBJECTS		= $(SOURCES:$(SRC_PATTERN)=$(OBJ_PATTERN))


# ------------------------- commands -------------------------
all: build-update


build-debug: create_directories create_executable create_data
	(cd $(OUT_DIR); ./$(EXECUTABLE))
build-full-debug: clean create_directories create_executable create_data clean
	(cd $(OUT_DIR); ./$(EXECUTABLE))
build-full: clean create_directories create_executable create_data clean


# ------------------------- create directories -------------------------
create_directories:
	#	+++ create binary directory +++
	mkdir -p $(BIN_DIR)
# 	+++ create object directory +++
	mkdir -p $(OBJ_DIR)
# 	+++ create out directory +++
	mkdir -p $(OUT_DIR)


# ------------------------- create executable -------------------------
create_executable: $(SOURCES) $(EXECUTABLE)


$(OBJECTS): $(OBJ_PATTERN) : $(SRC_PATTERN) # +++ compile +++
	$(CC) $(CFLAGS) -c $< -o $@


$(EXECUTABLE): $(OBJECTS) # +++ link +++
	$(CC) $(CFLAGS) $(LIBS) $(OBJECTS) -o $(OUT_DIR)/$(EXECUTABLE)


# ------------------------- create data -------------------------
create_data:
	rsync -a --delete $(DATA_DIR) $(OUT_DIR)/data


# ------------------------- extra -------------------------
clean: # +++ cleanup +++
	$(RM) -rf $(OBJ_DIR)


.PHONY: create_directories create_executable create_data clean build-debug build-prod