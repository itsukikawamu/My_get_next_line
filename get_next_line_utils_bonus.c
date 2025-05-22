/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikawamuk <ikawamuk@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 13:55:12 by ikawamuk          #+#    #+#             */
/*   Updated: 2025/05/19 23:35:54 by ikawamuk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

bool	expand_save(t_save *save, ssize_t rd_size)
{
	size_t	new_size;
	char	*new_save;
	size_t	i;

	new_size = save->buf_size;
	while ((size_t)rd_size > (new_size - save->str_len - 1))
	{
		if (new_size > SIZE_MAX / 2)
			return (false);
		new_size *= 2;
	}
	new_save = malloc(new_size);
	if (!new_save)
		return (false);
	i = 0;
	while (i < save->str_len + 1)
	{
		new_save[i] = save->data[i];
		i++;
	}
	save->buf_size = new_size;
	free(save->data);
	save->data = new_save;
	return (true);
}

t_list	*alloc_fd_node(int fd, t_list	*table[])
{
	int		hash;
	t_list	*cur;

	if (fd < 0 || !table)
		return (NULL);
	hash = fd % HASH_SIZE;
	cur = table[hash];
	while (cur && cur->fd != fd)
		cur = cur->next;
	if (!cur)
	{
		cur = malloc(sizeof(t_list));
		if (!cur)
			return (NULL);
		cur->fd = fd;
		cur->save.buf_size = (ssize_t)BUFFER_SIZE * 2;
		cur->save.data = malloc(cur->save.buf_size);
		if (!cur->save.data)
			return (free(cur), NULL);
		cur->save.data[0] = '\0';
		cur->save.str_len = 0;
		cur->next = table[hash];
		table[hash] = cur;
	}
	return (cur);
}

void	free_fd_node(int fd, t_list	*table[])
{
	int		hash;
	t_list	*cur;
	t_list	*prev;

	if (fd < 0 || !table)
		return ;
	hash = fd % HASH_SIZE;
	cur = table[hash];
	prev = NULL;
	while (cur)
	{
		if (cur->fd == fd)
		{
			if (prev == NULL)
				table[hash] = cur->next;
			else
				prev->next = cur->next;
			free(cur->save.data);
			free(cur);
			return ;
		}
		prev = cur;
		cur = cur->next;
	}
}

bool	has_new_line(char *str)
{
	if (!str)
		return (false);
	while (*str && *str != '\n')
		str++;
	if (*str == '\n')
		return (true);
	return (false);
}

ssize_t	ft_strlen(const char *s)
{
	ssize_t	len;

	if (!s)
		return (-1);
	len = 0;
	while (s[len])
		len++;
	return (len);
}
