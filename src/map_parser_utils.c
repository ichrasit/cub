#include "../Cub3D.h"

int	is_map_line(char *s)
{
	int i;

	if (!s)
		return (0);
	i = 0;
	while (s[i] == ' ' || s[i] == '\t')
		i++;
	return (s[i] == '1' || s[i] == '0');
}
