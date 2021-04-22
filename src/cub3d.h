/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cube3d.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: burswyck <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/17 13:23:28 by burswyck          #+#    #+#             */
/*   Updated: 2021/04/17 13:23:30 by burswyck         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include "../minilibx/mlx.h"
# include "../libft/libft.h"
# include "get_next_line.h"
# include <math.h>
# include <stdio.h> 
# include <unistd.h>
# include <fcntl.h>

# define INVCHARS "\t\v\r\f "
# define ESC 53
# define W 13
# define A 0
# define S 1
# define D 2
# define UP 126
# define DOWN 125
# define LEFT 123
# define RIGHT 124
# define SPEED 0.4
# define ROTATE_S 0.04
# define NONE 0xFF000000
# define UDIV 1
# define VDIV 1
# define VMOVE 0.0

typedef enum e_errors
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
	ERROR_TEXTUTE_EX,
	ERROR_TEXTUTE_FILE,
	ERROR_PLAYER_POSITION,
	ERROR_NO_PLAYER_POSITION,
	ERROR_MAP_CHAR,
}	t_errors;

typedef struct s_mlx
{
	void	*mlx_ptr;
}	t_mlx;

typedef struct s_win
{
	void	*win_ptr;
	int		x;
	int		y;
	int		max_width;
	int		max_height;
}	t_win;

typedef struct s_img
{
	void	*img_ptr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
	int		*addr;
}	t_img;

typedef struct s_tex
{
	int				*no;
	int				*so;
	int				*ea;
	int				*we;
	int				*sp;
	unsigned int	ceil;
	unsigned int	floor;
	int				fd;
}	t_tex;

typedef struct s_game
{
	char	**map;
	int		map_x;
	int		map_y;
	double	gpos_x;
	double	gpos_y;
	int		count_sprites;
}	t_game;

typedef struct s_map
{
	char			*line;
	struct s_map	*next;
}	t_map;

typedef struct s_color
{
	int	r;
	int	g;
	int	b;
}	t_color;

typedef struct s_ray
{
	double	camera_x;
	double	ray_dir_x;
	double	ray_dir_y;
	double	dir_x;
	double	dir_y;
	double	plane_x;
	double	plane_y;
	double	sdx;
	double	sdy;
	double	deltaDistX;
	double	deltaDistY;
	int		sx;
	int		mapX;
	int		mapY;
	double	perpWallDist;
	int		stepX;
	int		stepY;
	int		side;
	int		hit;
	int		h;
	int		lineHeight;
	int		drawStart;
	int		drawEnd;
	double	wallX;
	int		texX;
	double	step;
	double	texPos;
	int		texY;
	int		color;
	double	arcos;
	double	*zbuffer;
}	t_ray;

typedef struct s_sprite
{
	double	x;
	double	y;
}	t_sprite;

typedef struct s_sray
{
	double	sX;
	double	sY;
	double	invDet;
	double	trX;
	double	trY;
	int		sSX;
	int		vMS;
	int		sH;
	int		dSY;
	int		drawEndY;
	int		sW;
	int		dSX;
	int		dEX;
	int		texX;
	int		d;
	int		texY;
	int		color;
}	t_sray;

typedef struct s_all
{
	t_game		game;
	t_tex		tex;
	t_img		img;
	t_win		win;
	t_mlx		mlx;
	t_map		*map;
	t_ray		ray;
	char		*tmp;
	t_sprite	*sprite;
	t_sray		sray;
}	t_all;

int		screen_ray(t_all *all);
void	verLine(int drawStart, int drawEnd, int color, t_all *all);
int		error_str(int error);
int		file_parse(int fd, char *namefile, t_all *all, int error);
int		get_resolution(int *i, char *buf, t_all *all);
int		get_texture(int *i, char *buf, t_all *all, int **addr);
int		check_full_tex(t_all *all, int **addr, char *filename_tex);
int		get_color(int *i, char *buf, unsigned int *clr);
int		get_map(int *i, char *buf, t_all *all);
int		ft_lst_sz(t_map *lst);
void	ft_lstadd_b(t_map **lst, t_map *new);
t_map	*ft_lstl(t_map *lst);
t_map	*ft_lstn(char *content);
int		error_str(int error);
int		ft_atoi_cub(int *i, char *buf, int num);
int		get_resolution(int *i, char *buf, t_all *all);
void	verLine(int drawStart, int drawEnd, int color, t_all *all);
void	draw_walls(t_all *all);
void	calc_texture(t_all *all);
int		key_press(int key, t_all *all);
int		get_texture(int *i, char *buf, t_all *all, int **addr);
int		close_win(t_all *all);
int		screenshot	(t_all *all);
void	ft_sprite(t_all *all);
void	draw_sprite(t_all *all, int i);
int		get_color(int *i, char *buf, unsigned int *color);
int		check_map(t_all *all, int i, int j, char ch);
void	correct_position(t_all *all, int i, int j, char *ch);
int		close_win(t_all *all);
void	add_sprites(t_all *all, int i, int j, int count);
void	full_new_line(char *buffer, char *new_str);
char	*cor_start_ch(char *buffer);
void	correct_position(t_all *all, int i, int j, char *ch);
int		skip_spaces(char *line, int *i);

#endif
