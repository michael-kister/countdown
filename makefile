CC = clang

#play_letters : letters.c
#	CC -o $@ $^


play_numbers : numbers.c
	CC -o $@ $^

clean :
	rm play_numbers play_letters
