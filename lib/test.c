/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalipso <kalipso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/19 17:54:09 by kalipso           #+#    #+#             */
/*   Updated: 2024/11/07 01:21:55 by kalipso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/libft.h"
#include "../inc/minirt.h"
#include <dirent.h>

/*******************************************************************************
******************************************************************************/
// <!> - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -  </!>
///////////////////////////////////////////////////////////////////////////////]
/*******************************************************************************

			░█████╗░  ███████╗░█████╗░██╗██████╗░███████╗
			██╔══██╗  ██╔════╝██╔══██╗██║██╔══██╗██╔════╝
			███████║  █████╗░░███████║██║██████╔╝█████╗░░
			██╔══██║  ██╔══╝░░██╔══██║██║██╔══██╗██╔══╝░░
			██║░░██║  ██║░░░░░██║░░██║██║██║░░██║███████╗
			╚═╝░░╚═╝  ╚═╝░░░░░╚═╝░░╚═╝╚═╝╚═╝░░╚═╝╚══════╝

Lowercase: α β γ δ ε ζ η θ ι κ λ μ ν ξ ο π ρ σ τ υ φ χ ψ ω

Uppercase: Α Β Γ Δ Ε Ζ Η Θ Ι Κ Λ Μ Ν Ξ Ο Π Ρ Σ Τ Υ Φ Χ Ψ Ω
******************************************************************************/
///////////////////////////////////////////////////////////////////////////////]



///////////////////////////////////////////////////////////////////////////////]
// double	ft_atof(char *string, int *error)
// {
// 	double rtrn = 0.0;

// 	char **tab = split(string, ".");
// 	if (!tab || tab_size(tab) > 2)
// 		return (free_tab(tab), put(ERR6"(%s) not a correct number\n", string), (*error)++, rtrn);
// 	int err = 0;
// 	rtrn = (double)ft_atoi(tab[0], &err);
// 	if (err)
// 		return (put(ERR7"(%s) not a correct number\n", string), free_tab(tab), (*error)++, rtrn);
// 	if (tab[1])
// 	{
// 		double fraction = ft_atoi(tab[1], &err);
// 		if (err || fraction < 0)
// 			return (put(ERR8"(%s) bad fractional part\n", tab[1]), free_tab(tab), (*error)++, rtrn);
// 		int	frac_len = len(tab[1]);
// 		double div = 1.0;
// 		while (frac_len-- != 0)
// 			div *= 10.0;
// 		rtrn += fraction / div;
// 	}
// 	free_tab(tab);
// 	return (rtrn);
// }

///////////////////////////////////////////////////////////////////////////////]
// 		......../....*
// 		........*..../
int	render(t_data *data)
{
	int x;
	int y = -1;
	unsigned int color;
	while (++y < SIZE_SCREEN_Y)
	{
		x = -1;
		while (++x < SIZE_SCREEN_X)
		{
			color = 123 << 16 | (x % 256) << 8 | (y % 256);
			mlx_pixel_put(data->mlx, data->win, x, y, color);
			// put_pixel_bufferv2(data, x, y, color);
		}
	}
	mlx_put_image_to_window(data->mlx, data->win, data->buffer.img, 0, 0);
	return 0;
}

int	main(int ac, char **av, char **env)
{
	t_data	data;

	if (ac != 2)
		return (put(ERR "bad arguments\n"), 1);

	ft_memset(&data, 0, sizeof(t_data));
	data.mlx = mlx_init();
	if (!data.mlx)
		(put("--->MLX fait de la merde\n"), end(&data, 1));
	data.win = mlx_new_window(data.mlx, SIZE_SCREEN_X, SIZE_SCREEN_Y, "miniRT");
	data.buffer.img = mlx_new_image(data.mlx, SIZE_SCREEN_X, SIZE_SCREEN_Y);
	if (!data.win || !data.buffer.img)
		(put(ERRM"Problem initalisazing mlx (2)\n"), end(&data, 1));
	data.buffer.addr = mlx_get_data_addr(data.buffer.img, &data.buffer.bpp, &data.buffer.ll, &data.buffer.end);
	if (!data.buffer.addr)
		(put(ERRM"Problem initalisazing mlx (3)\n"), end(&data, 1));

	mlx_loop_hook(data.mlx, &render, &data);
	mlx_hook(data.win, KeyPress, KeyPressMask, &key_press, &data);
	mlx_hook(data.win, 17, 0, &end2, &data);

	mlx_loop(data.mlx);
	end(&data, 0);
	
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
// MAIN 1

	// t_folder	*folder;

	// folder = new_folder(NULL, str("/"));
	// if (!folder)
	// 	return (1);
	// print_folder(folder);
	// clean_folder(folder);


	// DIR 		*dir = opendir("/");
	// put("%p\n", dir);
	return (0);
}

///////////////////////////////////////////////////////////////////////////////]



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	MAIN 1
typedef struct s_folder
{
	DIR 		*dir;
	struct dirent *entry;
	char		**all_files;
	char 		*folder_path;
	int			floor;
	struct s_folder **next;

}	t_folder;

t_folder	*new_folder(t_folder *previous, char *folder_path)
{
	t_folder	(*folder) = (t_folder *)mem(0, sizeof(t_folder));
	if (!folder)
		return (NULL);
	if (previous)
	{
		folder->floor = previous->floor + 1;
		previous->next = (t_folder **)expand_tab((char **)previous->next, (char *)folder);
	}
	folder->folder_path = folder_path;
	folder->dir = opendir(folder_path);
	if (!folder->dir)
	{
		put("adsfhsjklljkjkjkjkjkjkjkjkjk");
		return (perror("opendir"), NULL);
	}
	folder->entry = 1;
	while (folder->entry)
	{
		folder->entry = readdir(folder->dir);
		if (!folder->entry || same_str(folder->entry->d_name, ".") || same_str(folder->entry->d_name, ".."))
			continue;
		folder->all_files = expand_tab(folder->all_files, str("%1s", folder->entry->d_name));
		if (folder->entry->d_type == DT_DIR || folder->entry->d_type == DT_LNK)
		{
			char *full_path = str("%1s/%1s", folder_path, folder->entry->d_name);
			if (!access(full_path, F_OK | R_OK | X_OK))// if 0, access ok
				new_folder(folder, full_path);
			else
				full_path = free_s(full_path);
		}
	}
	closedir(folder->dir);
	return (folder);
}

void	*clean_folder(t_folder *first)
{
	if (!first)
		return (NULL);
	free_tab(first->all_files);
	free_s(first->folder_path);
	t_folder	(**p_leaf) = first->next;
	if (p_leaf)
	{
		p_leaf = first->next - 1;
		while (*(++p_leaf))
			clean_folder(*p_leaf);
	}
	free_s(first->next);
	free_s(first);
	return (NULL);
}

void	print_folder(t_folder *first)
{
	if (!first)
		return ;
	put(ERR0"FOLDER: %s\n", first->folder_path);
	put("%#-.1t", first->all_files);
	t_folder	(**p_leaf) = first->next;
	if (p_leaf)
	{
		p_leaf = first->next - 1;
		while (*(++p_leaf))
			print_folder(*p_leaf);
	}
}

* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
