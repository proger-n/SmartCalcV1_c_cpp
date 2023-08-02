OS := $(shell uname)

ifeq ($(OS),Darwin)
	CC = -Wall -Wextra -Werror -std=c11
	FLAGS = -lcheck -lm -lpthread -fprofile-arcs -ftest-coverage
	OPEN_CMD = open
else
	CC = -Wall -Werror -Wextra
	FLAGS = -lcheck -lm -lpthread -lrt -lsubunit -fprofile-arcs -ftest-coverage
	OPEN_CMD = xdg-open
endif


all: clean install gcov_report

s21_smartcalc.a:
	gcc $(CC) -c functions/s21_*.c
	ar -crs s21_smartcalc.a s21_*.o

test: clean s21_smartcalc.a
	gcc $(CC) s21_smartcalc_test.c s21_smartcalc.a $(FLAGS) -lgcov -coverage -o test
	./test

gcov_report: clean
	gcc $(CC) s21_smartcalc_test.c functions/s21_*.c $(FLAGS) -o test
	./test
	lcov -t "./test"  -o report.info --no-external -c -d .
	genhtml -o report report.info
	$(OPEN_CMD) ./report/index.html

	rm -rf *.gcno *gcda *.gco

clang:
	cp ../materials/linters/.clang-format .
	clang-format -n functions/*.c
	clang-format -n *.c *.h
	clang-format -n qt/*.h
	clang-format -n qt/*.cpp

install: uninstall
	make clean
	mkdir build
ifeq ($(OS), Linux)	
	cd qt && qmake && make && make clean && rm Makefile && cd ../ && mv qt/calculator build
else
	cd qt && qmake && make && make clean && rm Makefile && cd ../ && mv qt/calculator.app build
endif
		
uninstall:
	rm -rf build*
	
dist: install
	rm -rf dist_calculator/
	mkdir dist_calculator/
	mkdir dist_calculator/src
ifeq ($(OS), Linux)	
	mv ./build/qt dist_calculator/src/
else
	mv ./build/calculator.app dist_calculator/src/
endif	
	tar cvzf dist_calculator.tgz dist_calculator/
	rm -rf dist_calculator/
	rmdir build
	
dvi:
	open dvi.txt
	
clean: uninstall
	rm -rf dist_calculator*
	rm -rf *.o *.a test
	rm -rf *.gcno *gcda *.gcov gcov
	rm -rf report report.info
	rm -rf *.dSYM
	rm -rf *.a

valgrind:
	valgrind --tool=memcheck --leak-check=yes --track-origins=yes ./test	

.PHONY: test gcov_report clean all s21_smartcalc.a clang
