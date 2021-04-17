/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_casting.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: burswyck <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/17 13:22:04 by burswyck          #+#    #+#             */
/*   Updated: 2021/04/17 13:22:07 by burswyck         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/cube3d.h"

void calul_ray(t_all *all)
{
    all->ray.camera_x = 2 * all->ray.sx / (double)all->win.x - 1;
    all->ray.ray_dir_x = all->ray.dir_x + all->ray.plane_x * all->ray.camera_x;
    all->ray.ray_dir_y = all->ray.dir_y + all->ray.plane_y * all->ray.camera_x; 
    all->ray.mapX = (int)all->game.gpos_x;
    all->ray.mapY = (int)all->game.gpos_y;
    all->ray.deltaDistX = fabs(1 / all->ray.ray_dir_x);
    all->ray.deltaDistY = fabs(1 / all->ray.ray_dir_y);
    all->ray.hit = 0;
}

void calc_step(t_all *all)
{
    if(all->ray.ray_dir_x < 0)
        {
            all->ray.stepX = -1;
            all->ray.sideDistX = (all->game.gpos_x - all->ray.mapX) * all->ray.deltaDistX;
        }
        else
        {
            all->ray.stepX = 1;
            all->ray.sideDistX = (all->ray.mapX + 1.0 - all->game.gpos_x) * all->ray.deltaDistX;
        }
        if(all->ray.ray_dir_y < 0)
        {
            all->ray.stepY = -1;
            all->ray.sideDistY = (all->game.gpos_y - all->ray.mapY) * all->ray.deltaDistY;
        }
        else
        {
            all->ray.stepY = 1;
            all->ray.sideDistY = (all->ray.mapY + 1.0 - all->game.gpos_y) * all->ray.deltaDistY;
        }
}

void calc_dda(t_all *all)
{
       while (all->ray.hit == 0)
        {
            if(all->ray.sideDistX < all->ray.sideDistY)
            {
            all->ray.sideDistX += all->ray.deltaDistX;
            all->ray.mapX += all->ray.stepX;
            all->ray.side = 0;
            }
            else
            {
            all->ray.sideDistY += all->ray.deltaDistY;
            all->ray.mapY += all->ray.stepY;
            all->ray.side = 1;
            }
            if(all->game.map[all->ray.mapX][all->ray.mapY] == '1') all->ray.hit = 1;
        }
}

void calc_walls_fc(t_all *all)
{
        if(all->ray.side == 0) all->ray.perpWallDist = (all->ray.mapX - all->game.gpos_x + (1 - all->ray.stepX) / 2) / all->ray.ray_dir_x;
        else all->ray.perpWallDist = (all->ray.mapY - all->game.gpos_y + (1 - all->ray.stepY) / 2) / all->ray.ray_dir_y;
        all->ray.lineHeight = (int)(all->ray.h / all->ray.perpWallDist);
        all->ray.drawStart = -all->ray.lineHeight / 2 + all->ray.h / 2;
        if(all->ray.drawStart < 0)all->ray.drawStart = 0;
        all->ray.drawEnd = all->ray.lineHeight / 2 + all->ray.h / 2;
        if(all->ray.drawEnd >= all->ray.h)all->ray.drawEnd = all->ray.h - 1;
}

int screen_ray(t_all *all)
{
    all->ray.sx = 0;
    all->img.img_ptr = mlx_new_image(all->mlx.mlx_ptr, all->win.x, all->win.y);
    all->img.addr = (int *)mlx_get_data_addr(all->img.img_ptr, &all->img.bits_per_pixel, &all->img.line_length, &all->img.endian);
    while (all->ray.sx < all->win.x)
        {
        calul_ray(all);
        calc_step(all);
        calc_dda(all);
        calc_walls_fc(all);
        // draw_walls(all);
        calc_texture(all);
        all->ray.sx++;
    }
    mlx_put_image_to_window(all->mlx.mlx_ptr, all->win.win_ptr, all->img.img_ptr, 0, 0);
    return (1);
}
