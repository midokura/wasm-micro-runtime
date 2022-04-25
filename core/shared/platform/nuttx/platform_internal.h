/*
 * Copyright (C) 2020 XiaoMi Corporation.  All rights reserved.
 * SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
 */

#ifndef _PLATFORM_INTERNAL_H
#define _PLATFORM_INTERNAL_H

#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <inttypes.h>
#include <limits.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/time.h>
#include <sys/mman.h>

#include <dirent.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/poll.h>
#include <sys/stat.h>
#include <nuttx/fs/ioctl.h>

#define O_DIRECTORY	O_DIRECT

#define O_UNDEF		-1L
#define O_NOFOLLOW			O_UNDEF
#define AT_SYMLINK_FOLLOW		O_UNDEF
#define AT_SYMLINK_NOFOLLOW		O_UNDEF
#define AT_REMOVEDIR			O_UNDEF
#define UTIME_NOW			O_UNDEF
#define UTIME_OMIT			O_UNDEF
#define SIGABRT				O_UNDEF
#define SIGBUS				O_UNDEF
#define SIGFPE				O_UNDEF
#define SIGHUP				O_UNDEF
#define SIGILL				O_UNDEF
#define SIGSEGV				O_UNDEF
#define SIGSYS				O_UNDEF
#define SIGTRAP				O_UNDEF
#define SIGTTIN				O_UNDEF
#define SIGCHLD				O_UNDEF
#define SIGTTOU				O_UNDEF
#define SIGURG				O_UNDEF
#define SIGVTALRM			O_UNDEF
#define SIGXCPU				O_UNDEF
#define SIGXFSZ				O_UNDEF

#define CLOCK_PROCESS_CPUTIME_ID 	2
#define CLOCK_THREAD_CPUTIME_ID		(clockid_t)3

inline int readlinkat(int dirfd, const char *path, char * buf, size_t bufsiz)
{
	return -1;
}

inline int openat(int fd, const char *path, int oflag, ...)
{
	return -1;
}

inline ssize_t preadv(int fd, void *buf, size_t nbyte, off_t offset)
{
	return -1;
}

inline ssize_t pwritev(int fd, const struct iovec *iov, int iovcnt,
                       off_t offset)
{
	return -1;
}

inline int mkdirat(int dirfd, const char *pathname, mode_t mode)
{
	return -1;
}

inline int linkat(int olddirfd, const char *oldpath, int newdirfd,
		  const char *newpath, int flags)
{
	return -1;
}

inline int symlinkat(const char *target, int newdirfd, const char *linkpath)
{
	return -1;
}

inline int fstatat(int dirfd, const char *pathname, struct stat *statbuf,
                   int flags)
{
	return -1;
}

inline DIR *fdopendir(int fd)
{
	return NULL;
}

inline int renameat(int olddirfd, const char *oldpath, int newdirfd,
                    const char *newpath)
{
	return -1;
}

inline int utimensat(int dirfd, const char *pathname,
                     const struct timespec times[2], int flags)
{
	return -1;
}

inline int unlinkat(int dirfd, const char *pathname, int flags)
{
	return -1;
}

inline int isatty(int fd)
{
	return -1;
}

typedef long __syscall_slong_t;

#ifdef __cplusplus
extern "C" {
#endif

#ifndef BH_PLATFORM_NUTTX
#define BH_PLATFORM_NUTTX
#endif

typedef pthread_t korp_tid;
typedef pthread_mutex_t korp_mutex;
typedef pthread_cond_t korp_cond;
typedef pthread_t korp_thread;

#define os_thread_local_attribute __thread

#define BH_APPLET_PRESERVED_STACK_SIZE (2 * BH_KB)

/* Default thread priority */
#define BH_THREAD_DEFAULT_PRIORITY 100

#define os_printf  printf
#define os_vprintf vprintf

/* On NuttX, time_t is uint32_t */
#define BH_TIME_T_MAX 0xffffffff

#ifdef __cplusplus
}
#endif

#endif /* end of _BH_PLATFORM_H */
