#ifndef WIN_COMPAT_H
#define WIN_COMPAT_H

#ifdef _WIN32
#include <fcntl.h>
#include <io.h>
#include <process.h>
#include <windows.h>

// Tipos básicos
#ifndef ssize_t
typedef long long ssize_t;
#endif

// pwrite: escrita posicional via _lseek + _write
static inline ssize_t pwrite(int fd, const void *buf, size_t count,
                             off_t offset) {
  off_t saved = _lseeki64(fd, 0, SEEK_CUR);
  if (saved == -1)
    return -1;
  if (_lseeki64(fd, offset, SEEK_SET) == -1)
    return -1;
  ssize_t written = _write(fd, buf, (unsigned int)count);
  _lseeki64(fd, saved, SEEK_SET);
  return written;
}

// mmap fallback
#define MAP_FAILED ((void *)-1)
#define PROT_READ 1
#define PROT_WRITE 2
#define MAP_SHARED 1
#define MAP_PRIVATE 2

static inline void *mmap(void *addr, size_t length, int prot, int flags, int fd,
                         off_t offset) {
  void *ptr =
      VirtualAlloc(NULL, length, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
  if (!ptr)
    return MAP_FAILED;
  if (_lseeki64(fd, offset, SEEK_SET) == -1) {
    VirtualFree(ptr, 0, MEM_RELEASE);
    return MAP_FAILED;
  }
  DWORD br;
  if (!ReadFile((HANDLE)_get_osfhandle(fd), ptr, (DWORD)length, &br, NULL)) {
    VirtualFree(ptr, 0, MEM_RELEASE);
    return MAP_FAILED;
  }
  return ptr;
}

static inline int munmap(void *addr, size_t length) {
  return VirtualFree(addr, 0, MEM_RELEASE) ? 0 : -1;
}
static inline int msync(void *addr, size_t length, int flags) { return 0; }

// usleep: definir apenas se não existir (evita ambiguidade com unistd.h do
// MinGW)
#ifndef usleep
#define usleep(usec) Sleep((usec + 999) / 1000)
#endif

// fdatasync e O_CLOEXEC
#define fdatasync(fd) _commit(fd)
#ifndef O_CLOEXEC
#define O_CLOEXEC 0
#endif

#else
#include <getopt.h>
#include <sys/mman.h>
#include <unistd.h>
#endif

#endif // WIN_COMPAT_H
