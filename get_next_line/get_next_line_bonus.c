/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naanapa <naanapa@student.42kocaeli.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 12:32:03 by naanapa           #+#    #+#             */
/*   Updated: 2024/03/05 12:32:17 by naanapa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"
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

static int	have_nl(char *str)
{
	size_t	i;

	if (!str)
		return (0);
	i = -1;
	while (str[++i] != '\0')
		if (str[i] == '\n')
			return (1);
	return (0);
}

static char	*extract_line(char *stash)
{
	char	*line;
	size_t	i;
	size_t	j;

	i = 0;
	if (!stash)
		return (free_stash(&stash, 0));
	while (stash[i] != '\n')
		i++;
	line = malloc(sizeof(char) * (i + 2));
	if (!line)
		return (free_stash(&line, 0));
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
	long		ret;
	static char	*stash [NUM_OF_FD];
	char		*line;

	line = 0;
	ret = BUFFER_SIZE;
	if (fd < 0 || BUFFER_SIZE <= 0)
		return (free_stash(&stash[fd], 0));
	while (ret > 0)
	{
		ret = read(fd, buf, BUFFER_SIZE);
		if ((ret <= 0 && !stash[fd]) || ret == -1)
			return (free_stash(&stash[fd], 0));
		buf[ret] = '\0';
		stash[fd] = copy_to_stash(stash[fd], buf);
		if (have_nl(stash[fd]))
		{
			line = extract_line(stash[fd]);
			if (!line)
				return (free_stash(&stash[fd], 0));
			return (stash[fd] = recreate_stash(stash[fd]), line);
		}
	}
	return (free_stash(&stash[fd], 1));
}
