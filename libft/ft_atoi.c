/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htekdemi <htekdemi@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 00:12:45 by htekdemi          #+#    #+#             */
/*   Updated: 2024/10/23 00:12:45 by htekdemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_atoi(const char *str)
{
	int	d;
	int	s;

	d = 1;
	s = 0;
	while (str[s] == 32 || (str[s] <= 13 && str[s] >= 9))
		str++;
	if (*str == '-')
	{
		d = d * -1;
		str++;
	}
	else if (*str == '+')
		str++;
	if (*str == '-' || *str == '+')
		return (0);
	while (*str >= '0' && *str <= '9')
	{
		s = (s * 10);
		s = s + (*str - '0');
		str++;
	}
	return (s * d);
}
