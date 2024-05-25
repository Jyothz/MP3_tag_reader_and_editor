#include <stdio.h>
#include <string.h>
#include "mp3view.h"
#include "types.h"

OperationType check_operation_type(char *argv)
{
	if(argv)
	{
		if(strcmp(argv,"-v")==0)
		{
			return e_view;
		}
		else if(strcmp(argv,"-e")==0)
		{
			return e_edit;
		}
		else if(strcmp(argv,"--help")==0)
		{
			return e_help;
		}
	}
	return e_unsupported;
}

Status read_and_validation(char *argv,ViewInfo *viewInfo)
{
	if(argv && strcmp(strstr(argv,"."),".mp3")==0)
	{
		viewInfo->fname=argv;
	}
	else
	{
		return e_failure;
	}
	if(open_files(viewInfo)==e_failure)
	{
		printf("INFO: FILE OPENING UNSUCCESSFUL\n");
		return e_failure;
	}

	if(check_header_and_version(viewInfo->fptr_src)==e_failure)
	{
		return e_failure;
	}
	return e_success;
}
Status open_files(ViewInfo *viewInfo)
{
	if((viewInfo->fptr_src=fopen(viewInfo->fname,"r"))==NULL)
	{
		perror("fopen");
		return e_failure;
	}
	return e_success;
}
Status check_header_and_version(FILE *fptr)
{
	char buff[3];
	fread(buff,3,1,fptr);
	if(strcmp(buff,"ID3")!=0)
	{
		return e_failure;
	}
	fread(buff,2,1,fptr);
	if(buff[0]==3 && buff[1]==0)
	{
		return e_success;
	}
	return e_failure;
}
int flag[6]={1,1,1,1,1,1};
Status view_content(ViewInfo *viewInfo)
{
	char str[4];
	fseek(viewInfo->fptr_src,10,SEEK_SET);
	while(flag[0] || flag[1] || flag[2] || flag[3] || flag[4] || flag[5])
	{
		fread(str,1,4,viewInfo->fptr_src);
		check_tags(viewInfo,str);
	}

	print("TITLE",viewInfo->Title+3,viewInfo->size[0]);
	print("ARTIST",viewInfo->Artist+3,viewInfo->size[1]);
	print("ALBUM",viewInfo->Album+3,viewInfo->size[2]);
	print("YEAR",viewInfo->Year,viewInfo->size[3]);
	print("MUSIC",viewInfo->Content_type+3,viewInfo->size[4]);
	print("COMMENT",viewInfo->Composer+6,viewInfo->size[5]);
	return e_success;
}
Status check_tags(ViewInfo *viewInfo,char *str)
{
	char size[4];
	int size1=0;
	static char buff[6][100];
	fread(size,4,1,viewInfo->fptr_src);
	get_size(size,&size1);
	fseek(viewInfo->fptr_src,2,SEEK_CUR);
	if(strcmp(str,"TIT2")==0)
	{
		read_tags(buff[0],size1,viewInfo->fptr_src,&viewInfo->Title,&viewInfo->size[0]);
		flag[0]=0;
	}
	else if(strcmp(str,"TPE1")==0)
	{
		read_tags(buff[1],size1,viewInfo->fptr_src,&viewInfo->Artist,&viewInfo->size[1]);
		flag[1]=0;
	}
	else if(strcmp(str,"TALB")==0)
	{
		read_tags(buff[2],size1,viewInfo->fptr_src,&viewInfo->Album,&viewInfo->size[2]);
		flag[2]=0;
	}
	else if(strcmp(str,"TYER")==0)
	{
		read_tags(buff[3],size1,viewInfo->fptr_src,&viewInfo->Year,&viewInfo->size[3]);
		flag[3]=0;
	}
	else if(strcmp(str,"TCON")==0)
	{
		read_tags(buff[4],size1,viewInfo->fptr_src,&viewInfo->Content_type,&viewInfo->size[4]);
		flag[4]=0;
	}
	else if(strcmp(str,"COMM")==0)
	{
		read_tags(buff[5],size1,viewInfo->fptr_src,&viewInfo->Composer,&viewInfo->size[5]);
		flag[5]=0;
	}
	else
	{
		fseek(viewInfo->fptr_src,size1,SEEK_CUR);
	}
	return e_success;
}
void read_tags(char *buff,int size,FILE *fptr,char **tag,int *tsize)
{
	fread(buff,size,1,fptr);
	*tag=buff;
	*tsize=size;
}
Status get_size(char *s,int *size)
{
	int i;
	for(i=0;i<4;i++)
	{
		(*size)=((*size)|s[i])<<((3-i)*8);
	}
	return e_success;
}
void print(char *str,char *s,int size)
{
	int i;
	printf("%-15.12s%-8s",str,":");
	for(i=0;i<size;i++)
	{
		printf("%c",s[i]);
	}
	printf("\n");
}

