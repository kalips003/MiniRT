/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt_const.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalipso <kalipso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 16:55:43 by marvin            #+#    #+#             */
/*   Updated: 2025/03/31 06:33:13 by kalipso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_CONST_H
# define MINIRT_CONST_H

///////////////////////////////////////////////////////////////////////////////]
// 
// 			░█▀▀█ ░█▀▀▀█ ░█▄─░█ ░█▀▀▀█ ▀▀█▀▀ ─█▀▀█ ░█▄─░█ ▀▀█▀▀ ░█▀▀▀█ 
// 			░█─── ░█──░█ ░█░█░█ ─▀▀▀▄▄ ─░█── ░█▄▄█ ░█░█░█ ─░█── ─▀▀▀▄▄ 
// 			░█▄▄█ ░█▄▄▄█ ░█──▀█ ░█▄▄▄█ ─░█── ░█─░█ ░█──▀█ ─░█── ░█▄▄▄█
//
///////////////////////////////////////////////////////////////////////////////]

# define PI 3.14159265358979323846
# define SQRT3 1.7320508075688771931766041
# define EPSILON 1e-6
# define EPSILON2 1e-8

# define SCALAR_LIGHT_DIST 200.0
# define HEIGHT_MAP_DISPLACEMENT 5.0

# define SPECULAR_ON_OFF 1
# define MAX_TR_DEPTH 4
# define MAX_MI_DEPTH 4
# define MAX_RAY_BOUNCE 3

# define SZ_Y 800
# define SZ_X 1000

#define CIRCLE 0
#define SPHERE 1
#define PLANE 2
#define CYLINDER 3
#define CONE 4
#define HYPERBOLOID 5
#define PARABOLOID 6
#define ARROW 7
#define SPRITE 8
#define OBJECT 9

#define DELTA_MOV 4.0
#define DELTA_ROTA (PI / 16)

///////////////////////////////////////////////////////////////////////////////]
# define MIN 0
# define MAX 1
# define X 0
# define Y 1
# define Z 2
# define VECTOR 0
# define QUATERNION 1
# define NOT_AA 1
# define AA 2
# define DONT_SET_DIST 0
# define SET_DIST 1
# define NOT_SHADOWS 0
# define CALC_SHADOWS 1
# define EXIT_SUCCESS 0
# define EXIT_FAILURE 1


///////////////////////////////////////////////////////////////////////////////]
// # define ANTI_ALIASING 0
# define RENDERING_LVL 2
# define RENDER_LVL_DEPTH 2
# define RENDER_MOVING_MOUSE 0
# define PRINT_DEPTH 2

# define FAKE_REFR_SCALAR 2
# define FAKE_REFR_POW 2

#define	NUM_THREAD	12





#endif