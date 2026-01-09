/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flood_fill.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+
	+:+     */
/*   By: htekdemi <htekdemi@student.42kocaeli.co    +#+  +:+
	+#+        */
/*                                                +#+#+#+#+#+
	+#+           */
/*   Created: 2026/01/09 18:56:35 by htekdemi          #+#    #+#             */
/*   Updated: 2026/01/09 18:56:35 by htekdemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Cub3D.h"

static int	can_add(t_ff *f, int x, int y)
{
	if (x < 0 || y < 0 || x >= f->m->w || y >= f->m->h)
		return (0);
	if (f->tab[y][x] == '1' || f->tab[y][x] == 'F' || f->tab[y][x] == ' ')
		return (0);
	return (1);
}

static void	add_pt(t_ff *f, int x, int y)
{
	f->t++;
	f->s[f->t].x = x;
	f->s[f->t].y = y;
}

static void	add_neighbors(t_ff *f, t_pt c)
{
	if (can_add(f, c.x + 1, c.y))
		add_pt(f, c.x + 1, c.y);
	if (can_add(f, c.x - 1, c.y))
		add_pt(f, c.x - 1, c.y);
	if (can_add(f, c.x, c.y + 1))
		add_pt(f, c.x, c.y + 1);
	if (can_add(f, c.x, c.y - 1))
		add_pt(f, c.x, c.y - 1);
}

static void	run_algo(char **tab, t_map *m, int x, int y)
{
	t_ff	f;
	t_pt	c;

	f.s = malloc(sizeof(t_pt) * (m->w * m->h));
	if (!f.s)
		return ;
	f.tab = tab;
	f.m = m;
	f.t = -1;
	add_pt(&f, x, y);
	while (f.t >= 0)
	{
		c = f.s[f.t--];
		if (c.x < 0 || c.y < 0 || c.x >= m->w || c.y >= m->h)
			continue ;
		if (tab[c.y][c.x] == '1' || tab[c.y][c.x] == 'F'
			|| tab[c.y][c.x] == ' ')
			continue ;
		tab[c.y][c.x] = 'F';
		add_neighbors(&f, c);
	}
	free(f.s);
}

int	check_flood(t_map *map)
{
	char	**dup;
	int		px;
	int		py;
	int		res;

	if (!map->grid)
		return (0);
	if (!find_pos(map, &px, &py))
		return (0);
	dup = dup_arr(map);
	if (!dup)
		return (0);
	run_algo(dup, map, px, py);
	res = scan_filled(dup, map);
	free_arr(dup, map->h);
	return (res);
}
