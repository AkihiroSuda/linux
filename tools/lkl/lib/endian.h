#ifndef _LKL_LIB_ENDIAN_H
#define _LKL_LIB_ENDIAN_H

#if defined(__FreeBSD__)
#include <sys/endian.h>
#elif defined(__ANDROID__)
#include <sys/endian.h>
#elif defined(__MINGW32__)
#include <winsock.h>
#define le32toh(x) (x)
#define le16toh(x) (x)
#define htole32(x) (x)
#define htole16(x) (x)
#define htole64(x) (x)
#define le64toh(x) (x)
#define htobe32(x) htonl(x)
#define htobe16(x) htons(x)
#define be32toh(x) ntohl(x)
#define be16toh(x) ntohs(x)
#else
#include <endian.h>
#endif

#ifndef htonl
#define htonl(x) htobe32(x)
#define htons(x) htobe16(x)
#define ntohl(x) be32toh(x)
#define ntohs(x) be16toh(x)
#endif


// below are from haiku/headers/compatibility/bsd/endian.h 
/*
 *  * General byte order swapping functions.
 *   */
#define bswap16(x)      __swap_int16(x)
#define bswap32(x)      __swap_int32(x)
#define bswap64(x)      __swap_int64(x)


#if BYTE_ORDER == LITTLE_ENDIAN
#define htobe16(x)      bswap16((x))
#define htobe32(x)      bswap32((x))
#define htobe64(x)      bswap64((x))
#define htole16(x)      ((uint16_t)(x))
#define htole32(x)      ((uint32_t)(x))
#define htole64(x)      ((uint64_t)(x))

#define be16toh(x)      bswap16((x))
#define be32toh(x)      bswap32((x))
#define be64toh(x)      bswap64((x))
#define le16toh(x)      ((uint16_t)(x))
#define le32toh(x)      ((uint32_t)(x))
#define le64toh(x)      ((uint64_t)(x))
#else /* BYTE_ORDER != LITTLE_ENDIAN */
#define htobe16(x)      ((uint16_t)(x))
#define htobe32(x)      ((uint32_t)(x))
#define htobe64(x)      ((uint64_t)(x))
#define htole16(x)      bswap16((x))
#define htole32(x)      bswap32((x))
#define htole64(x)      bswap64((x))

#define be16toh(x)      ((uint16_t)(x))
#define be32toh(x)      ((uint32_t)(x))
#define be64toh(x)      ((uint64_t)(x))
#define le16toh(x)      bswap16((x))
#define le32toh(x)      bswap32((x))
#define le64toh(x)      bswap64((x))
#endif /* BYTE_ORDER == LITTLE_ENDIAN */


#endif /* _LKL_LIB_ENDIAN_H */
