#include "../Cub3D.h"

#ifndef BUFFER_SIZE
# define BUFFER_SIZE 5
#endif

static char	*g_buffer[1024];

void	clear_gnl_fd(int fd)
{
	if (fd >= 0 && fd < 1024 && g_buffer[fd])
	{
		free(g_buffer[fd]);
		g_buffer[fd] = NULL;
	}
}

static char	*ft_strjoin_gnl(char *s1, char *s2)
{
	char	*result;
	int		i;
	int		j;

	if (!s1)
	{
		s1 = malloc(1);
		s1[0] = '\0';
	}
	result = malloc(ft_strlen(s1) + ft_strlen(s2) + 1);
	if (!result)
		return (NULL);
	i = 0;
	while (s1[i])
	{
		result[i] = s1[i];
		i++;
	}
	j = 0;
	while (s2[j])
		result[i++] = s2[j++];
	result[i] = '\0';
	free(s1);
	return (result);
}

static char	*extract_line(char *buffer)
{
	char	*line;
	int		i;

	if (!buffer)
		return (NULL);
	i = 0;
	if (!buffer[i])
		return (NULL);
	while (buffer[i] && buffer[i] != '\n')
		i++;
	line = malloc(i + 2);
	if (!line)
		return (NULL);
	i = 0;
	while (buffer[i] && buffer[i] != '\n')
	{
		line[i] = buffer[i];
		i++;
	}
	line[i] = '\0';
	if (buffer[i] == '\n')
		line[++i] = '\n';
	return (line);
}

static char	*update_buffer(char *buffer)
{
	char	*new_buffer;
	int		i;
	int		j;

	i = 0;
	while (buffer[i] && buffer[i] != '\n')
		i++;
	if (!buffer[i])
	{
		free(buffer);
		return (NULL);
	}
	new_buffer = malloc(ft_strlen(buffer) - i + 1);
	if (!new_buffer)
		return (NULL);
	i++;
	j = 0;
	while (buffer[i])
		new_buffer[j++] = buffer[i++];
	new_buffer[j] = '\0';
	free(buffer);
	return (new_buffer);
}

char	*get_next_line(int fd)
{
	char		*temp;
	char		*line;
	int			bytes_read;

	if (fd < 0 || BUFFER_SIZE <= 0 || fd >= 1024)
		return (NULL);
	temp = malloc(BUFFER_SIZE + 1);
	if (!temp)
		return (NULL);
	bytes_read = 1;
	while (bytes_read > 0 && (!g_buffer[fd] || !ft_strchr(g_buffer[fd], '\n')))
	{
		bytes_read = read(fd, temp, BUFFER_SIZE);
		if (bytes_read < 0)
		{
			free(temp);
			clear_gnl_fd(fd);
			return (NULL);
		}
		temp[bytes_read] = '\0';
		g_buffer[fd] = ft_strjoin_gnl(g_buffer[fd], temp);
	}
	free(temp);
	line = extract_line(g_buffer[fd]);
	g_buffer[fd] = update_buffer(g_buffer[fd]);
	if (!line)
		clear_gnl_fd(fd);
	return (line);
}
