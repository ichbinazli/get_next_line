/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naanapa <naanapa@student.42kocaeli.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 12:29:24 by naanapa           #+#    #+#             */
/*   Updated: 2024/03/05 13:59:19 by naanapa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <unistd.h>

static char	*copy_to_stash(char *stash, char *buff)
{
	char	*res;							//Sonuç karakter dizisini tutacak

	res = 0;								//NULL olarak başlatılır
	if (!stash && buff)						//Stash boşsa ve buff doluysa, buff'ın kopyası alınır.
	{
		res = ft_strdup(buff); 				//verilen bir karakter dizisinin kopyasını oluşturur ve bu kopyanın bir işaretçisini döndürür.
		if (!res)
			return (NULL);
		return (res);
	}
	res = ft_strjoin(stash, buff);			//Eğer stash doluysa veya buff boşsa, stash ile buff birleştirilir.
	free_stash(&stash, 0);
	return (res);
}

static int	have_nl(char *s)
{
	size_t	i;								//Satırda yeni satır karakterinin bulunup bulunmadığını belirlemek için

	if (!s)
		return (0);
	i = -1;
	while (s[++i] != '\0')
		if (s[i] == '\n')					//Karakter dizisi içinde \n kontrol
			return (1);						//yeni satır karakteri bulunursa, true
	return (0);								//yeni satır karakteri bulunamazsa false
}

static char	*extract_line(char *stash)
{
	char	*line;							//Satırın kopyalanacağı dizi
	size_t	i;
	size_t	j;

	i = 0;
	if (!stash)
		return (0);
	while (stash[i] != '\n')				//Yeni satır karakterinin konumunu bulmak için
		i++;
	line = malloc(sizeof(char) * (i + 2));	//Satırı kopyalamak için bellek tahsis edilir. '\n' karakteri de kopyalanacağı için +2 eklenir.
	if (!line)
		return (0);
	j = 0;
	while (j < i + 1)
	{
		line[j] = stash[j];					// Satır, stash'ten line'a kopyalanır.
		j++;
	}
	line[j] = '\0';
	return (line);
}

static char	*recreate_stash(char *stash)
{
	size_t	i;
	char	*res;							//Yeniden oluşturulmuş stash'i tutacak bir karakter dizisi

	i = 0;
	if (!stash)
		return (NULL);
	while (stash[i] != '\n')				//Yeniden oluşturma işlemi için yeni satır karakterinin konumunu bulmak için
		i++;
	if (stash[i + 1] == '\0')				//Yeniden oluşturma sonunda, satır sonuna geldiyse NULL döndürür.
		return (free_stash(&stash, 0));
	res = ft_substr(stash, i + 1, ft_strlen(stash));//Yeni stash, orijinal stash'ten satır sonrasını kopyalayarak oluşturulur.
	if (!res)								//Tahsis başarısızsa mevcut stash bellekten serbest bırakılır ve NULL döndürülür.
	{
		free_stash(&stash, 0);
		return (NULL);
	}
	free_stash(&stash, 0);					// Yeniden oluşturma tamamsa orijinal stash bellekten serbest bırakılır
	return (res); 							//ve yeniden oluşturulmuş stash döndürülür.
}

char	*get_next_line(int fd)
{
	char		buf[BUFFER_SIZE + 1];		//Bellekte okunan veriyi geçici olarak tutacak
	long		bytes_read;					//Okunan bayt sayısını takip etmek için
	static char	*stash = NULL;				//Önceki çağrılardan kalan veriyi saklamak için
	char		*line;						//Okunan satırın saklanacağı karakter dizisi

	line = 0;
	bytes_read = BUFFER_SIZE;
	if (fd < 0 || BUFFER_SIZE <= 0)			//Dosya tanımlayıcısının geçerliliği ve bellek boyutunun pozitif olup olmadığı kontrol edilir.
		return (free_stash(&stash, 0));
	while (bytes_read > 0)
	{
		bytes_read = read(fd, buf, BUFFER_SIZE);
		if ((bytes_read <= 0 && !stash) || bytes_read == -1)
			return (free_stash(&stash, 0));
		buf[bytes_read] = '\0';
		stash = copy_to_stash(stash, buf);	//Okunan veri stash alanına kopyalanır.
		if (have_nl(stash))					//Stash'te yeni satır karakteri var mı diye kontrol edilir.
		{
			line = extract_line(stash);		//Yeni satır karakteri bulunduğunda, stash'ten bir satır çıkarılır.
			if (!line)
				return (free_stash(&stash, 0));	//Satır çıkarılamazsa bellek serbest bırakılır ve işlev sonlandırılır.
			return (stash = recreate_stash(stash), line);	//Satır başarıyla çıkarıldıysa, kalan veri stash'te kalacak şekilde güncellenir ve satır döndürülür.
		}
	}
	return (free_stash(&stash, 1));
}
