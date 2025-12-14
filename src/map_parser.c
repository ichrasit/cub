#include "../Cub3D.h"
#include <stdio.h>


static char	**new_grid(int h, int w)
{
	char **g;
	int i;

	g = malloc(sizeof(char *) * (h + 1));
	if (!g)
		return (NULL);
	i = 0;
	while (i < h)
	{
		g[i] = malloc(w + 1);
		if (!g[i])
		{
			while (--i >= 0)
				free(g[i]);
			free(g);
			return (NULL);
		}
		ft_memset(g[i], ' ', w);
		g[i][w] = '\0';
		i++;
	}
	g[h] = NULL;
	return (g);
}

static int	is_map(char *s)
{
	int i;

	if (!s)
		return (0);
	i = 0;
	while (s[i] == ' ' || s[i] == '\t')
		i++;
	return (s[i] == '1' || s[i] == '0');
}

static int	is_blank(char *s)
{
	int i;

	if (!s)
		return (1);
	i = 0;
	while (s[i] && (s[i] == ' ' || s[i] == '\t'))
		i++;
	return (s[i] == '\0' || s[i] == '\n');
}

static int	contiguous_map_block(char *f)
{
	int		fd;
	char	*s;
	int		started;
	int		gap;
	int		line_no;

	fd = open(f, O_RDONLY);
	if (fd < 0)
		return (0);
	started = 0;
	gap = 0;
	line_no = 0;
	while (1)
	{
		s = get_next_line(fd);
		if (!s)
			break ;
		line_no++;
		ft_putnbr_fd(line_no, 2);
		ft_putstr_fd(": [", 2);
		if (ft_strlen(s) > 30)
		{
			write(2, s, 30);
			ft_putendl_fd("...]", 2);
		}
		else
		{
			ft_putstr_fd(s, 2);
			ft_putendl_fd("]", 2);
		}
		if (is_map(s))
		{
			if (gap)
			{
				ft_putendl_fd("Gap before map line", 2);
				free(s);
				close(fd);
				return (0);
			}
			ft_putendl_fd("  -> MAP LINE, started=1", 2);
			started = 1;
		}
		else if (started && is_blank(s))
		{
			ft_putendl_fd("  -> BLANK after map start, gap=1", 2);
			gap = 1;
		}
		free(s);
	}
	clear_gnl_fd(fd);
	close(fd);
	return (1);
}
/*
	
*/

static int	get_dim(char *f, int *h, int *w)
{
	int fd;
	char *s;
	int len;

	fd = open(f, O_RDONLY);
	if (fd < 0)
		return (0);
	*h = 0;
	*w = 0;
	while (42)
	{
		s = get_next_line(fd);
		if (!s)
			break ;
		if (is_map(s))
		{
			len = ft_strlen(s);
			if (len > 0 && s[len - 1] == '\n')
				len--;
			if (len > *w)
				*w = len;
			(*h)++;
		}
		free(s);
	}
	clear_gnl_fd(fd);
	close(fd);
	return (1);
}

static int	fill_grid(char *f, char **g, int h)
{
	int fd;
	char *s;
	int i;
	int j;

	fd = open(f, O_RDONLY);
	if (fd < 0)
		return (0);
	i = 0;
	while (i < h)
	{
		s = get_next_line(fd);
		if (!s)
			break ;
		if (is_map(s))
		{
			j = -1;
			while (s[++j] && s[j] != '\n')
				g[i][j] = s[j];
			i++;
		}
		free(s);
	}
	clear_gnl_fd(fd);
	close(fd);
	return (1);
}

int	parse_map(t_game *g, char *f)
{
	g->map = malloc(sizeof(t_map));
	if (!g->map)
		return (0);
	ft_bzero(g->map, sizeof(t_map));
	g->tex = malloc(sizeof(t_tex));
	if (!g->tex)
		return (0);
	ft_bzero(g->tex, sizeof(t_tex));
	g->plr = malloc(sizeof(t_plr));
	if (!g->plr)
		return (0);
	ft_bzero(g->plr, sizeof(t_plr));
	if (!read_config(g, f))
		return (0);
	if (!contiguous_map_block(f))
		return(ft_putendl_fd("Error\nMap block is not contiguous", 2), 0);
	if (!get_dim(f, &g->map->h, &g->map->w))
		return (0);
	g->map->grid = new_grid(g->map->h, g->map->w);
	if (!g->map->grid)
		return (0);
	if (!fill_grid(f, g->map->grid, g->map->h))
		return (0);
	return (validate_and_init_map(g));
}

/*
	parse map fonksiyonu oyun verileri için hafızada yer ayırır
	read config fonksiyonu ile duvar ve renk bilgilerini okur
	contigousus map block fonksiyonu ile harita da sızıntı var mı kontrol eder
	fill grid fonksiyonu haritanın boyutu ölçülür ve hafızaya kaydedilir.
	get dim fonksiyonu haritanın genişlik ve yükseklik değerlerini alır.
	validate and init map harita doğrumu ve player durumunu kontrol eder.
*/