/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_exec_proc.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatmtahmdab       #+#    #+#             */
/*   Updated: 2025/11/10 12:02:00 by fatmtahmdab      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mandatory.h"

static int	collect_backups(t_node *n, int *fds)
{
	int	k;

	k = 0;
	if (n->stdin_backup > 2)
		fds[k++] = n->stdin_backup;
	if (n->stdout_backup > 2)
		fds[k++] = n->stdout_backup;
	if (n->stderr_backup > 2)
		fds[k++] = n->stderr_backup;
	if (n->backup_stdin > 2)
		fds[k++] = n->backup_stdin;
	if (n->backup_stdout > 2)
		fds[k++] = n->backup_stdout;
	return (k);
}

static void	reset_backups(t_node *n)
{
	n->stdin_backup = -1;
	n->stdout_backup = -1;
	n->stderr_backup = -1;
	n->backup_stdin = -1;
	n->backup_stdout = -1;
}

static void	close_unique(int *fds, int n)
{
	int	i;
	int	j;
	int	seen;

	i = 0;
	while (i < n)
	{
		seen = 0;
		j = 0;
		while (j < i)
		{
			if (fds[j] == fds[i])
				seen = 1;
			j++;
		}
		if (!seen)
			close(fds[i]);
		i++;
	}
}

void	close_child_backups(t_node *node)
{
	int	fds[5];
	int	n;

	n = collect_backups(node, fds);
	close_unique(fds, n);
	reset_backups(node);
}

void	exec_proc_loop(char **paths, char **args, char **envp, t_node *node)
{
	size_t	n;

	n = ft_strlen(paths[node->i]) + ft_strlen(args[0]) + 2;
	node->path = malloc(n);
	if (!(node->path))
	{
		strarrfree(paths);
		set_exit_status_n(node, EXIT_FAILURE);
		cleanup_child_and_exit(args, envp, node);
	}
	ft_strlcpy(node->path, paths[node->i], n);
	ft_strlcat(node->path, "/", n);
	ft_strlcat(node->path, args[0], n);
	exec_proc_loop2(paths, args, envp, node);
}
