//#include <stdio.h>
//#include <sys/types.h>
//#include <sys/socket.h>
//#include <netinet/in.h>
//#include <arpa/inet.h>
//#include <netdb.h>
//#include <unistd.h>
//#include <string.h>
//#include <stdlib.h>
//#include <netinet/ip_icmp.h>
//#include <time.h>
//#include <fcntl.h>
//#include <signal.h>
//#include <time.h>
//#include <errno.h>
//
//// IPv4 header len without options
//#define IP4_HDRLEN 20
//
//// ICMP header len for echo req
//#define ICMP_HDRLEN 8
//
//// ping packet size
//#define PING_PKT_S 64
//// ping packet structure
//struct ping_pkt
//{
//    struct icmphdr hdr;
//    char msg[PING_PKT_S-sizeof(struct icmphdr)];
//};
//    /*
//     * Checksum Algo
//     */
//// Compute checksum (RFC 1071).Checksum algo
////check that the package we expect to get really arrive
//
//unsigned short calculate_checksum(unsigned short *paddress, int len) {
//    int nleft = len;
//    int sum = 0;
//    unsigned short *w = paddress;
//    unsigned short answer = 0;
//
//    while (nleft > 1) {
//        sum += *w++;
//        nleft -= 2;
//    }
//
//    if (nleft == 1) {
//        *((unsigned char *) &answer) = *((unsigned char *) w);
//        sum += answer;
//    }
//
//    // add back carry outs from top 16 bits to low 16 bits
//    sum = (sum >> 16) + (sum & 0xffff); // add hi 16 to low 16
//    sum += (sum >> 16);                 // add carry
//    answer = ~sum;                      // truncate to 16 bits
//
//    return answer;
//}
//
//
//int main() {
//    long double rtt_msec = 0;
//    struct timespec time_start, time_end, tfs, tfe;
//    struct ping_pkt pckt;
//    char message[256];
//    unsigned long dataLen = strlen(message) + 1;
//    int flag=1;
//    /*
//     * Create a Raw Socket
//     *
//     * Create a raw socket for IP-RAW (make IP-header by yourself)
//     */
//
//    int sock = -1;
//    if ((sock = socket(AF_INET, SOCK_RAW, IPPROTO_RAW)) == -1) {
//        fprintf(stderr, "socket() failed");
//        fprintf(stderr, "To create a raw socket, the process needs to be run by Admin/root user.\n\n");
//        return -1;
//    }
//
//    /*
//    * Create our own IP header
//    */
//    // This socket option IP_HDRINCL says that we are building IPv4 header by ourselves, and
//    // the networking in kernel is in charge only for Ethernet header.
//
//    const int flagOne = 1;
//    if (setsockopt(sock, IPPROTO_IP, IP_HDRINCL, &flagOne, sizeof(int)) == -1) {
//        fprintf(stderr, "setsockopt() failed");
//        return -1;
//    }
//
//    //filling packet
//    bzero(&pckt, sizeof(pckt));
//
//    pckt.hdr.type = ICMP_ECHO;
//    pckt.hdr.un.echo.id = getpid();
//    uint i = 0;
//    for (i = 0; i < sizeof(pckt.msg)-1; i++ )
//        pckt.msg[i] = i+'0';
//
//    pckt.msg[i] = 0;
//    pckt.hdr.checksum = calculate_checksum((unsigned short *) &pckt, sizeof(pckt));
//
//
//
//    struct ip iphdr; // IPv4 header- for present echo info
//    struct icmp icmphdr; // ICMP-header
//    //==================
//    // IP header
//    //==================
//
//    // IP protocol version (4 bits)
//    iphdr.ip_v = 4;
//
//    // IP header length (4 bits): Number of 32-bit words in header = 5
//    iphdr.ip_hl = IP4_HDRLEN / 4; // not the most correct
//
//    // Type of service (8 bits) - not using, zero it.
//    iphdr.ip_tos = 0;
//
//    // Total length of datagram (16 bits): IP header + ICMP header + ICMP data
//    iphdr.ip_len = htons(IP4_HDRLEN + ICMP_HDRLEN + dataLen);
//
//    // ID sequence number (16 bits): not in use since we do not allow fragmentation
//    iphdr.ip_id = 0;
//
//
////    char packet[IP_MAXPACKET];
////
////    // First, IP header.
////    memcpy(packet, &iphdr, IP4_HDRLEN);
////
////    // Next, ICMP header
////    memcpy((packet + IP4_HDRLEN), &icmphdr, ICMP_HDRLEN);
////
////    // After ICMP header, add the ICMP data.
////    memcpy(packet + IP4_HDRLEN + ICMP_HDRLEN, message, dataLen);
//
//
//
//    // Calculate the ICMP header checksum
//    icmphdr.icmp_cksum = calculate_checksum((unsigned short *) &pckt, sizeof(pckt));
////    memcpy((packet + IP4_HDRLEN), &icmphdr, ICMP_HDRLEN);
//
//
//    struct sockaddr_in dest_in;
//    memset(&dest_in, 0, sizeof(struct sockaddr_in));
//    dest_in.sin_family = AF_INET;
//
//    // The port is irrelevant for Networking and therefore was zeroed.
//    dest_in.sin_addr.s_addr = iphdr.ip_dst.s_addr;
//
//
//    /*
//     * Normal output performed
//     */
//    // Check if the socket has been connected
//    // Send the packet using sendto() for sending datagrams.
//    if (sendto(sock, &pckt, IP4_HDRLEN + ICMP_HDRLEN + dataLen, 0, (struct sockaddr *) &dest_in, sizeof(dest_in)) == -1) {
//        fprintf(stderr,"sendto() failed");// If the connection did not create and there was no sending, the function will return -1
//        // flag is whether packet was sent or not
//        flag=0;
//        return -1;
//    }
//    else //If connection created,and there was sending we will send the data
//    {
//        printf("Massage sent\n");
//    }
//
//
//
//
//
//
//    //receive packet
//    uint addressLength = sizeof(dest_in);
//
//    if ( recvfrom(sock, &pckt, sizeof(pckt), 0,
//                  (struct sockaddr*)&dest_in, &addressLength) <= 0)//&& msg_count>1)
//    {
//        printf("\nPacket receive failed!\n");
//    }
//
//    else
//    {
//        clock_gettime(CLOCK_MONOTONIC, &time_end);
//
//        double timeElapsed = ((double)(time_end.tv_nsec - time_start.tv_nsec))/1000000.0;
//        rtt_msec = (double)(time_end.tv_sec- time_start.tv_sec) * 1000.0 + timeElapsed;
//
//        // if packet was not sent, don't receive
//        if(flag)
//        {
//            if(!(pckt.hdr.type ==69 && pckt.hdr.code==0))
//            {
//                printf("Error..Packet received with ICMP type %d code %d\n", pckt.hdr.type, pckt.hdr.code);
//            }
//            else
//            {
////                printf("%d bytes from %s (h: %s)(%s) msg_seq=%d ttl=%d rtt = %Lf ms.\n",PING_PKT_S, ping_dom, rev_host,ping_ip, msg_count,ttl_val, rtt_msec);
//                printf("It took %Lf to return", rtt_msec);
//
////                msg_received_count++;
//            }
//        }
//    }
//
//
//    close(sock);
//}
//
//
//
//
//
