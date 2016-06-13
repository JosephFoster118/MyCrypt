//MyCrypt.c
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define false 0
#define true 1
#define bool unsigned char
#define MAX_KEY_SIZE 64

//Encrypts the file
void cryptFile(FILE* f, const char* key, const char* output_name)
{
	uint32_t file_size = 0;
	fseek(f,0,SEEK_END);
	file_size = ftell(f);
	rewind(f);
	char* output = NULL;
	output = (char*) malloc(file_size);
	uint32_t i = 0;
	for(i = 0; i < file_size; i++)
	{
		char data;
		char ops = key[i % strlen(key)]; //the key char
		fread(&data,1,1,f);
		output[i] = data ^ ops;
	}
	
	FILE* out = fopen(output_name,"w");
	
	fwrite(output,1,file_size,out);
	
	fclose(out);
	
	free(output);
}



int main(int argc, char *argv[])
{
	if(argc == 1)
	{
		printf("Type \"MyCrypt -h\" for help\n");
		return 1;
	}
	bool encrypting = false;
	bool decrypting = false;
	bool destroy_orig = false;
	char path[512];
	char key[MAX_KEY_SIZE];
	char output[512];
	memset(path, 0, 512);
	memset(output, 0, 512);
	memset(key, 0, MAX_KEY_SIZE);
	FILE* file = NULL;
	
	
	int c = 0;
	//load args
	while ((c = getopt (argc, argv, "edo:f:k:x")) != -1)
	{
		switch(c)
		{
			case 'e':
			{
				encrypting = true;
			}break;
			case 'd':
			{
				decrypting = true;
			}break;
			case 'f':
			{
				strcpy(path,optarg);
			}break;
			case 'x':
			{
				destroy_orig = true;
			}break;
			case 'o':
			{
				strcpy(output,optarg);
			}
					
		}
	}
	
	
	
	
	if((encrypting == false) && (decrypting == false))
	{
		printf("ERROR: Must define -e or -d\n");
		return 2;
	}
	
	if((encrypting == true) && (decrypting == true))
	{
		printf("ERROR: Cannon define both -e and -d, only one at a time\n");
		return 2;
	}
	
	//print mode
	if(encrypting == true)
	{
		printf("ENCRYPTING mode\n");
	}
	if(decrypting == true)
	{
		printf("DECRYPTING mode\n");
	}
	
	
	//file and key stuffs
	if(strlen(path) == 0)
	{
		printf("Input file: ");
		scanf("%512s",path);
	}
	file = fopen(path,"r");
	if(file == NULL)
	{
		printf("ERROR: could not open file \"%s\"\n",path);
		return 4;
	}
	
	
	
	if((strlen(output) == 0) && (encrypting == true))
	{
		sprintf(output,"%s_CRYPT",path);
		printf("%s\n",output);
	}
	
	
	if((strlen(output) == 0) && (decrypting == true))
	{
		char* loc;
		if((loc = strstr(path,"_CRYPT")) == NULL)
		{
			printf("ERROR: -o not defined and file does not have _CRYPT postfix\n");
			return 3;
		}
		else
		{
			int i = 0;
			for(i = 0; i < (loc - path); i++)
			{
				output[i] = path[i];
			}
		}
	}
	
	if(strlen(key) == 0)
	{
		printf("Key: ");
		char* iput = (char*) malloc(512);
		sprintf(iput,"%%%ds",MAX_KEY_SIZE);
		scanf(iput,key);
		free(iput);
	}
	
	//crypt
	cryptFile(file,key,output);
	
	if(destroy_orig == true)
	{
		remove(path);
	}
	
		
	return 0;
}

