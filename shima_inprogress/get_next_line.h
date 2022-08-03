/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shabibol <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/10 22:11:06 by shabibol          #+#    #+#             */
/*   Updated: 2022/04/13 18:19:27 by shabibol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <fcntl.h>

int		ft_strlen(const char *s1);
int		ft_gnl_strchr(char *s1);

char	*ft_strndup(char *src, int size);
char	*ft_strjoin(const char *s1, const char *s2);
char	*get_next_line(int fd);

void	ft_update_buffer(char *buffer, int size);
void	ft_update_line(char *line, int size);
#endif
