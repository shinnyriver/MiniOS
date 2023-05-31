#include "./headers.h"

void* makedir(void* temp_name)
{
    int check;
    char* fname = (char*)temp_name;
    check = mkdir(fname, 0755);
    if (check < 0)
    {
        perror("디렉토리 생성 오류 \n");
    }
    else
    {
        printf("dir made\n");
    }
}

void Mmkdir(int argc, char* argv[])//여러개의 디렉토리를 생성하는 함수.
{
    int num = argc - 1; //입력받은 함수&명령어의 개수 조작
    pthread_t p_thread[num];    //멀티스레드 생성
    int thr_check;
    if (!strcmp(argv[0], "mkdir")) {
        for (int i = 0; i < num; i++)
        {
            printf("*%s*\n",argv[i+1]);
            thr_check = pthread_create(&p_thread[i], NULL, makedir, (void*)argv[i + 1]);//스레드를 생성하여 makedir함수를 실행
            if (thr_check < 0)
            {
                perror("thread create error : \n");
            }
        }
    }
    for (int i = 0; i < num; i++)
    {
        pthread_join(p_thread[i], NULL);//스레드가 종료될떄까지 기다려줌
    }
}

void Omkdir(char* op, char* filename, char* filemode)//하나의 디렉토리를 생성하는 옵션
{
    int result = 0;
    int mode = (filemode[0] - 48) * 64 + (filemode[1] - 48) * 8 + (filemode[2] - 48);
    //int mode = strtol(filemode, NULL, 8);
    if (!strcmp(op, "-p"))
    {
        int t = strlen(filename);
        for (int i = 0; i <= t; i++){
            if (filename[i] == '/' || filename[i] == '\0') {//파일과 디렉토리를 / 기준으로 나눠줌
                filename[i] = '\0';//디렉토리 이름이 완료됐음을 표시해줌
                result = mkdir(filename, mode);
                filename[i] = '/';
                if(result<0)
                  printf("파일 생성에 실패했습니다\n");
            }
        }

    }

    else {
        result = mkdir(filename, mode);
        if(result<0)
          printf("같은 파일이거나 파일 생성에 실패했습니다\n");
    }
}