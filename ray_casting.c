#include "./includes/cube3d.h"


void verLine(int drawStart, int drawEnd, int color, t_all *all)
{
    while (drawStart < drawEnd)
    {
        all->img.addr[drawStart * all->win.x + all->ray.sx] = color;
        drawStart++;
    }
}

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

void init_ray(t_all *all)
{
    all->ray.sx = 0;
    for (int y = 0; y < all->win.y; y++)
        for (int x = 0; x < all->win.x; x++) 
            if (y < all->win.y / 2)all->tex.buf[y][x] = all->tex.ceil;
            else all->tex.buf[y][x] = all->tex.floor;
    all->img.img_ptr = mlx_new_image(all->mlx.mlx_ptr, all->win.x, all->win.y);
    all->img.addr = (int *)mlx_get_data_addr(all->img.img_ptr, &all->img.bits_per_pixel, &all->img.line_length, &all->img.endian);
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
        // verLine(0, (int)all->ray.h / 2, all->tex.floor, all);
        // verLine((int)all->ray.h / 2, all->ray.h - 1, all->tex.ceil, all);
        all->ray.lineHeight = (int)(all->ray.h / all->ray.perpWallDist);
        all->ray.drawStart = -all->ray.lineHeight / 2 + all->ray.h / 2;
        if(all->ray.drawStart < 0)all->ray.drawStart = 0;
        all->ray.drawEnd = all->ray.lineHeight / 2 + all->ray.h / 2;
        if(all->ray.drawEnd >= all->ray.h)all->ray.drawEnd = all->ray.h - 1;
}

void imageDraw(t_all *all)
{
    for (int y = 0; y < all->win.y; y++)
        for (int x = 0; x < all->win.x; x++)
            all->img.addr[y * all->win.x + x] = all->tex.buf[y][x];
}

void calc_texture(t_all *all)
{
    //texturing calculations
    //   int texNum = all->game.map[all->game.map_x][all->game.map_y] - 1; //1 subtracted from it so that texture 0 can be used!

      //calculate value of wallX
      double wallX; //where exactly the wall was hit
      if (all->ray.side == 0) 
        wallX = all->game.gpos_y + all->ray.perpWallDist * all->ray.ray_dir_y;
      else           
        wallX = all->game.gpos_x + all->ray.perpWallDist * all->ray.ray_dir_x;
      wallX -= floor((wallX));

    //   x coordinate on the texture
      int texX = (int)(wallX * (double)(64));
      if(all->ray.side == 0 && all->ray.ray_dir_x > 0) texX = 64 - texX - 1;
      if(all->ray.side == 1 && all->ray.ray_dir_y < 0) texX = 64 - texX - 1;

      double step = 1.0 * 64 / all->ray.lineHeight;
      // Starting texture coordinate
      double texPos = (all->ray.drawStart - all->win.y / 2 + all->ray.lineHeight / 2) * step;
      for(int y = all->ray.drawStart; y < all->ray.drawEnd; y++)
      {
        // Cast the texture coordinate to integer, and mask with (texHeight - 1) in case of overflow
        int texY = (int)texPos & (64 - 1);
        texPos += step;
        int color = all->tex.we[64 * texY + texX];
        //make color darker for y-sides: R, G and B byte each divided through two with a "shift" and an "and"
        if(all->ray.side == 1) color = (color >> 1) & 8355711;
        all->tex.buf[y][all->ray.sx] = color;
      }
}

void draw_walls(t_all *all)
{
    int  color;
    if (all->game.map[all->ray.mapX][all->ray.mapY] == '1')
            color = 0x0000FF00;
        if (all->game.map[all->ray.mapX][all->ray.mapY] == '2')
            color = 0x00FFFF00;

        //give x and y sides different brightness
        if(all->ray.side == 1) {color = color / 2;}
        if(all->ray.side == 2) {color = color / 2;}
        //draw the pixels of the stripe as a vertical line
        verLine(all->ray.drawStart, all->ray.drawEnd, color, all);
}

int screen_ray(t_all *all)
{
    init_ray(all);
    while (all->ray.sx < all->win.x)
        {
        calul_ray(all);
        calc_step(all);
        calc_dda(all);
        calc_walls_fc(all);
        // draw_walls(all);
        calc_texture(all);
        imageDraw(all);
        all->ray.sx++;
    }
    mlx_put_image_to_window(all->mlx.mlx_ptr, all->win.win_ptr, all->img.img_ptr, 0, 0);
    return (1);
}