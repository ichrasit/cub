#include "../Cub3D.h"

/*
	NO yazısından sonraki boşlukları atlar
	Dosya yolunun nerede bittiğini belirler
	Dosya yolunu için bellek ayırır ve kopyalar
*/
static int	get_path(char *s, char **p, int i)
{
	int len;

	while (s[i] && (s[i] == ' ' || s[i] == '\t'))
		i++;
	len = 0;
	while (s[i + len] && s[i + len] != '\n' && s[i + len] != ' ')
		len++;
	*p = malloc(len + 1);
	if (!*p)
		return (0);
	ft_strlcpy(*p, s + i, len + 1);
	return (1);
}

/*
	sadece sayıları hesaplar
	boşlukları atlar
	ft_atoi ile sayıya çevirir
*/
static int	get_val(char *s, int *i)
{
	int n;

	while (s[*i] && (s[*i] == ' ' || s[*i] == '\t'))
		(*i)++;
	if (!ft_isdigit(s[*i]))
		return (-1);
	n = ft_atoi(s + *i);
	while (s[*i] && ft_isdigit(s[*i]))
		(*i)++;
	while (s[*i] && (s[*i] == ' ' || s[*i] == '\t'))
		(*i)++;
	return (n);
}

/*
	rgb değerlerini okuması gerekir
	i = 1 çünkü f ve c karakterlerinden sonraki boşlukları atlamak için
	get val ile r g b değerlerini alır
	her değerin 0-255 arasında olup olmadığını kontrol eder

	*c = (r << 16) | (g << 8) | b;
	bu satır bilgisayar renkleri ile tek bir tamsayı int olarak saklar
	kırmızı 16 bit sola kaydırılır
	yeşil 8 bit sola kaydırılır
	mavi olduğu gibi kalır
	| or operatörü ile bunları birleştiririz sonuçta 0xRRGGBB formatında tek bir sayı oluşur.
*/
static int	get_rgb(char *s, int *c)
{
	int r;
	int g;
	int b;
	int i;

	i = 1;
	r = get_val(s, &i);
	if (s[i] == ',')
		i++;
	g = get_val(s, &i);
	if (s[i] == ',')
		i++;
	b = get_val(s, &i);
	if (r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255)
		return (0);
	*c = (r << 16) | (g << 8) | b;
	return (1);
}

static int	chk_line(char *s, t_game *g)
{
	int i;

	i = 0;
	while (s[i] == ' ' || s[i] == '\t')
		i++;
	if (!s[i])
		return (1);
	if (!ft_strncmp(s + i, "NO", 2))
		return (get_path(s, &g->tex->n_path, i + 2));
	if (!ft_strncmp(s + i, "SO", 2))
		return (get_path(s, &g->tex->s_path, i + 2));
	if (!ft_strncmp(s + i, "WE", 2))
		return (get_path(s, &g->tex->w_path, i + 2));
	if (!ft_strncmp(s + i, "EA", 2))
		return (get_path(s, &g->tex->e_path, i + 2));
	if (!ft_strncmp(s + i, "F", 1))
		return (get_rgb(s + i, &g->map->f_col));
	if (!ft_strncmp(s + i, "C", 1))
		return (get_rgb(s + i, &g->map->c_col));
	return (1);
}

int	read_config(t_game *g, char *f)
{
	int fd;
	char *s;
	int ok;

	fd = open(f, O_RDONLY);
	if (fd < 0)
		return (0);
	ok = 1;
	while (ok)
	{
		s = get_next_line(fd);
		if (!s)
			break ;
		if (is_map_line(s))
		{
			free(s);
			break ;
		}
		if (s[0] != '\n')
			ok = chk_line(s, g);
		free(s);
	}
	clear_gnl_fd(fd);
	close(fd);
	return (ok);
}

/*
	NO SO WE EA ile başlayan satırları bulup duvar dokularının dosya yollarını kaydeder
	F zemin ve C tavan satırlarını okur ve rgb değerlerini alır 
	is_map_line fonksiyonu ile haritada olabilecek satırları atlar
	chk_line fonksiyonu ile haritanın diğer satırlarını kontrol eder
*/