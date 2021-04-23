NAME = cub3D

FLAGS = -Wall -Wextra -Werror

MLX = libmlx.a

LIBFT = libft.a

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
		$(CC) -o $(NAME) -L. -lmlx -lft -framework OpenGL -framework Appkit $(OBJ)
	
$(OBJ): $(MLX) $(LIBFT)

$(MLX): $(F_MLX)
		make -C minilibx
		mv minilibx/libmlx.a .

$(LIBFT): $(F_LIBFT)
		make -C libft
		mv libft/libft.a .

$(F_LIBFT): libft/%.c=%.o

$(F_MLX): minilibx/%.c=%.o

clean:
		make -C minilibx clean
		make -C libft clean
		rm -rf $(OBJ) libft.a libmlx.a bitmap.bmp

fclean: clean
		make -C minilibx clean
		make -C libft clean
		rm -rf $(NAME)

re : fclean all
