/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_list.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: burswyck <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/17 13:22:31 by burswyck          #+#    #+#             */
/*   Updated: 2021/04/17 13:22:33 by burswyck         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

t_map	*ft_lstn(char *content)
{
	t_map	*new_elem;

	new_elem = malloc(sizeof(t_map));
	if (!new_elem)
		return (0);
	new_elem->line = content;
	new_elem->next = 0;
	return (new_elem);
}

t_map	*ft_lstl(t_map *lst)
{
	t_map	*last_lst;

	last_lst = lst;
	if (!lst)
		return (0);
	while (lst->next)
	{
		last_lst = lst;
		lst = lst->next;
	}
	if (!lst->next)
		return (lst);
	return (last_lst);
}

void	ft_lstadd_b(t_map **lst, t_map *new)
{
	if (!*lst)
	{
		*lst = new;
		return ;
	}
	if (!new)
		return ;
	ft_lstl(*lst)->next = new;
	new->next = 0;
}

int	ft_lst_sz(t_map *lst)
{
	int		count;
	t_map	*temp;

	temp = lst;
	count = 0;
	if (!temp)
		return (0);
	while (temp->next)
	{
		count++;
		temp = temp->next;
	}
	if (temp->next == NULL)
		count++;
	return (count);
}
