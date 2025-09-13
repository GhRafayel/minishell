/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rghazary <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 10:53:27 by rghazary          #+#    #+#             */
/*   Updated: 2024/09/13 10:53:29 by rghazary         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static long int	ft_itoa_length(int n)
{
	int	len;

	len = 0;
	if (n <= 0)
	{
		len++;
	}
	while (n != 0)
	{
		len++;
		n = n / 10;
	}
	return (len);
}

static char	*ft_itoa_put_item(char *str, unsigned int num, long int len)
{
	while (num > 0)
	{
		str[len--] = 48 + (num % 10);
		num /= 10;
	}
	return (str);
}

char	*ft_itoa(int n)
{
	long int		len;
	unsigned int	num;
	char			*str;

	len = ft_itoa_length(n);
	str = (char *)malloc((len + 1) * sizeof(char));
	if (str == NULL)
		return (NULL);
	str[len--] = '\0';
	if (n == 0)
		str[0] = '0';
	if (n < 0)
	{
		num = n * -1;
		str[0] = '-';
	}
	else
		num = n;
	str = ft_itoa_put_item(str, num, len);
	return (str);
}
