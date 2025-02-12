/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nveneros <nveneros@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 13:31:17 by nveneros          #+#    #+#             */
/*   Updated: 2025/02/12 13:18:06 by nveneros         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	main(int argc, char **argv, char **envp)
{
	int	pid1;
	int	pid2;
	int	fd1[2];

	if (argc != 5 || argv == NULL)
		return (ft_putstr_fd("Format is : infile cmd1 cmd2 outfile\n", 2),
			EXIT_FAILURE);
	if (pipe(fd1) == -1)
		return (perror(NULL), EXIT_FAILURE);
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
