/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nveneros <nveneros@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 13:32:19 by nveneros          #+#    #+#             */
/*   Updated: 2025/02/12 13:24:01 by nveneros         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "../lib/MYLIB42/mylib42.h"
# include <sys/stat.h>
# include <fcntl.h>
# include <sys/types.h>
# include <unistd.h>
# include <sys/wait.h>
# include <stdlib.h>
# include <stdio.h>
# include <string.h>

void	first_process_child(char *path_infile, char *cmd,
			int *pipe_fd, char **envp);
void	last_process_child(char *path_outfile, char *cmd,
			int *pipe_fd, char **envp);
char	*get_path_cmd(char **split_cmd, char **envp);
void	free_split(char **tab);
int		len_split(char **tab);

#endif