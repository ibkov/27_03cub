NAME = cub3D

INCLUDES = -I includes/ -I libft/ -I gnl/

FLAGS = -Wall -Wextra -Werror

CC = gcc -g

DIR_L = src/

LIST = 	cube3d.c \
		get_next_line.c \
		get_next_line_utils.c \
		ray_casting.c \
		parse.c \
		utils_list.c \
		print_error.c \
		get_file_args.c \
		utils_ray.c \
		key_press.c \
		bitmap.c \
		render_sprite.c \
		utils_cub.c \

SRC = $(addprefix $(DIR_L), $(LIST))

OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)	
		make -C minilibx
		mv minilibx/libmlx.a .
		make -C libft
		mv libft/libft.a .
		$(CC) -o $(NAME) -L. -lmlx -lft -framework OpenGL -framework Appkit $(OBJ)

clean:
		make -C minilibx clean
		make -C libft clean
		rm -rf $(OBJ) libft.a libmlx.a

fclean: clean
		make -C minilibx clean
		make -C libft clean
		rm -rf $(NAME)

re : fclean all
