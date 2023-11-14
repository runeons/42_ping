#include <stdio.h>
 
#include <linux/ip.h>
#include <linux/icmp.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
 
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
 
#include <time.h>
# include <stdlib.h>
# include <string.h>
// # include <signal.h>
// # include <sys/time.h>
// # include <sys/socket.h>
# include <errno.h>
// # include <arpa/inet.h>
// # include <netinet/ip_icmp.h>



extern int errno;
extern int h_errno;
int Verbose = 1;
 
struct ipheader
{
	unsigned char ip_hl:4,ip_v:4;
	unsigned char ip_tos;
	unsigned short int ip_len;
	unsigned short int ip_id;
	unsigned short int ip_off;
	unsigned char ip_ttl;
	unsigned char ip_p;
	unsigned short int ip_sum;
	unsigned int ip_src;
	unsigned int ip_dst;
};
 
struct icmpheader
{
	unsigned char icmp_type;
	unsigned char icmp_code;
	unsigned short int icmp_cksum;
	unsigned short int icmp_id;
	unsigned short int icmp_seq;
};
 
unsigned short in_cksum(unsigned short * addr,int len)
{
	register int sum     = 0;
	u_short answer       = 0;
	register u_short * w = addr;
	register int nleft   = len;
 
	while(nleft > 1)
	{
		sum += *w++;
		nleft -= 2;
	}
 
	if(nleft == 1)
	{
		*(u_char *)(&answer) = *(u_char *) w;
		sum += answer;
	}
 
	sum = (sum >> 16) + (sum & 0xffff);
 
	sum += (sum >> 16);
	answer = ~sum;
 
	return answer;
}
 
char *GetLocalIP()
{
	struct hostent * Host = gethostbyname((char *)getenv("HOSTNAME"));
	if(Host == NULL)
		return "0.0.0.0";
	char ** adr = Host->h_addr_list;
	return (char *)inet_ntoa(*(struct in_addr *) *adr);
}
 
int Ping(char * IP)
{
	int sock,optval;
	char *packet,*buffer;
 
	struct icmpheader * icmp;
 
	struct sockaddr_in peer;
	struct ipheader * ip;
 
	ip     = (struct ipheader *) malloc(sizeof(struct ipheader));
	icmp = (struct icmpheader *) malloc(sizeof(struct icmpheader));
	packet = (char *)	    malloc(sizeof(struct iphdr) + sizeof(struct icmphdr));
	buffer = (char *)	    malloc(sizeof(struct iphdr) + sizeof(struct icmphdr));
 
	ip   = (struct ipheader *) packet;
	icmp = (struct icmpheader *) (packet + sizeof(struct ipheader));
 
	if(Verbose)
		printf("Creating IP header : ");
 
	ip->ip_hl = 5;
	ip->ip_v  = 4;
	ip->ip_tos = 0;
	ip->ip_len = sizeof(struct ipheader) + sizeof(struct icmpheader);
	ip->ip_id = htonl(54321);
	ip->ip_off = 0;
	ip->ip_ttl = 255;
	ip->ip_p = IPPROTO_ICMP;
	ip->ip_sum = 0;
	char * IpSrc = (char *) malloc(32*sizeof(char));
	strcpy(IpSrc,(char *)GetLocalIP);
	if(strstr(IpSrc,"0.0.0.0"))
	{
		if(Verbose)
			printf("FAILED (unable to resolve local IP)\n");
		return -1;
	}
	ip->ip_src = inet_addr(IpSrc);
	ip->ip_dst = inet_addr(IP);
 
	if(Verbose)
	{
		printf("OK\n");
		printf("Creating socket : ");
	}
	sock = socket(AF_INET,SOCK_RAW,IPPROTO_ICMP);
	if(sock == -1)
	{
		if(Verbose)
			printf("FAILED (%s)\n",strerror(errno));
		return -1;
	}
	if(Verbose)
	{
		printf("OK\n");
		printf("Setting socket options : ");
	}
	int ret = setsockopt(sock,IPPROTO_IP,IP_HDRINCL,&optval,sizeof(int));
	if(ret != 0)
	{
		if(Verbose)
			printf("FAILED (%s)\n",strerror(errno));
		return -1;
	}
	if(Verbose)
	{
		printf("OK\n");
		printf("Creating ICMP header : ");
	}
 
	icmp->icmp_type = 8;
	icmp->icmp_code = 0;
	icmp->icmp_cksum = 0;
	icmp->icmp_cksum = in_cksum((unsigned short *)icmp,sizeof(struct icmpheader));
	icmp->icmp_id = 0;
	icmp->icmp_seq = 0;
 
	peer.sin_family = AF_INET;
	peer.sin_addr.s_addr = inet_addr(IP);
 
	if(Verbose)
	{
		printf("OK\n");
		printf("Sending ICMP packet : ");
	}
 
	int j = 0;
 
	int Start = clock()/1000;
	ret = sendto(sock, packet, ip->ip_len, 0, (struct sockaddr *)&peer, sizeof(struct sockaddr));
	if(ret != ip->ip_len)
	{
		if(Verbose)
			printf("FAILED (not all packet sent)\n");
		return -1;
	}
	if(Verbose)
	{
		printf("OK\n");
		printf("Waiting for ICMP reply : ");
	}
	ret = recv(sock,buffer,sizeof(struct ipheader)+sizeof(struct icmpheader),0);
	if(ret != (sizeof(struct ipheader)+sizeof(struct icmpheader)))
	{
		if(Verbose)
		{
			printf("FAILED (ICMP packet received but may be corrupted (didn't receive expected size))\n");
			printf("Just going on ...\n");
		}
	}
	int End = clock()/1000;
 
	if(Verbose)
	{
		printf("OK (%d ms)\n",End-Start);
	}
	close(sock);
 
	return End-Start;
}

int main(void)
{
	Ping("8.8.8.8");
}