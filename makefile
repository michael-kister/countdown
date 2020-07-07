CC = clang

countdown : main-new.c
	CC -o $@ $^

clean :
	rm countdown
