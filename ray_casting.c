#include "./includes/cube3d.h"

void verLine(int x, int drawStart, int drawEnd, int color, t_all *all)
{
    while (drawStart < drawEnd)
    {
        all->img.addr[drawStart * all->win.y + x] = color;
        drawStart++;
    }
}

void init_ray(t_all *all)
{
    all->ray.dir_x = -1;
    all->ray.dir_y = 0;
    all->ray.plane_x = 0;
    all->ray.plane_y = 0.66;
    all->img.img_ptr = mlx_new_image(all->mlx.mlx_ptr, all->win.x, all->win.y);
    all->img.addr = (int *)mlx_get_data_addr(all->img.img_ptr, &all->img.bits_per_pixel, &all->img.line_length, &all->img.endian);
    int x = 0;
    while (x < all->win.x)
        {
        //calculate ray position and direction
        all->ray.camera_x = 2 * x / (double)all->win.x - 1; //x-coordinate in camera space
        all->ray.ray_dir_x = all->ray.dir_x + all->ray.plane_x * all->ray.camera_x;
        all->ray.ray_dir_y = all->ray.dir_y + all->ray.plane_y * all->ray.camera_x;
        //which box of the map we're in
        int mapX = (int)all->game.gpos_x;
        int mapY = (int)all->game.gpos_y;

        //length of ray from current position to next x or y-side
        double sideDistX;
        double sideDistY;

        //length of ray from one x or y-side to next x or y-side
        double deltaDistX = fabs(1 / all->ray.ray_dir_x);
        double deltaDistY = fabs(1 / all->ray.ray_dir_y);
        double perpWallDist;

        //what direction to step in x or y-direction (either +1 or -1)
        int stepX;
        int stepY;

        int hit = 0; //was there a wall hit?
        int side; //was a NS or a EW wall hit?
        //calculate step and initial sideDist
        if(all->ray.ray_dir_x < 0)
        {
            stepX = -1;
            sideDistX = (all->game.gpos_x - mapX) * deltaDistX;
        }
        else
        {
            stepX = 1;
            sideDistX = (mapX + 1.0 - all->game.gpos_x) * deltaDistX;
        }
        if(all->ray.ray_dir_y < 0)
        {
            stepY = -1;
            sideDistY = (all->game.gpos_y - mapY) * deltaDistY;
        }
        else
        {
            stepY = 1;
            sideDistY = (mapY + 1.0 - all->game.gpos_y) * deltaDistY;
        }
        //perform DDA
        while (hit == 0)
        {
            //jump to next map square, OR in x-direction, OR in y-direction
            if(sideDistX < sideDistY)
            {
            sideDistX += deltaDistX;
            mapX += stepX;
            side = 0;
            }
            else
            {
            sideDistY += deltaDistY;
            mapY += stepY;
            side = 1;
            }
            //Check if ray has hit a wall
            if(all->game.map[mapX][mapY] == '1') hit = 1;
        }
        //Calculate distance projected on camera direction (Euclidean distance will give fisheye effect!)
        if(side == 0) perpWallDist = (mapX - all->game.gpos_x + (1 - stepX) / 2) / all->ray.dir_x;
        else          perpWallDist = (mapY - all->game.gpos_y + (1 - stepY) / 2) / all->ray.dir_y;

        int h = 600;
        //Calculate height of line to draw on screen
        int lineHeight = (int)(h / perpWallDist);

        //calculate lowest and highest pixel to fill in current stripe
        int drawStart = -lineHeight / 2 + h / 2;
        if(drawStart < 0)drawStart = 0;
        int drawEnd = lineHeight / 2 + h / 2;
        if(drawEnd >= all->win.y)drawEnd = h - 1;

        int color;
        //choose wall color
        if (all->game.map[mapX][mapY] == '1')
            color = 0x0000FF00;

        //give x and y sides different brightness
        if(side == 1) {color = color / 2;}

        //draw the pixels of the stripe as a vertical line
        verLine(x, drawStart, drawEnd, color, all);
        x++;
    }
    mlx_put_image_to_window(all->mlx.mlx_ptr, all->win.win_ptr, all->img.img_ptr, 0, 0);
}