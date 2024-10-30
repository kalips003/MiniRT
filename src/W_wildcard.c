/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   W_wildcard.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalipso <kalipso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 11:29:53 by kalipso           #+#    #+#             */
/*   Updated: 2024/09/08 13:26:13 by kalipso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void		sublim_wildcard(t_cmd *cmd);
static void	h_sublim_wildcard(t_cmd *cmd, int i, char ***rtrn);
t_folder	*new_folder_wildcard(t_folder *prev, char *path, char **pattern);
static void	h_new_folder_wildcard(t_folder *folder, char **pattern);
void		*clean_folder(t_folder *first);

///////////////////////////////////////////////////////////////////////////////]
// recieve: args = {echo, */*.c, -abc} > {echo, lib/file.c, .. src/f2.c, -abc}
void	sublim_wildcard(t_cmd *cmd)
{
	char		**rtrn;
	int			i;

	if (!cmd->cmd_arg)
		return ;
	rtrn = NULL;
	i = -1;
	while (cmd->cmd_arg[++i])
	{
		if (wii('*', cmd->cmd_arg[i]) >= 0)
			h_sublim_wildcard(cmd, i, &rtrn);
		else
			rtrn = expand_tab(rtrn, str("%1s", cmd->cmd_arg[i]));
	}
	free_tab(cmd->cmd_arg);
	cmd->cmd_arg = rtrn;
}

static void	h_sublim_wildcard(t_cmd *cmd, int i, char ***rtrn)
{
	t_folder	*folder;
	char		**pattern;
	char		**allmatch;

	pattern = cut_pattern(cmd->cmd_arg[i]);
	folder = new_folder_wildcard(NULL, str("%c", '.' + (cmd->cmd_arg[i][0] == '/')), pattern);
	allmatch = return_all_match(folder, tab_size(pattern));
	if (allmatch)
		*rtrn = expand_tab_tab(*rtrn, allmatch, 0b11);
	else
		*rtrn = expand_tab(*rtrn, str("%1s", cmd->cmd_arg[i]));
	clean_folder(folder);
	free_tab(pattern);
}

///////////////////////////////////////////////////////////////////////////////]
// create a new node, fill it with file & folder that match the n-th pattern
// path to match == pattern[folder->floor]
t_folder	*new_folder_wildcard(t_folder *prev, char *path, char **pattern)
{
	t_folder	*folder;

	folder = (t_folder *)mem(0, sizeof(t_folder));
	if (!folder)
		return (NULL);
	if (prev)
	{
		folder->floor = prev->floor + 1;
		prev->next = (t_folder **)expand_tab((char **)prev->next,
				(char *)folder);
	}
	folder->f_path = path;
	folder->dir = opendir(path);
	if (!folder->dir)
		return (perror("opendir"), NULL);
	folder->pattern_to_match = pattern[folder->floor];
	h_new_folder_wildcard(folder, pattern);
	closedir(folder->dir);
	return (folder);
}

///////////////////////////////////////////////////////////////////////////////]
static void	h_new_folder_wildcard(t_folder *folder, char **pattern)
{
	char	*full_path;

	folder->entry = (struct dirent *)1;
	while (folder->entry)
	{
		folder->entry = readdir(folder->dir);
		if (!folder->entry || same_str(folder->entry->d_name, ".")
			|| (same_str(folder->entry->d_name, "..") && !same_str(folder->pattern_to_match, ".."))
			|| (folder->entry->d_name[0] == '.'
				&& folder->pattern_to_match[0] != '.')
			|| !is_a_match(folder->pattern_to_match, folder->entry->d_name))
			continue ;
		folder->all_files = expand_tab(folder->all_files,
				str("%*s%.*c%1s", !!(folder->floor), folder->f_path, !!(folder->floor), '/', folder->entry->d_name));
		if (pattern[folder->floor + 1] && (folder->entry->d_type == DT_DIR
				|| folder->entry->d_type == DT_LNK))
		{
			full_path = str("%1s", folder->entry->d_name);
			if (!access(full_path, F_OK | R_OK | X_OK))
				new_folder_wildcard(folder, full_path, pattern);
			else
				full_path = free_s(full_path);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////]
// free tree folders
void	*clean_folder(t_folder *first)
{
	t_folder	**p_leaf;

	if (!first)
		return (NULL);
	free_tab(first->all_files);
	free_s(first->f_path);
	p_leaf = first->next;
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
