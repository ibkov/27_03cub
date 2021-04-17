/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_press.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: burswyck <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/17 13:21:35 by burswyck          #+#    #+#             */
/*   Updated: 2021/04/17 13:21:39 by burswyck         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/cube3d.h"

void	move(t_all *all, double c)
{
  if(all->game.map[(int)(all->game.gpos_x - all->ray.dir_x * c * SPEED)][(int)(all->game.gpos_y)] != '1') all->game.gpos_x -= all->ray.dir_x * c  * SPEED;
  if(all->game.map[(int)(all->game.gpos_x)][(int)(all->game.gpos_y - all->ray.dir_y * c  * SPEED)] != '1') all->game.gpos_y -= all->ray.dir_y * c  * SPEED;
  screen_ray(all);
}

void	ft_rotate(t_all *all, double c)
{
      double oldDirX = all->ray.dir_x;
      all->ray.dir_x = all->ray.dir_x * cos(c * ROTATE_S) - all->ray.dir_y * sin(c * ROTATE_S);
      all->ray.dir_y = oldDirX * sin(c * ROTATE_S) + all->ray.dir_y * cos(c * ROTATE_S);
      double oldPlaneX = all->ray.plane_x;
      all->ray.plane_x = all->ray.plane_x * cos(c * ROTATE_S) - all->ray.plane_y * sin(c * ROTATE_S);
      all->ray.plane_y = oldPlaneX * sin(c * ROTATE_S) + all->ray.plane_y * cos(c * ROTATE_S);
      screen_ray(all);
}

void ft_lr_move(t_all *all, double c)
{
  printf("x - %f, y - %f %f %f \n", all->game.gpos_x, all->game.gpos_y, all->ray.dir_x, all->ray.dir_y);
  all->game.gpos_x -= c * (all->ray.dir_y);
	if (all->game.map[(int)floor(all->game.gpos_y)][(int)floor(all->game.gpos_x)] == '1')
		all->game.gpos_x += c * (all->ray.dir_y * SPEED / 100);
	all->game.gpos_y += c * (all->ray.dir_x * SPEED / 100);
	if (all->game.map[(int)floor(all->game.gpos_y)][(int)floor(all->game.gpos_x)] == '1')
		all->game.gpos_y -= c * (all->ray.dir_x * SPEED / 100);
    screen_ray(all);
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


int key_press(int key, t_all *all)
{
  
  if (key == W)
		move(all, -1);
  if (key == S)
		move(all, 1);
  if (key == RIGHT)
		ft_rotate(all, -1);
  if (key == LEFT)
		ft_rotate(all, 1);
  if (key == D)
		ft_lr_move(all, 1);
  if (key == A)
		ft_lr_move(all, 1);
    return(1);
}
