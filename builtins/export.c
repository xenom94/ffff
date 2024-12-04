/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iabboudi <iabboudi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 17:05:45 by stakhtou          #+#    #+#             */
/*   Updated: 2024/12/04 20:13:30 by iabboudi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	export_helper(char *cmd, char ***env, int len)
{
	int		x;
	char	*new_var;
	int		i;

	i = 0;
	x = check_env(cmd, *env);
	if (!x)
	{
		new_var = ft_strdup(cmd);
		printf("here 0\n");
		add_to_env(env, new_var);
		free(new_var);
	}
	else if (x)
	{	
			printf("here 1\n");
		free((*env)[x]);
		(*env)[x] = ft_strdup(cmd);
	}
	while(cmd[i])
	{
			printf("here 2\n");
	if (cmd[i] == '+' && cmd[i + 1] == '=')
		{
			append_export(cmd, env, len);
			return ;
		}
		i++;
	}

}	

int	check_export(char *cmd)
{
	int	i;

	if (!ft_isalpha(cmd[0]) && cmd[0] != '_')
		return (ft_putstr_fd("Minishell: not a valid identifier\n", 2),
			g_vars.exit_status = 1, 0);
	i = 0;
	while (cmd[i] && cmd[i] != '=')
	{
		if (cmd[i] == '+' && cmd[i + 1] == '=')
		{
			if (i == 0)
				return (ft_putstr_fd("Minishell: not a valid identifier\n", 2),
					g_vars.exit_status = 1, 0);
			break ;
		}
		if (!ft_isalnum(cmd[i]) && cmd[i] != '_' && cmd[i] != '+')
			return (ft_putstr_fd("Minishell: not a valid identifier\n", 2),
				g_vars.exit_status = 1, 0);
		if (cmd[i] == '+' && cmd[i + 1] != '=')
			return (ft_putstr_fd("Minishell: export: not a valid identifier\n",
					2), g_vars.exit_status = 1, 0);
		i++;
	}
	if (i == 0)
		return (ft_putstr_fd("Minishell: not a valid identifier\n", 2),
			g_vars.exit_status = 1, 0);
	return (g_vars.exit_status = 0, 1);
}