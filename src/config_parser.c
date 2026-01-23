/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_parser.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htekdemi <htekdemi@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 18:56:29 by htekdemi          #+#    #+#             */
/*   Updated: 2026/01/09 18:56:29 by htekdemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Cub3D.h"

static int	chk_line(char *s, t_game *g)
{
	int	i;
	int	ret;

	i = 0;
	while (s[i] == ' ' || s[i] == '\t')
		i++;
	if (!s[i])
		return (1);
	ret = check_dup_tex(g, i, s);
	if (ret != -1)
		return (ret);
	ret = check_dup_tex2(g, i, s);
	if (ret != -1)
		return (ret);
	ret = check_dup_col(g, i, s);
	if (ret != -1)
		return (ret);
	return (0);
}

static int	process_config_line(char *s, t_game *g, int *empty)
{
	int	ok;

	if (s[0] == '\n')
	{
		(*empty)++;
		if (*empty > 1)
			return (0);
		return (1);
	}
	*empty = 0;
	ok = chk_line(s, g);
	return (ok);
}

static int	read_config_loop(int fd, t_game *g, int *empty)
{
	char	*s;
	int		ok;

	ok = 1;
	while (ok)
	{
		s = get_next_line(fd);
		if (!s)
			break ;
		if (is_map_line(s))
		{
			free(s);
			break ;
		}
		ok = process_config_line(s, g, empty);
		free(s);
	}
	return (ok);
}

int	read_config(t_game *g, char *f)
{
	int		fd;
	int		ok;
	int		empty;

	fd = open(f, O_RDONLY);
	if (fd < 0)
		return (0);
	empty = 0;
	ok = read_config_loop(fd, g, &empty);
	clear_gnl_fd(fd);
	close(fd);
	if (!ok)
		return (0);
	return (chk_complete(g));
}
