#include "../Cub3D.h"

void	set_ray(t_game *g, int x)
{
	t_ray	*r;
	double	cx;

	r = g->ray;
	cx = 2 * x / (double)WIN_W - 1;
	r->rdx = g->plr->dx + g->plr->cx * cx;
	r->rdy = g->plr->dy + g->plr->cy * cx;
	r->mx = (int)g->plr->px;
	r->my = (int)g->plr->py;
	r->ddx = fabs(1 / r->rdx);
	r->ddy = fabs(1 / r->rdy);
	r->hit = 0;
}

void	set_step(t_game *g)
{
	t_ray	*r;
	t_plr	*p;

	r = g->ray;
	p = g->plr;
	if (r->rdx < 0)
	{
		r->stx = -1;
		r->sdx = (p->px - r->mx) * r->ddx;
	}
	else
	{
		r->stx = 1;
		r->sdx = (r->mx + 1.0 - p->px) * r->ddx;
	}
	if (r->rdy < 0)
	{
		r->sty = -1;
		r->sdy = (p->py - r->my) * r->ddy;
	}
	else
	{
		r->sty = 1;
		r->sdy = (r->my + 1.0 - p->py) * r->ddy;
	}
}

void	run_dda(t_game *g)
{
	t_ray	*r;

	r = g->ray;
	while (r->hit == 0)
	{
		if (r->sdx < r->sdy)
		{
			r->sdx += r->ddx;
			r->mx += r->stx;
			r->side = 0;
		}
		else
		{
			r->sdy += r->ddy;
			r->my += r->sty;
			r->side = 1;
		}
		if (g->map->grid[r->my][r->mx] == '1')
			r->hit = 1;
	}
}

void	calc_wall(t_game *g)
{
	t_ray	*r;

	r = g->ray;
	if (r->side == 0)
		r->pwd = (r->sdx - r->ddx);
	else
		r->pwd = (r->sdy - r->ddy);
	r->lh = (int)(WIN_H / r->pwd);
	r->ds = -r->lh / 2 + WIN_H / 2;
	if (r->ds < 0)
		r->ds = 0;
	r->de = r->lh / 2 + WIN_H / 2;
	if (r->de >= WIN_H)
		r->de = WIN_H - 1;
}

t_img	*get_texture(t_game *g)
{
	t_ray	*r;

	r = g->ray;
	if (r->side == 0)
	{
		if (r->rdx > 0)
			return (g->tex->e);
		return (g->tex->w);
	}
	if (r->rdy > 0)
		return (g->tex->s);
	return (g->tex->n);
}
