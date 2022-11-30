CC = g++
CFLAGS = -g -Wall -Wextra 
OUTDIR = bin

main: test/cells_sep.cpp  src/sexpr.cpp test/pprint.cpp  test/main.cpp 
	$(CC) $(CFLAGS) -o $(OUTDIR)/$@ $^
