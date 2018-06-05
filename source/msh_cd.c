/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_cd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obamzuro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/19 13:52:21 by obamzuro          #+#    #+#             */
/*   Updated: 2018/05/23 15:51:52 by obamzuro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int			check_dir_err(char *pathname)
{
	struct stat		mystat;

	if (stat(pathname, &mystat) == -1)
	{
		ft_fprintf(2, "cd: no such file or directory: %s\n", pathname);
		return (1);
	}
	else if (!S_ISDIR(mystat.st_mode))
	{
		ft_fprintf(2, "cd: not a directory: %s\n", pathname);
		return (1);
	}
	else if (access(pathname, X_OK) == -1)
	{
		ft_fprintf(2, "cd: permission denied: %s\n", pathname);
		return (1);
	}
	else
		return (0);
}

static void			change_dir_home_sign(char **args, char ***env)
{
	char *line;

	if (args[1])
	{
		if (args[1][0] == '~')
		{
			line = ft_strjoin(get_env("HOME", *env), args[1] + 1);
			free(args[1]);
			args[1] = line;
		}
		if (args[2] && args[2][0] == '~')
		{
			line = ft_strjoin(get_env("HOME", *env), args[2] + 1);
			free(args[2]);
			args[2] = line;
		}
	}
}

static void			change_dir_one_arg(char **args, char ***env)
{
	char	*value;

	if (args[1][0] == '-' && !args[1][1])
	{
		value = get_env("OLDPWD", *env);
		if (!value)
			ft_fprintf(2, "cd: OLDPWD env missed\n");
		else if (!check_dir_err(value))
			chdir(value);
	}
	else if (!check_dir_err(args[1]))
		chdir(args[1]);
}

static void			change_dir_two_args(char **args, char *pwd)
{
	char			*line;
	char			*path;
	int				len;

	path = 0;
	if ((line = ft_strstr(pwd, args[1])))
	{
		len = ft_strlen(pwd) + ft_strlen(args[2]) - ft_strlen(args[1]) + 1;
		path = (char *)malloc(len);
		ft_strncat(path, pwd, line - pwd);
		ft_strcat(path, args[2]);
		ft_strcat(path, line + ft_strlen(args[1]));
		if (!check_dir_err(path))
		{
			ft_printf("%s\n", path);
			chdir(path);
		}
	}
	else
		ft_fprintf(2, "cd: string not in pwd: %s\n", args[1]);
	free(path);
}

void				change_dir(char **args, char ***env)
{
	char			*oldpwd;
	char			*pwd;
	char			*home;

	oldpwd = getcwd(0, 0);
	change_dir_home_sign(args, env);
	home = get_env("HOME", *env);
	if (args[0] && args[1] && args[2] && args[3])
		ft_fprintf(2, "cd: too many arguments\n");
	else if (args[0] && !args[1] && !home)
		ft_fprintf(2, "cd: HOME env missed\n");
	else if (args[0] && !args[1] && home && !check_dir_err(home))
		chdir(home);
	else if (args[0] && args[1] && !args[2])
		change_dir_one_arg(args, env);
	else if (args[0] && args[1] && args[2] && !args[3])
		change_dir_two_args(args, oldpwd);
	pwd = getcwd(0, 0);
	if (ft_strcmp(pwd, oldpwd))
		set_env("OLDPWD", oldpwd, env);
	set_env("PWD", pwd, env);
	free(oldpwd);
	free(pwd);
}
