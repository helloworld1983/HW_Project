default:
	$(CC) -o test2 D_recompile_test.c
	$(CC) -o derecompile D_recompile.c -lrt

dynamic:
	$(CC) -o test2 D_recompile_test.c
	$(CC) -DMYDY -o derecompile D_recompile.c -lrt

dump:
	$(CC) -c D_recompile_test.c
	$ objdump -d D_recompile_test.o > D_recompile_test 
	
clean:
	$(RM) -rf D_recompile_test.o D_recompile_test my_recompile derecompile test2
