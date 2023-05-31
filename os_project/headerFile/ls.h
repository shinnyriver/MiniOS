#include "./headers.h"

int ls(char* (*argv)[])
{
    DIR *dir = NULL;
    struct dirent *entry = NULL;
    struct stat sb;

    struct passwd *user_pw;
	struct group *g_id;
    int len;
	char* date;

    if((*argv)[1] == NULL) // ls 단일 명령어 = 디렉토리 목록 표시 (.과 ..제외)
    {
        dir = opendir(".");
        while((entry = readdir(dir))!=NULL)
        {
            if (strcmp(".", entry->d_name) == 0 || strcmp("..", entry->d_name) == 0){
                continue;
            }

            printf("%s   ",entry->d_name);
        }
        printf("\n");
        closedir(dir);
    }
    else if(strcmp((*argv)[1], "-a") == 0) // 숨김 속성 파일들 표시
    {
        dir = opendir("."); // .과 ..을 표시 해줘야 한다.
        while((entry = readdir(dir))!=NULL)
        {
                printf("%s   ",entry->d_name);
        }
        printf("\n");
        closedir(dir);

    }
    else if(strcmp((*argv)[1], "-l") == 0) // 파일이나 디렉토리의 세부 정보 표시
    {
        dir = opendir(".");
        while((entry = readdir(dir))!=NULL)
        {
            stat(entry->d_name, &sb);

            user_pw = getpwuid(sb.st_uid); // getpwuid 함수를 이용해 유저네임을 가져온다. (pwd.h)
            g_id = getgrgid(sb.st_gid); // getgrgid 함수를 이용해 그룹 이름을 가져온다. (grp.h)

            date =  ctime(&sb.st_mtime); // 수정한 날짜와 시간을 가져와 ctime 함수로 보기좋게 가공한다.
            len = strlen(date); // ctime 함수에 \n이 포함되어 결과가 뭉게져나옴 그래서 \n을 제거하기위해 길이를잰뒤 뒤에서하나를 떼어내기로 했다..

            if (strcmp(".", entry->d_name) == 0 || strcmp("..", entry->d_name) == 0){
                continue;
            }
            
            printf("%c", S_ISDIR(sb.st_mode)? 'd' : '-');
            printf("%c", sb.st_mode & S_IRUSR? 'r' : '-');
            printf("%c", sb.st_mode & S_IWUSR? 'w' : '-');
            printf("%c", sb.st_mode & S_IXUSR? 'x' : '-');
            printf("%c", sb.st_mode & S_IRGRP? 'r' : '-');
            printf("%c", sb.st_mode & S_IWGRP? 'w' : '-');
            printf("%c", sb.st_mode & S_IXGRP? 'x' : '-');
            printf("%c", sb.st_mode & S_IROTH? 'r' : '-');
            printf("%c", sb.st_mode & S_IWOTH? 'w' : '-');
            printf("%c", sb.st_mode & S_IXOTH? 'x' : '-');
            // 퍼미션 모드를 가져와 rwx와 디렉토리여부 d로 보기좋게 표현하는 파트.
            printf(" %ld\t %s\t %s\t %lld\t %.*s\t %s\n", (long) sb.st_nlink, user_pw->pw_name, g_id->gr_name,(long long) sb.st_size, len -1, date, entry->d_name);
        }
        closedir(dir);
    }
    else if(strcmp((*argv)[1], "-al") == 0) // 숨김 속성과 세부정보 모두 표시
    {
        dir = opendir(".");
        while((entry = readdir(dir))!=NULL)
        {
            stat(entry->d_name, &sb);

            user_pw = getpwuid(sb.st_uid); // getpwuid 함수를 이용해 유저네임을 가져온다. (pwd.h)
            g_id = getgrgid(sb.st_gid); // getgrgid 함수를 이용해 그룹 이름을 가져온다. (grp.h)

            date =  ctime(&sb.st_mtime); // 수정한 날짜와 시간을 가져와 ctime 함수로 보기좋게 가공한다.
            len = strlen(date); // ctime 함수에 \n이 포함되어 결과가 뭉게져나옴 그래서 \n을 제거하기위해 길이를잰뒤 뒤에서하나를 떼어내기로 했다..

            printf("%c", S_ISDIR(sb.st_mode)? 'd' : '-');
            printf("%c", sb.st_mode & S_IRUSR? 'r' : '-');
            printf("%c", sb.st_mode & S_IWUSR? 'w' : '-');
            printf("%c", sb.st_mode & S_IXUSR? 'x' : '-');
            printf("%c", sb.st_mode & S_IRGRP? 'r' : '-');
            printf("%c", sb.st_mode & S_IWGRP? 'w' : '-');
            printf("%c", sb.st_mode & S_IXGRP? 'x' : '-');
            printf("%c", sb.st_mode & S_IROTH? 'r' : '-');
            printf("%c", sb.st_mode & S_IWOTH? 'w' : '-');
            printf("%c", sb.st_mode & S_IXOTH? 'x' : '-');
            // 퍼미션 모드를 가져와 rwx와 디렉토리여부 d로 보기좋게 표현하는 파트.
            printf(" %ld\t %s\t %s\t %lld\t %.*s\t %s\n", (long) sb.st_nlink, user_pw->pw_name, g_id->gr_name,(long long) sb.st_size, len -1, date, entry->d_name);
        }
        closedir(dir);
    }
    else // 해당 경로의 디렉토리 목록 표시 또는 에러
    {
        dir = opendir((*argv)[1]);
        if(dir == NULL)
        {
            printf("failed open\n");
            return 1;
        }
        else
        {
            dir = opendir((*argv)[1]);
            while((entry = readdir(dir))!=NULL)
            {
                printf("%s   ",entry->d_name);
            }
            printf("\n");
            closedir(dir);
        }
    }

    return 0;
}