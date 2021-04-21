/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: burswyck <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/11 17:54:15 by burswyck          #+#    #+#             */
/*   Updated: 2021/04/17 13:27:10 by burswyck         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	unsigned char	*d;
	unsigned char	*s;

	if (!dst && !src)
		return (0);
	d = (unsigned char *)dst;
	s = (unsigned char *)src;
	if (d < s)
	{
		while (len--)
			*d++ = *s++;
	}
	else
	{
		d += len;
		s += len;
		while (len--)
			*--d = *--s;
	}
	return (dst);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*str_joined;
	int		len_all_str;

	if (!s1 && !s2)
		return (0);
	len_all_str = ft_strlen_gnl(s1) + ft_strlen_gnl(s2);
	str_joined = malloc(sizeof(str_joined) * (len_all_str) + 1);
	if (!(str_joined))
		return (0);
	ft_memmove(str_joined, s1, ft_strlen_gnl(s1));
	ft_memmove(str_joined + ft_strlen_gnl(s1), s2, ft_strlen_gnl(s2));
	str_joined[len_all_str] = '\0';
	free((char *)s1);
	return (str_joined);
}

char	*ft_strchr(const char *string, int symbol)
{
	if (!string)
		return (0);
	while (*string != symbol && *string)
	{
		string++;
	}
	if (symbol == '\0')
		return ((char *)string);
	if (*string == '\0')
		return (0);
	return ((char *)string);
}

int	ft_strlen_gnl(const char *str)
{
	int		i;

	i = 0;
	if (!str)
		return (0);
	while (str[i])
	{
		i++;
	}
	return (i);
}
