/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_exec.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obamzuro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/19 13:52:33 by obamzuro          #+#    #+#             */
/*   Updated: 2018/09/21 21:46:06 by obamzuro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"

static char		*ft_exec_path_find_comm(char **args, char **paths)
{
	int			i;
	struct stat	mystat;
	char		*temp;

	i = 0;
	while (paths[i])
	{
		temp = ft_strjoin_char(paths[i], args[0], '/');
		if (lstat(temp, &mystat) != -1)
		{
			if (access(temp, X_OK) == -1)
			{
				ft_fprintf(2, "21sh: Permission denied: %s\n", args[0]);
				free(temp);
				return (0);
			}
			else
				return (temp);
		}
		free(temp);
		++i;
	}
	ft_fprintf(2, "21sh: command not found: %s\n", args[0]);
	return (0);
}

static char		*ft_exec_path(char **args, char ***env)
{
	char		**paths;
	int			i;
	char		*ret;

	paths = ft_strsplit(get_env("PATH", *env), ':');
	i = 0;
	if (!paths)
	{
		ft_fprintf(2, "21sh: command not found: %s\n", args[0]);
		return (0);
	}
	ret = ft_exec_path_find_comm(args, paths);
	free_double_arr(paths);
	return (ret);
}

static int		ft_exec_check_err(char **args, char *comm)
{
	struct stat	tempstat;

	if (!comm)
		return (-1);
	if (lstat(comm, &tempstat) == -1)
	{
		ft_fprintf(2, "21sh: no such file or directory: %s\n", comm);
		if (comm != args[0])
			free(comm);
		return (-1);
	}
	if (!S_ISREG(tempstat.st_mode) || access(comm, X_OK) == -1)
	{
		if (S_ISDIR(tempstat.st_mode))
			ft_fprintf(2, "21sh: %s: is a directory\n", comm);
		else
			ft_fprintf(2, "21sh: Permission denied: %s\n", comm);
		if (comm != args[0])
			free(comm);
		return (-1);
	}
	return (0);
}

static int		ft_exec_fork(char **args, char ***env, char *comm)
{
	pid_t		process;

	process = fork();
	if (process == 0)
	{
		if (execve(comm, args, *env) == -1)
		{
			ft_fprintf(2, "21sh: File execution error: %s\n", comm);
			return (-1);
		}
	}
	else if (process < 0)
	{
		ft_fprintf(2, "21sh: Error creating a child thread\n");
		return (-1);
	}
	return (0);
}

int				ft_exec(char **args, char ***env, int forkneed)
{
	char		*comm;
	int			ret;

	if (!ft_strchr(args[0], '/') && 
			!(ft_is_str_in_args(args[0], 2, "..", ".")))
		comm = ft_exec_path(args, env);
	else
		comm = args[0];
	if (ft_exec_check_err(args, comm) == -1)
		return (-1);
	if (forkneed)
		ret = ft_exec_fork(args, env, comm);
	else if ((ret = execve(comm, args, *env) == -1))
		ft_fprintf(2, "21sh: File execution error: %s\n", comm);
	if (comm != args[0])
		free(comm);
	return (ret);
}
