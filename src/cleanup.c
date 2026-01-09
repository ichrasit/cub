/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htekdemi <htekdemi@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 18:56:25 by htekdemi          #+#    #+#             */
/*   Updated: 2026/01/09 18:56:27 by htekdemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Cub3D.h"

static void	free_map_grid(t_game *g)
{
	int	i;

	if (g->map && g->map->grid)
	{
		i = 0;
		while (i < g->map->h)
			free(g->map->grid[i++]);
		free(g->map->grid);
	}
}

static void	free_game_structs(t_game *g)
{
	if (g->map)
		free(g->map);
	if (g->plr)
		free(g->plr);
	if (g->ray)
		free(g->ray);
	if (g->key)
		free(g->key);
}

static void	free_mlx(t_game *g)
{
	if (g->scr)
	{
		if (g->scr->ptr)
			mlx_destroy_image(g->mlx, g->scr->ptr);
		free(g->scr);
	}
	if (g->win)
		mlx_destroy_window(g->mlx, g->win);
	if (g->mlx)
	{
		mlx_destroy_display(g->mlx);
		free(g->mlx);
	}
}

void	clean_exit(t_game *g, char *m, int c)
{
	if (m)
		ft_putendl_fd(m, 2);
	free_map_grid(g);
	free_game_structs(g);
	free_tex(g);
	if (g->tex)
		free(g->tex);
	free_mlx(g);
	exit(c);
}

int	end_game(t_game *g)
{
	clean_exit(g, NULL, 0);
	return (0);
}
