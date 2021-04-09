# ...
NAME = hmd
DFLAGS = -g -O0 -DDEBUG_H -fsanitize=address -fsanitize-undefined-trap-on-error 
CFLAGS = -Wall -Werror -Wno-unused -Wno-format-security -fPIC -std=c99 -Ivendor
SFLAGS = -Wall -Werror -Wno-unused -Wno-format-security -std=c99 -Ivendor -Iinclude
CC = clang
SRC = vendor/zwalker.c vendor/zhttp.c main.c
OBJ = $(SRC:.c=.o)
TARGET=


# lib - build shared object for use with hypno
lib: clean 
lib: objects
lib:
	$(CC) $(SFLAGS) -shared -o lib/lib$(NAME).so -lmarkdown $(OBJ)


# cli - Builds test harness program
cli: 
	$(CC) $(CFLAGS) -ldl -lsqlite3 -o bin/harness harness.c vendor/zhttp.c vendor/zwalker.c vendor/megadeth.c


# debug - turn debugging on
debug: CFLAGS += $(DFLAGS)
debug: SFLAGS += $(DFLAGS)
debug: $(TARGET) 
	@printf '' > /dev/null


# clean - remove all objects
clean:
	-find -type f -name "*.o" | xargs rm
	-rm bin/lib$(NAME).so


# objects - 
objects: $(OBJ)
	@printf '' > /dev/null


# test - Compile tests for files in tests/
test: clean
test: CFLAGS = -DDEBUG_H -Wall -Werror -Wno-unused -Wno-format-security -std=c99 -Ivendor
test: objects
test:
	$(CC) $(CFLAGS) -llua -lsqlite3 -o bin/test $(OBJ) tests.c

#lmain: SRC += lua.c
#lmain: CFLAGS += -Werror -Wno-unused -Wno-format-security -std=c99 -Ivendor -DRUN_MAIN $(DFLAGS)
#lmain: $(OBJ)
#lmain: clean
#	$(CC) $(CFLAGS) -c -o lua.o lua.c
#	$(CC) $(CFLAGS) -llua -lsqlite3 -o bin/lmain $(OBJ)


