// #include "../inc/minirt.h"

// ///////////////////////////////////////////////////////////////////////////////]
// // Normalize vector
// int	ft_normalize_vect_v2(t_vect *vect, int print_err)
// {
// 	double normal_magnitude;

// 	normal_magnitude = sqrt(vect->dx * vect->dx + vect->dy * vect->dy + vect->dz * vect->dz);
// 	if (normal_magnitude == 0.0 && print_err)
// 		return (1);
// 		// return (put("VECTOR = [%f, %f, %f]\n", vect->dx, vect->dy, vect->dz), 1);
// 	vect->dx = vect->dx / normal_magnitude;
// 	vect->dy = vect->dy / normal_magnitude;
// 	vect->dz = vect->dz / normal_magnitude;
// 	return (0);
// }

// ///////////////////////////////////////////////////////////////////////////////]
// // fills rtrnn with the coordonates of the rotated vector camera for [x,y]pixel
// // Q rotation combined = qβ.qα
// void	f_calculate_combined_quaternion(t_data *data, double angle_α, double angle_β, t_vect *rtrn)
// {
// 	t_camera *c = data->eye.c;
// 	t_camera_calc	calc;

// 	calc.cosA = cos(angle_α / 2);
// 	calc.sinA = sin(angle_α / 2);
// 	calc.cosB = cos(-angle_β / 2);
// 	calc.sinB = sin(-angle_β / 2);

// 	calc.Qw = calc.cosA*calc.cosB - calc.sinA*calc.sinB * (c->O.right.dx * c->O.up.dx + c->O.right.dy * c->O.up.dy + c->O.right.dz * c->O.up.dz);
// 	calc.Qi = calc.cosB*calc.sinA * c->O.up.dx + calc.cosA*calc.sinB * c->O.right.dx + calc.sinA*calc.sinB*(c->O.right.dy * c->O.up.dz - c->O.right.dz * c->O.up.dy);
// 	calc.Qj = calc.cosB*calc.sinA * c->O.up.dy + calc.cosA*calc.sinB * c->O.right.dy + calc.sinA*calc.sinB*(c->O.right.dz * c->O.up.dx - c->O.right.dx * c->O.up.dz);
// 	calc.Qk = calc.cosB*calc.sinA * c->O.up.dz + calc.cosA*calc.sinB * c->O.right.dz + calc.sinA*calc.sinB*(c->O.right.dx * c->O.up.dy - c->O.right.dy * c->O.up.dx);

// 	rtrn->dx = c->O.view.dx * (calc.Qw * calc.Qw + calc.Qi * calc.Qi - calc.Qj * calc.Qj - calc.Qk * calc.Qk) + 2*c->O.view.dy * (calc.Qi*calc.Qj - calc.Qw*calc.Qk) + 2*c->O.view.dz * (calc.Qi*calc.Qk + calc.Qw*calc.Qj);
// 	rtrn->dy = c->O.view.dy * (calc.Qw * calc.Qw + calc.Qj * calc.Qj - calc.Qi * calc.Qi - calc.Qk * calc.Qk) + 2*c->O.view.dz * (calc.Qj*calc.Qk - calc.Qw*calc.Qi) + 2*c->O.view.dx * (calc.Qi*calc.Qj + calc.Qw*calc.Qk);
// 	rtrn->dz = c->O.view.dz * (calc.Qw * calc.Qw + calc.Qk * calc.Qk - calc.Qi * calc.Qi - calc.Qj * calc.Qj) + 2*c->O.view.dx * (calc.Qi*calc.Qk - calc.Qw*calc.Qj) + 2*c->O.view.dy * (calc.Qj*calc.Qk + calc.Qw*calc.Qi);

// 	ft_normalize_vect(rtrn);
// }

// ///////////////////////////////////////////////////////////////////////////////]
// // fills rtrn with the coordonates of the rotated vector view object, around
// // 		angle_α ~ UP then around angle_β ~ RIGHT
// // Q rotation combined = qβ.qα
// void	f_calculate_combined_quaternion_better(t_obj *obj, double angle_α, double angle_β, t_vect *rotated)
// {
// 	t_camera_calc	c;

// 	c.cosA = cos(angle_α / 2);
// 	c.sinA = sin(-angle_α / 2);
// 	c.cosB = cos(angle_β / 2);
// 	c.sinB = sin(-angle_β / 2);

