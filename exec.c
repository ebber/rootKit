
/* Kernel Programming */
#define MODULE
#define LINUX
#define __KERNEL__

#include <linux/module.h>  /* Needed by all modules */
#include <linux/kernel.h>  /* Needed for KERN_ALERT */

static int ls_exec(void);

int init_module(void)
{
   printk("starting server\n");
   ls_exec();
	
   // A non 0 return means init_module failed; module can't be loaded.
   return 0;
}

static int ls_exec( void )
{
  int ret =-1;
  struct subprocess_info *sub_info;
  char *argv[] = { "/home/e/a.out", NULL };
  static char *envp[] = {
        "HOME=/",
        "TERM=linux",
        "PATH=/sbin:/bin:/usr/sbin:/usr/bin", NULL };

   printk("make call i \n");
   ret = call_usermodehelper(argv[0], argv, envp, UMH_WAIT_PROC);
   printk("%d \n",ret);
   return ret;
  sub_info = call_usermodehelper_setup( argv[0], argv, envp, GFP_ATOMIC, NULL, NULL,NULL);
  printk("did we setup?");
  if (sub_info == NULL)  {
	printk("FAILED SETUP\n");
	return -ENOMEM;
  }
  printk("SETUP SUCCEEDED");
  return call_usermodehelper_exec( sub_info, UMH_WAIT_PROC );
}


void cleanup_module(void)
{
  printk(KERN_ALERT "Goodbye world 1.\n");
}  

MODULE_LICENSE("GPL");

