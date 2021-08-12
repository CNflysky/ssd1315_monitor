DIR_CUR = $(shell pwd)
DIR_SRC = $(DIR_CUR)/src
DIR_INC = $(DIR_CUR)/include
DIR_OUT = $(DIR_CUR)

OBJ_C = $(wildcard ${DIR_SRC}/*.c)
OBJ_O = $(patsubst %.c,${DIR_OUT}/%.o,$(notdir ${OBJ_C}))
INSTALLANG = $(DIR_CUR)/locales/install_locale.sh
RMLANG = $(DIR_CUR)/locales/remove_locale.sh
SH = bash

TARGET = ssd1315

CC = gcc
CP = cp

DEBUG = -g -O0
CFLAGS += $(DEBUG)

LIB = -lgpiod

install_lang: $(TARGET)
	$(SH) $(INSTALLANG)

${TARGET}:${OBJ_O}
	$(CC) $(CFLAGS) $(OBJ_O) -o $@ $(LIB)

${DIR_OUT}/%.o : $(DIR_SRC)/%.c
	$(CC) $(CFLAGS) -c $< -o $@ $(LIB) -I$(DIR_INC)  

clean :
	$(RM) $(DIR_OUT)/*.o $(TARGET)
	$(SH) $(RMLANG)
	
