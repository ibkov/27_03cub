/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cube3d.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: burswyck <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/17 13:20:12 by burswyck          #+#    #+#             */
/*   Updated: 2021/04/17 13:20:25 by burswyck         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/cube3d.h"

char	**map_to_matrix(t_all *lst, int i, int j)
{
	t_map	*temp;
	char	**matrix;

	temp = lst->map;
	matrix = malloc(sizeof(char *) * (ft_lst_sz(lst->map) + 1));
	if (!matrix)
		return (NULL);
	while (i < ft_lst_sz(lst->map))
	{
		matrix[i] = malloc(sizeof(char) * (ft_strlen(temp->line) + 1));
		if (!matrix[i])
			return (NULL);
		j = 0;
		while (temp->line[j] != '\0')
		{
			matrix[i][j] = temp->line[j];
			j++;
		}
		matrix[i++][j] = '\0';
		free(temp->line);
		temp = temp->next;
	}
	matrix[i] = NULL;
	free(temp);
	return (matrix);
}

int	get_map(char *buf, t_all *all)
{
	ft_lstadd_b(&all->map, ft_lstn(cor_start_ch(buf)));
	return (SUCCESS);
}

int	run_game(int cr_bmp, char *namefile, t_all *all)
{
	int	error;

	error = file_parse(0, namefile, all, 0);
	if (error != SUCCESS)
		return (error_str(error));
	all->game.map = map_to_matrix(all, 0, 0);
	error = check_map(all, 0, 0, 0);
	if (error != SUCCESS)
		return (error_str(error));
	all->sprite = malloc(sizeof(t_sprite) * all->game.count_sprites);
	add_sprites(all, 0, 0, 0);
	all->ray.zbuffer = malloc(sizeof(double) * all->win.x);
	all->mlx.mlx_ptr = mlx_init();
	all->ray.h = all->win.y;
	all->win.win_ptr = mlx_new_window(all->mlx.mlx_ptr, \
	all->win.x, all->win.y, "Game cube 3D");
	if (cr_bmp == 1)
		return (screenshot(all));
	screen_ray(all);
	mlx_hook(all->win.win_ptr, 2, 0, key_press, all);
	mlx_hook(all->win.win_ptr, 17, 0, close_win, all);
	mlx_loop(all->mlx.mlx_ptr);
	return (SUCCESS);
}

void	init_struct(int cr_bmp, char *namefile)
{
	t_all	all;

	ft_bzero(&all, sizeof(t_all));
	ft_bzero(&all.win, sizeof(t_win));
	ft_bzero(&all.img, sizeof(t_img));
	ft_bzero(&all.tex, sizeof(t_tex));
	ft_bzero(&all.game, sizeof(t_game));
	all.tex.floor = NONE;
	all.tex.ceil = NONE;
	run_game(cr_bmp, namefile, &all);
}

int	main(int argc, char **argv)
{
	if (argc == 3 && !ft_strncmp(argv[1] + ft_strlen(argv[1]) - 4, ".cub", 4) && \
		!ft_strncmp(argv[2], "--save", 6))
		init_struct(1, argv[1]);
	else if (argc == 2 && !ft_strncmp(argv[1] + \
	ft_strlen(argv[1]) - 4, ".cub", 4))
		init_struct(0, argv[1]);
	else
		ft_putstr_fd("\033[31m\nERROR\nMAIN ARGUMENTS\n\n\033[0m", 2);
}
