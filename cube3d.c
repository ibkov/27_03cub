/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cube3d.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: burswyck <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/17 13:20:12 by burswyck          #+#    #+#             */
/*   Updated: 2021/04/17 13:20:25 by burswyck         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/cube3d.h"

int get_color(int *i, char *buf, unsigned int *color)
{
  t_color rgb;
  ft_bzero(&rgb, sizeof(t_color));
  if (*color != 0xFF000000)
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
  *color = rgb.r * 256 * 256 + rgb.g * 256 + rgb.b;
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

char		**map_to_matrix(t_all *lst, int i, int j)
{
	t_map	*temp;
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
			j++;
		}
		matrix[i][j] = '\0';
		temp = temp->next;
		i++;
	}
  matrix[i] = NULL;
  while (lst->map)
  {
    free(lst->map->line);
    lst->map = lst->map->next;
  }
  free(temp);
	return (matrix);
}

void correct_position(t_all *all, int i, int j, char *ch)
{
  *ch = all->game.map[i][j];
  all->game.gpos_x = (double)i + 0.5; 
  all->game.gpos_y = (double)j + 0.5;
  all->ray.dir_y = (*ch == 'E' || *ch == 'W') ? 1 : 0;
  all->ray.dir_y *= (*ch == 'W') ? -1 : 1;
  all->ray.dir_x = (*ch == 'S' || *ch == 'N') ? 1 : 0;
  all->ray.dir_x *= (*ch == 'N') ? -1 : 1;
  all->ray.plane_y = (*ch == 'S' || *ch == 'N') ? 0.66 : 0;
  all->ray.plane_y *= (*ch == 'S') ? -1 : 1;
  all->ray.plane_x = (*ch == 'W' || *ch == 'E') ? 0.66 : 0;
  all->ray.plane_x *= (*ch == 'W') ? -1 : 1;
}

void add_sprites(t_all *all, int i, int j, int count)
{
  while (all->game.map[i])
  {
    j = 0;
    while (all->game.map[i][j] != '\0')
    { 
      if (all->game.map[i][j] == '2')
      {
        all->sprite[count].x = (double)i + 0.5;
        all->sprite[count].y = (double)j + 0.5;
        count++;
      }
      j++;
    }
    i++;
  }
}

int check_map(t_all *all, int i, int j, char ch)
{
  while (all->game.map[i])
  {
    j = 0;
    while (all->game.map[i][j] != '\0')
    { 
      if (!ft_strchr("NSWE012", all->game.map[i][j]))
        return (ERROR_MAP_CHAR);
      if (ft_strchr("NSWE", all->game.map[i][j]) && !ch)
        correct_position(all, i, j, &ch);
      else if (ft_strchr("NSWE", all->game.map[i][j]) && ch)
        return(ERROR_PLAYER_POSITION);
      if (all->game.map[i][j] == '2')
        all->game.count_sprites++;
      j++;
    }
    i++;
  }
  if (!ch)
    return(ERROR_NO_PLAYER_POSITION);
  return (SUCCESS);
}


int get_map(int *i, char *buf, t_all *all)
{
  *i = *i;
  ft_lstadd_b(&all->map, ft_lstn(cor_start_ch(buf)));
  return (SUCCESS);
}

int run_game(int cr_bmp, char *namefile, t_all *all)
{
    int error;

    error = 0;
    if ((error = file_parse(0, namefile, all, 0)) != SUCCESS)
      return error_str(error);
    all->game.map = map_to_matrix(all, 0, 0);
    if ((error = check_map(all, 0, 0, 0)) != SUCCESS)
      return error_str(error);
    all->sprite = malloc(sizeof(t_sprite) * all->game.count_sprites);
    add_sprites(all, 0, 0, 0);
    all->ray.zbuffer = malloc(sizeof(double) * all->win.x);
    all->mlx.mlx_ptr = mlx_init();
    all->ray.h = all->win.y;
    all->win.win_ptr = mlx_new_window(all->mlx.mlx_ptr, all->win.x, all->win.y, "Game cube 3D");
    if (cr_bmp == 1)
      return (screenshot(all));
    screen_ray(all);
    mlx_hook(all->win.win_ptr, 2, 0, key_press, all);
    mlx_hook(all->win.win_ptr, 17, 0, close_win, all);
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
  all.tex.floor = NONE;
  all.tex.ceil = NONE;
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
