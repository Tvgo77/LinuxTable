#include "syscall_wrapper.h"

void unix_error(const char *msg) {
    fprintf(stderr, "%s: %s\n", msg, strerror(errno));
    exit(0);
}

int Open(const char *pathname, int flags, mode_t mode) {
    int rc;

    if ((rc = open(pathname, flags, mode))  < 0)
	unix_error("Open error");
    return rc;
}

int Close(int fd) {
    int rc;
    
    if ((rc = close(fd)) < 0)
    unix_error("Close error");
    return rc;
}

int Read(int fd, void* output, size_t nbytes) {
    int rc;

    if ((rc = read(fd, output, nbytes))  < 0)
	unix_error("Read error");
    return rc;
}

int Write(int fd, void* buf, size_t nbytes) {
    int rc;

    if ((rc = write(fd, buf, nbytes)) < 0)
    unix_error("Write error");
    return rc;
}

off_t Lseek(int fd, off_t offset, int whence) {
    off_t rc;

    if ((rc = lseek(fd, offset, whence)) < 0)
    unix_error("Lseek error");
    return rc;
}

