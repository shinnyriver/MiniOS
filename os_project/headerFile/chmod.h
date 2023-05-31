#include "./headers.h"

int change_permissions(const char *path, int mode, int recursive) {
    struct stat st;
    if (lstat(path, &st) == -1) {
        perror("lstat 오류");
        return -1;
    }

    if (S_ISDIR(st.st_mode) && recursive) {
        DIR *dir = opendir(path);
        if (dir == NULL) {
            perror("디렉토리 열기 오류");
            return -1;
        }

        struct dirent *entry;
        while ((entry = readdir(dir)) != NULL) {
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
                continue;

            char new_path[PATH_MAX];
            snprintf(new_path, sizeof(new_path), "%s/%s", path, entry->d_name);
            change_permissions(new_path, mode, recursive);
        }

        closedir(dir);
    }

    if (chmod(path, mode) == -1) {
        perror("chmod 오류");
		return -1;
    }
	else return 0;
}

void my_chmod(char* (*argv)[])
{
	int num;
	int size = 256;
	char op;
	int Roption = 0;
        char curDir[256];
        getcwd(curDir, size); //현재 디렉토리 위치
        // printf("[%s]>> ", curDir);

    int arc = 0; 
	extern int optind;

    while ((*argv)[arc] != NULL){
		arc++;
	}
    
	optind = 1;
	while((op = getopt(arc, (*argv), "R")) != -1)
	{
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
		num = strtol((*argv)[1], NULL, 8); //사용자가 숫자를 입력하더라도 일단 문자열로 인식하기 때문에 그 문자열을 숫자로 바꾸기 위해 strtol함수 사용
										 //strtol 함수에서 3번째 인자는 진법을 나타내므로 현재 8진법 // (*argv)[1]부분을 8진수로 바꾸는 부분

		if (num == 0) //strtol함수로 반환된 숫자는 실패했을 경우에도 0이 나오므로 사용자가 진짜로 0을 입력한건지, 또는
		{			  //잘못 입력해서 0이 반환된건지 구분을 해줘야한다.
			if ((*argv)[1] == 0) //사용자가 0을 의도한게 아닌, 다른 문자열을 잘못 입력했을 경우 함수를 종료
			{
				printf("Error: check number\n");
				return;
			}
		}

		if (chmod((*argv)[2], num) == -1) //chmod() 함수로 권한을 바꾸는데 실패했을 경우 에러 메시지 출력
			perror("Error:");
		else
			printf("Success\n");
	}
	else if (arc < 4)
	{
		printf("Error: more arguments are needed\n");
		return;
	}
	else if (Roption)
	{
		num = strtol((*argv)[2], NULL, 8); // (*argv)[2]부분을 8진수로 바꾸는 부분

		if(change_permissions((*argv)[3], num, Roption) == -1)
			perror("Error: ");
		else
			printf("Success\n");
	}
}
