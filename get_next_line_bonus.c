/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucavall <lucavall@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/19 14:10:17 by lucavall          #+#    #+#             */
/*   Updated: 2024/01/10 14:32:29 by lucavall         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

static char	*fd_reader(int fd)
{
	char	*line;
	int		chars_read;

	line = malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!line)
		return (NULL);
	chars_read = read(fd, line, BUFFER_SIZE);
	if (chars_read <= 0)
	{
		free(line);
		return (NULL);
	}
	line[chars_read] = '\0';
	return (line);
}

static char	*process_line(char **buffer)
{
	char	*line;
	char	*left_chars;
	int		len;

	len = 0;
	while (((*buffer)[len] != '\n') && ((*buffer)[len] != '\0'))
		len++;
	if ((*buffer)[len] == '\n')
	{
		line = ft_substr(*buffer, 0, len + 1);
		left_chars = ft_substr(*buffer, len +1, ft_strlen(*buffer) - (len +1));
		free(*buffer);
		*buffer = left_chars;
		return (line);
	}
	return (NULL);
}

static char	*get_next_line_helper(char **buffer, int fd)
{
	char	*tmp_buffer;
	char	*line_read;
	char	*line;

	line = process_line(buffer);
	if (line)
		return (line);
	line_read = fd_reader(fd);
	if (!line_read)
	{
		line = ft_strdup(*buffer);
		free(*buffer);
		*buffer = NULL;
		if (*line)
			return (line);
		free(line);
		return (NULL);
	}
	tmp_buffer = ft_strjoin(*buffer, line_read);
	free(*buffer);
	*buffer = tmp_buffer;
	free(line_read);
	return (get_next_line_helper(buffer, fd));
}

char	*get_next_line(int fd)
{
	static char	*buffer[MAX_FD];

	if (fd < 0 || fd >= MAX_FD || BUFFER_SIZE <= 0 || read(fd, NULL, 0) < 0)
	{
		free(buffer[fd]);
		buffer[fd] = NULL;
		return (NULL);
	}
	if (!buffer[fd])
		buffer[fd] = ft_strdup("");
	return (get_next_line_helper(&buffer[fd], fd));
}

/* #include <fcntl.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
	int fd[argc - 1];
	char *line;
	int i;

	if (argc < 2)
	{
		printf("Usage: %s <filename1> <filename2> ...\n", argv[0]);
		return (1);
	}
	for (i = 1; i < argc; i++)
	{
		fd[i - 1] = open(argv[i], O_RDONLY);
		if (fd[i - 1] == -1)
			return (1);
	}
	for (i = 0; i < argc - 1; i++)
	{
		printf("Reading file %s:\n", argv[i + 1]);
		line = get_next_line(fd[i]);
		while (line != NULL)
		{
			printf("%s\n", line);
			free(line);
			line = get_next_line(fd[i]);
		}
		close(fd[i]);
	}
	return (0);
} */