// 	c.Qw = c.cosA*c.cosB - c.sinA*c.sinB * (obj->right.dx * obj->up.dx + obj->right.dy * obj->up.dy + obj->right.dz * obj->up.dz);
// 	c.Qi = c.cosB*c.sinA * obj->up.dx + c.cosA*c.sinB * obj->right.dx + c.sinA*c.sinB*(obj->right.dy * obj->up.dz - obj->right.dz * obj->up.dy);
// 	c.Qj = c.cosB*c.sinA * obj->up.dy + c.cosA*c.sinB * obj->right.dy + c.sinA*c.sinB*(obj->right.dz * obj->up.dx - obj->right.dx * obj->up.dz);
// 	c.Qk = c.cosB*c.sinA * obj->up.dz + c.cosA*c.sinB * obj->right.dz + c.sinA*c.sinB*(obj->right.dx * obj->up.dy - obj->right.dy * obj->up.dx);

// 	rotated->dx = obj->view.dx * (c.Qw * c.Qw + c.Qi * c.Qi - c.Qj * c.Qj - c.Qk * c.Qk) + 2*obj->view.dy * (c.Qi*c.Qj - c.Qw*c.Qk) + 2*obj->view.dz * (c.Qi*c.Qk + c.Qw*c.Qj);
// 	rotated->dy = obj->view.dy * (c.Qw * c.Qw + c.Qj * c.Qj - c.Qi * c.Qi - c.Qk * c.Qk) + 2*obj->view.dz * (c.Qj*c.Qk - c.Qw*c.Qi) + 2*obj->view.dx * (c.Qi*c.Qj + c.Qw*c.Qk);
// 	rotated->dz = obj->view.dz * (c.Qw * c.Qw + c.Qk * c.Qk - c.Qi * c.Qi - c.Qj * c.Qj) + 2*obj->view.dx * (c.Qi*c.Qk - c.Qw*c.Qj) + 2*obj->view.dy * (c.Qj*c.Qk + c.Qw*c.Qi);

// 	ft_normalize_vect(rotated);
// }

// // CONST ROTATION 5 degree
// # define COS_ROTA 0.99619469809174554520
// # define COS_ROTA2 0.99240387650610406567
// # define SIN_ROTA 0.08715574274765816587
// # define SIN_ROTA2 0.00759612349389596903
// # define COSSIN_ROTA 0.08682408883346516559

// ///////////////////////////////////////////////////////////////////////////////]
// // rotate the camera (eye) vector of fixed angle (with vector operation)
// void	rotation_camera(t_data *data, t_vect *axis_rota, int posi_neg)
// {
// 	t_vect *c = &data->eye.c->O.view;

// 	double Vx2 = axis_rota->dx * axis_rota->dx;
// 	double Vy2 = axis_rota->dy * axis_rota->dy;
// 	double Vz2 = axis_rota->dz * axis_rota->dz;

// 	double resultx = COS_ROTA2 * c->dx + SIN_ROTA2 * (c->dx * (Vx2 - Vy2 - Vz2) + 2 * axis_rota->dx * (axis_rota->dy * c->dy + axis_rota->dz * c->dz)) + 2 * (posi_neg) * COSSIN_ROTA * (axis_rota->dy * c->dz - axis_rota->dz * c->dy);
// 	double resulty = COS_ROTA2 * c->dy + SIN_ROTA2 * (c->dy * (Vy2 - Vx2 - Vz2) + 2 * axis_rota->dy * (axis_rota->dx * c->dx + axis_rota->dz * c->dz)) + 2 * (posi_neg) * COSSIN_ROTA * (axis_rota->dz * c->dx - axis_rota->dx * c->dz);
// 	double resultz = COS_ROTA2 * c->dz + SIN_ROTA2 * (c->dz * (Vz2 - Vx2 - Vy2) + 2 * axis_rota->dz * (axis_rota->dx * c->dx + axis_rota->dy * c->dy)) + 2 * (posi_neg) * COSSIN_ROTA * (axis_rota->dx * c->dy - axis_rota->dy * c->dx);
// 	c->dx = resultx;
// 	c->dy = resulty;
// 	c->dz = resultz;

// 	ft_normalize_vect(c);
// 	create_vector_space(&data->eye.c->O);
// }

// ///////////////////////////////////////////////////////////////////////////////]
// // rotate the obj (eye) vector of fixed angle (with vector operation)
// void	rotation_obj_v2(t_obj *obj, t_vect *axis_rota, int posi_neg)
// {
// 	t_vect	cross;
// 	double	dot;

