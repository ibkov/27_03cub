/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_file_args.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: burswyck <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/17 13:21:14 by burswyck          #+#    #+#             */
/*   Updated: 2021/04/17 13:21:18 by burswyck         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	ft_atoi_cub(int *i, char *buf, int num)
{
	while (ft_strchr(INVCHARS, buf[(*i)]))
		(*i)++;
	while (buf[*i] >= '0' && buf[*i] <= '9')
	{
		num = num * 10 + (buf[*i] - 48);
		(*i)++;
	}
	return (num);
}

int	get_resolution(int *i, char *buf, t_all *all)
{
	if (all->win.x || all->win.y)
		return (ERROR_RESOLUTION);
	(*i)++;
	all->win.x = ft_atoi_cub(i, buf, 0);
	all->win.y = ft_atoi_cub(i, buf, 0);
	mlx_get_screen_size(&all->win.max_width, &all->win.max_height);
	if (all->win.x > all->win.max_width)
		all->win.x = all->win.max_width;
	if (all->win.y > all->win.max_height)
		all->win.y = all->win.max_height;
	while (ft_strchr(INVCHARS, buf[(*i)]))
		(*i)++;
	if (all->win.x < 1 || all->win.y < 1 || buf[*i - 1] != '\0')
		return (ERROR_END_lINE);
	return (SUCCESS);
}

int	get_color(int *i, char *buf, unsigned int *color)
{
	t_color	rgb;

	ft_bzero(&rgb, sizeof(t_color));
	if (*color != NONE)
		return (ERROR);
	(*i)++;
	rgb.r = ft_atoi_cub(i, buf, 0);
	(*i)++;
	rgb.g = ft_atoi_cub(i, buf, 0);
	(*i)++;
	rgb.b = ft_atoi_cub(i, buf, 0);
	while (!ft_strchr(INVCHARS, buf[(*i)]))
		(*i)++;
	if (buf[*i] || rgb.r > 255 || rgb.b > 255 || rgb.r > 255)
		return (ERROR);
	*color = rgb.r * 256 * 256 + rgb.g * 256 + rgb.b;
	return (SUCCESS);
}

int	check_map(t_all *all, int i, int j, char ch)
{
	while (all->game.map[i])
	{
		j = 0;
		while (all->game.map[i][j] != '\0')
		{
			if (!ft_strchr("NSWE012", all->game.map[i][j]))
				return (ERROR_MAP_CHAR);
			if (ft_strchr("NSWE", all->game.map[i][j]) && !ch)
				correct_position(all, i, j, &ch);
			else if (ft_strchr("NSWE", all->game.map[i][j]) && ch)
				return (ERROR_PLAYER_POSITION);
			if (all->game.map[i][j] == '2')
				all->game.count_sprites++;
			j++;
		}
		i++;
	}
	if (!ch)
		return (ERROR_NO_PLAYER_POSITION);
	return (SUCCESS);
}