/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iabboudi <iabboudi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 17:05:45 by stakhtou          #+#    #+#             */
/*   Updated: 2024/11/16 23:28:25 by iabboudi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int check_for_cmd(char ***cmd)
{
	while (*cmd && (*cmd)[0] && (*cmd)[0][0] == '\0')
		(*cmd)++;
	if (!*cmd || !(*cmd)[0])
		return 0;
	return 1;
}

void set_and_check(pid_t pid, int status)
{
	waitpid(pid, &status, 0);
	handle_exit_status(status);
}

void	execute_cmd(char **cmd)
{
	pid_t	pid;
	char	*path;
	int		status;

	if (!check_for_cmd(&cmd))
		return ;
	path = get_path(cmd);
	if (path == NULL)
	{
		printf("minishell: %s: command not found\n", cmd[0]);
		g_vars.exit_status = 127;
		return ;
	}
	pipe_signals();
	pid = fork();
	if (pid == 0)
	{
		setup_child_signals();
		execute_child(cmd, path);
	}
	else
		set_and_check(pid, status);
	all_signals();
	free(path);
}

void	exec(t_command *cmd)
{
	char	**args;
	int		i;

	i = 0;
	args = malloc(sizeof(char *) * (double_pointer_len(cmd->args) + 1));
	if (!args)
		return ;
	while (cmd->args[i])
	{
		args[i] = ft_strdup(cmd->args[i]);
		i++;
	}
	args[i] = NULL;
	execute_cmd(args);
	ft_free(args);
}

void	execute_single_cmd(t_command *cmd, char **env)
{
	int	cmd_type;
   
	all_signals();
	cmd_type = is_builtin(cmd);
	if (cmd->redirections)
		ft_redict(cmd, env);
	else
	{
		if (cmd_type == -1)
			exec(cmd);
		else
			execute_builtin(cmd, env, cmd_type);	
	}
}

void	setup_child_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	signal(SIGTSTP, SIG_DFL);
}

void	setup_redirections(t_command *cmd, int in_fd, int out_fd)
{
	int	red_in;
	int	red_out;

	red_in = get_in(cmd->redirections, in_fd);
	red_out = get_out(cmd, out_fd);
	if (red_in != STDIN_FILENO)
	{
		dup2(red_in, STDIN_FILENO);
		close(red_in);
	}
	else if (in_fd != STDIN_FILENO)
	{
		dup2(in_fd, STDIN_FILENO);
		close(in_fd);
	}
	if (red_out != STDOUT_FILENO)
	{
		dup2(red_out, STDOUT_FILENO);
		close(red_out);
	}
	else if (out_fd != STDOUT_FILENO)
	{
		dup2(out_fd, STDOUT_FILENO);
		close(out_fd);
	}
}
