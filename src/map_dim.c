/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_dim.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htekdemi <htekdemi@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 18:57:02 by htekdemi          #+#    #+#             */
/*   Updated: 2026/01/09 18:57:03 by htekdemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Cub3D.h"

int	is_map(char *s)
{
	int i;

	if (!s)
		return (0);
	i = 0;
	while (s[i] == ' ' || s[i] == '\t')
		i++;
	return (s[i] == '1' || s[i] == '0');
}

int	get_dim(char *f, int *h, int *w)
{
	int		fd;
	char	*s;
	int		len;

	fd = open(f, O_RDONLY);
	if (fd < 0)
		return (0);
	*h = 0;
	*w = 0;
	while (42)
	{
		s = get_next_line(fd);
		if (!s)
			break ;
		if (is_map(s))
		{
			len = ft_strlen(s);
			if (len > 0 && s[len - 1] == '\n')
				len--;
			if (len > *w)
				*w = len;
			(*h)++;
		}
		free(s);
	}
	clear_gnl_fd(fd);
	close(fd);
	return (1);
}

int	fill_grid(char *f, char **g, int h)
{
	int		fd;
	char	*s;
	int		i;
	int		j;

	fd = open(f, O_RDONLY);
	if (fd < 0)
		return (0);
	i = 0;
	while (i < h)
	{
		s = get_next_line(fd);
		if (!s)
			break ;
		if (is_map(s))
		{
			j = -1;
			while (s[++j] && s[j] != '\n')
				g[i][j] = s[j];
			i++;
		}
		free(s);
	}
	clear_gnl_fd(fd);
	close(fd);
	return (1);
}
