#include "./includes/cube3d.h"

int error_str(int error)
{
  if (error == ERROR_READING_FILE)
    ft_putstr_fd("\033[31m\nERROR\nNo such file in directory\n\n\033[0m", 2);
  else if (error == ERROR_MAP)
    ft_putstr_fd("\033[33mCorrect you map.\n\n\033[0m", 2);
  else if (error == ERROR_MAP_MANY_POSITIONS)
    ft_putstr_fd("\033[31m\nERROR\nYour map has no one gamer position\n\n\033[0m", 2);
  else if (error == ERROR_MAP_NO_POSITIONS)
    ft_putstr_fd("\033[31m\nERROR\nYour map has no gamer position\n\n\033[0m", 2);
  else if (error == ERROR_MAP_NOT_CLOSED)
    ft_putstr_fd("\033[31m\nERROR\nYour map not closed\n\n\033[0m", 2);
  else if (error == ERROR_RESOLUTION)
    ft_putstr_fd("\033[31m\nERROR\nResolution is not correct\n\n\033[0m", 2);
  else if (error == ERROR_END_lINE)
    ft_putstr_fd("\033[31m\nERROR\nInvalid chars in arguments\n\n\033[0m", 2);
  else if (error == ERROR)
    ft_putstr_fd("\033[31m\nERROR\nInvalid arguments\n\n\033[0m", 2);
  else if (error == ERROR_TEXTUTE_FILE)
    ft_putstr_fd("\033[31m\nERROR\nFile texture not found\n\033[0m", 2);
   else if (error == ERROR_TEXTUTE_EX)
    ft_putstr_fd("\033[31m\nERROR\nInvalid extention texture file\n\n\033[0m", 2);
  return (ERROR);
}

int		ft_atoi_cub(int *i, char *buf, int num) 
{
	while (ft_strchr(INVCHARS, buf[(*i)]))
    (*i)++;
	while (buf[*i] >= '0' && buf[*i] <= '9')
	{
		num = num * 10 + (buf[*i] - 48);
		(*i)++;
	}
	return (num);
}

int get_resolution(int *i, char *buf, t_all *all)
{
  if (all->win.x || all->win.y)
    return (ERROR_RESOLUTION);
  (*i)++;
  all->win.x = ft_atoi_cub(i, buf, 0);
  all->win.y = ft_atoi_cub(i, buf, 0);
  mlx_get_screen_size(&all->win.max_width, &all->win.max_height);
  if (all->win.x > all->win.max_width)
    all->win.x = all->win.max_width;
  if (all->win.y > all->win.max_height)
    all->win.y = all->win.max_height;
  while (!ft_strchr(INVCHARS, buf[(*i)]))
    (*i)++;
  if (all->win.x < 1 || all->win.y < 1 || buf[*i] != '\0')
    return (ERROR_END_lINE);
  return (SUCCESS);
}

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

t_map	*ft_lstn(char *content)
{
	t_map	*new_elem;

	if (!(new_elem = malloc(sizeof(t_map))))
		return (0);
	new_elem->line = content;
	new_elem->next = 0;
	return (new_elem);
}

t_map	*ft_lstl(t_map *lst)
{
	t_map	*last_lst;

	last_lst = lst;
	if (!lst)
		return (0);
	while (lst->next)
	{
		last_lst = lst;
		lst = lst->next;
	}
	if (!lst->next)
		return (lst);
	return (last_lst);
}

void	ft_lstadd_b(t_map **lst, t_map *new)
{
	if (!*lst)
	{
		*lst = new;
		return ;
	}
	if (!new)
		return ;
	ft_lstl(*lst)->next = new;
	new->next = 0;
}

int		ft_lst_sz(t_map *lst)
{
	int		count;
	t_map	*temp;

	temp = lst;
	count = 0;
	if (!temp)
		return (0);
	while (temp->next)
	{
		count++;
		temp = temp->next;
	}
	if (temp->next == NULL)
		count++;
	return (count);
}

char		**map_to_matrix(t_map *lst)
{
	t_map	*temp;
	int		i;
	int		j;
	char	**matrix;

	temp = lst;
	if (!(matrix = malloc(sizeof(char*) * (ft_lst_sz(lst) + 1))))
		return (NULL);
	i = 0;
	while (i < ft_lst_sz(lst))
	{
		if (!(matrix[i] = malloc(sizeof(char) * (ft_strlen(temp->line) + 1))))
			return (NULL);
		j = 0;
		while (temp->line[j] != '\0')
		{
			matrix[i][j] = temp->line[j];
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
  static int a;
  ft_lstadd_b(&all->map, ft_lstn(cor_start_ch(buf)));
  all->game.map = map_to_matrix(all->map);
  ft_putstr_fd(all->game.map[a], 1);
  ft_putstr_fd("\n", 1);
  a++;
  return (SUCCESS);
}

int fetch_line_file(int i, char *buf, t_all *all)
{
  int error;

  error = 0;
  while (ft_strchr(INVCHARS, buf[i]))
    i++;
  if (buf[i] == '1' && buf[i] != '\0')
    error = get_map(&i, buf, all);
  if (buf[i] == 'R' && buf[i + 1] == ' ')
    error = get_resolution(&i ,buf , all);
  if (buf[i] == 'N' && buf[i + 1] == 'O' && buf[i + 2] == ' ')
    error = get_texture(&i ,buf, all, &all->tex.no);
  if (buf[i] == 'S' && buf[i + 1] == 'O' && buf[i + 2] == ' ')
    error = get_texture(&i ,buf, all, &all->tex.so);
  if (buf[i] == 'E' && buf[i + 1] == 'A' && buf[i + 2] == ' ')
    error = get_texture(&i ,buf, all, &all->tex.ea);
  if (buf[i] == 'W' && buf[i + 1] == 'E' && buf[i + 2] == ' ')
    error = get_texture(&i ,buf, all, &all->tex.we);
  if (buf[i] == 'S' && buf[i + 1] == ' ')
    error = get_texture(&i ,buf, all, &all->tex.sp);
  if (buf[i] == 'F' && buf[i + 1] == ' ')
    error = get_color(&i ,buf, &all->tex.floor);
  if (buf[i] == 'C' && buf[i + 1] == ' ')
    error = get_color(&i ,buf, &all->tex.ceil);
  if (all->win.x && all->win.y && all->tex.no && all->tex.we && all->tex.so && all->tex.ea)
    error = 1;
  
  return ((error == 1 ? SUCCESS : error));
}

int file_parse(int fd, char *namefile, t_all *all, int error)
{
  char *buffer;
  
  if ((fd = open(namefile, O_RDONLY)) == -1)
    return (ERROR_READING_FILE);
  while ((get_next_line(fd, &buffer)) > ERROR)
  {
    if ((error = fetch_line_file(0, buffer, all)) != SUCCESS)
      return (error);
    free(buffer);
  }
  if ((error = fetch_line_file(0, buffer, all)) != SUCCESS)
      return (error);
  free(buffer);
  close(fd);
  return (SUCCESS);
}


int run_game(int cr_bmp, char *namefile, t_all *all)
{
    int error;

    error = 0;
    if ((error = file_parse(0, namefile, all, 0)) != SUCCESS)
      return error_str(error);
    all->mlx.mlx_ptr = mlx_init();
    all->game.gpos_x = 2;
    all->game.gpos_y = 2;
    all->win.win_ptr = mlx_new_window(all->mlx.mlx_ptr, all->win.x, all->win.y, "Game cube 3D");
    ft_putstr_fd("\ngame start\n", 1);
    init_ray(all);
    cr_bmp = 0;
    namefile = 0;
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