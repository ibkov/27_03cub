/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_sprite.c                                      :+:      :+:    :+: */
/*                                                    +:+ +:+         +:+     */
/*   By: burswyck <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/17 13:22:19 by burswyck          #+#    #+#             */
/*   Updated: 2021/04/17 13:22:22 by burswyck         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/cube3d.h"

void	sort_sprites(int *order, double *dist, t_all *all)
{
	int		i;
	int		j;
	double	tmp;
	int		temp_i;

	j = 0;
	while (j < all->game.count_sprites - 1)
	{
		i = 0;
		while (i < all->game.count_sprites - j - 1)
		{
			if (dist[i] < dist[i + 1])
			{
				tmp = dist[i];
				dist[i] = dist[i + 1];
				dist[i + 1] = tmp;
				temp_i = order[i];
				order[i] = order[i + 1];
				order[i + 1] = temp_i;
			}
			i++;
		}
		j++;
	}
}

void	calc_sprite_ray(t_all *all, int *spriteOrder, int i)
{
	all->sray.sX = all->sprite[spriteOrder[i]].x - all->game.gpos_x;
	all->sray.sY = all->sprite[spriteOrder[i]].y - all->game.gpos_y;
	all->sray.invDet = 1.0 / (all->ray.plane_x * all->ray.dir_y - \
	all->ray.dir_x * all->ray.plane_y);
	all->sray.trX = all->sray.invDet * (all->ray.dir_y * all->sray.sX - \
	all->ray.dir_x * all->sray.sY);
	all->sray.trY = all->sray.invDet * (-all->ray.plane_y * all->sray.sX + \
	all->ray.plane_x * all->sray.sY);
	all->sray.sSX = (int)((all->win.x / 2) * \
	(1 + all->sray.trX / all->sray.trY));
	all->sray.vMS = (int)(VMOVE / all->sray.trY);
	all->sray.sH = abs((int)(all->win.y / (all->sray.trY))) / VDIV;
	all->sray.dSY = -all->sray.sH / 2 + all->win.y / 2 + all->sray.vMS;
	if (all->sray.dSY < 0)
		all->sray.dSY = 0;
	all->sray.drawEndY = all->sray.sH / 2 + all->win.y / 2 + all->sray.vMS;
	if (all->sray.drawEndY >= all->win.y)
		all->sray.drawEndY = all->win.y - 1;
	all->sray.sW = abs((int)(all->win.y / (all->sray.trY))) / UDIV;
	all->sray.dSX = -all->sray.sW / 2 + all->sray.sSX;
	if (all->sray.dSX < 0)
		all->sray.dSX = 0;
	all->sray.dEX = all->sray.sW / 2 + all->sray.sSX;
	if (all->sray.dEX >= all->win.x)
		all->sray.dEX = all->win.x - 1;
}

void	put_sprites(t_all *all, int y, int stripe)
{
	stripe = all->sray.dSX;
	while (stripe < all->sray.dEX)
	{
		all->sray.texX = (int)(256 * (stripe - (-all->sray.sW / 2 + all->sray.sSX)) * \
		64 / all->sray.sW) / 256;
		if (all->sray.trY > 0 && stripe > 0 && stripe < all->win.x && \
		all->sray.trY < all->ray.zbuffer[stripe])
		{
			y = all->sray.dSY;
			while (y < all->sray.drawEndY)
			{
				all->sray.d = (y - all->sray.vMS) * 256 - all->win.y * \
				128 + all->sray.sH * 128;
				all->sray.texY = ((all->sray.d * 64) / all->sray.sH) / 256;
				all->sray.color = all->tex.sp[64 * all->sray.texY + \
				all->sray.texX];
				if ((all->sray.color & 0x00FFFFFF) != 0)
					all->img.addr[y * all->win.x + stripe] = all->sray.color;
				y++;
			}
		}
		stripe++;
	}
}

void	draw_sprite(t_all *all, int i)
{
	int		*spriteOrder;
	double	*spriteDistance;

	spriteOrder = malloc(sizeof(int) * all->game.count_sprites + 1);
	spriteDistance = malloc(sizeof(double) * all->game.count_sprites + 1);
	while (i < all->game.count_sprites)
	{
		spriteOrder[i] = i;
		spriteDistance[i] = ((all->game.gpos_x - all->sprite[i].x) * \
		(all->game.gpos_x - all->sprite[i].x) + (all->game.gpos_y - all->sprite[i].y) * \
		(all->game.gpos_y - all->sprite[i].y));
		i++;
	}
	sort_sprites(spriteOrder, spriteDistance, all);
	i = 0;
	while (i < all->game.count_sprites)
	{
		calc_sprite_ray(all, spriteOrder, i);
		put_sprites(all, 0, 0);
		i++;
	}
	free(spriteOrder);
	free(spriteDistance);
}
