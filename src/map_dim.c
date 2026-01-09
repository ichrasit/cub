/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_dim.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muhaoz <muhaoz@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 18:57:02 by htekdemi          #+#    #+#             */
/*   Updated: 2026/01/09 20:24:55 by muhaoz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Cub3D.h"

int	is_map(char *s)
{
	int	i;

	if (!s)
		return (0);
	i = 0;
	while (s[i] == ' ' || s[i] == '\t')
		i++;
	return (s[i] == '1' || s[i] == '0');
}

static void	process_dim_line(char *s, int *h, int *w)
{
	int	len;

	if (is_map(s))
	{
		len = ft_strlen(s);
		if (len > 0 && s[len - 1] == '\n')
			len--;
		if (len > *w)
			*w = len;
		(*h)++;
	}
}

int	get_dim(char *f, int *h, int *w)
{
	int		fd;
	char	*s;

	fd = open(f, O_RDONLY);
	if (fd < 0)
		return (0);
	*h = 0;
	*w = 0;
	s = get_next_line(fd);
	while (s)
	{
		process_dim_line(s, h, w);
		free(s);
		s = get_next_line(fd);
	}
	clear_gnl_fd(fd);
	close(fd);
	return (1);
}

static void	copy_line(char *s, char **g, int *i)
{
	int	j;

	j = -1;
	while (s[++j] && s[j] != '\n')
		g[*i][j] = s[j];
	(*i)++;
}

int	fill_grid(char *f, char **g, int h)
{
	int		fd;
	char	*s;
	int		i;

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
			copy_line(s, g, &i);
		free(s);
	}
	clear_gnl_fd(fd);
	close(fd);
	return (1);
}
