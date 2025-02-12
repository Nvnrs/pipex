/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nveneros <nveneros@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 11:41:06 by nveneros          #+#    #+#             */
/*   Updated: 2025/02/12 13:14:06 by nveneros         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	*get_var_path_in_env(char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		if (strncmp(envp[i], "PATH=", 5) == 0)
			break ;
		i++;
	}
	return (envp[i]);
}

char	*tests_path_for_find_cmd(char *cmd, char **tests_path)
{
	int		i;
	char	*path_cmd;
	char	*path_with_slash;

	i = 1;
	while (tests_path[i])
	{
		path_with_slash = ft_strjoin(tests_path[i], "/");
		path_cmd = ft_strjoin(path_with_slash, cmd);
		if (access(path_cmd, F_OK) == 0 && (access(path_cmd, X_OK) == 0))
		{
			free(path_with_slash);
			return (path_cmd);
		}
		free(path_with_slash);
		free(path_cmd);
		i++;
	}
	return (NULL);
}

char	*skip_name_of_var(char *first_path)
{
	char	*new_path_skip;
	int		i;

	i = 0;
	while (first_path[i] != '/')
		i++;
	new_path_skip = ft_strdup(&first_path[i]);
	free(first_path);
	return (new_path_skip);
}

char	**get_paths_in_path_env(char *path_env)
{
	char	**paths_in_path_env;

	paths_in_path_env = ft_split(path_env, ':');
	paths_in_path_env[0] = skip_name_of_var(paths_in_path_env[0]);
	return (paths_in_path_env);
}

char	*get_path_cmd(char **split_cmd, char **envp)
{
	char	*path_env;
	char	**paths_in_path_env;
	char	*path_cmd;

	path_cmd = split_cmd[0];
	if (str_contain_c(path_cmd, '/'))
	{
		if (access(path_cmd, F_OK) == -1 || (access(path_cmd, X_OK) == -1))
		{
			perror(path_cmd);
			free_split(split_cmd);
			exit(EXIT_FAILURE);
		}
		return (ft_strdup(path_cmd));
	}
	if (len_split(envp) == 0)
		return (NULL);
	path_env = get_var_path_in_env(envp);
	paths_in_path_env = get_paths_in_path_env(path_env);
	path_cmd = tests_path_for_find_cmd(path_cmd, paths_in_path_env);
	free_split(paths_in_path_env);
	return (path_cmd);
}
