/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   g_micro_paint.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sjacki <sjacki@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/19 23:36:12 by alexandr          #+#    #+#             */
/*   Updated: 2021/10/21 20:28:39 by sjacki           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

typedef struct s_main
{
	FILE			*file;
	int				width;
	int				height;
	char			back_ch;
	char			**buf;

	char			type_sqr;
	char			ch_sqr;
	float			pos_x_sqr;
	float			pos_y_sqr;
	float			width_sqr;
	float			height_sqr;
}					t_main;

int	ft_putstr(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		write(1, &str[i], 1);
		i++;
	}
	return (i);
}

int	ft_err(char *str)
{
	ft_putstr("Error: ");
	ft_putstr(str);
	ft_putstr("\n");
	return (1);
}

int	init_struct(t_main *mn, FILE *file)
{
	mn->back_ch = 0;
	mn->buf = NULL;
	mn->ch_sqr = 0;
	mn->file = file;
	mn->height_sqr = 0;
	mn->width_sqr = 0;
	mn->width = 0;
	mn->height = 0;
	mn->pos_x_sqr = 0;
	mn->pos_y_sqr = 0;
	mn->type_sqr = 0;
	return (0);
}

int	get_map(t_main *mn)
{
	int	scan_ret;
	int	x;
	int	y;

	x = 0;
	if ((scan_ret = fscanf(mn->file, "%d %d %c\n", &mn->width, &mn->height, &mn->back_ch)) != 3)
		return (1);
	if (mn->width <= 0 || mn->width > 300 || mn->height <= 0 || mn->height > 300)
		return (1);
	if (!(mn->buf = (char **)malloc(mn->height * sizeof(char *))))
		return (1);
	while (x < mn->height)
	{
		y = 0;
		if (!(mn->buf[x] = (char *)malloc(mn->width * sizeof(char *))))
			return (1);
		while (y < mn->width)
		{
			mn->buf[x][y] = mn->back_ch;
			y++;
		}
		x++;
	}
	return (0);
}

int		check_hit(float x, float y, t_main *mn)
{
	if (x < mn->pos_x_sqr || x > mn->pos_x_sqr + mn->width_sqr || y < mn->pos_y_sqr \
		|| y > mn->pos_y_sqr + mn->height_sqr)
		return (0);
	if (x - mn->pos_x_sqr < 1 || mn->pos_x_sqr + mn->width_sqr - x < 1 \
	|| y - mn->pos_y_sqr < 1 || mn->pos_y_sqr + mn->height_sqr - y < 1)
		return (2);
	return (1);
}

int	get_sqr(t_main *mn)
{
	int	x;
	int	y;
	int	hit;

	y = 0;
	if (mn->width_sqr <= 0 || mn->height_sqr <= 0 || \
		(mn->type_sqr != 'R' && mn->type_sqr != 'r'))
		return (1);
	while (y < mn->height)
	{
		x = 0;
		while (x < mn->width)
		{
			hit = check_hit((float)x, (float)y, mn);
			if (hit == 2 || (hit == 1 && mn->type_sqr == 'R'))
				mn->buf[y][x] = mn->ch_sqr;
			x++;
		}
		y++;
	}
	return (0);
}

int			draw_map(t_main *mn)
{
	int	x;
	int	y;

	x = 0;
	while (x < mn->height)
	{
		y = 0;
		while (y < mn->width)
		{
			write (1, &mn->buf[x][y], 1);
			y++;
		}
		write (1, "\n", 1);
		x++;
	}
	return (0);
}

int			start_alg(t_main *mn)
{
	int		scan_ret;
	if (get_map(mn))
		return (1);
	while ((scan_ret = fscanf(mn->file, "%c %f %f %f %f %c\n", &mn->type_sqr, &mn->pos_x_sqr, &mn->pos_y_sqr, &mn->width_sqr, &mn->height_sqr, &mn->ch_sqr)) == 6)
	{
		if (get_sqr(mn))
			return (1);
	}
	if (scan_ret != -1)
			return (1);
	if (draw_map(mn))
	 	return (1);
	return (0);
}

int			main(int argc, char **argv)
{
	FILE	*file;
	t_main	mn;

	if (argc != 2)
		return (ft_err("argument"));
	if (!(file = fopen(argv[1], "r")))
		return (ft_err("Operation file corrupted"));
	init_struct(&mn, file);
	if (start_alg(&mn))
		return (ft_err("Operation file corrupted"));
	return (0);
}
