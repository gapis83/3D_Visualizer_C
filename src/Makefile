CC = gcc
CFLAGS =-std=c11 -Wall -Werror -Wextra -g
SOURCES= ./parser/s21_viewer.c
OBJECTS=$(SOURCES:.c=.o)
OS = $(shell uname)
UNIT_TESTS=./parser/viewer_tests.c
VIEW=makefile.qt
PROFILE=./3DViewer_v1/3DViewer_v1.pro

ifeq ($(OS), Darwin)
	TEST_LIBS=-lcheck
	EXEC=3DViewer_v1.app
else
	TEST_LIBS=-lcheck -lsubunit -pthread -lrt -lm
	EXEC=3DViewer_v1
endif

all: clean_lib s21_viewer.a tests

s21_viewer.a: clean $(OBJECTS)
	@ar rcs s21_viewer.a $(OBJECTS)
	@rm -f *.o

tests: clean $(UNIT_TESTS) s21_viewer.a
	@$(CC) $(CFLAGS) $(UNIT_TESTS) $(SOURSES) -o test $(TEST_LIBS) -L. --coverage
	@./test
	
check: tests
	cppcheck --enable=all --suppress=missingIncludeSystem --inconclusive --check-config ./parser/s21_viewer.c ./parser/s21_viewer.h ./parser/viewer_tests.c ./3DViewer_v1/glviewer.cpp ./3DViewer_v1/glviewer.h ./3DViewer_v1/main.cpp ./3DViewer_v1/mainwindow.h ./3DViewer_v1/selectcolorbutton.cpp ./3DViewer_v1/selectcolorbutton.h
	cp ../materials/linters/.clang-format ./parser
	clang-format -n ./parser/*.c ./parser/*.h
	rm -rf ./parser/.clang-format
	cp ../materials/linters/.clang-format ./3DViewer_v1
	clang-format -n ./3DViewer_v1/glviewer.cpp ./3DViewer_v1/glviewer.h ./3DViewer_v1/main.cpp ./3DViewer_v1/mainwindow.cpp ./3DViewer_v1/mainwindow.h ./3DViewer_v1/selectcolorbutton.cpp ./3DViewer_v1/selectcolorbutton.h
	rm -rf ./3DViewer_v1/.clang-format
ifeq ($(OS), Darwin)
	leaks --atExit -- test
else
	valgrind --leak-check=full --track-origins=yes --show-leak-kinds=all -s --log-file=VALGRIND_RESULT.txt ./test #--verbose - to see more
endif

install:
	make clean
	mkdir build
	qmake -o $(VIEW) $(PROFILE) && make -f $(VIEW) && make clean -f $(VIEW) && rm $(VIEW) && mv $(EXEC) build 
	open ./build/$(EXEC)

uninstall:
	rm -rf build*
	
dvi:
	cp ./about/info.html .
	open info.html

dist:
	rm -rf Archive_3DViewer_v1.app/
	mkdir Archive_3DViewer_v1/
	mkdir Archive_3DViewer_v1/src
	mv ./build/$(EXEC) Archive_3DViewer_v1/src
	tar cvzf Archive_3DViewer_v1.tgz Archive_3DViewer_v1/
	rm -rf Archive_3DViewer_v1/

gcov_report: gcov_report_build
	@rm -rf *.gcda
	@rm -rf *.gcno
	@rm -rf *.info

gcov_report_build:
	@$(CC) $(FLAGS) --coverage $(UNIT_TESTS) $(SOURSES) -o gcov_report $(TEST_LIBS)
	@./gcov_report
	@lcov -t "./gcov_report" -o gcov_report.info -c -d .
	@genhtml -o report gcov_report.info
	open ./report/index.html

clean_lib: 
	@rm -rf ./parser/*.a
	@rm -rf ./parser/*.o
	@rm -rf ./*.a

clean_test:
	@rm -rf *.{a,odSYM,out}
	@rm -rf gcov_report
	@rm -rf report
	@rm -rf s21_test*
	@rm -rf .DS_Store 
	@rm -rf gcov_test
	@rm -rf build
	@rm -rf *.gcno
	@rm -rf *.gcda
	@rm -rf *.info
	@rm -rf *.dSYM
	@rm -rf .clang_format
	@rm -rf VALGRIND_RESULT*

clean: clean_lib clean_test
	@rm -rf a.out
	@rm -rf s21_3d_viewer main test gcov_test main.dSYM

# memory:
# 	valgrind \
# 		-s \
# 		--track-origins=yes \
# 		--show-leak-kinds=all \
# 		./test; exit 0;
