#include <stdio.h>
#include <string.h>
#include "mp3edit.h"
#include "mp3view.h"

char * tag_name;
Status read_and_validate(char *argv[],EditInfo *editInfo)
{
	if(argv[1]==0 || strcmp(argv[1],"-e"))
	{
		return e_failure;
	}

	if(argv[2])
	{
		if(strcmp(argv[2],"-t")==0)
		{
			editInfo->tag="TIT2";
			tag_name="TITLE";

		}
		else if(strcmp(argv[2],"-a")==0)
		{
			editInfo->tag="TPE1";
			tag_name="ARTIST";
		}
		else if(strcmp(argv[2],"-A")==0)
		{
			editInfo->tag="TALB";
			tag_name="ALBUM";
		}
		else if(strcmp(argv[2],"-y")==0)
		{
			editInfo->tag="TYER";
			tag_name="YEAR";
		}
		else if(strcmp(argv[2],"-m")==0)
		{
			editInfo->tag="TCON";
			tag_name="MUSIC";
		}
		else if(strcmp(argv[2],"-c")==0)
		{
			editInfo->tag="COMM";
			tag_name="COMMENT";
		}
		else
		{
			return e_failure;
		}

	}
	else
	{
		return e_failure;
	}
	if(argv[3])
	{
		editInfo->new_name=argv[3];
	}
	else
	{
		return e_failure;
	}
	if(argv[4] && strcmp(strstr(argv[4],"."),".mp3")==0)
	{
		editInfo->fname=argv[4];
	}
	else
	{
		return e_failure;
	}

	if(open_file(editInfo)==e_failure)
	{
		return e_failure;
	}
	return e_success;
}

Status open_file(EditInfo *editInfo)
{
	if((editInfo->fptr=fopen(editInfo->fname,"r+"))==NULL)
	{
		perror("fopen");
		return e_failure;
	}

	if((editInfo->fptr_cpy=fopen("my_cpy.mp3","w+"))==NULL)
	{
		perror("fopen");
		return e_failure;
	}
	return e_success;
}
int rflag=1;
Status edit_content(EditInfo *editInfo)
{
	char ch[10];
	char str[4];
	fread(ch,10,1,editInfo->fptr);
	fwrite(ch,10,1,editInfo->fptr_cpy);
	while(rflag)
	{
		fread(str,4,1,editInfo->fptr);
		fwrite(str,4,1,editInfo->fptr_cpy);
		str[4]='\0';
		if(strcmp(editInfo->tag,str))
		{
			copy_data(editInfo);
		}
		else
		{
			edit_tag(editInfo);
		}
	}
	copy_remaining_data(editInfo->fptr,editInfo->fptr_cpy);
	rewind(editInfo->fptr);
	rewind(editInfo->fptr_cpy);
	copy_remaining_data(editInfo->fptr_cpy,editInfo->fptr);

	printf("\n-------------%s %s-------------\n\n","CHANGE THE",tag_name);

	printf("%-10.8s: %s\n\n",tag_name,editInfo->new_name);

	printf("--------%s CHANGED SUCCESSFULLY--------\n\n",tag_name);

	return e_success;
}
char ch;
Status copy_data(EditInfo *editInfo)
{
	char size[4];
	int size1;
	fread(size,4,1,editInfo->fptr);
	fwrite(size,4,1,editInfo->fptr_cpy);
	get_size(size,&size1);
	fread(size,2,1,editInfo->fptr);
	fwrite(size,2,1,editInfo->fptr_cpy);
	for(int i=0;i<size1;i++)
	{
		fread(&ch,1,1,editInfo->fptr);
		fwrite(&ch,1,1,editInfo->fptr_cpy);
	}
	return e_success;
}

Status edit_tag(EditInfo *editInfo)
{
	int size=strlen(editInfo->new_name);
	int ssize;
	char buff[10];
	fread(buff,4,1,editInfo->fptr);
	get_size(buff,&ssize);
	for(int i=3;i>=0;i--)
	{
		buff[3-i]=(size>>i*8)&0xff;
	}

	if(strcmp(editInfo->tag,"TYER"))
	{
		buff[3]+=6;
		fwrite(buff,4,1,editInfo->fptr_cpy);
		if(strcmp(editInfo->tag,"COMM")==0)
		{
			fread(buff,8,1,editInfo->fptr);
			fwrite(buff,8,1,editInfo->fptr_cpy);
		}
		else
		{
			fread(buff,5,1,editInfo->fptr);
			fwrite(buff,5,1,editInfo->fptr_cpy);
		}
	}
	else
	{
		buff[3]+=4;
		fwrite(buff,4,1,editInfo->fptr_cpy);
		fread(buff,3,1,editInfo->fptr);
		fwrite(buff,3,1,editInfo->fptr_cpy);
	}

	fwrite(editInfo->new_name,size,1,editInfo->fptr_cpy);
	buff[0]=buff[1]=buff[2]=0;
	fwrite(buff,3,1,editInfo->fptr_cpy);
	fseek(editInfo->fptr,ssize-3,SEEK_CUR);
	rflag=0;
	return e_success;
}

void copy_remaining_data(FILE *fptr_src,FILE *fptr_dest)
{
	char ch;
	while(1)
	{
		fread(&ch,1,1,fptr_src);
		if(feof(fptr_src))
		{
			return;
		}
		fwrite(&ch,1,1,fptr_dest);
	}
}
