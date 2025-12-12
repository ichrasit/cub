/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htekdemi@student.42kocaeli.com.tr <htek    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/20 15:59:58 by htekdemi@st       #+#    #+#             */
/*   Updated: 2024/10/20 15:59:59 by htekdemi@st      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t	s;
	size_t	d;
	size_t	di;
	size_t	si;

	si = ft_strlen(src);
	if (!dst && dstsize == 0)
		return (si);
	d = ft_strlen(dst);
	di = d;
	if (dstsize <= di)
		return (dstsize + si);
	s = 0;
	while (src[s] && d + 1 < dstsize)
	{
		dst[d] = src[s];
		s++;
		d++;
	}
	dst[d] = '\0';
	return (di + si);
}
