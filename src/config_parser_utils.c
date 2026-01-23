/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_parser_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muhaoz <muhaoz@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/23 00:00:00 by htekdemi          #+#    #+#             */
/*   Updated: 2026/01/23 20:19:04 by muhaoz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Cub3D.h"

int	validate_path(char *s, int i, int len)
{
	if (len < 5 || ft_strncmp(s + i + len - 4, ".xpm", 4) != 0)
		return (0);
	while (s[i + len] && (s[i + len] == ' ' || s[i + len] == '\t'))
		len++;
	if (s[i + len] && s[i + len] != '\n')
		return (0);
	return (1);
}

int	get_path(char *s, char **p, int i)
{
	int	len;
	int	fd;

	while (s[i] && (s[i] == ' ' || s[i] == '\t'))
		i++;
	len = 0;
	while (s[i + len] && s[i + len] != '\n' && s[i + len] != ' '
		&& s[i + len] != '\t')
		len++;
	if (!validate_path(s, i, len))
		return (0);
	len = 0;
	while (s[i + len] && s[i + len] != '\n' && s[i + len] != ' '
		&& s[i + len] != '\t')
		len++;
	*p = malloc(len + 1);
	if (!*p)
		return (0);
	ft_strlcpy(*p, s + i, len + 1);
	fd = open(*p, O_RDONLY);
	if (fd < 0)
		return (0);
	close(fd);
	return (1);
}

int	get_val(char *s, int *i)
{
	int	n;

	while (s[*i] && (s[*i] == ' ' || s[*i] == '\t'))
		(*i)++;
	if (!ft_isdigit(s[*i]))
		return (-1);
	n = ft_atoi(s + *i);
	while (s[*i] && ft_isdigit(s[*i]))
		(*i)++;
	while (s[*i] && (s[*i] == ' ' || s[*i] == '\t'))
		(*i)++;
	return (n);
}

int	get_rgb(char *s, int *c)
{
	int	r;
	int	g;
	int	b;
	int	i;

	i = 1;
	r = get_val(s, &i);
	if (r < 0 || s[i] != ',')
		return (0);
	i++;
	g = get_val(s, &i);
	if (g < 0 || s[i] != ',')
		return (0);
	i++;
	b = get_val(s, &i);
	if (b < 0)
		return (0);
	while (s[i] && (s[i] == ' ' || s[i] == '\t'))
		i++;
	if (s[i] && s[i] != '\n')
		return (0);
	if (r > 255 || g > 255 || b > 255)
		return (0);
	*c = (r << 16) | (g << 8) | b;
	return (1);
}
