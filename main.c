#include "cube3d.h"

void            my_mlx_pixel_put(t_img *img, int x, int y, int color)
{
    char    *dst;

    dst = img->addr + (y * img->line_length + x * (img->bits_per_pixel / 8));
    *(unsigned int*)dst = color;
}

void draw_line(t_img *img, double x1, double y1, double x2, double y2)
{
    int delta_x;
    int delta_y;
    int lenght;

    lenght = 0;
    delta_x = fabs(round(x1) - round(x2)); 
    delta_y = fabs(round(y1) - round(y2));
    
    lenght = fmax(delta_x, delta_y);

    if (lenght == 0)
    {
        my_mlx_pixel_put(img, x1, y1, 0x00FFFFFF);
        return ;
    }

    double dx = (x2 - x1) / lenght;
    double dy = (y2 - y1) / lenght;
    double x;
    double y;
    
    x = round(x1);
    y = round(y1);

    while (lenght--)
    {
        x += dx;
        y += dy;
        my_mlx_pixel_put(img, round(y), round(x), 0x00FFFFFF);
    }
}

void rectangle(t_img *img, int x, int y, int w, int h)
{
    int i = x;
    while(i < x + w)
    {
        my_mlx_pixel_put(img, round(i), round(y),  0x00FFFFFF);
        my_mlx_pixel_put(img, round(i++), round(y + h),  0x00FFFFFF);
    }
    i = y;
    while(i <= y + h)
    {
        my_mlx_pixel_put(img, round(x), round(i),  0x00FFFFFF);
        my_mlx_pixel_put(img, round(x + w), round(i++),  0x00FFFFFF);
    }
}

void drawCircle(t_img *img, int x0, int y0, int radius) {
	int x = 0;
	int y = radius;
	int delta = 1 - 2 * radius;
	int error = 0;
	while(y >= 0) {
		my_mlx_pixel_put(img, x0 + x, y0 + y, 0x00FFFF00);
		my_mlx_pixel_put(img, x0 + x, y0 - y, 0x00FFFF00);
		my_mlx_pixel_put(img, x0 - x, y0 + y, 0x00FFFF00);
		my_mlx_pixel_put(img, x0 - x, y0 - y, 0x00FFFF00);
		error = 2 * (delta + y) - 1;
		if(delta < 0 && error <= 0) {
			++x;
			delta += 2 * x + 1;
			continue;
		}
		error = 2 * (delta - x) - 1;
		if(delta > 0 && error > 0) {
			--y;
			delta += 1 - 2 * y;
			continue;
		}
		++x;
		delta += 2 * (x - y);
		--y;
	}
}

int ft_redraw(t_all *all)
{

    // mlx_destroy_image(all->win.mlx, all->img.img);
    all->img.img = mlx_new_image(all->win.mlx, all->win.screen_w, all->win.screen_h);
    all->img.addr = mlx_get_data_addr(&all->img.img, &all->img.bits_per_pixel, &all->img.line_length,
                                 &all->img.endian);
    int i;
    int j;
    i = 0;

    // while (i < 6)
    // {
    //     j = 0;
    //     while (j < 6)
    //     {
    //         rectangle(all->img.img, j * ((int)(all->win.screen_w / 6) - 1), i * ((int)(all->win.screen_h / 6) - 1), ((int)(all->win.screen_w / 6) - 1), ((int)(all->win.screen_h / 6) - 1));
    //         j++;
    //     }
    //     i++;
    // }
    drawCircle(all->img.img, all->game.pos_x, 200, 10);
    mlx_put_image_to_window(all->win.mlx, all->win.mlx_win, all->img.img, 0, 0);
    return (0);
}


int             main(void)
{
    t_all  all;
    all.win.screen_w = 600;
    all.win.screen_h = 600;
    all.game.pos_x = 300;
    all.game.pos_y = 300;

    int worldMap[6][6]=
{
  {1,1,1,1,1,1},
  {1,0,0,0,0,1},
  {1,0,0,0,0,1},
  {1,0,0,0,0,1},
  {1,0,0,0,0,1},
  {1,1,1,1,1,1},
};
    // all.game.map = &worldMap;
    worldMap[0][0] = 1;
    all.win.mlx = mlx_init();
    all.win.mlx_win = mlx_new_window(all.win.mlx, all.win.screen_w, all.win.screen_h, "Hello world!");
    all.img.img = mlx_new_image(all.win.mlx, all.win.screen_w, all.win.screen_h);
    all.img.addr = mlx_get_data_addr(all.img.img, &all.img.bits_per_pixel, &all.img.line_length,
                                 &all.img.endian);
    // my_mlx_pixel_put(&img, 5, 5, 0x00FFFFFF);
    // draw_line(&img, 20, 30, 50, 600);
    int i = 0;
    int j = 0;
    // while (i < 6)
    // {
    //     j = 0;
    //     while (j < 6)
    //     {
    //         if (worldMap[i][j] == 1)
    //             rectangle(&all.img, j * ((int)(all.win.screen_w / 6) - 1), i * ((int)(all.win.screen_h / 6) - 1), ((int)(all.win.screen_w / 6) - 1), ((int)(all.win.screen_h / 6) - 1));
    //         j++;
    //     }
    //     i++;
    // }
    drawCircle(&all.img, all.game.pos_x, all.game.pos_y, 10);
    mlx_put_image_to_window(all.win.mlx, all.win.mlx_win, all.img.img, 0, 0);
    mlx_loop_hook(all.win.mlx, ft_redraw, &all);
    mlx_loop(all.win.mlx);
    return (0);
}