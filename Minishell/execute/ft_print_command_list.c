/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_command_list.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ablondel <ablondel@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/25 14:41:39 by ysoroko           #+#    #+#             */
/*   Updated: 2021/09/07 17:20:14 by ablondel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/*
** char	*ft_role_color(int m)
** This function is used to determine the color for printing
** based on the corresponding value in macros_tab in our t_command structure
** Returns the string containing the color
*/

static char	*ft_role_color(int m)
{
	if (m == ERROR)
		return (BOLDRED);
	else if (m == COMMAND)
		return (BOLDGREEN);
	else if (m == FLAG)
		return (BOLDWHITE);
	else if (m == COMMAND_ARG)
		return (BOLDCYAN);
	else if (m == REDIR_L || m == REDIR_LL || m == REDIR_R || m == REDIR_RR)
		return (BOLDMAGENTA);
	else if (m == REDIR_ARG || m == R_REDIR_ARG || m == RR_REDIR_ARG
		|| m == L_REDIR_ARG || m == LL_REDIR_ARG)
		return (BOLDBLUE);
	else if (m == PIPE)
		return (BOLDYELLOW);
	else
		return (BOLDRED);
}

/*
** char	*ft_role_str(int m)
** This function is used to determine the string for printing
** based on the corresponding value in macros_tab in our t_command structure
** Returns the corresponding string
*/

static char	*ft_role_str(int m)
{
	const char	*tab[] = {"ERROR", "REDIR_R", "REDIR_RR", "REDIR_L",
					"REDIR_LL", "REDIR_ARG", "PIPE", "COMMAND", "FLAG",
					"COMMAND_ARG", "R_REDIR_ARG", "L_REDIR_ARG",
					"RR_REDIR_ARG", "LL_REDIR_ARG"};

	if (m > ft_str_tab_len((char **)tab) || m < 0)
		return ("Undefined");
	else
		return ((char *)(tab[m]));
}

/*
** void	ft_print_tab_header(int	s)
** This function simply prints the header of the tab which will be like:
** ____________________________________________________________
** i        | Str_tab_all      | Macros tab       | Execve_tab 
** ____________________________________________________________
*/

static void	ft_print_tab_header(int	s)
{
	printf(BOLDWHITE);
	ft_print_line_of_chars('_', LINE_LENGTH);
	printf("\n%*s | %*s | %*s | %*s\n", PLACE_FOR_I, "i", s, "str_tab_all", s,
		"role_macros", s, "str_tab_for_execve");
	ft_print_line_of_chars('_', LINE_LENGTH);
	printf("\n");
	printf(COLOR_RESET);
}

/*
** char	*ft_print_execve_tab(char **tab, int i)
** This function allows us to print execve_tab next to str_tab_all and macros
** despite execve_tab usually being shorter than those 2.
** It simply returns the execve_tab[i] element if element i exists
** or an empty string otherwise
*/

static char	*ft_print_execve_element(char **tab, int i)
{
	int	len;

	len = ft_str_tab_len(tab);
	if (i >= len)
		return ("");
	else
		return (tab[i]);
}


int		ft_check_file_permissions(char *filename)
{
	struct stat	sb;

	if (!filename)
		return (-1);
	if (stat(filename, &sb) == 0)
	{
		if (sb.st_mode & S_IXUSR)
			printf("File {%s} exists and is executable\n", filename);
		if (sb.st_mode & S_IRUSR)
			printf("File {%s} exists and is readable\n", filename);
		if (sb.st_mode & S_IWUSR)
			printf("File {%s} exists and is writable\n", filename);
		return (0);
	}
	printf("File {%s} does not exists.\n", filename);
	return (-1);
}

/*void	ft_free_buffer(t_command *cmd)
{
	int	i;

	i = 0;
	if (!cmd->buffer)
		return ;
	while (cmd->buffer[i])
		i++;
	while (i >= 0)
	{
		free(cmd->buffer[i]);
		i--;
	}
	cmd->buffer = NULL;
	free(cmd->buffer);
}*/

int	ft_strcmp_end(char *s1, char *s2)
{
	int i;
	int j;

	i = ft_strlen(s1) - 1;
	j = ft_strlen(s2) - 1;
	while (s1[i] == s2[j])
	{
		if (i == 0)
			return (0);
		i--;
		j--;
	}
	return (-1);
}

