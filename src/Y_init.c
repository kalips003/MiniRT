/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Y_init.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalipso <kalipso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 06:21:51 by kalipso           #+#    #+#             */
/*   Updated: 2024/10/30 13:23:18 by kalipso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

typedef int (*t_ft_object)(t_data *data, char **split);

typedef struct s_dico_pair
{
	char		*name;
	t_ft_object	exe;
}	t_dico_pair;

static const t_dico_pair	dico[] = {
{"A", parse_A},
{"C", parse_C},
{"L", parse_L},
{"pl", parse_pl},
{"sp", parse_sp},
{"cy", parse_cy},
{NULL, NULL}
};

///////////////////////////////////////////////////////////////////////////////]

int	initialization(int ac, char **av, char **env, t_data *data);
int	ft_parse_line(t_data *data, char *line);

///////////////////////////////////////////////////////////////////////////////]
// ini
int	initialization(int ac, char **av, char **env, t_data *data)
{
	if (ac != 2)
		(put(ERR2"Wrong number of args\n"), exit(1));
	int dot = wii('.', av[1]);
	if (dot == -1 || !same_str("rt", &av[1][dot]))
		(put(ERR1"MiniRT takes a single map with .rt extension as argument\n"), exit(1));
	int fd = open(av[1], O_RDONLY);
	if (fd < 0)
		(perror(ERR9"Cant open file"), exit(1));
	ft_memset(data, 0, sizeof(t_data));

	char *line;
	while (1)
	{
		line = gnl(fd);
		if (!line)
			break ;
		if (ft_parse_line(data, &line))
			(close(fd), end(data, 1));
	}
	close(fd);
}

int	ft_parse_line(t_data *data, char **line)//		line = 0 char **
{
// if line empty, skip it
	if (!line[0])
		return (free_s(line), 0);

// split the raw
	char **param = split(line, " \t");
	if (!param)
		return (put(ERRM"split\n"), free_s(line), 2);
	line = free_s(line);

// find the identifier in the dico, process the object with the appropriate function
	int	i;
	i = -1;
	while (dico[++i].name)
	{
		if (same_str(param[0], dico[i].name))
		{
			if (dico[i].exe(data, split + 1))
				return (free_tab(param), put(ERR3"parsing error\n"), 1);
			else
				break ;
		}
	}
	free_tab(param);
	if (!dico[i].name)
		return (put(ERR4"unknown object\n"), 1);

	return (0);
}