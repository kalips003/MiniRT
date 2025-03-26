/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   B_builtin_func.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalipso <kalipso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 04:12:38 by kalipso           #+#    #+#             */
/*   Updated: 2024/07/16 04:12:38 by kalipso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

t_tri			*h_parse_face(char **spl, t_model *model, t_tri *prev, t_mat *current_mat);
static t_tri	*expand_list_tri(t_tri *previous, char **spl, t_mat *current_mat);
static void		h_vvnt(char *v, int *vvtvn);

///////////////////////////////////////////////////////////////////////////////]
// handle: faces: "f 45/20/17 55/21/17 56/22/17 12/13/17 11/12/17 46/23/17"
t_tri	*h_parse_face(char **spl, t_model *model, t_tri *prev, t_mat *current_mat)
{
	t_tri	*new_tri;
	int		poly_size;
	int		i;

	poly_size = tab_size(spl) - 1;
	if (poly_size < 3)
		return (prev);
	if (poly_size > 10)
		printf(ERR0"face size too big: %d\n", poly_size);
	new_tri = expand_list_tri(prev, (char *[]){spl[1], spl[2], spl[3]}, current_mat);
	if (!model->tree.f)
		model->tree.f = new_tri;
	// if (!model->first)
	// 	model->first = new_tri;
	i = 2;
	while (++i < poly_size)
		new_tri = expand_list_tri(new_tri, (char *[]){spl[1], spl[i], spl[i + 1]}, current_mat);
	return (new_tri);
}

// "45/20/17"
static t_tri	*expand_list_tri(t_tri *previous, char **spl, t_mat *current_mat)
{
	t_tri	*new_tri;

	new_tri = mem(0, sizeof(t_tri));
	if (!new_tri)
		return (put(ERRM"ft: expand_tab_coor\n"), previous);
	h_vvnt(*spl, &new_tri->p1);
	h_vvnt(*(spl + 1), &new_tri->p2);
	h_vvnt(*(spl + 2), &new_tri->p3);
	if (new_tri->p1 < 0 || new_tri->p2 < 0 || new_tri->p3 < 0)
		return (printf(ERR"bad polygon\n"), free_s(new_tri), previous);
	new_tri->mat = current_mat;
	if (previous)
		previous->next = new_tri;
	return (new_tri);
}

static void	h_vvnt(char *v, int *vvtvn)
{
	int	i;
	int	err;

	i = 0;
	err = 0;
	vvtvn[0] = ft_atoi_v2(v, &i, &err) - 1;
	i += (v[i] == '/');
	vvtvn[1] = ft_atoi_v2(v, &i, &err) - 1;
	i += (v[i] == '/');
	vvtvn[2] = ft_atoi_v2(v, &i, &err) - 1;
}
