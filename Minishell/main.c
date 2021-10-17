/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysoroko <ysoroko@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/25 13:52:17 by ysoroko           #+#    #+#             */
/*   Updated: 2021/10/17 14:29:01 by ysoroko          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include/minishell.h"

/*
** FT_CLEANUP_AND_FREE
** This function is responsible for freeing everything that was malloc'd
** during a single user's input
** It frees the input saved as a string, and also the entire t_list and
** all of its content
*/

static void	ft_cleanup_and_free(char **str, t_dl_lst *lst)
{
	ft_free_str(str);
	ft_dl_lstclear(lst, &ft_free_t_command);
}

int	ft_user_input_error(char *str)
{
	if (!str)
		kill(getpid(), SIGUSR1);
	if (ft_str_only_has_chars_from_charset(str, SPACES))
		return (1);
	if (ft_str_has_unclosed_quotes(str))
	{
		ft_minishell_error("Unclosed quotes error");
		return (1);
	}
	return (0);
}

void	ft_prompt(void)
{
	char		*user_input_str;
	t_dl_lst	*input_as_dl_command_list;

	user_input_str = readline(PROMPT_NAME);
	if (ft_user_input_error(user_input_str))
		return ;
	add_history(user_input_str);
	input_as_dl_command_list = ft_input_parsing(user_input_str);
	ft_execute(input_as_dl_command_list);
	ft_cleanup_and_free(&user_input_str, input_as_dl_command_list);
}

// 1) variable SHLVL -> doit +1 quand on ouvre un nouveau 
//minishell(dans le notre), doit -1 quand on ferme le notre
// 2) mettre à jour $? dans export + toutes les fonctions
//		qu'on appelle avec exeve
// 3) "extern int	errno;" dans minishell.h ->non accepté par la norme
// 		(Global variable must start with g_)

int	main(int ac, char **av, char **env)
{
	char	origin[1024];
	char	*builtins_path;

	(void)ac;
	(void)av;
	builtins_path = NULL;
	ft_duplicate_env(env);
	getcwd(origin, 1024);
	g_glob.path = ft_strjoin_exit(origin, "/builtins/");
	g_glob.main_pid = getpid();
	g_glob.exit_status = 0;
	while (1)
	{
		g_glob.fork_ret = g_glob.main_pid;
		ft_setup_signals();
		ft_prompt();
	}
	return (1);
}
