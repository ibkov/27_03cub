#include "./includes/cube3d.h"


int check_full_tex(t_all *all, int **addr, char *filename_tex)
{
  void *img;
  int array[5];

  if (!(ft_strncmp(filename_tex + ft_strlen(filename_tex) - 4, ".xmp", 4)))
    return (ERROR_TEXTUTE_EX);
  if ((all->tex.fd = open(filename_tex, O_RDONLY)) == -1)
    return (ERROR_TEXTUTE_FILE);
  close(all->tex.fd);
  img = mlx_xpm_file_to_image(&all->mlx.mlx_ptr, filename_tex, &array[0], &array[1]);
  if (array[0] != 64 || array[1] != 64 || !img)
    return (ERROR);
  *addr = (int *)mlx_get_data_addr(img, &array[2], &array[3], &array[4]);
  free(img);
  return(SUCCESS);

}

int get_texture(int *i, char *buf, t_all *all, int **addr)
{
  int rem;
  char *filename_tex;
  int error;

  error = 0;
  if (*addr != NULL)
		return (ERROR);
  (*i) += 2;
  while (ft_strchr(INVCHARS, buf[(*i)]))
    (*i)++;
  rem = (*i);
  while (!ft_strchr(INVCHARS, buf[(*i)]))
    (*i)++;
  if (!(filename_tex = (char*)malloc(sizeof(char) * (*i - rem + 1))))
    return (ERROR_MALLOC);
  *i = rem;
  rem = 0;
  while(buf[*i] != ' ' && buf[*i] != '\0')
    filename_tex[rem++] = buf[(*i)++];
  filename_tex[rem] = '\0';
  error = check_full_tex(all, addr, filename_tex);
  free(filename_tex);
  return (error);
}

int get_color(int *i, char *buf, unsigned int *clr)
{
  t_color rgb;
  ft_bzero(&rgb, sizeof(t_color));
  if (*clr != 0xFF000000)
    return (ERROR);
  (*i)++;
  rgb.r = ft_atoi_cub(i, buf, 0);
  (*i)++;
  rgb.g = ft_atoi_cub(i, buf, 0);
  (*i)++;
  rgb.b = ft_atoi_cub(i, buf, 0);
  while (!ft_strchr(INVCHARS, buf[(*i)]))
    (*i)++;
  if (buf[*i] || rgb.r > 255 || rgb.b > 255 || rgb.r > 255)
    return (ERROR);
  *clr = rgb.r * 256 * 256 + rgb.g * 256 + rgb.b;
  return (SUCCESS);
}

void full_new_line(char *buffer, char *new_str)
{
  int i;
  int j;

  i = 0;
  j = 0;
  while (buffer[i])
  {
    if (buffer[i] != ' ')
    {
      new_str[j] = buffer[i];
      j++;
    }
    i++;
  }
  new_str[j] = '\0';
}

char *cor_start_ch(char *buffer)
{
  int i;
  int cs;
  char *new_str;

  i = 0;
  cs = 0;
  while (buffer[i])
  {
    if (buffer[i] == ' ')
      cs++;
    i++;
  }
  if (!(new_str = (char*)malloc(sizeof(char) * (ft_strlen(buffer) - cs + 1))))
    return (NULL);
  full_new_line(buffer, new_str);
  return (new_str);
}

char		**map_to_matrix(t_all *lst)
{
	t_map	*temp;
	int		i;
	int		j;
	char	**matrix;

	temp = lst->map;
	if (!(matrix = malloc(sizeof(char*) * (ft_lst_sz(lst->map) + 1))))
		return (NULL);
	i = 0;
	while (i < ft_lst_sz(lst->map))
	{
		if (!(matrix[i] = malloc(sizeof(char) * (ft_strlen(temp->line) + 1))))
			return (NULL);
		j = 0;
		while (temp->line[j] != '\0')
		{
			matrix[i][j] = temp->line[j];
      if (matrix[i][j] == 'N')
      {
        lst->game.gpos_x = i; 
        lst->game.gpos_y = j;
      }
			j++;
		}
		matrix[i][j] = '\0';
		temp = temp->next;
		i++;
	}
	return (matrix);
}

int get_map(int *i, char *buf, t_all *all)
{
  *i = *i;
  ft_lstadd_b(&all->map, ft_lstn(cor_start_ch(buf)));
  all->game.map = map_to_matrix(all);
  return (SUCCESS);
}

