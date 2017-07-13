/**********************************************************************
           DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
                    Version 2, December 2004

 Copyright (C) 2017 Yeonji <yeonji@ieee.org>

 Everyone is permitted to copy and distribute verbatim or modified
 copies of this license document, and changing it is allowed as long
 as the name is changed.

            DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
   TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION

  0. You just DO WHAT THE FUCK YOU WANT TO.
**********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/stat.h>

#define block_size 128

char * input_from_stdin(void)
{
	char * buf = NULL;
	char * buf_new = NULL;

	size_t buf_size = 0;
	size_t buf_index = 0;

	char input;

	buf_new = malloc(sizeof(128*block_size));

	if(buf_new == NULL){
		printf("\n\033[31mFATAL: Cannot malloc memory for input\033[0m\n");
		exit(-1);
	}

	buf = buf_new;
	buf_size += block_size;

	printf("==> ");

	while((input=getchar())!= EOF){
		if(buf_index==buf_size){
			/* extend memory */
			buf_new = realloc(buf,(buf_size+block_size)*sizeof(char));
			if(buf_new == NULL){
				printf("\n\033[31mFATAL: Cannot realloc memory for input\033[0m\n");
				free(buf);
				exit(-1);
			}
			buf = buf_new;
			buf_size += block_size;
		}

		if (input!='\n'){
			buf[buf_index]=input;
			buf_index++;
		}else{
			printf("==> ");
		}
		
	}

	buf[buf_index]='\0';

	printf("\n\n");

	return buf;
}

char * input_from_file(const char * file_name)
{
	struct stat statbuf;
	FILE * fp = NULL;
	size_t size;

	char * buf = NULL;
	size_t buf_index = 0;

	char ch;

	stat(file_name,&statbuf);
    size=statbuf.st_size;

    fp = fopen(file_name, "r");

    if(fp==NULL){
    	printf("\n\033[31mFATAL: Cannot open input file\033[0m\n");
    	exit(-1);
    }

    buf = malloc((size+1)*sizeof(char));

    while(fread(&ch,1,1,fp)>0){
    	buf[buf_index]=ch;
    	buf_index++;
    }

    buf[buf_index]='\0';

    fclose(fp);

    return buf;
}

int main(int argc, const char ** argv)
{
	int * cells = NULL;
	size_t cell_index = 0;
	size_t cell_size = 0;

	char * bf = NULL;
	size_t bf_index = 0;

	int bracket = 0;
	char ch;

	if(argc == 1){
		/* NONE args, enter interactive enviorment */
		bf = input_from_stdin();
	}else if(argc == 2){
		bf = input_from_file(argv[1]);
	}else{
		printf("\n\033[31mIllegal input args\033[0m\n");
		exit(-1);
	}

	cells = malloc(128*sizeof(int));
	cell_size = 128;

	for(bf_index = 0; bf[bf_index]; bf_index++) {
		ch = bf[bf_index];
		if(isspace(ch)) // skip whitespace 
			continue;

		//interpret brainfuck
		switch (ch){
			case '+': // increment pointer value
				cells[cell_index]++;
				break;
			case '-': // decrement pointer value
				cells[cell_index]--;
				break;
			case '>': // increment pointer
				if(cell_index + 1 != cell_size) // check if next is initialized
					cell_index++;
				else
				{
					cells = (int *)realloc(cells, sizeof(int) * ++cell_size);
					cell_index++;
					cells[cell_index] = 0;
				}
				break;
			case '<': // decrement pointer
				if(cell_index > 0)
					cell_index--;
				else
				{
					printf("ERROR: tried to access invalid (less than 0) cell\n");
					return 1;
				}
				break;
			case '.': // output pointer value
				putchar(cells[cell_index]);
				break;
			case ',': // read in value
				cells[cell_index] = getchar();
				break;
			case '[': // start bracket loop // loops could probably be faster
				if(!cells[cell_index]){
					bracket++;
					while(bracket){
						bf_index++;
						if(bf[bf_index] == ']')
							bracket--;
						else if(bf[bf_index] == '[')
							bracket++;
					}
				}
				break;
			case ']': // end bracket loop
				if(cells[cell_index]){
					bracket++;
					while(bracket){
						bf_index--;
						if(bf[bf_index] == '[')
							bracket--;
						else if(bf[bf_index] == ']')
							bracket++;
					}
				}
				break;
			default:
				printf("ERROR: non-brainfuck character encountered: %d\n", (int)ch); 
				// print error and ascii of error causing character
				// change this later to allow for comments
			return 1;
		}
	}
}