/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: burswyck <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/17 13:21:51 by burswyck          #+#    #+#             */
/*   Updated: 2021/04/17 13:21:54 by burswyck         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	skip_spaces(char *line, int *i)
{
	while ((line[*i] == ' ' || line[*i] == '\t' || line[*i] == '\n') \
	|| (line[*i] == '\r' || line[*i] == '\v' || line[*i] == '\f'))
		(*i)++;
	return (1);
}

int	check_full_tex(t_all *all, int **addr, char *filename_tex)
{
	void	*img;
	int		array[5];

	if (ft_strncmp(filename_tex + ft_strlen(filename_tex) - 4, ".xpm", 4))
		return (ERROR_TEXTUTE_EX);
	all->tex.fd = open(filename_tex, O_RDONLY);
	if (all->tex.fd == -1)
		return (ERROR_TEXTUTE_FILE);
	close(all->tex.fd);
	img = mlx_xpm_file_to_image(&all->mlx.mlx_ptr, filename_tex, \
	&array[0], &array[1]);
	if (array[0] != 64 || array[1] != 64 || !img)
		return (ERROR);
	*addr = (int *)mlx_get_data_addr(img, &array[2], &array[3], &array[4]);
	free(img);
	return (SUCCESS);
}

int	get_texture(int *i, char *buf, t_all *all, int **addr)
{
	int		error;

	error = 0;
	if (*addr != NULL)
		return (ERROR);
	(*i) += 2;
	skip_spaces(buf, i);
	all->tex.rm = (*i);
	while (!ft_strchr(INVCHARS, buf[(*i)]))
		(*i)++;
	all->tex.flt = (char *)malloc(sizeof(char) * (*i - all->tex.rm + 1));
	if (!all->tex.flt)
		return (ERROR_MALLOC);
	*i = all->tex.rm;
	all->tex.rm = 0;
	while (buf[*i] != ' ' && buf[*i] != '\0')
		all->tex.flt[all->tex.rm++] = buf[(*i)++];
	all->tex.flt[all->tex.rm] = '\0';
	error = check_full_tex(all, addr, all->tex.flt);
	free(all->tex.flt);
	skip_spaces(buf, i);
	if (buf[*i] != '\0')
		return (ERROR);
	all->tex.count_args++;
	return (error);
}

int	fetch_line_file(int i, char *buf, t_all *all, int error)
{
	skip_spaces(buf, &i);
	if (buf[i] == '\0')
		return (SUCCESS);
	if (buf[i] == '1' && all->tex.count_args == 8)
		error = get_map(&i, buf, all);
	if (buf[i] == 'R' && buf[i + 1] == ' ')
		error = get_resolution(&i, buf, all);
	if (buf[i] == 'N' && buf[i + 1] == 'O' && buf[i + 2] == ' ')
		error = get_texture(&i, buf, all, &all->tex.no);
	if (buf[i] == 'S' && buf[i + 1] == 'O' && buf[i + 2] == ' ')
		error = get_texture(&i, buf, all, &all->tex.so);
	if (buf[i] == 'E' && buf[i + 1] == 'A' && buf[i + 2] == ' ')
		error = get_texture(&i, buf, all, &all->tex.ea);
	if (buf[i] == 'W' && buf[i + 1] == 'E' && buf[i + 2] == ' ')
		error = get_texture(&i, buf, all, &all->tex.we);
	if (buf[i] == 'S' && buf[i + 1] == ' ')
		error = get_texture(&i, buf, all, &all->tex.sp);
	if (buf[i] == 'F' && buf[i + 1] == ' ')
		error = get_color(&i, buf, &all->tex.floor, all);
	if (buf[i] == 'C' && buf[i + 1] == ' ')
		error = get_color(&i, buf, &all->tex.ceil, all);
	if (error == 1 && all->win.x && all->win.y && all->tex.sp && \
	all->tex.no && all->tex.we && all->tex.so && all->tex.ea)
		return (SUCCESS);
	return (error);
}

int	file_parse(int fd, char *namefile, t_all *all, int error)
{
	char	*buffer;

	fd = open(namefile, O_RDONLY);
	if (fd == -1)
		return (ERROR_READING_FILE);
	while ((get_next_line(fd, &buffer)) == 1)
	{
		error = fetch_line_file(0, buffer, all, 0);
		if (error != SUCCESS)
			return (error);
		free(buffer);
	}
	error = fetch_line_file(0, buffer, all, 0);
	if (error != SUCCESS)
		return (error);
	free(buffer);
	close(fd);
	return (SUCCESS);
}
