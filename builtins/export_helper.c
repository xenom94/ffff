/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_helper.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iabboudi <iabboudi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 17:05:45 by stakhtou          #+#    #+#             */
/*   Updated: 2024/12/04 20:54:46 by iabboudi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	print_export(char *env)
{
	int	i;

	i = 0;
	printf("declare -x ");
	if (ft_strchr(env, '='))
	{
		while (env[i] != '=')
		{
			printf("%c", env[i++]);
		}
		printf("=\"");
		i++;
		printf("%s\"\n", (env + i));
	}
	else
		printf("%s\n", env);
}

int index_at(const char * str, char c){
	int i = 0;
	
	while (str[i]){
		if (str[i] == c)
            return i;
        i++;
	}
	return -1;
}

int	check_env(char *cmd, char **env)
{
	int	i;
	char *env_key;
	
	i = 0;
	char *key = ft_substr(cmd,0, index_at(cmd,	'='));
	while (env[i])
	{
		env_key = ft_substr(env[i],0, index_at(env[i],'='));
		if (!ft_strcmp(env_key, key))
		{
			free(key);
			free(env[i]);
			free(env_key);
			env[i] = ft_strdup(cmd);
			return (i);
		}
		else
			i++;
		free(env_key);
	}
	free(key);
	return (0);
}

void	add_to_envp(char ***env, char *new_var)
{
	int		len;
	char	**new_env;
	int		i;

	len = double_pointer_len(*env);
	new_env = malloc(sizeof(char *) * (len + 2));
	if (new_env)
	{
		i = 0;
		while (i < len)
		{
			new_env[i] = (*env)[i];
			i++;
		}
		new_env[i] = ft_strdup(new_var);
		new_env[i + 1] = NULL;
		*env = new_env;
	}
}