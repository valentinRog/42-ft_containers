CXX = c++

CXXFLAGS = -Wall -Wextra -Werror -std=c++98

SRC = main.cpp

OBJ_STD = std.o
OBJ_FT = ft.o

NAME_STD = std
NAME_FT = ft

STD_OUTPUT = std.output
FT_OUTPUT = ft.output

OUTPUT_DIFF = output.diff

all: $(NAME_STD) $(NAME_FT)

$(OBJ_STD): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -D NS=std -c -o $@
$(OBJ_FT): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -D NS=ft -c -o $@

$(NAME_STD): $(OBJ_STD)
	$(CXX) $(CXXFLAGS) $(OBJ_STD) -o $@
$(NAME_FT): $(OBJ_FT)
	$(CXX) $(CXXFLAGS) $(OBJ_FT) -o $@

run: all
	time ./$(NAME_STD) > $(STD_OUTPUT)
	time ./$(NAME_FT) > $(FT_OUTPUT)
	diff $(STD_OUTPUT) $(FT_OUTPUT) > $(OUTPUT_DIFF)

clean:
	$(RM) $(OBJ_STD) $(OBJ_FT) $(STD_OUTPUT) $(FT_OUTPUT) $(OUTPUT_DIFF)

fclean: clean
	$(RM) $(NAME_STD) $(NAME_FT)

re: fclean all
