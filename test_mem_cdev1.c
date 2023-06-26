#include<stdio.h>
#include<string.h>
#include<fcntl.h>
#include<stdlib.h>
#include<malloc.h>
#define MAX_BYTES 10
#define DEV_NAME "/dev/MY_MEM"
int main(int argc,char* argv[])
{
int fd;
int ret = 0;
char ch=0;
char *kbuf=NULL;
 if(argc != 2){
                printf(" print <./a.out><name>\n");
                exit(12);
  }
        char device_name[11] = "\0";
       // printf("device_name=%s\n", device_name);
        fd = open(DEV_NAME, O_RDWR);
        if (fd < 0) {
                perror("open call\n");
                return -1;
        }
	printf("test %s \n",argv[1]);	
	ret = write(fd,argv[1],strlen(argv[1]));
	if(ret != strlen(argv[1])){
		printf("error in write\n");
	}	
	//we should knew how many bytes we want
	kbuf = calloc(MAX_BYTES,1);
	if(kbuf == NULL){
		printf("error in calloc\n");
		exit(13);
	}
	ret = read(fd,kbuf,10);
	if(ret < 0){
		printf("error in read\n");
	}	
	printf("in user_space kbuf=%s\n",kbuf);

}
