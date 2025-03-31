/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Y_init.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalipso <kalipso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 06:21:51 by kalipso           #+#    #+#             */
/*   Updated: 2025/03/07 00:41:06 by kalipso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

///////////////////////////////////////////////////////////////////////////////]

void		initialization(int ac, char **av, t_data *data);
void		read_file(int ac, char **av, t_data *data);
static void	ini_mlx(t_data *data);

///////////////////////////////////////////////////////////////////////////////]
// ini
void	initialization(int ac, char **av, t_data *data)
{
	struct stat	file_stat;

	ft_memset(data, 0, sizeof(t_data));
	data->av = av;
	data->zoom = DELTA_MOV;
	ini_mlx(data);
	read_file(ac, av, data);
	srand(time(NULL));
	if (tab_size((char **)data->bgl) != 1)
		return (put(ERR7"Only ONE background light!\n"), end(data, 1, 1));
	if (tab_size((char **)data->camera) < 1)
		return (put(ERR7"Need a Camera\n"), end(data, 1, 1));
	stat(data->av[1], &file_stat);
	data->last_modif_time = file_stat.st_mtime;
	data->eye.c = data->camera[0];
}

///////////////////////////////////////////////////////////////////////////////]
void	read_file(int ac, char **av, t_data *data)
{
	char	*line;
	int		dot;
	int		fd;

	if (ac != 2)
		(put(ERR2"Wrong number of args\n"), end(data, 1, 1));
	dot = wii('.', av[1]);
	if (dot == -1 || !same_str(".rt", &av[1][dot]))
		(put(ERR1"MiniRT takes a single map with .rt \
			extension as argument\n"), end(data, 1, 1));
	fd = open(av[1], O_RDONLY);
	if (fd < 0)
		(perror(ERR9"Cant open file"), end(data, 1, 1));
	while (1)
	{
		line = gnl(fd);
		if (!line)
			break ;
		if (ft_parse_line(data, line))
			(close(fd), end(data, 1, 1));
	}
	close(fd);
}

///////////////////////////////////////////////////////////////////////////////]
static void	ini_mlx(t_data *data)
{
	data->mlx = mlx_init();
	if (!data->mlx)
		(put(ERRM"MLX fait de la merde\n"), end(data, 1, 1));
	data->win = mlx_new_window(data->mlx, SZ_X, SZ_Y, "miniRT");
	if (!data->win)
		(put(ERRM"Problem initalisazing mlx (2)\n"), end(data, 1, 1));
	data->buffer.img = mlx_new_image(data->mlx, SZ_X, SZ_Y);
	data->buff_aa.img = mlx_new_image(data->mlx, SZ_X * 2, SZ_Y * 2);
	if (!data->buffer.img || !data->buff_aa.img)
		(put(ERRM"Problem initializing buffers\n"), end(data, 1, 1));
	data->buffer.addr = mlx_get_data_addr(data->buffer.img, &data->buffer.bpp, \
		&data->buffer.ll, &data->buffer.end);
	data->buff_aa.addr = mlx_get_data_addr(data->buff_aa.img, \
		&data->buff_aa.bpp, &data->buff_aa.ll, &data->buff_aa.end);
	mlx_loop_hook(data->mlx, &ft_loop, data);
	mlx_hook(data->win, KeyPress, KeyPressMask, &key_press, data);
	mlx_hook(data->win, ButtonPress, ButtonPressMask, &mouse_clic, data);
	mlx_hook(data->win, ButtonRelease, ButtonReleaseMask, &mouse_release, data);
	mlx_hook(data->win, MotionNotify, PointerMotionMask, &mouse_move, data);
	mlx_hook(data->win, 17, 0, &end2, data);
}
