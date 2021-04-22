/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_cub.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: burswyck <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/17 13:22:19 by burswyck          #+#    #+#             */
/*   Updated: 2021/04/17 13:22:22 by burswyck         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	add_sprites(t_all *all, int i, int j, int count)
{
	while (all->game.map[i])
	{
		j = 0;
		while (all->game.map[i][j] != '\0')
		{
			if (all->game.map[i][j] == '2')
			{
				all->sprite[count].x = (double)i + 0.5;
				all->sprite[count].y = (double)j + 0.5;
				count++;
			}
			j++;
		}
		i++;
	}
}

void	full_new_line(char *buffer, char *new_str)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (buffer[i])
	{
		if (buffer[i] != ' ')
		{
			new_str[j] = buffer[i];
			j++;
		}
		i++;
	}
	new_str[j] = '\0';
}

char	*cor_start_ch(char *buffer)
{
	int		i;
	int		cs;
	char	*new_str;

	i = 0;
	cs = 0;
	while (buffer[i])
	{
		if (buffer[i] == ' ')
			cs++;
		i++;
	}
	new_str = (char *)malloc(sizeof(char) * (ft_strlen(buffer) - cs + 1));
	if (!new_str)
		return (NULL);
	full_new_line(buffer, new_str);
	return (new_str);
}

void	correct_position(t_all *all, int i, int j, char *ch)
{
	*ch = all->game.map[i][j];
	all->game.gpos_x = (double)i + 0.5;
	all->game.gpos_y = (double)j + 0.5;
	if (*ch == 'E' || *ch == 'W')
		all->ray.dir_y = 1;
	if (*ch == 'W')
		all->ray.dir_y *= -1;
	if (*ch == 'S' || *ch == 'N')
		all->ray.dir_x = 1;
	if (*ch == 'N')
		all->ray.dir_x *= -1;
	if (*ch == 'S' || *ch == 'N')
		all->ray.plane_y = 0.66;
	if (*ch == 'S')
		all->ray.plane_y *= -1;
	if (*ch == 'W' || *ch == 'E')
		all->ray.plane_x = 0.66;
	if (*ch == 'W')
		all->ray.plane_x *= -1;
}