int		close_win(t_all *all)
{
	int	i;
  all->win.win_ptr = 0;
	i = 0;
	// while (i < s->map.y)
	// 	free(s->map.tab[i++]);
	// free(s->map.tab);
	// free(all->tex.no);
	// free(s->tex.s);
	// free(s->tex.e);
	// free(s->tex.w);
	// free(s->tex.i);
	// if (win == 1)
	// 	mlx_destroy_window(s->mlx.ptr, s->win.ptr);
	// free(s->mlx.ptr);
	exit(0);
	return (1);
}

void	move_f(t_all *all, double c)
{
  c = 0.2;
  if(all->game.map[(int)(all->game.gpos_x + all->ray.dir_x)][(int)(all->game.gpos_y)] == '0') all->game.gpos_x += all->ray.dir_x;
  if(all->game.map[(int)(all->game.gpos_x)][(int)(all->game.gpos_y + all->ray.dir_y)] == '0') all->game.gpos_y += all->ray.dir_y;
  screen_ray(all);
}

void	move_b(t_all *all, double c)
{
  if(all->game.map[(int)(all->game.gpos_x - all->ray.dir_x * c)][(int)(all->game.gpos_y)] == '0') all->game.gpos_x -= all->ray.dir_x * c;
  if(all->game.map[(int)(all->game.gpos_x)][(int)(all->game.gpos_y - all->ray.dir_y * c)] == '0') all->game.gpos_y -= all->ray.dir_y * c;
  screen_ray(all);
}

void	ft_rotate(t_all *all, double c)
{
      double oldDirX = all->ray.dir_x;
      all->ray.dir_x = all->ray.dir_x * cos(-c * ROTATE_S) - all->ray.dir_y * sin(-c  * ROTATE_S);
      all->ray.dir_y = oldDirX * sin(-c  * ROTATE_S) + all->ray.dir_y * cos(-c  * ROTATE_S);
      double oldPlaneX = all->ray.plane_x;
      all->ray.plane_x = all->ray.plane_x * cos(-c  * ROTATE_S) - all->ray.plane_y * sin(-c  * ROTATE_S);
      all->ray.plane_y = oldPlaneX * sin(-c  * ROTATE_S) + all->ray.plane_y * cos(-c  * ROTATE_S);
      screen_ray(all);
}


int key_press(int key, t_all *all)
{
  
  if (key == W)
		move_f(all, 1);
  if (key == S)
		move_b(all, 1);
  if (key == D)
		ft_rotate(all, 1);
  if (key == A)
		ft_rotate(all, -1);
    return(1);
}

int run_game(int cr_bmp, char *namefile, t_all *all)
{
    int error;

    error = 0;
    if ((error = file_parse(0, namefile, all, 0)) != SUCCESS)
      return error_str(error);
    all->mlx.mlx_ptr = mlx_init();
    all->ray.dir_x = -1;
    all->ray.dir_y = 0;
    all->ray.plane_x = 0;
    all->ray.plane_y = 0.66;
    all->ray.h = all->win.y;
    all->win.win_ptr = mlx_new_window(all->mlx.mlx_ptr, all->win.x, all->win.y, "Game cube 3D");
    ft_putstr_fd("\ngame start\n", 1);
    screen_ray(all);
    cr_bmp = 0;
    namefile = 0;
    mlx_hook(all->win.win_ptr, 2, 0, key_press, all);
    mlx_hook(all->win.win_ptr, 17, 0, close_win, all);
    // mlx_loop_hook(all->win.win_ptr, screen_ray, all);
    mlx_loop(all->mlx.mlx_ptr);
    
    return(SUCCESS);
}


void init_struct(int cr_bmp, char *namefile)
{
  t_all all;
  ft_bzero(&all, sizeof(t_all));
  ft_bzero(&all.win, sizeof(t_win));
  ft_bzero(&all.img, sizeof(t_img));
  ft_bzero(&all.tex, sizeof(t_tex));
  ft_bzero(&all.game, sizeof(t_game));
  all.tex.floor = 0xFF000000;
  all.tex.ceil = 0xFF000000;
  run_game(cr_bmp, namefile, &all);
}

int main(int argc, char **argv)
{
    if (argc == 3 && !ft_strncmp(argv[1] + ft_strlen(argv[1]) - 4, ".cub", 4) && \
      !ft_strncmp(argv[2], "--save", 6))
      init_struct(1, argv[1]);
    else if (argc == 2 && !ft_strncmp(argv[1] + ft_strlen(argv[1]) - 4, ".cub", 4))
      init_struct(0, argv[1]);
    else
      ft_putstr_fd("\033[31m\nERROR\nMAIN ARGUMENTS\n\n\033[0m", 2);
}