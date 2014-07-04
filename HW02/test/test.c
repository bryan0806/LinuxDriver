#include<fcntl.h>	//open
#include<unistd.h>	//exit
#include<sys/ioctl.h>	//ioctl
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define IOCTL_CLEAR_MSG _IO(0,0)

static char *dev="/dev/chardev",message[100];
int file_desc;

void clear_mem(){
    if( ioctl(file_desc,IOCTL_CLEAR_MSG,NULL)<0 ){
	printf("ioctl_clear_msg failed\n");
        exit(-1);
    }
}

main()
{
  char choice;
  printf("要做什麼？r)ead w)rite c)lean memory q)uit:");
  scanf("%c",&choice);
  printf("You have choiced %c\n",choice);

  while(choice != 'q'){
  // open the device file, ex: /dev/hello
    if((file_desc=open(dev, O_RDWR))<0){ // if no outer () would handout
	printf("Cannot open device file: %s\n",dev);
	exit(-1);
    }
    
    if(choice == 'r'){  //read from device
    if(read(file_desc,message,sizeof(message))<0){
	printf("read failed\n");
	exit(-1);
    }
    printf("get_msg message:%s\n",message);
    }else if(choice=='w'){
//    memset(message,'\0',100);
    
  // write to the device file
    if(write(file_desc,"wrote into kernel!!\n",19)<0){ // write "wrote" into file_desc
	perror("write failed\n");
    } 
    }else if(choice=='c'){
	clear_mem();
    }
 
  // close file
    close(file_desc);
  
  printf("要做什麼？r)ead w)rite c)lean memory q)uit:");
  scanf("%c",&choice);

  }   
  return 0;
}


