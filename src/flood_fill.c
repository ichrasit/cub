#include "../Cub3D.h"


static int	chk_void(char **m, int x, int y, t_map *mp)
{
	if (y == 0 || y == mp->h - 1 || x == 0 || x >= mp->w - 1)
		return (0);
	if (m[y - 1][x] == ' ' || m[y + 1][x] == ' ' || m[y][x - 1] == ' ' || m[y][x
		+ 1] == ' ')
		return (0);
	return (1);
}

static int	scan_filled(char **m, t_map *mp)
{
	int i;
	int j;

	i = 0;
	while (i < mp->h)
	{
		j = 0;
		while (j < mp->w && m[i][j])
		{
			if (m[i][j] == 'F')
			{
				if (!chk_void(m, j, i, mp))
					return (0);
			}
			j++;
		}
		i++;
	}
	return (1);
}

static void	fill(char **m, int x, int y, t_map *mp)
{
	if (y < 0 || y >= mp->h || x < 0 || x >= mp->w)
		return ;
	if (m[y][x] == '1' || m[y][x] == 'F' || m[y][x] == ' ')
		return ;
	m[y][x] = 'F';
	fill(m, x + 1, y, mp);
	fill(m, x - 1, y, mp);
	fill(m, x, y + 1, mp);
	fill(m, x, y - 1, mp);
}

int	check_flood(t_map *map)
{
	char **dup;
	int px;
	int py;
	int res;

	if (!map->grid)
		return (0);
	if (!find_pos(map, &px, &py))
		return (0);
	dup = dup_arr(map);
	if (!dup)
		return (0);
	fill(dup, px, py, map);
	res = scan_filled(dup, map);
	free_arr(dup, map->h);
	return (res);
}
