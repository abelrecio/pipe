#include <stdio.h>
#include <stdlib.h>

size_t	ft_strlcpy(char *dest, const char *src, size_t size)
{
	size_t	i;
	size_t	src_len;

	src_len = 0;
	i = 0;
	while (src[src_len])
		src_len++;
	if (size > 0)
	{
		while (i < size - 1 && src[i] != '\0')
		{
			dest[i] = src[i];
			i++;
		}
		dest[i] = '\0';
	}
	return (src_len);
}

int	word_counter(char *str, char sep)
{
	int	count;
	int	i;

	count = 0;
	i = 0;
	while (str[i] != '\0')
	{
		if (str[i] == sep && str[i] != '\0')
			i++;
		if (str[i] != sep && str[i] != '\0')
			count++;
		;
		while (str[i] != sep && str[i] != '\0')
			i++;
	}
	return (count);
}

int	ft_strlen(char *str)
{
	int	count;

	count = 0;
	while (str[count] != '\0')
		count++;
	return (count);
}

void	asigner(char **split, char *arena, const char *str, char sep)
{
	int	i;
	int	j;
	int	word_index;

	i = 0;
	j = 0;
	word_index = 0;
	while (str[i] != '\0')
	{
		while (str[i] == sep && str[i] != '\0')
			i++;
		if (str[i] != '\0')
		{
			split[word_index] = &arena[j];
			while (str[i] != sep && str[i] != '\0')
			{
				arena[j] = str[i];
				i++;
				j++;
			}
			arena[j] = '\0';
			j++;
			word_index++;
		}
	}
	split[word_index] = NULL;
}

char	**ft_split(char const *str, char c)
{
	char	**split;
	char	*arena;
	int		word_count;

	word_count = word_counter(str, c);
	split = malloc((word_count + 1) * sizeof(char *));
	if (!split)
		return (NULL);
	arena = malloc(ft_strlen(str) + 1);
	if (!arena)
		return (NULL);
	asigner(split, arena, str, c);
	return (split);
}

int	main(void)
{
	char	**split;
	int		i;

	i = 0;
	split = ft_split("hola buenas tardes", ' ');
	while (split[0][i] != NULL)
	{
		printf("%s\n", split[i]);
		i++;
	}
	return (0);
}
