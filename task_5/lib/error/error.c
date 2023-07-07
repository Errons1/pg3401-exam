#include "error.h"

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

char * errnoLookUp() {
    switch (errno) {
        case 1: return "EPERM Operation not permitted\n";
        case 2: return "ENOENT No such file or directory\n";
        case 3: return "ESRCH No such process\n";
        case 4: return "EINTR Interrupted system call\n\n";
        case 5: return "EIO Input/output error\n";
        case 6: return "ENXIO No such device or address\n";
        case 7: return "E2BIG Argument list too long\n";
        case 8: return "ENOEXEC Exec format error\n";
        case 9: return "EBADF Bad file descriptor\n";
        case 10: return "ECHILD No child processes\n";
        case 11: return "EAGAIN Resource temporarily unavailable\n";
        case 12: return "ENOMEM Cannot allocate memory\n";
        case 13: return "EACCES Permission denied\n";
        case 14: return "EFAULT Bad address\n";
        case 15: return "ENOTBLK Block device required\n";
        case 16: return "EBUSY Device or resource busy\n";
        case 17: return "EEXIST File exists\n";
        case 18: return "EXDEV Invalid cross-device link\n";
        case 19: return "ENODEV No such device\n";
        case 20: return "ENOTDIR Not a directory\n";
        case 21: return "EISDIR Is a directory\n";
        case 22: return "EINVAL Invalid argument\n";
        case 23: return "ENFILE Too many open files in system\n";
        case 24: return "EMFILE Too many open files\n";
        case 25: return "ENOTTY Inappropriate ioctl for device\n";
        case 26: return "ETXTBSY Text file busy\n";
        case 27: return "EFBIG File too large\n";
        case 28: return "ENOSPC No space left on device\n";
        case 29: return "ESPIPE Illegal seek\n";
        case 30: return "EROFS Read-only file system\n";
        case 31: return "EMLINK Too many links\n";
        case 32: return "EPIPE Broken pipe\n";
        case 33: return "EDOM Numerical argument out of domain\n";
        case 34: return "ERANGE Numerical result out of range\n";
        case 35: return "EDEADLK Resource deadlock avoided\n";
        case 36: return "ENAMETOOLONG File name too long\n";
        case 37: return "ENOLCK No locks available\n";
        case 38: return "ENOSYS Function not implemented\n";
        case 39: return "ENOTEMPTY Directory not empty\n";
        case 40: return "ELOOP Too many levels of symbolic links\n";
        case 41: return "not implemented\n";
        case 42: return "ENOMSG No message of desired type\n";
        case 43: return "EIDRM Identifier removed\n";
        case 44: return "ECHRNG Channel number out of range\n";
        case 45: return "EL2NSYNC Level 2 not synchronized\n";
        case 46: return "EL3HLT Level 3 halted\n";
        case 47: return "EL3RST Level 3 reset\n";
        case 48: return "ELNRNG Link number out of range\n";
        case 49: return "EUNATCH Protocol driver not attached\n";
        case 50: return "ENOCSI No CSI structure available\n";
        case 51: return "EL2HLT Level 2 halted\n";
        case 52: return "EBADE Invalid exchange\n";
        case 53: return "EBADR Invalid request descriptor\n";
        case 54: return "EXFULL Exchange full\n";
        case 55: return "ENOANO No anode\n";
        case 56: return "EBADRQC Invalid request code\n";
        case 57: return "EBADSLT Invalid slot\n";
        case 58: return "not implemented\n";
        case 59: return "EBFONT Bad font file format\n";
        case 60: return "ENOSTR Device not a stream\n";
        case 61: return "ENODATA No data available\n";
        case 62: return "ETIME Timer expired\n";
        case 63: return "ENOSR Out of streams resources\n";
        case 64: return "ENONET Machine is not on the network\n";
        case 65: return "ENOPKG Package not installed\n";
        case 66: return "EREMOTE Object is remote\n";
        case 67: return "ENOLINK Link has been severed\n";
        case 68: return "EADV Advertise error\n";
        case 69: return "ESRMNT Srmount error\n";
        case 70: return "ECOMM Communication error on send\n";
        case 71: return "EPROTO Protocol error\n";
        case 72: return "EMULTIHOP Multihop attempted\n";
        case 73: return "EDOTDOT RFS specific error\n";
        case 74: return "EBADMSG Bad message\n";
        case 75: return "EOVERFLOW Value too large for defined data type\n";
        case 76: return "ENOTUNIQ Name not unique on network\n";
        case 77: return "EBADFD File descriptor in bad state\n";
        case 78: return "EREMCHG Remote address changed\n";
        case 79: return "ELIBACC Can not access a needed shared library\n";
        case 80: return "ELIBBAD Accessing a corrupted shared library\n";
        case 81: return "ELIBSCN .lib section in a.out corrupted\n";
        case 82: return "ELIBMAX Attempting to link in too many shared libraries\n";
        case 83: return "ELIBEXEC Cannot exec a shared library directly\n";
        case 84: return "EILSEQ Invalid or incomplete multibyte or wide character\n";
        case 85: return "ERESTART Interrupted system call should be restarted\n";
        case 86: return "ESTRPIPE Streams pipe error\n";
        case 87: return "EUSERS Too many users\n";
        case 88: return "ENOTSOCK Socket operation on non-socket\n";
        case 89: return "EDESTADDRREQ Destination address required\n";
        case 90: return "EMSGSIZE Message too long\n";
        case 91: return "EPROTOTYPE Protocol wrong type for socket\n";
        case 92: return "ENOPROTOOPT Protocol not available\n";
        case 93: return "EPROTONOSUPPORT Protocol not supported\n";
        case 94: return "ESOCKTNOSUPPORT Socket type not supported\n";
        case 95: return "EOPNOTSUPP Operation not supported\n";
        case 96: return "EPFNOSUPPORT Protocol family not supported\n";
        case 97: return "EAFNOSUPPORT Address family not supported by protocol\n";
        case 98: return "EADDRINUSE Address already in use\n";
        case 99: return "EADDRNOTAVAIL Cannot assign requested address\n";
        case 100: return "ENETDOWN Network is down\n";
        case 101: return "ENETUNREACH Network is unreachable\n";
        case 102: return "ENETRESET Network dropped connection on reset\n";
        case 103: return "ECONNABORTED Software caused connection abort\n";
        case 104: return "ECONNRESET Connection reset by peer\n";
        case 105: return "ENOBUFS No buffer space available\n";
        case 106: return "EISCONN Transport endpoint is already connected\n";
        case 107: return "ENOTCONN Transport endpoint is not connected\n";
        case 108: return "ESHUTDOWN Cannot send after transport endpoint shutdown\n";
        case 109: return "ETOOMANYREFS Too many references: cannot splice\n";
        case 110: return "ETIMEDOUT Connection timed out\n";
        case 111: return "ECONNREFUSED Connection refused\n";
        case 112: return "EHOSTDOWN Host is down\n";
        case 113: return "EHOSTUNREACH No route to host\n";
        case 114: return "EALREADY Operation already in progress\n";
        case 115: return "EINPROGRESS Operation now in progress\n";
        case 116: return "ESTALE Stale file handle\n";
        case 117: return "EUCLEAN Structure needs cleaning\n";
        case 118: return "ENOTNAM Not a XENIX named type file\n";
        case 119: return "ENAVAIL No XENIX semaphores available\n";
        case 120: return "EISNAM Is a named type file\n";
        case 121: return "EREMOTEIO Remote I/O error\n";
        case 122: return "EDQUOT Disk quota exceeded\n";
        case 123: return "ENOMEDIUM No medium found\n";
        case 124: return "EMEDIUMTYPE Wrong medium type\n";
        case 125: return "ECANCELED Operation canceled\n";
        case 126: return "ENOKEY Required key not available\n";
        case 127: return "EKEYEXPIRED Key has expired\n";
        case 128: return "EKEYREVOKED Key has been revoked\n";
        case 129: return "EKEYREJECTED Key was rejected by service\n";
        case 130: return "EOWNERDEAD Owner died\n";
        case 131: return "ENOTRECOVERABLE State not recoverable\n";
        case 132: return "ERFKILL Operation not possible due to RF-kill\n";
        case 133: return "EHWPOISON Memory page has hardware error\n";
        default: return "Undefined error\n";
    }
}

int error_warning(char *szMessage, int iCheck) {
    if (iCheck < 0) {
        printf("%s. Error %d - %s\n", szMessage, errno, errnoLookUp());
        return iCheck;
    }

    return iCheck;
}

void error_exit(char *szMessage, int iCheck) {
    if (iCheck < 0) {
        if (errno == 0) {
            errno = -1;
        }

        printf("%s. Error: %d - %s\n", szMessage, errno, errnoLookUp());
        exit(errno);
    }
}