// 	cross = ft_cross_product(axis_rota, &obj->view);
// 	dot = ft_dot_product(axis_rota, &obj->view);
// 	ft_normalize_vect(&cross);
// 	t_vect	new_view = {
// 		obj->view.dx * COS_ROTA + cross.dx * (posi_neg) * SIN_ROTA + axis_rota->dx * dot * (1 - COS_ROTA),
// 		obj->view.dy * COS_ROTA + cross.dy * (posi_neg) * SIN_ROTA + axis_rota->dy * dot * (1 - COS_ROTA),
// 		obj->view.dz * COS_ROTA + cross.dz * (posi_neg) * SIN_ROTA + axis_rota->dz * dot * (1 - COS_ROTA)
// 	};
// 	ft_normalize_vect(&new_view);

// 	cross = ft_cross_product(axis_rota, &obj->up);
// 	dot = ft_dot_product(axis_rota, &obj->up);
// 	ft_normalize_vect(&cross);

// 	t_vect	new_up = {
// 		obj->up.dx * COS_ROTA + cross.dx * (posi_neg) * SIN_ROTA + axis_rota->dx * dot * (1 - COS_ROTA),
// 		obj->up.dy * COS_ROTA + cross.dy * (posi_neg) * SIN_ROTA + axis_rota->dy * dot * (1 - COS_ROTA),
// 		obj->up.dz * COS_ROTA + cross.dz * (posi_neg) * SIN_ROTA + axis_rota->dz * dot * (1 - COS_ROTA)
// 	};
// 	ft_normalize_vect(&new_up);

// 	cross = ft_cross_product(&new_up, &new_view);
// 	// cross = ft_vect_cross_product(&new_view, &new_up);
// 	ft_normalize_vect(&cross);
// 	// cross = (t_vect){-cross.dx, -cross.dy, -cross.dz};
// 	*obj = (t_obj){obj->c0, new_view, new_up, cross};
// }

// t_vect	ft_vect_refracted(t_vect *incident, t_vect *normal, double gamma_incident, double gamma_obj)
// {
// 	t_vect refracted;
// 	t_vect	Ipara;
// 	t_vect	Iortho;
// 	double	dot = ft_dot_product(incident, normal);

// // Incident_∥​
// 	Ipara = (t_vect){
// 		dot * normal->dx,
// 		dot * normal->dy,
// 		dot * normal->dz
// 	};
// // Incident_⊥
// 	Iortho = (t_vect){
// 		incident->dx - Ipara.dx,
// 		incident->dy - Ipara.dy,
// 		incident->dz - Ipara.dz
// 	};
// 	double sinθ1 = sqrt(Iortho.dx * Iortho.dx + Iortho.dy *Iortho.dy + Iortho.dz * Iortho.dz);
// 	double sinθ2 = gamma_incident / gamma_obj * sinθ1;
// 	if (sinθ2 > 1.0)
// 		printf("error reflection");
// 	t_vect	Rortho = (t_vect){
// 		gamma_incident / gamma_obj * Iortho.dx,
// 		gamma_incident / gamma_obj * Iortho.dy,
// 		gamma_incident / gamma_obj * Iortho.dz
// 	};
// 	double s = -sqrt(1 - sinθ2 * sinθ2);
// 	t_vect	Rpara = (t_vect){
// 		s * normal->dx,
// 		s * normal->dy,
// 		s * normal->dz
// 	};
// 	refracted = (t_vect){
// 		Rortho.dx + Rpara.dx,
// 		Rortho.dy + Rpara.dy,
// 		Rortho.dz + Rpara.dz
// 	};

// 	return (refracted);
// }

// t_vect	ft_vect_refracted_v3(t_vect *incident, t_vect *normal, double gamma_incident, double gamma_obj)
// {
// 	double cos_i = -ft_dot_product(incident, normal);
// 	double index = 1.0 / gamma_obj;
// 	double cos_r = sqrt(1.0 - index * index * (1 - cos_i * cos_i));

// 	t_vect refracted = (t_vect){
// 		index * incident->dx + (index * cos_i - cos_r) * normal->dx,
// 		index * incident->dy + (index * cos_i - cos_r) * normal->dy,
// 		index * incident->dz + (index * cos_i - cos_r) * normal->dz
// 	};
// }

// ///////////////////////////////////////////////////////////////////////////////]
// void	ft_specular_v1(t_data *data, t_calcul_px *c, t_light *lights)
// {
// 	t_vect reflected_light;
// 	double	cos_angle;
// 	double	adjusted_intensity;

// 	reflected_light = ft_vect_reflected(&c->v_light, &c->v_normal);
// 	cos_angle = fmin(1.0, ft_dot_product(&c->v, &reflected_light));
// 	if (cos_angle < 0.0)
// 		return ;
// 	adjusted_intensity = ((t_obj2*)c->object)->param.specular * lights->ratio * pow(cos_angle, ((t_obj2*)c->object)->param.shiny);

