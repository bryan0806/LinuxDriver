#include <linux/module.h> /* Specifically, a module */
#include <linux/kernel.h> /* We're doing kernel work */
#include <linux/proc_fs.h> /* Necessary because we use the proc fs */

//To do: declare a struct proc_dir_entry
#define PROCFS_NAME "test"
struct proc_dir_entry * Our_Proc_File;

ssize_t procfile_read(char *buffer,char **buffer_location,off_t offset, int buffer_length, int *eof, void *data)
{
  // To do: return a message to user space
  int len=0;
  static int count = 0;

  if(offset > 0) { // 表示buffer 裡面還有東西沒有被讀取
    len = 0;
  }else{
    //Fill the buffer and get its length
    len = sprintf(buffer,"For the %d time, go away!\n",count++);
  }
  

  return len;
}

int init_module()
{
    int rv = 0;

    // To do: create a proc entry and register its callback function
    Our_Proc_File = create_proc_entry(PROCFS_NAME, 0644, NULL);
    printk("<1>Trying to create /proc/%s:\n", PROCFS_NAME);

    if(Our_Proc_File == NULL){
	rv = -ENOMEM;
    }else{
	Our_Proc_File -> read_proc = procfile_read;// 這邊我們只註冊讀取函式
    }


    return rv;
}

void cleanup_module()
{
    // To do: remove the proc entry
    remove_proc_entry(PROCFS_NAME,NULL);
    printk(KERN_INFO "/proc/test removed\n");
}
