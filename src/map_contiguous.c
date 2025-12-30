#include "../Cub3D.h"

/*
	bir harita dosyasında harita karakterleri 1, 0, N, vs. bir bütün halinde olmalıdır.
	Eğer haritanın ortasında boş bir satır bırakılırsa oyun motoru bunu iki ayrı ada gibi algılar.
	Oyuncu boşlukta yürüyebilir. Bu fonksiyon haritanın tek parça olup olmadığını kontrol eder.
	Haritanın bütünlüğünü kontrol eder.
	eğer harita ortasında boşluk varsa 0 döner.
	eğer boşluktan sonra tekrar harita satırı görürse bu harita ikiye bölünmüştür hata döner
*/
static int	is_blank_line(char *s)
{
	int i;

	if (!s)
		return (1);
	i = 0;
	while (s[i] && (s[i] == ' ' || s[i] == '\t'))
		i++;
	return (s[i] == '\0' || s[i] == '\n');
}

static void	print_line_debug(char *s, int line_no)
{
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
}

static int	process_line(char *s, int *started, int *gap)
{
	if (is_map_line(s))
	{
		if (*gap)
		{
			ft_putendl_fd("Gap before map line", 2);
			return (0);
		}
		ft_putendl_fd("  -> MAP LINE, started=1", 2);
		*started = 1;
	}
	else if (*started && is_blank_line(s))
	{
		ft_putendl_fd("  -> BLANK after map start, gap=1", 2);
		*gap = 1;
	}
	return (1);
}

static int	check_loop(int fd, int *started, int *gap)
{
	char	*s;
	int		line_no;

	line_no = 0;
	while (1)
	{
		s = get_next_line(fd);
		if (!s)
			break ;
		line_no++;
		print_line_debug(s, line_no);
		if (!process_line(s, started, gap))
		{
			free(s);
			return (0);
		}
		free(s);
	}
	return (1);
}
/*
	get ile dosyayi okur
	harita icerisinde bosluk var mi kontrol ediyor icerigi uygunmu diye
	process line ile
	gap 1 ise harita modundayken bos satir geldi
	dosya biterse veya baska config 
*/

int	contiguous_map_block(char *f)
{
	int	fd;
	int	started;
	int	gap;
	int	result;

	fd = open(f, O_RDONLY);
	if (fd < 0)
		return (0);
	started = 0;
	gap = 0;
	result = check_loop(fd, &started, &gap);
	clear_gnl_fd(fd);
	close(fd);
	return (result);
}
/* contiguous_map_block 

	Bu fonksiyon harita butunlugunu kontrol eder
	Harita icersinde bosluk var mi yokmu kontrol eder

*/
