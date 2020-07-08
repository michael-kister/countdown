CC = gcc

# add -pg for debugging
CFLAGS = -Wall

countdown : main-new.c
	$(CC) $(CFLAGS) -o $@ $^

clean :
	rm countdown
