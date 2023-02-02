all: smsh part1 part2 part3

smsh: smsh.h smsh1.c splitline.c execute.c
	gcc -o smsh smsh1.c splitline.c execute.c

part1: smsh.h smsh2.c splitline.c execute.c process.c
	gcc -o smsh2 smsh2.c splitline.c execute.c process.c

part2: smsh.h smsh3.c splitline.c redirect.c
	gcc -o smsh3 smsh3.c splitline.c redirect.c

part3: smsh.h smsh4.c splitline.c redirect.c
	gcc -o smsh4 smsh4.c splitline.c redirect.c