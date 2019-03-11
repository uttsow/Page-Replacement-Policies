FILES = Pages.o runMe.o
NAME = Assignment3
CC = g++
CFLAG = -Wall -Wextra -DDEBUG -g -pedantic -std=c++14
OPTION = $(CFLAG) -c

all: $(NAME)


$(NAME): $(FILES)
	$(CC) $(CFLAG) $(FILES) -o $(NAME)


Pages.o: Pages.cpp
	$(CC) $(OPTION) Pages.cpp -o Pages.o

runMe.o: runMe.cpp
	$(CC) $(OPTION) runMe.cpp -o runMe.o

clean:
	rm *.o $(NAME)

run:
	./$(NAME)

run_gnu:
	./$(NAME)
	gnuplot -e "title='No locality Workload'" -e "input_filename='noLocality_data.csv'" plot_hit_rates.plt > noLocality_plot.png
	gnuplot -e "title='80-20 Workload'" -e "input_filename='8020_data.csv'" plot_hit_rates.plt > 8020_plot.png
	gnuplot -e "title='Looping Workload'" -e "input_filename='looping_data.csv'" plot_hit_rates.plt > looping_plot.png







checkmem: all
	valgrind ./$(NAME)
