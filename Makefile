## makefile used for testing only!!!!
##
##
##@author Timotej Kamensky xkamen24 +421 944 687 328 
##@brief makefile for unit testing symbol hash table
##@date 22.11.2020
##@note to work, has to me placed in main folder, possible conflict with the actual makefile!

##all basically copypasted from the internet: https://www.ccs.neu.edu/home/skotthe/classes/cs5600/fall/2015/labs/intro-check.html

CC = gcc
CFLAGS = -c -Wall -O0 -g
PROFILE_FLAGS = -fprofile-arcs -ftest-coverage 
TST_LIBS = -lcheck -lm -lpthread -lrt -lsubunit
COV_LIBS = -lgcov -coverage
SRC_DIR= src
TST_DIR= tests
SRC_FILES = $(addprefix $(SRC_DIR)/, *.c) 
TST_FILES = $(addprefix $(TST_DIR)/, *.c)
GCOV = gcovr 
GCONV_FLAGS = -r . --html --html-details -g


all: coverage_report.html

symtable.o:  src/symtable.c src/symtable.h
	$(CC) $(CFLAGS) -c src/symtable.c

symtable_tests.o: tests/symtable_test.c
	$(CC) $(CFLAGS) -c tests/symtable_test.c

symtable_valgrind.o: tests/symtable_valgrind.c
	$(CC) $(CFLAGS) -c tests/symtable_valgrind.c

table_hierarchy.o: src/table_hierarchy.c src/table_hierarchy.h
	$(CC) $(CFLAGS) -c src/table_hierarchy.c

table_hierarchy_unit_tests.o: tests/table_hierarchy_unit_tests.c
	$(CC) $(CFLAGS) -c tests/table_hierarchy_unit_tests.c

table_hierarchy_valgrind.o: tests/table_hierarchy_valgrind.c
	$(CC) $(CFLAGS) -c tests/table_hierarchy_valgrind.c

dynamic_string.o: src/dynamic_string.c src/dynamic_string.h
	$(CC) $(CFLAGS) -c src/dynamic_string.c

newscanner.o: src/newscanner.c src/newscanner.h src/dynamic_string.h src/newscanner.c 
	$(CC) $(CFLAGS) src/newscanner.c src/newscanner.h src/dynamic_string.h src/newscanner.c -c src/newscanner.c

tokenlist_test.o: tests/tokenlist_test.c src/newscanner.h
	$(CC) $(CFLAGS) -c tests/tokenlist_test.c



check_table_hierarchy_unit_tests: table_hierarchy.o table_hierarchy_unit_tests.o
	$(CC)  -o tests/check_table_hierarchy_unit_tests

check_table_hierarchy_memory_tests: table_hierarchy.o table_hierarchy_valgrind.o symtable.o dynamic_string.o
	$(CC) table_hierarchy.o table_hierarchy_valgrind.o symtable.o dynamic_string.o -o check_table_hierarchy_memory_tests

check_symtable_unit_tests: symtable.o symtable_tests.o
	$(CC) symtable.o symtable_test.o $(TST_LIBS) $(COV_LIBS) -o check_symtable_tests

check_symtable_memory_tests: symtable.o symtable_valgrind.o dynamic_string.o
	$(CC) symtable.o symtable_valgrind.o dynamic_string.o -o check_symtable_memory_tests

tokenlist_test: newscanner.o  tokenlist_test.o dynamic_string.o
	$(CC) newscanner.o tokenlist_test.o dynamic_string.o -o tokenlist_test
#####################################################################################################################################################
##############################   USE THESE DOWN  ######################################################################################################
table_testcases: clean check_symtable_unit_tests
	 ./check_symtable_tests 

table_testmemory: clean check_symtable_memory_tests
	valgrind --leak-check=full  ./check_symtable_memory_tests

table_hierarchy_unit_tests: clean check_table_hierarchy_unit_tests
	./check_table_hierarchy_unit_tests

hierarchy_memory: clean check_table_hierarchy_memory_tests
	valgrind --leak-check=full ./check_table_hierarchy_memory_tests

tokenlist_valgrind: clean tokenlist_test
	valgrind --leak-check=full ./tokenlist_test
#############################   USE THESE UP    #######################################################################################################
#####################################################################################################################################################
coverage_report.html: test
	$(GCOV) $(GCONV_FLAGS) -o coverage_report.html

.PHONY: clean all
#should hide "no files to delete" error..a bit unconventional, i know
clean:
	rm *.o    2> /dev/null || echo > /dev/null
	rm *.html 2> /dev/null || echo > /dev/null
	rm *.gcda 2> /dev/null || echo > /dev/null
	rm *.gcno 2> /dev/null || echo > /dev/null
	rm check_symtable_tests 2> /dev/null || echo > /dev/null
