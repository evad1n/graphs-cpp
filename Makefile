CXXFLAGS=-Wall -Werror
NAME=graph

all:
	g++ *.cpp -o $(NAME).exe

run: all
	./$(NAME).exe

debug:
	g++ -g *.cpp -o $(NAME).exe

clean:
	rm -f *.exe *.o *.csv

clean-out:
	rm -f out/*.csv

3d: all
	./$(NAME).exe -w sample-inputs/*-1000-*
