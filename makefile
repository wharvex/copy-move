p3: p3_main.c p3_lib.c
	rm -f move && gcc -o copy p3_main.c p3_lib.c -lm && ln copy move
