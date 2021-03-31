#ifndef CUBE3D_H
# define CUBE3D_H

# include "../minilibx/mlx.h"
# include "../libft/libft.h"
# include "../gnl/get_next_line.h"
# include <math.h>
# include <stdio.h> 
# include <unistd.h>
# include <fcntl.h>

# define INVCHARS "\t\v\r\f\n "

typedef enum s_errors
{
	ERROR,
	SUCCESS,
	ERROR_READING_FILE,
	ERROR_MAP,
	ERROR_MAP_MANY_POSITIONS,
	ERROR_MAP_NO_POSITIONS,
	ERROR_MAP_NOT_CLOSED,
	ERROR_RESOLUTION,
	ERROR_END_lINE,
	ERROR_MALLOC,


} 				t_errors;

typedef struct	s_win
{
	void 			*mlx_ptr;
	void			*win_ptr;
	int				x;
	int				y;
	int 			max_width;
	int 			max_height;
}				t_win;

typedef struct	s_img
{
	void        	*img_ptr;
    int         	bits_per_pixel;
    int         	line_length;
    int         	endian;
	unsigned int	*addr;
}				t_img;

typedef struct s_tex
{
	int 	*no;
	int		*so;
	int 	*ea;
	int 	*we;
	int 	ceil;
	int 	floor;
	int 	fd;
} 				t_tex;

typedef struct s_game
{
	char	**map;
	int		map_x;
	int 	map_y;
	int 	gpos_x;
	int		gpos_y;
}				t_game;

typedef struct s_all
{
	t_game 	game;
	t_tex	tex;
	t_img   img;
	t_win   win;
} 				t_all;


#endif
