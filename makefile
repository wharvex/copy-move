p3: p3_main.c p3_lib.c
	gcc -o copy p3_main.c p3_lib.c -lm && rm -f move && ln copy move
