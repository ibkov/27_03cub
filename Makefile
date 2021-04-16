NAME = cube3D

SRC = 	cube3d.c \
		gnl/get_next_line.c \
		gnl/get_next_line_utils.c \
		ray_casting.c \
		parse.c \
		utils_list.c \
		print_error.c \
		get_file_args.c \
		utils_ray.c



OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
		make -C minilibx
		mv minilibx/libmlx.a .
		make -C libft
		mv libft/libft.a .
		gcc -Wall -g -Werror -Wextra -c $(SRC)
		gcc -Wall -g -Werror -Wextra -L. -lmlx -lft -framework OpenGL -framework Appkit $(OBJ) -o $(NAME)

clean:
		make -C minilibx clean
		rm -rf $(OBJ) libft.a libmlx.a

fclean: clean
		make -C minilibx clean
		rm -rf $(NAME)

re : fclean all