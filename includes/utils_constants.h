
#ifndef UTILS_CONSTANTS_H
# define UTILS_CONSTANTS_H

# define DEBUG 0

# define MAX_IP_LEN             16
# define MAX_HOSTNAME_LEN       100
# define TTL_VALUE              64
# define SEQUENCE_INTERVAL_S    1
# define BUFFER_LEN             512

# define ICMP_ECHO              8

# define IP_TOTAL_LEN           84
# define IP_HEADER_LEN          20
# define ICMP_HEADER_LEN        8   // sizeof(struct icmphdr)
# define ICMP_PAYLOAD_LEN       56  // IP_TOTAL_LEN - sizeof(struct icmphdr) = 76

// ECHO QUERY MESSAGES
# define ICMP_ECHO_REQUEST      8
# define ICMP_ECHO_REPLY        0

// TIMESTAMP QUERY MESSAGES
# define ICMP_TIME_REQUEST      13
# define ICMP_TIME_REPLY        14

// ERROR-REPORTING MESSAGES
# define ICMP_ERR_UNREACHABLE   3
# define ICMP_ERR_SOURCE_QUENCH 4
# define ICMP_ERR_TIME_EXCEEDED 11
# define ICMP_ERR_PARAMETER     12
# define ICMP_ERR_REDIRECTION   5


#endif
