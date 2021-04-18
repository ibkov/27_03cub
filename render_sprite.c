/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_sprite.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: burswyck <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/17 13:22:19 by burswyck          #+#    #+#             */
/*   Updated: 2021/04/17 13:22:22 by burswyck         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/cube3d.h"

void draw_sprite(t_all *all)
{
    //after sorting the sprites, do the projection and draw them
    for(int i = 0; i < numSprites; i++)
    {
      //translate sprite position to relative to camera
      double spriteX = sprite[spriteOrder[i]].x - all->game.gpos_x;
      double spriteY = sprite[spriteOrder[i]].y - all->game.gpos_y;

      //transform sprite with the inverse camera matrix
      // [ planeX   dirX ] -1                                       [ dirY      -dirX ]
      // [               ]       =  1/(planeX*dirY-dirX*planeY) *   [                 ]
      // [ planeY   dirY ]                                          [ -planeY  planeX ]

      double invDet = 1.0 / (planeX * dirY - dirX * planeY); //required for correct matrix multiplication

      double transformX = invDet * (dirY * spriteX - dirX * spriteY);
      double transformY = invDet * (-planeY * spriteX + planeX * spriteY); //this is actually the depth inside the screen, that what Z is in 3D, the distance of sprite to player, matching sqrt(spriteDistance[i])

      int spriteScreenX = int((w / 2) * (1 + transformX / transformY));

      //parameters for scaling and moving the sprites
      #define uDiv 1
      #define vDiv 1
      #define vMove 0.0
      int vMoveScreen = (int)(vMove / transformY);

      //calculate height of the sprite on screen
      int spriteHeight = abs((int)(all->win.y / (transformY))) / vDiv; //using "transformY" instead of the real distance prevents fisheye
      //calculate lowest and highest pixel to fill in current stripe
      int drawStartY = -spriteHeight / 2 + all->win.y / 2 + vMoveScreen;
      if(drawStartY < 0) drawStartY = 0;
      int drawEndY = spriteHeight / 2 + all->win.y / 2 + vMoveScreen;
      if(drawEndY >= all->win.y ) drawEndY = all->win.y  - 1;

      //calculate width of the sprite
      int spriteWidth = abs((int)(all->win.y / (transformY))) / uDiv;
      int drawStartX = -spriteWidth / 2 + spriteScreenX;
      if(drawStartX < 0) drawStartX = 0;
      int drawEndX = spriteWidth / 2 + spriteScreenX;
      if(drawEndX >= all->win.x) drawEndX = all->win.x - 1;

      //loop through every vertical stripe of the sprite on screen
      for(int stripe = drawStartX; stripe < drawEndX; stripe++)
      {
        int texX = (int)(256 * (stripe - (-spriteWidth / 2 + spriteScreenX)) * 64 / spriteWidth) / 256;
        //the conditions in the if are:
        //1) it's in front of camera plane so you don't see things behind you
        //2) it's on the screen (left)
        //3) it's on the screen (right)
        //4) ZBuffer, with perpendicular distance
        if(transformY > 0 && stripe > 0 && stripe < all->win.x && transformY < ZBuffer[stripe])
        for(int y = drawStartY; y < drawEndY; y++) //for every pixel of the current stripe
        {
          int d = (y-vMoveScreen) * 256 - all->win.y * 128 + spriteHeight * 128; //256 and 128 factors to avoid floats
          int texY = ((d * 64) / spriteHeight) / 256;
          int color = all->tex.sp[sprite[spriteOrder[i]].texture][64 * texY + texX]; //get current color from the texture
          if((color & 0x00FFFFFF) != 0) buffer[y][stripe] = color; //paint pixel if it isn't black, black is the invisible color
        }
    }
}

// unsigned int	ft_spixel(t_all *s, int index, unsigned int col)
// {
// 	int	t;
// 	int	r;
// 	int	g;
// 	int	b;

// 	if (col >= NONE)
// 		return (s->img.adr[index]);
// 	else if (col < 256 * 256 * 256)
// 		return (col);
// 	t = col / (256 * 256 * 256);
// 	r = (col / (256 * 256) % 256) * (1 - (double)t / 256);
// 	g = (col / 256 % 256) * (1 - (double)t / 256);
// 	b = (col % 256) * (1 - (double)t / 256);
// 	r += (s->img.adr[index] / (256 * 256) % 256) * ((double)t / 256);
// 	g += (s->img.adr[index] / 256 % 256) * ((double)t / 256);
// 	b += (s->img.adr[index] % 256) * ((double)t / 256);
// 	return (r * 256 * 256 + g * 256 + b);
// }

// void			ft_sdraw(t_all *s, int loc, double dist)
// {
// 	unsigned int	col;
// 	double			size;
// 	int				index;
// 	int				i;
// 	int				j;

// 	i = 0;
// 	j = 0;
// 	size = s->win.y / dist / 2;
// 	loc = loc - size / 2;
// 	while (i < size)
// 	{
// 		while ((loc + i >= 0 && loc + i < s->win.x) &&
// 				(j < size && s->stk[loc + i].d > dist))
// 		{
// 			col = 64 * floor(64 * (double)j / size) + (double)i / size * 64;
// 			col = s->tex.i[col];
// 			index = loc + i + (s->win.y / 2 + j) * s->win.x;
// 			if (index < s->win.x * s->win.y)
// 				s->img.adr[index] = ft_spixel(s, index, col);
// 			j++;
// 		}
// 		i++;
// 		j = 0;
// 	}
// }

// void			ft_slocate(t_all *s, double dirx, double diry, double dist)
// {
// 	double	angle;

// 	dirx = (dirx - s->pos.x) / dist;
// 	diry = (diry - s->pos.y) / dist;
// 	if (diry <= 0)
// 		angle = acos(dirx) * 180 / M_PI;
// 	else
// 		angle = 360 - acos(dirx) * 180 / M_PI;
// 	angle = s->dir.a - angle + 33;
// 	if (angle >= 180)
// 		angle -= 360;
// 	else if (angle <= -180)
// 		angle += 360;
// 	ft_sdraw(s, angle * s->win.x / 66, dist);
// }

// void			ft_sorder(t_all *s)
// {
// 	t_spr	tmp;
// 	int		i;
// 	int		j;

// 	i = 0;
// 	while (i < s->map.spr - 1)
// 	{
// 		j = i + 1;
// 		while (j < s->map.spr)
// 		{
// 			if (s->spr[i].d < s->spr[j].d)
// 			{
// 				tmp = s->spr[i];
// 				s->spr[i] = s->spr[j];
// 				s->spr[j] = tmp;
// 			}
// 			j++;
// 		}
// 		i++;
// 	}
// }

// void			ft_sprite(t_all *all)
// {
// 	int		i;
// 	double	dist;

// 	dist = hypot(all->ray.dir_x, all->ray.dir_y);
// 	if (all->ray.dir_y <= 0)
// 		all->ray.arcos = acos(all->ray.dir_x / dist) * 180 / M_PI;
// 	else
// 		all->ray.arcos = 360 - acos(all->ray.dir_x / dist) * 180 / M_PI;
// 	i = 0;
// 	while (i < s->map.spr)
// 	{
// 		s->spr[i].d = hypot(s->spr[i].x - s->pos.x, s->spr[i].y - s->pos.y);
// 		i++;
// 	}
// 	ft_sorder(s);
// 	i = 0;
// 	while (i < s->map.spr)
// 	{
// 		ft_slocate(s, s->spr[i].x, s->spr[i].y, s->spr[i].d);
// 		i++;
// 	}
// 	free(s->stk);
// }
