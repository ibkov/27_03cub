#include "./includes/cube3d.h"

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