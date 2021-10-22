/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_paint.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sjacki <sjacki@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/21 21:10:19 by sjacki            #+#    #+#             */
/*   Updated: 2021/10/22 21:40:33 by sjacki           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

typedef struct s_main
{
	FILE	*file;
	int		width;
	int		height;
	char	back_ch;
	char	**buf;

	char	type_c;
	float	x;
	float	y;
	float	r;
	char	ch_c;
}				t_main;

int	ft_err(char *str)
{
	int	x;

	x = 0;
	write (1, "Error: ", 7);
	while (str[x])
	{
		write (1, &str[x], 1);
		x++;
	}
	write (1, "\n", 1);
	return (1);
}

void	init_struct(t_main *m, FILE *file)
{
	m->file = file;
	m->buf = NULL;
	m->width = 0;
	m->height = 0;
	m->back_ch = 0;
	m->type_c = 0;
	m->x = 0;
	m->y = 0;
	m->r = 0;
	m->ch_c = 0;
}

void	draw_buf(t_main *m)
{
	int	x;
	int	y;

	y = 0;
	while (y < m->height)
	{
		x = 0;
		while (x < m->width)
		{
			write (1, &m->buf[y][x], 1);
			x++;
		}
		write (1, "\n", 1);
		y++;
	}
}

int	get_map(t_main *m)
{
	int	x;
	int	y;

	y = 0;
	if ((fscanf(m->file, "%d %d %c\n", &m->width, &m->height, &m->back_ch)) != 3)
		return (1);
	if (m->width <= 0 || m->width > 300 || m->height <= 0 || m->height > 300)
		return (1);
	if (!(m->buf = (char **)malloc(sizeof(char *) * m->height)))
		return (1);
	while (y < m->height)
	{
		x = 0;
		if (!(m->buf[y] = (char *)malloc(m->width)))
			return (1);
		while (x < m->width)
		{
			m->buf[y][x] = m->back_ch;
			x++;
		}
		y++;
	}
	return (0);
}

int	check_hit(float x, float y, t_main *m)
{
	float distance;

	distance = sqrtf(powf(x - m->x, 2.) + powf(y - m->y, 2.));
	if (distance <= m->r)
	{
		if ((m->r - distance) < 1.00000000)
			return (2);
		return (1);
	}
	return (0);
}

int	get_c(t_main *m)
{
	int x;
	int y;
	int	hit;

	y = 0;
	if (m->r <= 0.00000000 || (m->type_c != 'C' && m->type_c != 'c'))
		return (1);
	while (y < m->height)
	{
		x = 0;
		while (x < m->width)
		{
			hit = check_hit((float)x, (float)y, m);
			if (hit == 2 || (m->type_c == 'C' && hit))
				m->buf[y][x] = m->ch_c;
			x++;
		}
		y++;
	}
	return (0);
}

int	start_alg(t_main *m)
{
	int ret;
	if (get_map(m))
		return (1);
	while ((ret = fscanf(m->file, "%c %f %f %f %c\n", &m->type_c, &m->x, &m->y, &m->r, &m->ch_c)) == 5)
	{
		if (get_c(m))
			return (1);
	}
	if (ret != -1)
		return (1);
	draw_buf(m);
	return (0);
}

int	main(int argc, char **argv)
{
	FILE	*file;
	t_main	m;

	if (argc != 2)
		return (ft_err("argument"));
	if (!(file = fopen(argv[1], "r")))
		return (ft_err("Operation file corrupted"));
	init_struct(&m, file);
	if (start_alg(&m))
		return (ft_err("Operation file corrupted"));
}