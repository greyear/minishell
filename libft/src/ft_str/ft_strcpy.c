
#include "../../include/libft.h"

char	*ft_strcpy(char *dst, const char *src)
{
	size_t	lens;

	if (!dst || !src) // Can I add these checks if original lib function segfaults here?
		return (NULL);
	lens = ft_strlen(src);
	ft_memcpy(dst, src, lens);
	dst[lens] = '\0';
	return (dst);
}
