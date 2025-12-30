#include "../Cub3D.h"

static int	chk_void(char **m, int x, int y, t_map *mp)
{
	if (y == 0 || x == mp->h - 1 || x == 0 || x >= mp->w - 1)
		return 0;
	if(m[y - 1][x] == ' ' || m[y + 1][x] == ' ' ||
		m[y][x - 1] == ' ' || m[y][x + 1] == ' ')
			return 0;
	return 1;
}

static int	scan_filled(char **m, t_map *mp)
{
	int i;
	int j;

	i = 0;

	while(i < mp->h)
	{
		j = 0;
		while(j < mp->w && m[i][j])
		{
			if(m[i][j] == 'F')
			{
				if(!chk_void(m, j, i, mp))
					return 0;
			}
			j++;

		}
		i++;
	}
	return 1;
}

static void	add_pt(t_pt *s, int *t, int x, int y)
{
	*t = 1;
	s[*t].x = x;
	s[*t].y = y;
}

static void	run_algo(char **tab, t_map *m, int x, int y)
{
	t_pt	*s;
	int	t;
	t_pt	c;

	s = malloc(sizeof(t_pt) * (m->w * m->h));
	if(!s)
		return;
	t = -1;
	add_pt(s, &t, x, y);
	while(t >= 0)
	{
		c = s[t--];
		if(c.x < 0 || c.y < 0 || c.x >= m->w || c.y >= m->h)
			continue;
		if(tab[c.y][c.x] == '1' || tab[c.y][c.x] == 'F' || tab[c.y][c.x] == ' ')
			continue;
		tab[c.y][c.x] = 'F';
		add_pt(s, &t, c.x + 1, c.y);
		add_pt(s, &t, c.x - 1, c.y);
		add_pt(s, &t, c.x, c.y + 1);
		add_pt(s, &t, c.x, c.y - 1);
	
	}
	free(s);
}


int	check_flood(t_map *map)
{
	char **dup;
	int	px;
	int py;
	int res;

	if (!map->grid)
		return 0;
	if (!find_pos(map, &px, &py))
		return 0;
	dup = dup_arr(map);
	if(!dup)
		return 0;
	run_algo(dup, map, px, py);
	res = scan_filled(dup, map);
	free_arr(dup, map->h);
	return res;
}
