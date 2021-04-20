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

#include "./includes/cube3d.h"

int check_full_tex(t_all *all, int **addr, char *filename_tex)
{
  void *img;
  int array[5];

  if (!(ft_strncmp(filename_tex + ft_strlen(filename_tex) - 4, ".xmp", 4)))
    return (ERROR_TEXTUTE_EX);
  if ((all->tex.fd = open(filename_tex, O_RDONLY)) == -1)
    return (ERROR_TEXTUTE_FILE);
  close(all->tex.fd);
  img = mlx_xpm_file_to_image(&all->mlx.mlx_ptr, filename_tex, &array[0], &array[1]);
  if (array[0] != 64 || array[1] != 64 || !img)
    return (ERROR);
  *addr = (int *)mlx_get_data_addr(img, &array[2], &array[3], &array[4]);
  free(img);
  return(SUCCESS);
}

int get_texture(int *i, char *buf, t_all *all, int **addr)
{
  int rem;
  char *filename_tex;
  int error;

  error = 0;
  if (*addr != NULL)
		return (ERROR);
  (*i) += 2;
  while (ft_strchr(INVCHARS, buf[(*i)]))
    (*i)++;
  rem = (*i);
  while (!ft_strchr(INVCHARS, buf[(*i)]))
    (*i)++;
  if (!(filename_tex = (char*)malloc(sizeof(char) * (*i - rem + 1))))
    return (ERROR_MALLOC);
  *i = rem;
  rem = 0;
  while(buf[*i] != ' ' && buf[*i] != '\0')
    filename_tex[rem++] = buf[(*i)++];
  filename_tex[rem] = '\0';
  error = check_full_tex(all, addr, filename_tex);
  free(filename_tex);
  return (error);
}

int fetch_line_file(int i, char *buf, t_all *all)
{
  int error;

  error = 0;
  while (ft_strchr(INVCHARS, buf[i]))
    i++;
  if (buf[i] == '1' && buf[i] != '\0')
    error = get_map(&i, buf, all);
  if (buf[i] == 'R' && buf[i + 1] == ' ')
    error = get_resolution(&i ,buf , all);
  if (buf[i] == 'N' && buf[i + 1] == 'O' && buf[i + 2] == ' ')
    error = get_texture(&i ,buf, all, &all->tex.no);
  if (buf[i] == 'S' && buf[i + 1] == 'O' && buf[i + 2] == ' ')
    error = get_texture(&i ,buf, all, &all->tex.so);
  if (buf[i] == 'E' && buf[i + 1] == 'A' && buf[i + 2] == ' ')
    error = get_texture(&i ,buf, all, &all->tex.ea);
  if (buf[i] == 'W' && buf[i + 1] == 'E' && buf[i + 2] == ' ')
    error = get_texture(&i ,buf, all, &all->tex.we);
  if (buf[i] == 'S' && buf[i + 1] == ' ')
    error = get_texture(&i ,buf, all, &all->tex.sp);
  if (buf[i] == 'F' && buf[i + 1] == ' ')
    error = get_color(&i ,buf, &all->tex.floor);
  if (buf[i] == 'C' && buf[i + 1] == ' ')
    error = get_color(&i ,buf, &all->tex.ceil);
  if (error == SUCCESS && all->win.x && all->win.y && all->tex.no && all->tex.we && all->tex.so && all->tex.ea)
    error = 1;
  return ((error == 1 ? SUCCESS : error));
}

int file_parse(int fd, char *namefile, t_all *all, int error)
{
  char *buffer;
  
  if ((fd = open(namefile, O_RDONLY)) == -1)
    return (ERROR_READING_FILE);
  while ((get_next_line(fd, &buffer)) > ERROR)
  {
    if ((error = fetch_line_file(0, buffer, all)) != SUCCESS)
      return (error);
    free(buffer);
  }
  if ((error = fetch_line_file(0, buffer, all)) != SUCCESS)
      return (error);
  free(buffer);
  close(fd);
  return (SUCCESS);
}
