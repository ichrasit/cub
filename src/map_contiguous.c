/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_contiguous.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muhaoz <muhaoz@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 18:56:59 by htekdemi          #+#    #+#             */
/*   Updated: 2026/01/09 23:44:29 by muhaoz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Cub3D.h"

static int	is_blank_line(char *s)
{
	int	i;

	if (!s)
		return (1);
	i = 0;
	while (s[i] && (s[i] == ' ' || s[i] == '\t'))
		i++;
	return (s[i] == '\0' || s[i] == '\n');
}

static int	process_line(char *s, int *started, int *gap)
{
	if (is_map_line(s))
	{
		if (*gap)
			return (0);
		*started = 1;
	}
	else if (*started && is_blank_line(s))
		*gap = 1;
	return (1);
}

static int	check_loop(int fd, int *started, int *gap)
{
	char	*s;

	while (1)
	{
		s = get_next_line(fd);
		if (!s)
			break ;
		if (!process_line(s, started, gap))
		{
			free(s);
			return (0);
		}
		free(s);
	}
	return (1);
}

int	contiguous_map_block(char *f)
{
	int	fd;
	int	started;
	int	gap;
	int	result;

	fd = open(f, O_RDONLY);
	if (fd < 0)
		return (0);
	started = 0;
	gap = 0;
	result = check_loop(fd, &started, &gap);
	clear_gnl_fd(fd);
	close(fd);
	return (result);
}
