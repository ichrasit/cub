/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_contiguous.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htekdemi <htekdemi@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 18:56:59 by htekdemi          #+#    #+#             */
/*   Updated: 2026/01/09 18:57:00 by htekdemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Cub3D.h"

static int	is_blank_line(char *s)
{
	int i;

	if (!s)
		return (1);
	i = 0;
	while (s[i] && (s[i] == ' ' || s[i] == '\t'))
		i++;
	return (s[i] == '\0' || s[i] == '\n');
}

static void	print_line_debug(char *s, int line_no)
{
	ft_putnbr_fd(line_no, 2);
	ft_putstr_fd(": [", 2);
	if (ft_strlen(s) > 30)
	{
		write(2, s, 30);
		ft_putendl_fd("...]", 2);
	}
	else
	{
		ft_putstr_fd(s, 2);
		ft_putendl_fd("]", 2);
	}
}

static int	process_line(char *s, int *started, int *gap)
{
	if (is_map_line(s))
	{
		if (*gap)
		{
			ft_putendl_fd("Gap before map line", 2);
			return (0);
		}
		ft_putendl_fd("  -> MAP LINE, started=1", 2);
		*started = 1;
	}
	else if (*started && is_blank_line(s))
	{
		ft_putendl_fd("  -> BLANK after map start, gap=1", 2);
		*gap = 1;
	}
	return (1);
}

static int	check_loop(int fd, int *started, int *gap)
{
	char	*s;
	int		line_no;

	line_no = 0;
	while (1)
	{
		s = get_next_line(fd);
		if (!s)
			break ;
		line_no++;
		print_line_debug(s, line_no);
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
