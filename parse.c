#include "./includes/cube3d.h"

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
  if (all->win.x && all->win.y && all->tex.no && all->tex.we && all->tex.so && all->tex.ea)
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