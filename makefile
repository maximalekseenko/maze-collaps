EXECUTABLE  = maze-collapse

CC          = g++
LDFLAGS     =
CFLAGS      = -std=c++20
CFLAGS_DEBUG= -Wall -fsanitize=address -fdiagnostics-color=always
LIBS	    = -lncursesw
 
CODE_DIR 	= ./src/code/
DATA_DIR 	= ./src/data/

BIN_DIR     = ./bin/
OBJ_DIR 	= ./bin/obj/
OUT_PREF	?= build
OUT_DIR		= $(BIN_DIR)/$(OUT_PREF)

SOURCES 	= $(wildcard $(CODE_DIR)/*.cpp) $(wildcard $(CODE_DIR)/**/*.cpp)

OBJ_PATTERN = $(OBJ_DIR)%.o
SRC_PATTERN = $(CODE_DIR)%.cpp
OBJECTS		= $(SOURCES:$(SRC_PATTERN)=$(OBJ_PATTERN))

define cc-command
$(CC) $(CFLAGS) -c $< -o $@
endef


# ------------------------- commands -------------------------
all: build-debug-full


run:
	(cd $(OUT_DIR); ./$(EXECUTABLE))


build-debug: CFLAGS += $(CFLAGS_DEBUG)
build-debug: build run


build-debug-full: clean-full build-debug


build: create_directories create_executable create_data


build-prod: clean-full build clean


.PHONY: all run build-debug build-debug-full build-prod build


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


# +++ compile +++
$(OBJECTS): $(OBJ_PATTERN) : $(SRC_PATTERN) 
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@


# +++ link +++
$(EXECUTABLE): $(OBJECTS) 
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