/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalipso <kalipso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/19 17:54:09 by kalipso           #+#    #+#             */
/*   Updated: 2024/12/15 14:21:05 by kalipso          ###   ########.fr       */
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
double	ft_atof(char *string, int *error)
{
	double rtrn = 0.0;

	char **tab = split(string, ".");
	if (!tab || tab_size(tab) > 2)
		return (free_tab(tab), put(ERR6"(%s) not a correct number\n", string), (*error)++, rtrn);
	int err = 0;
	rtrn = ft_atoi(tab[0], &err);
	if (err < 0)
		return (put(ERR7"(%s) not a correct number\n", string), free_tab(tab), (*error)++, rtrn);
	if (tab[1])
	{
		double fraction = ft_atoi(tab[1], &err);
		if (err < 0 || fraction < 0)
			return (put(ERR8"(%s) bad fractional part\n", tab[1]), free_tab(tab), (*error)++, rtrn);
		int	frac_len = len(tab[1]);
		while (frac_len-- != 0)
			fraction /= 10.0;
		rtrn += fraction;
	}
	free_tab(tab);
	if (err == 1 && rtrn > 0)
		rtrn *= -1;
	return (rtrn);
}

# define PI 3.14159265358979323846
// COS (pi / 36)
# define COS_ROTA 0.99619469809174554520
# define COS_ROTA2 0.99240387650610406567
# define SIN_ROTA 0.08715574274765816587
# define SIN_ROTA2 0.00759612349389596903
# define COSSIN_ROTA 0.08682408883346516559

int	main(int ac, char **av, char **env)
{
	if (access("texture/big_ball.xpm", R_OK) != -1)
    	printf("File is readable\n");
	else
    	perror("File is not readable");
	
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
