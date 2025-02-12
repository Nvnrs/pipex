/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nveneros <nveneros@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 13:31:17 by nveneros          #+#    #+#             */
/*   Updated: 2025/02/12 10:45:12 by nveneros         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"


void print_2d_tab(char **tab)
{
	int i;

	i = 0;
	while (tab[i])
	{
		dprintf(2, "%s\n", tab[i]);
		i++;
	}
	ft_printf("\n");
}


char *get_var_path_in_env(char **envp)
{
	int i;

	i = 0;
	while (envp[i])
	{
		if (strncmp(envp[i], "PATH=", 5) == 0)
			break;
		i++;
	}
	// ft_printf("%s", envp[i]);
	return (envp[i]);
}





char *tests_path_for_find_cmd(char *cmd, char **tests_path)
{
	int		i;
	char	*path_cmd;
	char	*path_with_slash;
	

	i = 1;
	while (tests_path[i])
	{
		path_with_slash = ft_strjoin(tests_path[i], "/");
		path_cmd = ft_strjoin(path_with_slash, cmd);
		if (access(path_cmd, F_OK) == 0)
		{
			// ft_printf("%s\n", path_cmd);
			free(path_with_slash);
			return (path_cmd);
		}
		free(path_with_slash);
		free(path_cmd);
		i++;
	}
	return (NULL);
}

void	free_split(char **tab)
{
	int i;

	i = 0;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
}


char *skip_name_of_var(char *first_path)
{
	char *new_path_skip;

	int i;

	i = 0;
	while (first_path[i] != '/')
		i++;
	new_path_skip = ft_strdup(&first_path[i]);
	free(first_path);
	return (new_path_skip);
}

char **get_paths_in_path_env(char *path_env)
{
	char	**paths_in_path_env;

	paths_in_path_env = ft_split(path_env, ':');
	paths_in_path_env[0] = skip_name_of_var(paths_in_path_env[0]);
	// print_2d_tab(paths_in_path_env);
	return (paths_in_path_env);
}

int	len_split(char **tab)
{
	int i;

	i = 0;
	while (tab[i])
		i++;
	return (i);
}

char *get_path_cmd(char **split_cmd, char **envp)
{
	char *path_env;
	char **paths_in_path_env;
	char *path_cmd;
	
	path_cmd = split_cmd[0];
	if (str_contain_c(path_cmd, '/'))
		return (ft_strdup(path_cmd));
	if (len_split(envp) == 0)
		return (NULL);
	path_env = get_var_path_in_env(envp);
	paths_in_path_env = get_paths_in_path_env(path_env);
	path_cmd = tests_path_for_find_cmd(path_cmd, paths_in_path_env);
	free_split(paths_in_path_env);
	return (path_cmd);
}



void	run_cmd(char *cmd, char **envp)
{
	char **split_cmd;
	char *path_cmd;

	split_cmd =  ft_split(cmd, ' ');
	if (len_split(split_cmd) == 0)
	{
		free(split_cmd);
		exit(EXIT_FAILURE);
	}
	path_cmd = get_path_cmd(split_cmd, envp);
	if (path_cmd == NULL)
	{
		ft_putstr_fd("command not found :", 2);
		ft_putstr_fd(split_cmd[0], 2);
		ft_putstr_fd("\n", 2);
		free_split(split_cmd);
		exit(EXIT_FAILURE);
	}
	if (execve(path_cmd, split_cmd, envp) == -1)
	{
		perror(path_cmd);
		free(path_cmd);
		free_split(split_cmd);
		exit(EXIT_FAILURE);
	}
}

void	first_process_child(char *path_infile, char *cmd, int *pipe_fd, char **envp)
{
	int infile;

	infile = open(path_infile, O_RDONLY);
	if (infile < 0)
	{
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		perror(path_infile);
		exit(EXIT_FAILURE);
	}
	close(pipe_fd[0]);
	dup2(infile, STDIN_FILENO);
	dup2(pipe_fd[1], STDOUT_FILENO);
	close(infile);
	close(pipe_fd[1]);
	run_cmd(cmd, envp);
}

void	last_process_child(char *path_outfile, char *cmd, int *pipe_fd, char **envp)
{
	int file;

	file = open(path_outfile, O_CREAT| O_WRONLY | O_TRUNC, 0666);
	if (file < 0)
	{
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		perror(path_outfile);
		exit(EXIT_FAILURE);
	}
	close(pipe_fd[1]);
	dup2(pipe_fd[0], STDIN_FILENO);
	dup2(file, STDOUT_FILENO);
	close(file);
	close(pipe_fd[0]);
	run_cmd(cmd, envp);
}

int	main(int argc, char **argv, char **envp)
{
	int pid1;
	int pid2;
	int fd1[2];

	if (argc != 5 || argv == NULL)
		return (ft_putstr_fd("Format is : infile cmd1 cmd2 outfile\n", 2), EXIT_FAILURE);
	if (pipe(fd1) == -1)
		return perror(NULL), (EXIT_FAILURE);
	pid1 = fork();
	if (pid1 == 0)
		first_process_child(argv[1], argv[2], fd1, envp);
	pid2 = fork();
	if (pid2 == 0)
		last_process_child(argv[4], argv[3], fd1, envp);
	close(fd1[1]);
	close(fd1[0]);
	waitpid(pid1, NULL, 0);
	waitpid(pid2, NULL, 0);
	return (EXIT_SUCCESS);
}

