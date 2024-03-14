/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naanapa <naanapa@student.42kocaeli.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 12:29:24 by naanapa           #+#    #+#             */
/*   Updated: 2024/03/05 13:59:19 by naanapa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <unistd.h>

static char	*copy_to_stash(char *stash, char *buff)
{
	char	*res;

	res = 0;
	if (!stash && buff)
	{
		res = ft_strdup(buff);
		if (!res)
			return (NULL);
		return (res);
	}
	res = ft_strjoin(stash, buff);
	free_stash(&stash, 0);
	return (res);
}

static int	have_nl(char *s)
{
	size_t	i;

	if (!s)
		return (0);
	i = -1;
	while (s[++i] != '\0')
		if (s[i] == '\n')
			return (1);
	return (0);
}

static char	*extract_line(char *stash)
{
	char	*line;
	size_t	j;

	i = 0;
	if (!stash)
		return (0);
	while (stash[i] != '\n')
		i++;
	line = malloc(sizeof(char) * (i + 2));
	if (!line)
		return (0);
	j = 0;
	while (j < i + 1)
	{
		line[j] = stash[j];
		j++;
	}
	line[j] = '\0';
	return (line);
}

static char	*recreate_stash(char *stash)
{
	size_t	i;
	char	*res;

	i = 0;
	if (!stash)
		return (NULL);
	while (stash[i] != '\n')
		i++;
	if (stash[i + 1] == '\0')
		return (free_stash(&stash, 0));
	res = ft_substr(stash, i + 1, ft_strlen(stash));
	if (!res)
	{
		free_stash(&stash, 0);
		return (NULL);
	}
	free_stash(&stash, 0);
	return (res);
}

char	*get_next_line(int fd)
{
	char		buf[BUFFER_SIZE + 1];
	long		bytes_read;
	static char	*stash = NULL;
	char		*line;

	line = 0;
	bytes_read = BUFFER_SIZE;
	if (fd < 0 || BUFFER_SIZE <= 0)
		return (free_stash(&stash, 0));
	while (bytes_read > 0)
	{
		bytes_read = read(fd, buf, BUFFER_SIZE);
		if ((bytes_read <= 0 && !stash) || bytes_read == -1)
			return (free_stash(&stash, 0));
		buf[bytes_read] = '\0';
		stash = copy_to_stash(stash, buf);
		if (have_nl(stash))
		{
			line = extract_line(stash);
			if (!line)
				return (free_stash(&stash, 0));
			return (stash = recreate_stash(stash), line);
		}
	}
	return (free_stash(&stash, 1));
}
