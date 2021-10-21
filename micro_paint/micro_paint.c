/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   micro_paint.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sjacki <sjacki@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/21 19:02:17 by sjacki            #+#    #+#             */
/*   Updated: 2021/10/21 20:45:27 by sjacki           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

typedef struct s_main
{
	FILE	*file;
	int		width;
	int		height;
	char	back_ch;
	char	**buf;

	char	type_sqr;
	char	ch_sqr;
	float	x;
	float	y;
	float	width_sqr;
	float	height_sqr;
}				t_main;

int	ft_putstr(char *str)
{
	int	x;

	x = 0;
	while (str[x])
	{
		write (1, &str[x], 1);
		x++;
	}
	return (x);
}

int	ft_err(char *str)
{
	ft_putstr("Error: ");
	ft_putstr(str);
	ft_putstr("\n");
	return (1);
}

void	init_struct(t_main *m, FILE *file)
{
	m->file = file;
	m->width = 0;
	m->height = 0;
	m->back_ch = 0;
	m->buf = NULL;
	m->type_sqr = 0;
	m->ch_sqr = 0;
	m->x = 0;
	m->y = 0;
	m->width_sqr = 0;
	m->height_sqr = 0;
}

int	get_map(t_main *m)
{
	int	ret;
	int	x;
	int	y;

	x = 0;
	if ((ret = fscanf(m->file, "%d %d %c\n", &m->width, &m->height, &m->back_ch)) != 3)
		return (1);
	if (m->width <= 0 || m->height <= 0 || m->width > 300 || m->height > 300)
		return (1);
	if (!(m->buf = (char **)malloc(sizeof(char *) * m->height)))
		return (1);
	while (x < m->height)
	{
		y = 0;
		if (!(m->buf[x] = (char*)malloc(m->width)))
			return (1);
		while (y < m->width)
		{
			m->buf[x][y] = m->back_ch;
			y++;
		}
		x++;
	}
	return (0);
}

void	draw_map(t_main *m)
{
	int x;
	int y;

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

int	check_hit(float x, float y, t_main *m)
{
	if (x < m->x || x > m->x + m->width_sqr || y < m->y || y > m->y + m->height_sqr)
		return (0);
	if (x - m->x < 1 || m->x + m->width_sqr - x < 1 || y - m->y < 1 || m->y + m->height_sqr - y < 1)
		return (2);
	return (1);
}

int	get_sqr(t_main *m)
{
	int	x;
	int	y;
	int hit;

	y = 0;
	if (m->width_sqr <= 0 || m->height_sqr <= 0 || (m->type_sqr != 'R' && m->type_sqr != 'r'))
		return (1);
	while (y < m->height)
	{
		x = 0;
		while (x < m->width)
		{
			hit = check_hit((float)x, (float)y, m);
			if (hit == 2 || (hit == 1 && m->type_sqr == 'R'))
				m->buf[y][x] = m->ch_sqr;
			x++;
		}
		y++;
	}
	return (0);
}

int	start_alg(t_main *m)
{
	int	ret;

	if (get_map(m))
		return (1);
	while ((ret = fscanf(m->file, "%c %f %f %f %f %c\n", &m->type_sqr, &m->x, &m->y, &m->width_sqr, &m->height_sqr, &m->ch_sqr)) == 6)
	{
		if (get_sqr(m))
			return (1);
	}
	if (ret != -1)
		return (1);
	draw_map(m);
	return (0);
}

void	ft_exit(t_main *m)
{
	int x;

	x = 0;
	fclose(m->file);
	while (x < m->height)
	{
		free(m->buf[x]);
		x++;
	}
	free(m->buf);
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
	ft_exit(&m);
}
