#include "../Cub3D.h"

void	strafe(t_game *g, double d)
{
	double	nx;
	double	ny;
	double	sx;
	double	sy;

	sx = g->plr->cx * MOV_SPD * d;
	sy = g->plr->cy * MOV_SPD * d;
	nx = g->plr->px + sx;
	ny = g->plr->py + sy;
	if (!is_wall(g, nx, g->plr->py))
		g->plr->px = nx;
	if (!is_wall(g, g->plr->px, ny))
		g->plr->py = ny;
}

void	rotate(t_game *g, double d)
{
	double	od;
	double	op;
	double	s;
	double	c;

	s = sin(ROT_SPD * d);
	c = cos(ROT_SPD * d);
	od = g->plr->dx;
	g->plr->dx = g->plr->dx * c - g->plr->dy * s;
	g->plr->dy = od * s + g->plr->dy * c;
	op = g->plr->cx;
	g->plr->cx = g->plr->cx * c - g->plr->cy * s;
	g->plr->cy = op * s + g->plr->cy * c;
}
