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
    if (cr_bmp == 1)
      return (screenshot(all));
    screen_ray(all);
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
