#include "./headers.h"

bool checkColon(const char* str) {
    while (*str != '\0') {
        if (*str == ':') {
            return true;
        }
        str++;
    }
    return false;
}

void removeCharacter(char* str, char ch) {
    int len = strlen(str);
    int i, j;

    for (i = j = 0; i < len; i++) {
        if (str[i] != ch) {
            str[j] = str[i];
            j++;
        }
    }
    str[j] = '\0';
}

void my_chown(char* (*argv)[], int arc)
{
    int num;
    int size = 256;
    char op;
    int Roption = 0;
    extern int optind;

    char curDir[256];
    getcwd(curDir, size);

    optind = 1;

	while((op = getopt(arc, (*argv), "R")) != -1)
	{
        printf("op:%d\n",op);
		switch(op)
		{
			case 'R':
				Roption = 1;
				break;
			case '?':
				return;
			default:
				printf("Error: please try again\n");
				return;
		}
	}

    if (arc < 3) //chmod 명령어는 파일 접근 권한을 위한 인자와 파일명을 위한 인자 2가지가 필요하므로 하나라도 없으면 에러 출력!
	{
		printf("Error: %d more arguments are needed\n", arc);
	}
    else if (Roption == 0)
	{
		//(*argv)[0] -> chown
        //(*argv)[1] -> o:g
        //(*argv)[2] -> filename

        char* group_name;
        char* owner_name;
        char* o_g = (*argv)[1];

        char* tmp[2];
        int cnt = 0;

        if(checkColon(o_g))
        {
            int len = strlen(o_g);
            if(o_g[0] == ':') // :[groupname]
            {
                struct stat file_stat;

                if(stat((*argv)[2], &file_stat) == -1){
                    perror("stat");
                    return;
                }

                removeCharacter(o_g, ':');

                struct group* group_info = getgrnam(o_g);

                if(group_info == NULL){
                    fprintf(stderr, "Failed to get GID for the specified user or group.\n");
                    return;
                } 
                gid_t group = group_info->gr_gid;
                    
                if (chown((*argv)[2], file_stat.st_uid, group) == -1) {
                    perror("chown");
                    return;
                }

                printf("chown succeeded.\n");
            }
            else if(o_g[len-1] == ':') // [username]:
            {
                struct stat file_stat;

                if(stat((*argv)[2], &file_stat) == -1){
                    perror("stat");
                    return;
                }
                printf("***start****\n");
                struct passwd* owner_info = getpwnam(o_g);
                printf("*******\n");
                if(owner_info == NULL){
                    fprintf(stderr, "Failed to get UID for the specified user or group.\n");
                    return;
                }
                uid_t owner = owner_info->pw_uid;

                if (chown((*argv)[2], owner, file_stat.st_gid) == -1) {
                    perror("chown");
                    return;
                }

                printf("chown succeeded.\n");
            }
            else
            {
                char* tmp[2];
                int idx = 0;

                char* token = strtok(o_g, ":");
                while (token != NULL) {
                    tmp[idx] = token;
                    idx++;
                    
                    token = strtok(NULL, ":");
                }
                
                struct group* group_info = getgrnam(tmp[1]); 
                struct passwd* owner_info = getpwnam(tmp[0]);

                if (owner_info == NULL || group_info == NULL) {
                    fprintf(stderr, "Failed to get UID or GID for the specified user or group.\n");
                    return;
                }

                gid_t group = group_info->gr_gid;
                uid_t owner = owner_info->pw_uid;

                if (chown((*argv)[2], owner, group) == -1) {
                    perror("chown");
                    return;
                }

                printf("chown succeeded.\n");
            }
        }
        else
        {
            struct stat file_stat;

            if(stat((*argv)[2], &file_stat) == -1){
                perror("stat");
                return;
            }
            
            struct passwd* owner_info = getpwnam(o_g);
            uid_t owner = owner_info->pw_uid;

            if (chown((*argv)[2], owner, file_stat.st_gid) == -1) 
            {
                perror("chown");
                return;
            }
            printf("chown succeeded.\n");
        }
    }
}