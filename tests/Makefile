CC = clang
CFLAGS = -Wall -Wextra -g
SRC = /home/a000040/proj/C/data_structures/src
INCLUDE = /home/a000040/proj/C/data_structures/include 
HASH = $(SRC)/hash_fun.c $(SRC)/hash_table.c 
hash.o: $(HASH) 
	$(CC) $(CFLAGS)
insert_get.o: test_htstr_insersion_retrival.c 
	$(CC) $(CFLAGS) -I $(INCLUDE) -c test_htstr_insersion_retrival.c -o test_htstr_insersion_retrival.o 
insert_get: hash_fun.o hash_table.o test_htstr_insersion_retrival.o 
	$(CC) $(CFLAGS) hash_fun.o hash_table.o test_htstr_insersion_retrival.o -o test_htstr_insersion_retrival.out



