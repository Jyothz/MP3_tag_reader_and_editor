#include <stdio.h>
#include <string.h>
#include "types.h"
#include "mp3view.h"
#include "mp3edit.h"
int main(int argc,char *argv[])
{
	int status;
	ViewInfo viewInfo;
	EditInfo editInfo;
	char str[]="-------------------------------------------------------------------------------------------------------------";
	if(argc>1)
	{
		status=check_operation_type(argv[1]);

		if(status==e_view)
		{
			status=read_and_validation(argv[2],&viewInfo);
			if(status==e_success)
			{
				printf("\n%.30sSELECTED VIEW%.30s\n\n",str,str);
				printf("%.73s\n",str);
				printf("%54s\n","MP3 TAG READER AND EDITOR FOR ID3v2");
				printf("%.73s\n",str);
				view_content(&viewInfo);
				printf("%.73s\n\n",str);
				printf("%.22s%s%.21s\n\n",str,"DETAILS DISPLAYED SUCCESSFULLY",str);
			}
			else
			{
				printf("Read input as: ./a.out -v mp3filename\n");
			}

			return 0;
		}
		else if(status == e_edit)
		{
			printf("\n%.30sSELECTED EDIT%.30s\n\n",str,str);
			if(read_and_validate(argv,&editInfo)==e_success)
			{
				printf("\n%.27sSELECTED EDIT OPTION%.26s\n\n",str,str);
				edit_content(&editInfo);
				printf("\n");
			}
			else
			{
				printf("Read input as: ./a.out -e -t/-a/-A/-m/-y/-c changedname mp3filename\n");

			}
			return 0;
		}
		else if(status== e_help)
		{
			printf("%.20sHELP MENU%.20s\n\n",str,str);
			printf("1. -v -> to view mp3 file contents\n");
			printf("2. -e -> to edit mp3 file contents\n");
			printf("%9c2.1 -t -> to edit song title\n",' ');
			printf("%9c2.2 -a -> to edit artist name\n",' ');
			printf("%9c2.3 -A -> to edit album name\n",' ');
			printf("%9c2.4 -y -> to edit year\n",' ');
			printf("%9c2.5 -m -> to edit comment\n",' ');
			printf("%9c2.6 -c -> to edit compose\n",' ');
			printf("%.49s\n",str);
			return 0;
		}
	}
	printf("View -> ./a.out -v mp3filename\n");
	printf("Edit -> ./a.out -e -t/-a/-A/-m/-y/-c changing_text mp3filename\n");
	printf("Hepl -> ./a.out --help\n");
	return 0;
}

