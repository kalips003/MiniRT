/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalipso <kalipso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/19 17:54:09 by kalipso           #+#    #+#             */
/*   Updated: 2025/03/04 20:36:08 by kalipso          ###   ########.fr       */
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

int	main(int ac, char **av, char **env)
{
	int i = 0;
	double color = 0;
	double	new_color;
	while (i < 50)
	{
		if (i % 2)
			new_color = 100;
		else
			new_color = 0;
		color = (color * i + new_color) / (i + 1);
		printf("color = %.3f\n", color);
		i++;
	}

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
