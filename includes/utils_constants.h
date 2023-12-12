
#ifndef UTILS_CONSTANTS_H
# define UTILS_CONSTANTS_H

# define DEBUG 0
# define VERBOSE 0

# define MAX_INT                2147483647
# define MIN_INT                -2147483648

# define HEX_BASE_U             "0123456789ABCDEF"
# define HEX_BASE_L             "0123456789abcdef"

# define MAX_IP_LEN             16
# define MAX_HOSTNAME_LEN       128
# define TTL_VALUE              64
# define SEQUENCE_INTERVAL_S    1
# define STR_BUFFER_LEN         512
# define PATTERN_LEN            32

# define IP_TOTAL_LEN           84
# define IP_HEADER_LEN          20
# define ICMP_HEADER_LEN        8
# define ICMP_TIMESTAMP_LEN     8
# define ICMP_PAYLOAD_LEN       48
# define ICMP_DATA_LEN          40

# define IP_FLAGS_MASK          0xE000
# define IP_OFFSET_MASK         0x1FFF
# define IP_OFFSET_SHIFT        13

// ECHO QUERY MESSAGES
# define ICMP_ECHO_REQUEST      8
# define ICMP_ECHO_REPLY        0

// ERROR-REPORTING MESSAGES
# define ICMP_ERR_UNREACHABLE   3
# define ICMP_ERR_TIME_EXCEEDED 11
// # define ICMP_ERR_SOURCE_QUENCH 4   // INFORMATIONAL
// # define ICMP_ERR_PARAMETER     12  // INFORMATIONAL
// # define ICMP_ERR_REDIRECTION   5   // INFORMATIONAL

// TIMESTAMP QUERY MESSAGES
// # define ICMP_TIME_REQUEST      13  // INFORMATIONAL
// # define ICMP_TIME_REPLY        14  // INFORMATIONAL

#endif
