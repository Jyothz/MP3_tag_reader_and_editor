#ifndef EDIT_H
#define EDIT_H


#include "types.h"

typedef struct
{
	char *fname;
	FILE *fptr;
	FILE *fptr_cpy;
	char Title[100];
	char Artist[100];
	char Album[100];
	char Year[5];
	char Content[100];
	char Composer[100];
	char *tag;
	char *new_name;
}EditInfo;

Status read_and_validate(char *argv[],EditInfo *editInfo);

Status open_file(EditInfo *editInfo);

Status edit_content(EditInfo *editInfo);

Status edit_tag(EditInfo *editInfo);

Status check_tag(EditInfo *editInfo);

Status copy_data(EditInfo *editInfo);

//void copy_remaining_data(EditInfo *editInfo);

void copy_remaining_data(FILE *fptr_src,FILE *fptr_dest);

#endif
