#include <stdio.h>

int main()
{
	char in;
	char data[1000];
	int p = 0;
	while((in = getchar()) != '\n'){
		if((in != ' '))
			data[p++] = in;
	}
	puts(data);
	return 0;
}
