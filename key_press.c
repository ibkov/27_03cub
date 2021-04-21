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
	if (all->game.map[(int)(all->game.gpos_x - all->ray.dir_x * \
		c * SPEED)][(int)(all->game.gpos_y)] != '1')
		all->game.gpos_x -= all->ray.dir_x * c * SPEED;
	if (all->game.map[(int)(all->game.gpos_x)] \
		[(int)(all->game.gpos_y - all->ray.dir_y * c * SPEED)] != '1')
		all->game.gpos_y -= all->ray.dir_y * c * SPEED;
	screen_ray(all);
}

void	ft_rotate(t_all *all, double c)
{
	double	oldDirX;
	double	oldPlaneX;

	oldDirX = all->ray.dir_x;
	all->ray.dir_x = all->ray.dir_x * cos(c * ROTATE_S) - \
	all->ray.dir_y * sin(c * ROTATE_S);
	all->ray.dir_y = oldDirX * sin(c * ROTATE_S) + \
	all->ray.dir_y * cos(c * ROTATE_S);
	oldPlaneX = all->ray.plane_x;
	all->ray.plane_x = all->ray.plane_x * cos(c * ROTATE_S) - \
	all->ray.plane_y * sin(c * ROTATE_S);
	all->ray.plane_y = oldPlaneX * sin(c * ROTATE_S) + \
	all->ray.plane_y * cos(c * ROTATE_S);
	screen_ray(all);
}

void	ft_lr_move(t_all *all, double c)
{
	if (all->game.map[(int)(all->game.gpos_y + all->ray.dir_y * (SPEED * c))]
	[(int)(all->game.gpos_x - all->game.gpos_x * (SPEED * c))] != '1')
	{
		all->game.gpos_y -= -all->ray.dir_x * (0.2 * c);
		all->game.gpos_x -= all->ray.dir_y * (0.2 * c);
	}
	screen_ray(all);
}

int	close_win(t_all *all)
{
	int	i;

	// all->win.win_ptr = 0;
	i = 0;
	while (all->game.map[i])
		free(all->game.map[i++]);
	free(all->game.map);
	free(all->tex.no);
	free(all->tex.we);
	free(all->tex.ea);
	free(all->tex.so);
	free(all->tex.sp);
	mlx_destroy_window(all->mlx.mlx_ptr, all->win.win_ptr);
	free(all->mlx.mlx_ptr);
	exit(0);
}

int	key_press(int key, t_all *all)
{
	if (key == W)
		move(all, -1);
	else if (key == S)
		move(all, 1);
	else if (key == RIGHT)
		ft_rotate(all, -1);
	else if (key == LEFT)
		ft_rotate(all, 1);
	else if (key == D)
		ft_lr_move(all, -1);
	else if (key == A)
		ft_lr_move(all, 1);
	else if (key == ESC)
		close_win(all);
	return (SUCCESS);
}