// 	c->diffuse.x += lights->color.r * adjusted_intensity;
// 	c->diffuse.y += lights->color.g * adjusted_intensity;
// 	c->diffuse.z += lights->color.b * adjusted_intensity;
// }

// ///////////////////////////////////////////////////////////////////////////////]
// int	ft_diffuse_v1(t_data *data, t_calcul_px *c, t_light *lights)
// {
// 	double adjusted_intensity;

// 	c->dist_light = dist_two_points(&c->inter, &lights->xyz);
// 	c->v_light = vect_ab_norm(&c->inter, &lights->xyz);
// 	c->cos_angle = ft_dot_product(&c->v_light, &c->v_normal);
// 	if (c->cos_angle < EPSILON || something_block_the_light(data, c, lights))
// 		return (0);
// 	adjusted_intensity = lights->ratio * c->cos_angle;
// 	adjusted_intensity = SCALAR_LIGHT_DIST * adjusted_intensity / (c->dist_light * c->dist_light);

// 	c->diffuse.x += c->argb.r * lights->color.r / 255.0 * adjusted_intensity;
// 	c->diffuse.y += c->argb.g * lights->color.g / 255.0 * adjusted_intensity;
// 	c->diffuse.z += c->argb.b * lights->color.b / 255.0 * adjusted_intensity;
// 	return (1);
// }

// ///////////////////////////////////////////////////////////////////////////////]
// ///////////////////////////////////////////////////////////////////////////////]
// ///////////////////////////////////////////////////////////////////////////////]
// t_argb	what_is_behind_sublim(t_data *data, t_calcul_px *calcul, int sublim)
// {
// 	t_calcul_px	c;
// 	double	incident_gamma = ((t_obj2*)calcul->object)->param.gamma;
// 	double	refracted_gamma = 1.0;

// 	if (calcul->transparence_depth == TRANSPARENCE_BOUNCES)
// 		return (calcul->argb);
// 	// ft_memset(&c, 0, sizeof(t_calcul_px));

// 	c.transparence_depth = calcul->transparence_depth + 1;
// 	c.reflected_depth = calcul->reflected_depth;
// 	// c.current_gamma = ((t_obj2*)calcul->object)->param.gamma;
// 	// c.previous_gamma = calcul->current_gamma;

// 	c.print = calcul->print + !!(calcul->print);
// 	c.c0 = new_moved_point(&calcul->inter, &calcul->v, EPSILON);
// 	if (((t_obj2*)calcul->object)->type == PLANE || ((t_obj2*)calcul->object)->type == SPRITE)
// 		c.v = calcul->v;
// 	else
// 		c.v = ft_vect_refracted(&calcul->v, &calcul->v_normal, incident_gamma, refracted_gamma);
// 	c.dist = -1.0;

// 	calculate_pixel_color(data, &c, sublim);
// 	return (c.argb);
// }
// ///////////////////////////////////////////////////////////////////////////////]
// t_argb	what_is_behind_v1(t_data *data, t_calcul_px *calcul)
// {
// 	printf(ERR"THIS SHOULDNT PRINT (what_is_behind E2)\n");

// 	t_calcul_px	c;

// 	if (calcul->transparence_depth == TRANSPARENCE_BOUNCES)
// 		return (calcul->argb);
// 	// ft_memset(&c, 0, sizeof(t_calcul_px));

// 	c.transparence_depth = calcul->transparence_depth + 1;
// 	c.reflected_depth = calcul->reflected_depth;
// 	// c.current_gamma = ((t_obj2*)calcul->object)->param.gamma;
// 	// c.previous_gamma = calcul->current_gamma;
// 	c.print = calcul->print + !!(calcul->print);

// 	// c.current_obj = calcul->object;
// 	// if (c.current_obj == calcul->current_obj)
// 	// 	c.current_gamma = 1.0;
// 	c.c0 = new_moved_point(&calcul->inter, &calcul->v, EPSILON);
// 	if (((t_obj2*)calcul->object)->type == PLANE || ((t_obj2*)calcul->object)->type == SPRITE)
// 		c.v = calcul->v;
// 	// else
// 	// 	c.v = ft_vect_refracted_v2(&calcul->v, &calcul->v_normal, calcul->current_gamma, c.current_gamma);
// 	c.dist = -1.0;

// 	// if (calcul->print)
// 	// 	printf("%d) previous_γ: %.2f\tcurrent_γ: %.2f\n", calcul->print, calcul->previous_gamma, calcul->current_gamma);
// 	calculate_pixel_color(data, &c, RENDERING_LVL);
// 	return (c.argb);
// }