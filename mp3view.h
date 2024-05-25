#ifndef MP3_VIEW
#define MP3_VIEW

#include "types.h"

typedef struct
{
	char *Title;
	char *Artist;
	char *Album;
	char *Content_type;
	char *Composer;
	char *Year;
	int size[6];
	/*file name and file pointer*/
	char *fname;
	FILE *fptr_src;
}ViewInfo;


OperationType check_operation_type(char *argv);

Status read_and_validation(char *argv,ViewInfo *viewInfo);

Status open_files(ViewInfo *viewInfo);

Status view_content(ViewInfo *viewInfo);

Status check_header_and_version(FILE * fptr);

Status check_tags(ViewInfo *viewInfo,char *str);

Status get_size(char *s,int *size);

void print(char *str,char *s,int size);

void get_tags(int size,FILE *fptr,char *tag);

void read_tags(char *buff,int size,FILE *fptr,char **tag,int *tsize);
#endif
