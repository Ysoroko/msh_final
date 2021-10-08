/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ablondel <ablondel@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/01 18:40:43 by ablondel          #+#    #+#             */
/*   Updated: 2021/10/08 04:51:36 by ablondel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	len(char *s)
{
	int	i;

	i = 0;
	if (!s)
		return (0);
	while (s[i])
		i++;
	return (i);
}

int	main(int ac, char **av, char **env)
{
	char	buf[1024];

	(void)ac;
	(void)av;
	(void)env;
	getcwd(buf, 1024);
	printf("%s\n", buf);
	return (0);
}