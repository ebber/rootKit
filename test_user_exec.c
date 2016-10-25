#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

static int ls_exec(void);

int main(){
   printf("starting server\n");
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
	"PWD=/",
        "TERM=linux",
        "PATH=/sbin:/bin:/usr/sbin:/usr/bin", NULL };

   printf("make call i \n");
   ret = execve(argv[0], argv, envp);
   printf("%d %s \n",ret, strerror(errno));
   return ret;
}




