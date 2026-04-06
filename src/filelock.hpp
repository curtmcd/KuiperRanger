#ifndef filelock_hpp
#define filelock_hpp

#ifdef __unix__
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/file.h>
#endif

struct FileLock {
    FileLock(const char *fname)
    {
#ifdef __unix__
	if ((fd = open(fname, O_RDWR)) >= 0)
	    while (flock(fd, LOCK_EX) < 0 && errno == EINTR)
		;
#endif
    }

    ~FileLock()
    {
#ifdef __unix__
	if (fd > 0)
	    close(fd);
#endif
    }

#ifdef __unix__
private:
    int fd;
#endif
};

#endif // !filelock_hpp
