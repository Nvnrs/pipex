/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   processes.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nveneros <nveneros@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 11:38:54 by nveneros          #+#    #+#             */
/*   Updated: 2025/02/12 13:15:29 by nveneros         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	run_cmd(char *cmd, char **envp)
{
	char	**split_cmd;
	char	*path_cmd;

	split_cmd = ft_split(cmd, ' ');
	if (len_split(split_cmd) == 0)
	{
		free(split_cmd);
		exit(EXIT_FAILURE);
	}
	path_cmd = get_path_cmd(split_cmd, envp);
	if (path_cmd == NULL)
	{
		ft_putstr_fd(split_cmd[0], 2);
		ft_putstr_fd(": command not found", 2);
		ft_putstr_fd("\n", 2);
		free_split(split_cmd);
		exit(EXIT_FAILURE);
	}
	if (execve(path_cmd, split_cmd, envp) == -1)
	{
		free(path_cmd);
		free_split(split_cmd);
		exit(EXIT_FAILURE);
	}
}

void	first_process_child(char *path_infile, char *cmd,
	int *pipe_fd, char **envp)
{
	int	infile;

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

void	last_process_child(char *path_outfile, char *cmd,
	int *pipe_fd, char **envp)
{
	int	file;

	file = open(path_outfile, O_CREAT | O_WRONLY | O_TRUNC, 0666);
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
