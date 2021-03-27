/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: burswyck <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/11 17:53:28 by burswyck          #+#    #+#             */
/*   Updated: 2021/01/08 15:59:00 by burswyck         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

int		free_memory(char *buffer, int error)
{
	free(buffer);
	return (error);
}

char	*ft_check_last(char *last)
{
	int		i;
	int		j;
	char	*clear;

	i = 0;
	j = 0;
	if (!last)
		return (0);
	while (last[i] && last[i] != '\n')
		i++;
	if (!last[i])
	{
		free_memory(last, 0);
		return (0);
	}
	if (!(clear = malloc(sizeof(char) * ((ft_strlen(last) - i) + 1))))
		return (0);
	i++;
	while (last[i])
		clear[j++] = last[i++];
	clear[j] = '\0';
	free(last);
	return (clear);
}

char	*to_line(char *last)
{
	int		count_ch;
	char	*temp;

	count_ch = 0;
	if (!last)
		return (0);
	while (last[count_ch] && last[count_ch] != '\n')
		count_ch++;
	if (!(temp = (char*)malloc(sizeof(char) * (count_ch + 1))))
		return (0);
	count_ch = 0;
	while (last[count_ch] && last[count_ch] != '\n')
	{
		temp[count_ch] = last[count_ch];
		count_ch++;
	}
	temp[count_ch] = '\0';
	return (temp);
}

int		get_next_line(int fd, char **line)
{
	char			*buffer;
	static char		*last;
	int				fl_read;

	fl_read = 1;
	if (fd < 0 || !line || BUFFER_SIZE < 1)
		return (-1);
	if (!(buffer = (char *)malloc(sizeof(char) * (BUFFER_SIZE + 1))))
		return (-1);
	while (fl_read != 0 && !ft_strchr(last, '\n'))
	{
		if ((fl_read = read(fd, buffer, BUFFER_SIZE)) == -1)
			return (free_memory(buffer, -1));
		buffer[fl_read] = '\0';
		last = ft_strjoin(last, buffer);
	}
	free(buffer);
	*line = to_line(last);
	last = ft_check_last(last);
	return (!fl_read) ? 0 : 1;
}
