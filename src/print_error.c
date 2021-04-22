/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_error.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: burswyck <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/17 13:22:19 by burswyck          #+#    #+#             */
/*   Updated: 2021/04/17 13:22:22 by burswyck         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	error_str_next(int error)
{
	if (error == ERROR_MAP_CHAR)
		ft_putstr_fd("\033[31m\nError\nNot valid char in map\n\n\033[0m", 2);
	else if (error == ERROR_COLOR)
		ft_putstr_fd("\033[31m\nError\ncolor floor or ceil\n\n\033[0m", 2);
	else if (error == ERROR_CLOSED_MAP)
		ft_putstr_fd("\033[31m\nError\nMap not closed\n\n\033[0m", 2);
	return (ERROR);
}

int	error_str(int error)
{
	if (error == ERROR_READING_FILE)
		ft_putstr_fd("\033[31m\nError\nNo such file in directory\n\n\033[0m", 2);
	else if (error == ERROR_MAP)
		ft_putstr_fd("\033[33m\nError\nCorrect you map.\n\n\033[0m", 2);
	else if (error == ERROR_MAP_MANY_POSITIONS)
		ft_putstr_fd("\033[31m\nError\nMap has no one gamer position\n\n\033[0m", 2);
	else if (error == ERROR_MAP_NO_POSITIONS)
		ft_putstr_fd("\033[31m\nError\nMap has no gamer position\n\n\033[0m", 2);
	else if (error == ERROR_MAP_NOT_CLOSED)
		ft_putstr_fd("\033[31m\nError\nYour map not closed\n\n\033[0m", 2);
	else if (error == ERROR_RESOLUTION)
		ft_putstr_fd("\033[31m\nError\nResolution is not correct\n\n\033[0m", 2);
	else if (error == ERROR_END_lINE)
		ft_putstr_fd("\033[31m\nError\nInvalid chars in arguments\n\n\033[0m", 2);
	else if (error == ERROR)
		ft_putstr_fd("\033[31m\nError\nInvalid arguments\n\n\033[0m", 2);
	else if (error == ERROR_TEXTUTE_FILE)
		ft_putstr_fd("\033[31m\nError\nFile texture not found\n\033[0m", 2);
	else if (error == ERROR_TEXTUTE_EX)
		ft_putstr_fd("\033[31m\nError\nInvalid extention texture file\n\n\033[0m", 2);
	else if (error == ERROR_PLAYER_POSITION)
		ft_putstr_fd("\033[31m\nError\nNot one player position on map\n\n\033[0m", 2);
	else if (error == ERROR_NO_PLAYER_POSITION)
		ft_putstr_fd("\033[31m\nError\nNo player position on map\n\n\033[0m", 2);
	return (error_str_next(error));
}
