# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
#			user-configurable variables			      #
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
OBJ_DIR ?= target
ARCH ?= x86_64

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
#			toolchain default configuration			      #
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
CC := $(ARCH)-linux-gnu-gcc
CPPFLAGS := -Iinclude
CFLAGS := 			\
	-Wint-conversion

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
#				recipes					      #
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
all: $(OBJ_DIR)/cat $(OBJ_DIR)/echo $(OBJ_DIR)/grep $(OBJ_DIR)/kill $(OBJ_DIR)/rm $(OBJ_DIR)/wc

$(OBJ_DIR)/cat:
	$(CC) $(CPPFLAGS) $(CFLAGS) src/cat.c -o $(OBJ_DIR)/cat

$(OBJ_DIR)/echo:
	$(CC) $(CPPFLAGS) $(CFLAGS) src/echo.c -o $(OBJ_DIR)/echo

$(OBJ_DIR)/grep:
	$(CC) $(CPPFLAGS) $(CFLAGS) src/grep.c -o $(OBJ_DIR)/grep

$(OBJ_DIR)/kill:
	$(CC) $(CPPFLAGS) $(CFLAGS) src/kill.c -o $(OBJ_DIR)/kill

$(OBJ_DIR)/rm:
	$(CC) $(CPPFLAGS) $(CFLAGS) src/rm.c -o $(OBJ_DIR)/rm

$(OBJ_DIR)/wc:
	$(CC) $(CPPFLAGS) $(CFLAGS) src/wc.c -o $(OBJ_DIR)/wc
