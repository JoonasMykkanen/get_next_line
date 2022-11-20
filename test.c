#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifndef BUFFER_SIZE
# define BUFFER_SIZE 42
#endif


size_t	ft_strlen(char const *s);

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	size_t			i;
	unsigned char	*d;
	unsigned char	*s;

	d = (unsigned char *)dst;
	s = (unsigned char *)src;
	i = 0;
	if (d == NULL && s == NULL)
		return (d);
	while (i < n)
	{
		d[i] = s[i];
		i++;
	}
	return (dst);
}

void	*ft_memchr(const void *s, int c, size_t n)
{
	int				i;
	unsigned char	ch;
	void			*ptr;
	void			*src;

	i = 0;
	ch = c;
	src = (void *)s;
	while (n > 0)
	{
		if (*((unsigned char *)s + i) == ch)
		{
			ptr = &src[i];
			return (ptr);
		}
		n--;
		i++;
	}
	return (NULL);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	int		i;
	int		j;
	char	*str;

	str = (char *)malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 1));
	if (str == NULL)
		return (NULL);
	i = 0;
	j = 0;
	while (s1 != NULL && s1[j] != '\0')
	{
		str[i] = s1[j];
		i++;
		j++;
	}
	j = 0;
	while (s2 != NULL && s2[j] != '\0')
	{
		str[i] = s2[j];
		i++;
		j++;
	}
	str[i] = '\0';	
	return (str);
}

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	int				i;
	unsigned char	*str1;
	unsigned char	*str2;

	i = 0;
	str1 = (unsigned char *)s1;
	str2 = (unsigned char *)s2;
	if (!s1 || !s2)
		return (0);
	while (n > 0 && str1[i] != '\0' && str1[i] == str2[i])
	{
		n--;
		i++;
	}
	if (n == 0)
		return (0);
	return (str1[i] - str2[i]);
}

size_t	ft_strlen(char const *s)
{
	int	i;

	i = 0;
	if (s == NULL || !s)
	{
		return (0);
	}
	if (ft_strncmp(s, "", 1) == 0)
	{
		return (0);
	}
	while (s[i] != '\0')
		i++;
	return (i);
}
static struct s_static
{
	int		index;
	char	*buff;
}			s;

static void	handle_overflow(unsigned int ret, char *temp)
{
	unsigned int	len;
	char 			*storage;

	len = 0;
	s.index = 0;
	free(s.buff);
	s.buff = NULL;
	while (temp[len] != '\n' && len <= ret)
	{
		s.index += 1;
		len++;
	}
	s.index += 1;
	len++;
	len = ret - len;
	if (len == 0)
		return ;
	storage = malloc(sizeof(char) * (len + 1));
	ft_memcpy(storage, temp + s.index, len);
	storage[len] = '\0';
	s.buff = malloc(sizeof(char) * (len + 1));
	if (!s.buff || len < 1)
	{
		free(storage);
		return ;
	}
	ft_memcpy(s.buff, storage, len + 1);
	free(storage);
}

static char	*build_str(char *storage, char *temp, int ret, int *trigger)
{
	static int	index = 0;
	char		*buf;
	int			i;	

	i = -1;
	if (ft_strncmp(temp, "42", 2) == 0)
		return (ft_strjoin("", storage));
	buf = malloc(sizeof(char) * (index + ret + 2));
	if (!buf)
		return(NULL);
	if (storage)
		ft_memcpy(buf, storage, index);
	while (++i < ret)
	{
		buf[index + i] = temp[i];
		if (temp[i] == '\n')
		{
			buf[index + i + 1] = '\0';
			*trigger = 1;
			index = 0;
			return (buf);
		}
	}
	buf[index + i] = '\0';
	index += i;
	return (buf);
}

static char	*read_line(int fd, int ret, char *buf)
{
	char	*temp;
	int		trigger;
	char	*storage;

	trigger = 0;
	storage = NULL;
	ret = read(fd, buf, BUFFER_SIZE);
	if (ret == -1)
		return (NULL);
	while (ret)
	{
		temp = NULL;
		if (ft_strlen(storage) != 0)
			temp = build_str(storage, "42", ft_strlen(storage), &trigger);
		free(storage);
		storage = build_str(temp, buf, ret, &trigger);
		free(temp);
		if (trigger == 1)
		{
			handle_overflow(ret, buf);
			return (storage);
		}
		ret = read(fd, buf, BUFFER_SIZE);
	}
	return (storage);
}

char	*line_from_buff(int len)
{
	char	*line;
	char 	*temp;
	int		size;

	if (ft_memchr(s.buff, '\n', ft_strlen(s.buff)))
		while (s.buff[++len] != '\n');
	else
		len = ft_strlen(s.buff);
	size = ft_strlen(s.buff) - (size_t)len;
	line = malloc(sizeof(char) * (len + 2));
	ft_memcpy(line, s.buff, len + 1);
	line[len + 1] = '\0';
	if (size > len)
	{
		temp = malloc(sizeof(char) * (size + 1));
		ft_memcpy(temp, s.buff + len + 1, size);
		temp[size] = '\0';
		free(s.buff);
		s.buff = malloc(sizeof(char) * (size + 1));
		ft_memcpy(s.buff, temp, size + 1);
		free(temp);
		return (line);
	}
	free(s.buff);
	s.buff = NULL;
	return (line);
}

char	*get_next_line(int fd)
{
	char		buf[BUFFER_SIZE];
	char		*storage;
	char		*line;
	char		*temp;

	if (s.buff)
	{
		if (ft_memchr(s.buff, '\n', ft_strlen(s.buff)))
			return (line_from_buff(-1));
		line = line_from_buff(-1);
		storage = read_line(fd, 0, buf);
		if (storage == NULL && ft_strlen(line) < 1)
		{
			free(line);
			free(storage);
			return (NULL);
		}
		temp = ft_strjoin(line, storage);
		free(line);
		free(storage);
		return (temp);
	}
	return (read_line(fd, 0, buf));
}


int	main()
{
	int		fd;
	char	*file;
	char 	*line;

	file = "files/file.txt";
	fd = open(file, O_RDONLY);

	line = get_next_line(fd);
	printf("%s", line);
	free(line);
	fflush(stdout);
	while (line)
	{
		line = get_next_line(fd);
		printf("%s", line);
		free(line);
		fflush(stdout);
	}
	close(fd);
	return (0);
}