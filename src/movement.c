#include "../Cub3D.h"

int	is_wall(t_game *g, double x, double y)
{
	int mx;
	int my;

	mx = (int)x;
	my = (int)y;
	if (mx < 0 || mx >= g->map->w || my < 0 || my >= g->map->h)
		return (1);
	if (g->map->grid[my][mx] == '1')
		return (1);
	return (0);
}

static void	move(t_game *g, double d)
{
	double nx;
	double ny;
	double sx;
	double sy;

	sx = g->plr->dx * MOV_SPD * d;
	sy = g->plr->dy * MOV_SPD * d;
	nx = g->plr->px + sx;
	ny = g->plr->py + sy;
	if (!is_wall(g, nx, g->plr->py))
		g->plr->px = nx;
	if (!is_wall(g, g->plr->px, ny))
		g->plr->py = ny;
}

void	inputs(t_game *g)
{
	if (g->key->w)
		move(g, 1.0);
	if (g->key->s)
		move(g, -1.0);
	if (g->key->a)
		strafe(g, -1.0);
	if (g->key->d)
		strafe(g, 1.0);
	if (g->key->l)
		rotate(g, -1.0);
	if (g->key->r)
		rotate(g, 1.0);
}
