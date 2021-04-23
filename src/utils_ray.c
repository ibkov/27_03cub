/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_ray.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: burswyck <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/17 13:22:47 by burswyck          #+#    #+#             */
/*   Updated: 2021/04/17 13:22:49 by burswyck         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	verLine(int drawStart, int drawEnd, int color, t_all *all)
{
	while (drawStart < drawEnd)
	{
		all->img.addr[drawStart * all->win.x + all->ray.sx] = color;
		drawStart++;
	}
}

void	draw_walls(t_all *all)
{
	int	color;

	if (all->game.map[all->ray.mapX][all->ray.mapY] == '1')
		color = 0x0000FF00;
	if (all->game.map[all->ray.mapX][all->ray.mapY] == '2')
		color = 0x00FFFF00;
	if (all->ray.side == 1)
		color = color / 2;
	if (all->ray.side == 2)
		color = color / 2;
	verLine(all->ray.drawStart, all->ray.drawEnd, color, all);
}

void	draw_fc_wall(t_all *all, int y)
{
	while (y < all->win.y)
	{
		if (y < all->win.y / 2)
			all->img.addr[y++ *all->win.x + all->ray.sx] = all->tex.ceil;
		else
			all->img.addr[y++ *all->win.x + all->ray.sx] = all->tex.floor;
	}
	y = all->ray.drawStart;
	while (y < all->ray.drawEnd)
	{
		all->ray.texY = (int)all->ray.texPos & (64 - 1);
		all->ray.texPos += all->ray.step;
		all->ray.color = all->tex.no[64 * all->ray.texY + all->ray.texX];
		if (all->ray.side == 1 && all->ray.ray_dir_y > 0)
			all->ray.color = all->tex.ea[64 * all->ray.texY + all->ray.texX];
		if (all->ray.side == 1 && all->ray.ray_dir_y < 0)
			all->ray.color = all->tex.we[64 * all->ray.texY + all->ray.texX];
		if (all->ray.side == 0 && all->ray.ray_dir_x > 0)
			all->ray.color = all->tex.so[64 * all->ray.texY + all->ray.texX];
		if (all->ray.side == 0 && all->ray.ray_dir_x < 0)
			all->ray.color = all->tex.no[64 * all->ray.texY + all->ray.texX];
		all->img.addr[y++ *all->win.x + all->ray.sx] = all->ray.color;
	}
}

void	calc_texture(t_all *all)
{
	if (all->ray.side == 0)
		all->ray.wallX = all->game.gpos_y + \
		all->ray.perpWallDist * all->ray.ray_dir_y;
	else
		all->ray.wallX = all->game.gpos_x + \
		all->ray.perpWallDist * all->ray.ray_dir_x;
	all->ray.wallX -= floor((all->ray.wallX));
	all->ray.texX = (int)(all->ray.wallX * (double)(64));
	if (all->ray.side == 0 && all->ray.ray_dir_x > 0)
		all->ray.texX = 64 - all->ray.texX - 1;
	if (all->ray.side == 1 && all->ray.ray_dir_y < 0)
		all->ray.texX = 64 - all->ray.texX - 1;
	all->ray.step = 1.0 * 64 / all->ray.lineHeight;
	all->ray.texPos = (all->ray.drawStart - all->win.y / 2 + \
	all->ray.lineHeight / 2) * all->ray.step;
	draw_fc_wall(all, 0);
}
