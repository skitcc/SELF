$(shell mkdir -p out unit_tests)

CC := gcc

CFLAGS := -std=c99 -Wall -Werror -Wpedantic -Wextra -Wvla -Wfloat-conversion -Wfloat-equal -Iinc -g3
LINKFLAGS := -lcheck -lrt -lpthread -lsubunit -lm

SRC_DIR := ./src/
INC_DIR := ./inc/
OUT_DIR := ./out/
UNIT_DIR := ./unit_tests/

SRCS := $(wildcard $(SRC_DIR)*.c)
UNIT_TESTS := $(wildcard $(UNIT_DIR)*.c)

APP_OBJS := $(patsubst $(SRC_DIR)%.c, $(OUT_DIR)%.o, $(SRCS))
UNITONLY_OBJS := $(patsubst $(UNIT_DIR)%.c, $(OUT_DIR)%.o, $(UNIT_TESTS))
NOMAIN_OBJS := $(filter-out $(OUT_DIR)main.o, $(APP_OBJS)) 

all: app.exe unit_tests.exe

app.exe : $(APP_OBJS)
	@$(CC) -o $@ $^

$(OUT_DIR)%.o : $(SRC_DIR)%.c
	@$(CC) $(CFLAGS) -c -MD $< -o $@

unit_tests.exe : $(UNITONLY_OBJS) $(NOMAIN_OBJS)
	@$(CC) -o $@ $^ $(LINKFLAGS)

$(OUT_DIR)%.o : $(UNIT_DIR)%.c
	@$(CC) $(CFLAGS) -c -MD $< -o $@

include $(wildcard $(OUT_DIR)*.d)

.PHONY : all clean build_coverage collect_coverage unit func

build_coverage : CFLAGS += --coverage -O0
build_coverage : $(APP_OBJS)
	@$(CC) $(CFLAGS) $^ -o app.exe

collect_coverage : build_coverage func
	@bash -c 'cd $(OUT_DIR) && for file in *.gcda; do \
    	    echo -n "Покрытие кода файла $$(basename $${file} .gcda).c: "; \
    	    gcov "$$file" 2>/dev/null | grep -oEm 1 "[0-9]+\.[0-9]+%"; \
    		done'

unit : unit_tests.exe
	@./unit_tests.exe

func : app.exe
	@./func_tests/scripts/func_tests.sh

clean :
	@$(RM) $(OUT_DIR)* ./*.exe