void	ft_load_hdoc_buffer(t_command *cmd)
{
	char	*tmp;
	int 	ret;

	printf("keyword = %s\n", cmd->keyword[cmd->keyword_index]);
	while (ret > 0)
	{
		write(1, "> ", 2);
		printf("buffer content = %s\n", cmd->buffer);
		printf("cmp = %d\n", ft_strncmp(cmd->keyword[cmd->keyword_index], cmd->buffer, ft_strlen(cmd->buffer) - 1));
		ret += read(STDIN_FILENO, cmd->buffer, 1024);
		cmd->buffer[ret] = '\0';
		if (ft_strncmp(cmd->keyword[cmd->keyword_index], cmd->buffer, ft_strlen(cmd->buffer) - 1) == 0 || ret == 0)
		{
			//cmd->buffer[ret] = '\0';
			return ;
		}
	}
	return ;
	//while (1)
	//{
	//	write(1, "> ", 2);
	//	ret = read(0, cmd->buffer, 4096);
	//	//cmd->buffer[ret] = '\0';
	//	printf("[%d]---{%s}\n", ret, cmd->buffer);
	//	if (strcmp(cmd->buffer, "exit\n") == 0)
	//		exit(0);
	//	//cmd->buffer_index++;
	//}
	////cmd->buffer[cmd->buffer_index] = 0;
}

void	ft_add_redir_file(t_command *cmd, int m, int i)
{
	if (m == REDIR_R || m == REDIR_RR)
	{
		if (cmd->outfile)
			free(cmd->outfile);
		cmd->outfile = ft_strdup(cmd->str_tab_all[i + 1]);
		if (!cmd->outfile)
		{
			printf("Failed to copy outfile name.\n");
			exit(EXIT_FAILURE);
		}
		if (m == REDIR_R)
			cmd->redir_type_out = 1;
		else
			cmd->redir_type_out = 2;
	}
	else if (m == REDIR_L || m == REDIR_LL)
	{
		if (m == REDIR_L)
		{
			cmd->redir_type_in = 3;
			if (cmd->infile)
				free(cmd->infile);
			cmd->infile = ft_strdup(cmd->str_tab_all[i + 1]);
			if (!cmd->infile)
			{
				printf("Failed to copy infile name.\n");
				exit(EXIT_FAILURE);
			}
		}
		else if (m == REDIR_LL)
		{
			cmd->redir_type_in = 4;
			if (cmd->keyword[cmd->keyword_index])
				free(cmd->keyword[cmd->keyword_index]);
			cmd->keyword[cmd->keyword_index] = ft_strdup(cmd->str_tab_all[i + 1]);
			if (!cmd->keyword[cmd->keyword_index])
			{
				printf("Failed to copy keyword.\n");
				exit(EXIT_FAILURE);
			}
			ft_load_hdoc_buffer(cmd);
			cmd->keyword_index++;
			cmd->keyword[cmd->keyword_index] = 0;
		}
	}
	ft_check_file_permissions(cmd->infile);
	ft_check_file_permissions(cmd->outfile);
	return ;
}

void	ft_print_tab(char **tab)
{
	int i = 0;

	if (!tab)
		printf("NO DATA\n");
	else
		while (tab[i])
			printf("___|%s|___\n", tab[i++]);
	printf("\n");
}

/*
** FT_PRINT_COMMAND_LIST
** A debugging function used to print the list of our commands and related
** flags/arguments/redirections to make sure everything is running smoothly
*/

void	ft_print_command_list(void *current_command)
{
	t_command	*command;
	int			s;
	char		*str;
	int			m;
	int			i;

	s = -18;
	i = -1;
	printf("\n");
	command = (t_command *)(current_command);
	ft_print_tab_header(s);
	command->keyword = (char**)malloc(sizeof(char*) * 1024);
	command->keyword_index = 0;
	
	while (command->str_tab_all[++i])
	{
		str = (command->str_tab_all)[i];
		m = (command->role_macros)[i];
		printf("%*d | %*s | %s%*d: %*s%s | %*s\n", PLACE_FOR_I, i, s, str,
		ft_role_color(m), -2, m, s + 4, ft_role_str(m), COLOR_RESET, s,
			ft_print_execve_element(command->str_tab_for_execve, i));
		if (m >= 1 && m <= 4)
		{
			ft_add_redir_file(command, m, i);
		}
	}
	printf("[REDIRECTIONS]\n");
	printf("FDIN		|%s|\n", command->infile);
	printf("FDOUT		|%s|\n", command->outfile);
	printf("KEYWORD		|\n");
	ft_print_tab(command->keyword);
	printf("BUFFER		|%s|\n", command->buffer);
	printf("RTYPEIN		|%d|\n", command->redir_type_in);
	printf("RTYPEOUT	|%d|\n\n", command->redir_type_out);
	printf("[EXECVE]\n");
	ft_print_tab(command->str_tab_for_execve);
	ft_print_line_of_chars('_', LINE_LENGTH);
	printf("\n\n\n");
	//char buffer[12] = "1\n2\n3\n";
	char *arg[12] = {"/usr/bin/wc", "-l", NULL};
	//char buf[10];
	//int ret = 0;
	pid_t p1 = fork();
	if (p1 == 0)
	{
		//write(1, "> ", 2);
		//while (read(STDIN_FILENO, buf, 1024) > 0)
		//{
		//	write(1, "> ", 2);
		//	buf[ret] = '\0';
		//}
		execve(arg[0], arg, NULL);
	}
	wait(NULL);
}
