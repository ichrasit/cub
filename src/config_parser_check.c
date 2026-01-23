/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_parser_check.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muhaoz <muhaoz@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/23 00:00:00 by htekdemi          #+#    #+#             */
/*   Updated: 2026/01/23 20:19:08 by muhaoz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Cub3D.h"

int	check_dup_tex(t_game *g, int i, char *s)
{
	if (!ft_strncmp(s + i, "NO", 2) && s[i + 2] == ' ')
	{
		if (g->tex->n_path)
			return (0);
		return (get_path(s, &g->tex->n_path, i + 2));
	}
	if (!ft_strncmp(s + i, "SO", 2) && s[i + 2] == ' ')
	{
		if (g->tex->s_path)
			return (0);
		return (get_path(s, &g->tex->s_path, i + 2));
	}
	return (-1);
}

int	check_dup_tex2(t_game *g, int i, char *s)
{
	if (!ft_strncmp(s + i, "WE", 2) && s[i + 2] == ' ')
	{
		if (g->tex->w_path)
			return (0);
		return (get_path(s, &g->tex->w_path, i + 2));
	}
	if (!ft_strncmp(s + i, "EA", 2) && s[i + 2] == ' ')
	{
		if (g->tex->e_path)
			return (0);
		return (get_path(s, &g->tex->e_path, i + 2));
	}
	return (-1);
}

int	check_dup_col(t_game *g, int i, char *s)
{
	if (!ft_strncmp(s + i, "F", 1) && s[i + 1] == ' ')
	{
		if (g->map->f_col != -1)
			return (0);
		return (get_rgb(s + i, &g->map->f_col));
	}
	if (!ft_strncmp(s + i, "C", 1) && s[i + 1] == ' ')
	{
		if (g->map->c_col != -1)
			return (0);
		return (get_rgb(s + i, &g->map->c_col));
	}
	return (-1);
}

int	chk_complete(t_game *g)
{
	if (!g->tex->n_path || !g->tex->s_path
		|| !g->tex->w_path || !g->tex->e_path)
		return (0);
	if (g->map->f_col == -1 || g->map->c_col == -1)
		return (0);
	return (1);
}
