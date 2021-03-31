# include "includes/cube3d.h"

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

int check_full_tex(t_all *all, char *filename_tex)
{
  void *img;
  int array[5];

  if (!ft_strncmp(filename_tex + ft_strlen(filename_tex) - 4, ".xmp", 4))
    return (ERROR);
  if ((all->tex.fd = open(filename_tex, O_RDONLY)) == -1)
    return (ERROR);
  close(all->tex.fd);
  img = mlx_xpm_file_to_image(&all->win.mlx_ptr, filename_tex, &array[0], &array[1]);
  if (array[0] != 64 || array[1] != 64 || !img)
    return (ERROR);
  all->img.addr = (unsigned int *)mlx_get_data_addr(img, &array[2], &array[3], &array[4]);
  free(img);
  return(SUCCESS);

}

int get_texture(int *i, char *buf, t_all *all)
{
  int rem;
  char *filename_tex; 

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
  check_full_tex(all, filename_tex);
  free(filename_tex);
  return (SUCCESS);
}

int fetch_line_file(int i, char *buf, t_all *all)
{
  int error;

  error = 0;
  while (ft_strchr(INVCHARS, buf[i]))
    i++;
  if (buf[i] == 'R' && buf[i + 1] == ' ')
    error = get_resolution(&i ,buf , all);
  if (buf[i] == 'N' && buf[i + 1] == 'O' && buf[i + 2] == ' ')
    error = get_texture(&i ,buf , all);
  if (all->win.x && all->win.y)
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
  close(fd);
  return (SUCCESS);
}

int run_game(int cr_bmp, char *namefile, t_all *all)
{
    int error;

    error = 0;
    if ((error = file_parse(0, namefile, all, 0)) != SUCCESS)
      return error_str(error);
    all->win.mlx_ptr = mlx_init();
    ft_putstr_fd("\ngame start\n", 1);
    cr_bmp = 0;
    namefile = 0;
    
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