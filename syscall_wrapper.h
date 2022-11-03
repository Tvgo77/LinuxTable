#pragma once

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cerrno>

void unix_error(const char *msg);

int Open(const char *pathname, int flags, mode_t mode); 

int Close(int fd);

int Read(int fd, void* output, size_t nbytes);

int Write(int fd, void* buf, size_t nbytes);

off_t Lseek(int fd, off_t offset, int whence);