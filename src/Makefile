CC = gcc
CFLAGS = -Wall -Werror -Wextra -std=c11
GCOV_FLAGS = --coverage

SRC = s21_matrix.c
OBJ = $(SRC:.c=.o)
TEST_SRC = s21_matrix_test.c
TEST_OBJ = $(TEST_SRC:.c=.o)
HEADER = s21_matrix.h

OS := $(shell uname -s)

ifeq ($(OS),Darwin)
    HOMEBREW_PREFIX := $(shell brew --prefix 2>/dev/null || echo /opt/homebrew)
    CFLAGS += -I$(HOMEBREW_PREFIX)/include
    LDFLAGS = -L$(HOMEBREW_PREFIX)/lib
    CHECK_LIBS = -lcheck
else
    LDFLAGS = 
    CHECK_LIBS = -lcheck -lsubunit -lm -lpthread
endif

.PHONY: all clean test gcov_report rebuild

all: s21_matrix.a

s21_matrix.a: $(OBJ)
	ar rcs s21_matrix.a $(OBJ)
	ranlib s21_matrix.a 2>/dev/null || true

%.o: %.c $(HEADER)
	$(CC) $(CFLAGS) -c $< -o $@

test: test_matrix
	./test_matrix

test_matrix: $(OBJ) $(TEST_OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(TEST_OBJ) -o test_matrix $(LDFLAGS) $(CHECK_LIBS)

$(TEST_OBJ): $(TEST_SRC) $(HEADER)
	$(CC) $(CFLAGS) -c $< -o $@

gcov_report: CFLAGS += $(GCOV_FLAGS)
gcov_report: clean test_matrix
	./test_matrix
	lcov -t "s21_matrix" -o s21_matrix.info -c -d . --rc lcov_branch_coverage=1
	genhtml -o report s21_matrix.info --rc lcov_branch_coverage=1
	@echo "Отчёт создан в папке report/"

clean:
	rm -f *.o *.gcno *.gcda
	rm -f test_matrix s21_matrix.a
	rm -f *.info
	rm -rf report

rebuild: clean all
