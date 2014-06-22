#include<fcntl.h>	//open
#include<unistd.h>	//exit
#include<sys/ioctl.h>	//ioctl
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
static char *dev="/dev/chardev",message[100];

main()
{
  int file_desc;

  // open the device file, ex: /dev/hello
    if((file_desc=open(dev, O_RDWR))<0){ // if no outer () would handout
	printf("Cannot open device file: %s\n",dev);
	exit(-1);
    }
  // read from the device file
    
//    memset(message,'\0',100);
    if(read(file_desc,message,sizeof(message))<0){
	printf("read failed\n");
	exit(-1);
    }
    printf("get_msg message:%s\n",message);
    

  // write to the device file
    if(write(file_desc,"wrote into kernel!!",19)<0){ // write "wrote" into file_desc
	perror("write failed\n");
    } 
 
  // close file
    close(file_desc);
  return 0;
}
