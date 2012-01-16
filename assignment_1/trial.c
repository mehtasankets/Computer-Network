#include <stdio.h>

void main()
{
	char data[128];
	FILE *fp = fopen("sanket", "r");
	while(fscanf(fp, "%s", data) != EOF)
	{
		printf("Here : %s\n", data);
	}
}
