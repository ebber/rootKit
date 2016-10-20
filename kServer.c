
/* Kernel Programming */
#define MODULE
#define LINUX
#define __KERNEL__

#include <linux/module.h>  /* Needed by all modules */
#include <linux/kernel.h>  /* Needed for KERN_ALERT */
#include <linux/init.h>
#include <linux/slab.h>

#include <linux/errno.h>
#include <linux/types.h>

#include <linux/netdevice.h>
#include <linux/ip.h>
#include <linux/in.h>
#include <linux/socket.h>

#include <linux/ctype.h>
#include <net/sock.h>
#include <net/tcp.h>
#include <net/inet_connection_sock.h>
#include <net/request_sock.h>

#define MAX_CLIENTS 1
#define DEFAULT_PORT 2325  //handoff maybe ?
#define CONNECT_PORT 1337
#define MODULE_NAME "kServer"

struct socket listen_sock;

int socket_write(struct socket *sock, char *buf, int len);
int server_listen(void);

int init_module(void)
{
   printk("STARTING SERVER\n");
   server_listen();
	
   // A non 0 return means init_module failed; module can't be loaded.
   return 0;
}

int server_listen(void) {
    int err;
    struct socket *socket, *csock;
    struct sockaddr_in sin;

    
    err  = sock_create(AF_INET,SOCK_STREAM,IPPROTO_TCP,&socket);

    if(err<0) {
	printk(KERN_ERR "CREATE SOCKET ERROR\n");
	return -1;
    }

    sin.sin_addr.s_addr = htonl(INADDR_ANY);
    sin.sin_family = AF_INET;
    sin.sin_port=htons(CONNECT_PORT);

    err = socket->ops->bind(socket,(struct sockaddr*)&sin,sizeof(sin));
    if(err<0) {
	printk(KERN_ERR "BIDN ADDRESS ERROR\n");
	return -1;
    }
    
   err = socket->ops->listen(socket, MAX_CLIENTS);
    if(err<0) {
	printk(KERN_ERR "LISTEN ERROR\n");
	return -1;
    }

   //the socket we will pass off the client too
   csock = (struct socket*) kmalloc(sizeof(struct socket), GFP_KERNEL);
   //possible that 3rd arg is length of connection socket 
   err = socket->ops->accept(socket, csock, O_NONBLOCK);
    if(err<0) {
	printk(KERN_ERR "ACCEPT ERROR\n");
        sock_release(csock);
        kfree(csock);
	return err;
    }

   socket_write(csock, "PLS\n\0",5);

    sock_release(csock);
    kfree(csock);
    return 0;

}

int socket_write(struct socket *sock, char *buf, int len) {
  struct msghdr msg;
  struct iovec iov; //no understanding of this rn
  struct iov_iter iter;
  mm_segment_t oldfs; //weird shit bc have to find the true file sytem but still restoring
  printk(KERN_INFO "sending");
	iov.iov_base=buf;
	iov.iov_len=len;
 
        iov_iter_init(&iter, 0, &iov, 1, 1); 

	//not understood
	msg.msg_control=NULL;
	msg.msg_controllen=0;
	msg.msg_flags=0;
        msg.msg_iter = iter;
	//msg.msg_iov=&iov;
	//msg.msg_iovlen=1;
	msg.msg_name=0;
	msg.msg_namelen=0;

	oldfs=get_fs();
	set_fs(KERNEL_DS);
	printk(KERN_INFO "ktcp_send.sock_sendmsg");
	int size;	
	size=sock_sendmsg(sock,&msg,len);
	
	printk(KERN_INFO "message sent!");
	set_fs(oldfs);

  printk(KERN_INFO "Think we have successful snd");
  return size; //assume length of packet??
}


void cleanup_module(void)
{
  printk(KERN_ALERT "Goodbye world 1.\n");
}  

MODULE_LICENSE("GPL");

