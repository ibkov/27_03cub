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
	int	check;

	skip_spaces(buf, i);
	check = *i;
	while (buf[*i] >= '0' && buf[*i] <= '9')
	{
		num = num * 10 + (buf[*i] - 48);
		(*i)++;
	}
	if (check == *i)
		return (-100);
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
	skip_spaces(buf, i);
	if (all->win.x < 1 || all->win.y < 1 || buf[*i] != '\0')
		return (ERROR_END_lINE);
	all->tex.count_args++;
	return (SUCCESS);
}

int	get_color(int *i, char *buf, unsigned int *color, t_all *all)
{
	t_color	rgb;

	ft_bzero(&rgb, sizeof(t_color));
	if (*color != NONE)
		return (ERROR_COLOR);
	(*i)++;
	rgb.r = ft_atoi_cub(i, buf, 0);
	if (!buf[(*i)++])
		return (ERROR_COLOR);
	rgb.g = ft_atoi_cub(i, buf, 0);
	if (!buf[(*i)++])
		return (ERROR_COLOR);
	rgb.b = ft_atoi_cub(i, buf, 0);
	skip_spaces(buf, i);
	if (buf[*i] || rgb.r > 255 || rgb.b > 255 || rgb.g > 255 \
	|| rgb.r < 0 || rgb.b < 0 || rgb.g < 0)
		return (ERROR_COLOR);
	*color = rgb.r * 256 * 256 + rgb.g * 256 + rgb.b;
	all->tex.count_args++;
	return (SUCCESS);
}

int	check_map_closed(int i, int j, t_all *all)
{
	if (all->game.map[i][j] == '0' || all->game.map[i][j] == 'N' \
	|| all->game.map[i][j] == 'E' || all->game.map[i][j] == 'S' \
	|| all->game.map[i][j] == 'W')
	{
		if (i == 0 || i == all->game.map_size - 1)
			return (0);
		if (j == 0 || j == ft_strlen(&all->game.map[i][0]) - 1)
			return (0);
		if ((i != 0 || i != all->game.map_size - 1) && (j != 0 || \
			j != ft_strlen(&all->game.map[i][0]) - 1))
			if (all->game.map[i - 1][j] == ' ' || all->game.map[i + 1][j] == ' ' \
			|| all->game.map[i][j + 1] == ' ' || all->game.map[i][j - 1] == ' ' \
			|| !all->game.map[i - 1][j] || !all->game.map[i + 1][j] || !all->game.map[i][j + 1] \
			|| !all->game.map[i][j - 1])
				return (0);
	}
	return (1);
}

int	check_map(t_all *all, int i, int j, char ch)
{
	while (all->game.map[i])
	{
		j = 0;
		while (all->game.map[i][j] != '\0')
		{
			if (!ft_strchr("NSWE012 ", all->game.map[i][j]))
				return (ERROR_MAP_CHAR);
			if (ft_strchr("NSWE", all->game.map[i][j]) && !ch)
				correct_position(all, i, j, &ch);
			else if (ft_strchr("NSWE", all->game.map[i][j]) && ch)
				return (ERROR_PLAYER_POSITION);
			if (!check_map_closed(i, j, all))
				return (ERROR_CLOSED_MAP);
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
