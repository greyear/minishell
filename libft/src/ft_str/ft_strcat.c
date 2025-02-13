
#include "../../include/libft.h"

char	*ft_strcat(char *dst, const char *src)
{
	size_t	lend;
	size_t	lens;

	if (!dst || !src) // Can I add these checks if original lib function segfaults here?
		return (NULL);
	lend = ft_strlen(dst);
	lens = ft_strlen(src);
	ft_memcpy(dst + lend, src, lens);
	dst[lend + lens] = '\0';
	return (dst);
}
