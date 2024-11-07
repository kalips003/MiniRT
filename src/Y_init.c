/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Y_init.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalipso <kalipso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 06:21:51 by kalipso           #+#    #+#             */
/*   Updated: 2024/11/07 01:31:58 by kalipso          ###   ########.fr       */
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

void	initialization(int ac, char **av, t_data *data);
static int		ft_parse_line(t_data *data, char *line);

///////////////////////////////////////////////////////////////////////////////]
// ini
void	initialization(int ac, char **av, t_data *data)
{
	if (ac != 2)
		(put(ERR2"Wrong number of args\n"), exit(1));
	int dot = wii('.', av[1]);
	if (dot == -1 || !same_str(".rt", &av[1][dot]))
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
		if (ft_parse_line(data, line))
			(close(fd), end(data, 0));
	}
	close(fd);

// MLX
	data->mlx = mlx_init();
	if (!data->mlx)
		(put(ERRM"MLX fait de la merde\n"), end(data, 1));
	data->win = mlx_new_window(data->mlx, SIZE_SCREEN_X, SIZE_SCREEN_Y, "miniRT");
	data->buffer.img = mlx_new_image(data->mlx, SIZE_SCREEN_X, SIZE_SCREEN_Y);
	if (!data->win || !data->buffer.img)
		(put(ERRM"Problem initalisazing mlx (2)\n"), end(data, 1));
	data->buffer.addr = mlx_get_data_addr(data->buffer.img, &data->buffer.bpp, &data->buffer.ll, &data->buffer.end);
	if (!data->buffer.addr)
		(put(ERRM"Problem initalisazing mlx (3)\n"), end(data, 1));
	mlx_loop_hook(data->mlx, &ft_loop_empty, data);
	// mlx_loop_hook(data->mlx, &ft_loop, data);
	mlx_hook(data->win, KeyPress, KeyPressMask, &key_press, data);
	mlx_hook(data->win, KeyRelease, KeyReleaseMask, &key_release, data);
	mlx_hook(data->win, 17, 0, &end2, data);

// CHECK GOOD NUMBER OF CAMERA AND SO ON...

// DATA
	data->px = data->camera[0]->fov * (PI / 180) / SIZE_SCREEN_X;
	data->eye = data->camera[0];
	data->x0 = -(SIZE_SCREEN_X / 2) * data->px;
	data->y0 = -(SIZE_SCREEN_Y / 2) * data->px;
}

///////////////////////////////////////////////////////////////////////////////]
static int	ft_parse_line(t_data *data, char *line)
{
// if line empty, skip it
	if (line[0] == '\n' || line[0] == '#')
		return (free_s(line), 0);
// split the raw
	char **params = split(line, " \t");
	free_s(line);
	if (!params)
		return (put(ERRM"split\n"), 2);
// find the identifier in the dico, process the object with the appropriate function
	int	i;
	i = -1;
	while (dico[++i].name)
	{
		if (same_str(params[0], dico[i].name))
		{
			if (dico[i].exe(data, params + 1))
				return (free_tab(params), put(ERR3"parsing error\n"), 1);
			else
				break ;
		}
	}
	free_tab(params);
	if (!dico[i].name)
		return (put(ERR4"unknown object\n"), 1);
	return (0);
}