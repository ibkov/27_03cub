/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bitmap.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: burswyck <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/17 13:20:12 by burswyck          #+#    #+#             */
/*   Updated: 2021/04/17 13:20:25 by burswyck         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/cube3d.h"

void	bdata(t_all *all, int fd)
{
	int				i;
	int				j;
	unsigned char	buf[4];

	i = all->win.x * (all->win.y - 1);
	while (i >= 0)
	{
		j = 0;
		while (j < all->win.x)
		{
			buf[0] = (unsigned char)(all->img.addr[i] % 256);
			buf[1] = (unsigned char)(all->img.addr[i] / 256 % 256);
			buf[2] = (unsigned char)(all->img.addr[i] / 256 / 256 % 256);
			buf[3] = (unsigned char)(0);
			write(fd, buf, 4);
			i++;
			j++;
		}
		i -= 2 * all->win.x;
	}
}

void	binfo(t_all *s, int fd)
{
	int				n;
	unsigned char	header[40];

	n = 0;
	while (n < 40)
		header[n++] = (unsigned char)(0);
	header[0] = (unsigned char)(40);
	n = s->win.x;
	header[4] = (unsigned char)(n % 256);
	header[5] = (unsigned char)(n / 256 % 256);
	header[6] = (unsigned char)(n / 256 / 256 % 256);
	header[7] = (unsigned char)(n / 256 / 256 / 256);
	n = s->win.y;
	header[8] = (unsigned char)(n % 256);
	header[9] = (unsigned char)(n / 256 % 256);
	header[10] = (unsigned char)(n / 256 / 256 % 256);
	header[11] = (unsigned char)(n / 256 / 256 / 256);
	header[12] = (unsigned char)(1);
	header[14] = (unsigned char)(32);
	write(fd, header, 40);
}

void	bfile(t_all *s, int fd)
{
	int				n;
	unsigned char	header[14];

	n = 0;
	while (n < 14)
		header[n++] = (unsigned char)(0);
	header[0] = (unsigned char)(66);
	header[1] = (unsigned char)(77);
	n = s->win.x * s->win.y * 4 + 54;
	header[2] = (unsigned char)(n % 256);
	header[3] = (unsigned char)(n / 256 % 256);
	header[4] = (unsigned char)(n / 256 / 256 % 256);
	header[5] = (unsigned char)(n / 256 / 256 / 256);
	header[10] = (unsigned char)(54);
	write(fd, header, 14);
}

int	screenshot(t_all *all)
{
	int		fd;

	screen_ray(all);
	fd = open("bitmap.bmp", O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU);
	bfile(all, fd);
	binfo(all, fd);
	bdata(all, fd);
	close(fd);
	free(all->img.img_ptr);
	free(all->img.addr);
	exit(0);
}
