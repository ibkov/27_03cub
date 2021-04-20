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

void sortSprites(int* order, double* dist, t_all *all)
{
	int				i;
	int				j;
	double			tmp;
	int				temp_i;

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

void draw_sprite(t_all *all)
{
	int spriteOrder[all->game.count_sprites];
	double spriteDistance[all->game.count_sprites];
	for(int i = 0; i < all->game.count_sprites; i++)
    {
      spriteOrder[i] = i;
      spriteDistance[i] = ((all->game.gpos_x - all->sprite[i].x) * (all->game.gpos_x - all->sprite[i].x) + (all->game.gpos_y - all->sprite[i].y) * (all->game.gpos_y - all->sprite[i].y)); //sqrt not taken, unneeded
    }
    sortSprites(spriteOrder, spriteDistance, all);
    for(int i = 0; i < all->game.count_sprites; i++)
    {
      double spriteX = all->sprite[spriteOrder[i]].x - all->game.gpos_x;
      double spriteY = all->sprite[spriteOrder[i]].y - all->game.gpos_y;
      double invDet = 1.0 / (all->ray.plane_x * all->ray.dir_y - all->ray.dir_x * all->ray.plane_y); //required for correct matrix multiplication

      double transformX = invDet * (all->ray.dir_y * spriteX - all->ray.dir_x * spriteY);
      double transformY = invDet * (-all->ray.plane_y * spriteX + all->ray.plane_x * spriteY); //this is actually the depth inside the screen, that what Z is in 3D, the distance of sprite to player, matching sqrt(spriteDistance[i])

      int spriteScreenX = (int)((all->win.x / 2) * (1 + transformX / transformY));
      #define uDiv 1
      #define vDiv 1
      #define vMove 0.0
      int vMoveScreen = (int)(vMove / transformY);
      int spriteHeight = abs((int)(all->win.y / (transformY))) / vDiv;
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
        if(transformY > 0 && stripe > 0 && stripe < all->win.x && transformY < all->ray.zbuffer[stripe])
        for(int y = drawStartY; y < drawEndY; y++) //for every pixel of the current stripe
			{
			int d = (y-vMoveScreen) * 256 - all->win.y * 128 + spriteHeight * 128; //256 and 128 factors to avoid floats
			int texY = ((d * 64) / spriteHeight) / 256;
			int color = all->tex.sp[64 * texY + texX]; //get current color from the texture
			if((color & 0x00FFFFFF) != 0) all->img.addr[y * all->win.x + stripe] =  color; //paint pixel if it isn't black, black is the invisible color
			}
    	}	
	}
}