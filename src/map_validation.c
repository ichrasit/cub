#include "../Cub3D.h"


static int	chk_char(char c)
{
	return (c == '0' || c == '1' || c == 'N' || c == 'S' || c == 'E' || c == 'W'
		|| c == ' ' || c == '\n' || c == '\r');
}

static int	cnt_plr(char **g, int h)
{
	int i;
	int j;
	int n;

	n = 0;
	i = 0;
	while (i < h)
	{
		j = 0;
		while (g[i][j])
		{
			if (ft_strchr("NSEW", g[i][j]))
				n++;
			j++;
		}
		i++;
	}
	return (n);
}

int	valid_chars(t_map *m)
{
	int i;
	int j;

	if (!m || !m->grid)
		return (0);
	i = 0;
	while (i < m->h)
	{
		j = 0;
		while (m->grid[i][j])
		{
			if (!chk_char(m->grid[i][j]))
				return (0);
			j++;
		}
		i++;
	}
	if (cnt_plr(m->grid, m->h) != 1)
		return (0);
	return (1);
}

static int	row_has_nonspace(char *row)
{
	int i;

	i = 0;
	while (row[i])
	{
		if (row[i] != ' ')
			return (1);
		i++;
	}
	return (0);
}

int	no_empty_gaps(t_map *m)
{
	int i;
	int seen;
	int gap;

	if (!m || !m->grid)
		return (0);
	seen = 0;
	gap = 0;
	i = 0;
	while (i < m->h)
	{
		if (row_has_nonspace(m->grid[i]))
		{
			if (gap)
				return (0);
			seen = 1;
		}
		else if (seen)
			gap = 1;
		i++;
	}
	return (1);
}
