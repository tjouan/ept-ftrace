#include  "ftrace.h"

#include  <stdio.h>

#ifdef    __linux__
#include  <string.h>
#endif

t_systab  systab[] = {
  {"int", "syscall", "int, ...", "int syscall(int, ...)"},
  {"void", "exit", "int", "void exit(int)"},
  {"int", "fork", "", "int fork()"},
  {"ssize_t", "read", "int, void *, size_t", "ssize_t read(int, void *, size_t)"},
  {"ssize_t", "write", "int, const void *, size_t", "ssize_t write(int, const void *, size_t)"},
  {"int", "open", "const char *, int, ...", "int open(const char *, int, ...)"},
  {"int", "close", "int", "int close(int)"},
  {"int", "wait4", "int, int *, int, struct rusage *", "int wait4(int, int *, int, struct rusage *)"},
  {"int", "wait4", "int, int *, int, struct rusage *", "int wait4(int, int *, int, struct rusage *)"},
  {"int", "link", "const char *, const char *", "int link(const char *, const char *)"},
  {"int", "unlink", "const char *", "int unlink(const char *)"},
  {"int", "chdir", "const char *", "int chdir(const char *)"},
  {"int", "fchdir", "int", "int fchdir(int)"},
  {"int", "mknod", "const char *, mode_t, dev_t", "int mknod(const char *, mode_t, dev_t)"},
  {"int", "chmod", "const char *, mode_t", "int chmod(const char *, mode_t)"},
  {"int", "chown", "const char *, uid_t, gid_t", "int chown(const char *, uid_t, gid_t)"},
  {"int", "break", "char *", "int break(char *)"},
  {"int", "getfsstat", "struct statfs *, long, int", "int getfsstat(struct statfs *, long, int)"},
  {"int", "getfsstat", "struct statfs *, long, int", "int getfsstat(struct statfs *, long, int)"},
  {"pid_t", "getpid", "", "pid_t getpid()"},
  {"int", "mount", "const char *, const char *, int, void *", "int mount(const char *, const char *, int, void *)"},
  {"int", "unmount", "const char *, int", "int unmount(const char *, int)"},
  {"int", "setuid", "uid_t", "int setuid(uid_t)"},
  {"uid_t", "getuid", "", "uid_t getuid()"},
  {"uid_t", "geteuid", "", "uid_t geteuid()"},
  {"int", "ptrace", "int, pid_t, caddr_t, int", "int ptrace(int, pid_t, caddr_t, int)"},
  {"ssize_t", "recvmsg", "int, struct msghdr *, int", "ssize_t recvmsg(int, struct msghdr *, int)"},
  {"ssize_t", "sendmsg", "int, const struct msghdr *, int", "ssize_t sendmsg(int, const struct msghdr *, int)"},
  {"ssize_t", "recvfrom", "int, void *, size_t, int, struct sockaddr *, unsigned int *", "ssize_t recvfrom(int, void *, size_t, int, struct sockaddr *, unsigned int *)"},
  {"int", "accept", "int, struct sockaddr *, unsigned int *", "int accept(int, struct sockaddr *, unsigned int *)"},
  {"int", "getpeername", "int, struct sockaddr *, unsigned int *", "int getpeername(int, struct sockaddr *, unsigned int *)"},
  {"int", "getsockname", "int, struct sockaddr *, unsigned int *", "int getsockname(int, struct sockaddr *, unsigned int *)"},
  {"int", "access", "const char *, int", "int access(const char *, int)"},
  {"int", "chflags", "const char *, u_long", "int chflags(const char *, u_long)"},
  {"int", "fchflags", "int, u_long", "int fchflags(int, u_long)"},
  {"void", "sync", "", "void sync()"},
  {"int", "kill", "int, int", "int kill(int, int)"},
  {"int", "kill", "int, int", "int kill(int, int)"},
  {"pid_t", "getppid", "", "pid_t getppid()"},
  {"pid_t", "getppid", "", "pid_t getppid()"},
  {"int", "dup", "int", "int dup(int)"},
  {"int", "pipe", "", "int pipe()"},
  {"gid_t", "getegid", "", "gid_t getegid()"},
  {"int", "profil", "caddr_t, size_t, u_long, u_int", "int profil(caddr_t, size_t, u_long, u_int)"},
  {"int", "ktrace", "const char *, int, int, int", "int ktrace(const char *, int, int, int)"},
  {"int", "ktrace", "const char *, int, int, int", "int ktrace(const char *, int, int, int)"},
  {"gid_t", "getgid", "", "gid_t getgid()"},
  {"gid_t", "getgid", "", "gid_t getgid()"},
  {"int", "__getlogin", "char *, size_t", "int __getlogin(char *, size_t)"},
  {"int", "setlogin", "const char *", "int setlogin(const char *)"},
  {"int", "acct", "const char *", "int acct(const char *)"},
  {"int", "acct", "const char *", "int acct(const char *)"},
  {"int", "acct", "const char *", "int acct(const char *)"},
  {"int", "ioctl", "int, u_long, ...", "int ioctl(int, u_long, ...)"},
  {"int", "ioctl", "int, u_long, ...", "int ioctl(int, u_long, ...)"},
  {"int", "revoke", "const char *", "int revoke(const char *)"},
  {"int", "symlink", "const char *, const char *", "int symlink(const char *, const char *)"},
  {"int", "readlink", "const char *, char *, size_t", "int readlink(const char *, char *, size_t)"},
  {"int", "execve", "const char *, char *const *, char *const *", "int execve(const char *, char *const *, char *const *)"},
  {"mode_t", "umask", "mode_t", "mode_t umask(mode_t)"},
  {"int", "chroot", "const char *", "int chroot(const char *)"},
  {"int", "chroot", "const char *", "int chroot(const char *)"},
  {"int", "chroot", "const char *", "int chroot(const char *)"},
  {"int", "chroot", "const char *", "int chroot(const char *)"},
  {"int", "chroot", "const char *", "int chroot(const char *)"},
  {"int", "vfork", "", "int vfork()"},
  {"int", "sbrk", "intptr_t", "int sbrk(intptr_t)"},
  {"int", "sstk", "int", "int sstk(int)"},
  {"int", "sstk", "int", "int sstk(int)"},
  {"int", "vadvise", "int", "int vadvise(int)"},
  {"int", "munmap", "void *, size_t", "int munmap(void *, size_t)"},
  {"int", "mprotect", "void *, size_t, int", "int mprotect(void *, size_t, int)"},
  {"int", "madvise", "void *, size_t, int", "int madvise(void *, size_t, int)"},
  {"int", "mincore", "void *, size_t, char *", "int mincore(void *, size_t, char *)"},
  {"int", "getgroups", "int, gid_t *", "int getgroups(int, gid_t *)"},
  {"int", "setgroups", "int, const gid_t *", "int setgroups(int, const gid_t *)"},
  {"int", "getpgrp", "", "int getpgrp()"},
  {"int", "setpgid", "int, int", "int setpgid(int, int)"},
  {"int", "setitimer", "int, const struct itimerval *, struct itimerval *", "int setitimer(int, const struct itimerval *, struct itimerval *)"},
  {"int", "setitimer", "int, const struct itimerval *, struct itimerval *", "int setitimer(int, const struct itimerval *, struct itimerval *)"},
  {"int", "setitimer", "int, const struct itimerval *, struct itimerval *", "int setitimer(int, const struct itimerval *, struct itimerval *)"},
  {"int", "getitimer", "int, struct itimerval *", "int getitimer(int, struct itimerval *)"},
  {"int", "getitimer", "int, struct itimerval *", "int getitimer(int, struct itimerval *)"},
  {"int", "getitimer", "int, struct itimerval *", "int getitimer(int, struct itimerval *)"},
  {"int", "getitimer", "int, struct itimerval *", "int getitimer(int, struct itimerval *)"},
  {"int", "dup2", "int, int", "int dup2(int, int)"},
  {"int", "fcntl", "int, int, ...", "int fcntl(int, int, ...)"},
  {"int", "select", "int, fd_set *, fd_set *, fd_set *, struct timeval *", "int select(int, fd_set *, fd_set *, fd_set *, struct timeval *)"},
  {"int", "fsync", "int", "int fsync(int)"},
  {"int", "setpriority", "int, int, int", "int setpriority(int, int, int)"},
  {"int", "socket", "int, int, int", "int socket(int, int, int)"},
  {"int", "connect", "int, const struct sockaddr *, unsigned int", "int connect(int, const struct sockaddr *, unsigned int)"},
  {"int", "connect", "int, const struct sockaddr *, unsigned int", "int connect(int, const struct sockaddr *, unsigned int)"},
  {"int", "getpriority", "int, int", "int getpriority(int, int)"},
  {"int", "getpriority", "int, int", "int getpriority(int, int)"},
  {"int", "getpriority", "int, int", "int getpriority(int, int)"},
  {"int", "getpriority", "int, int", "int getpriority(int, int)"},
  {"int", "bind", "int, const struct sockaddr *, unsigned int", "int bind(int, const struct sockaddr *, unsigned int)"},
  {"int", "setsockopt", "int, int, int, const void *, unsigned int", "int setsockopt(int, int, int, const void *, unsigned int)"},
  {"int", "listen", "int, int", "int listen(int, int)"},
  {"int", "listen", "int, int", "int listen(int, int)"},
  {"int", "listen", "int, int", "int listen(int, int)"},
  {"int", "listen", "int, int", "int listen(int, int)"},
  {"int", "listen", "int, int", "int listen(int, int)"},
  {"int", "listen", "int, int", "int listen(int, int)"},
  {"int", "listen", "int, int", "int listen(int, int)"},
  {"int", "listen", "int, int", "int listen(int, int)"},
  {"int", "gettimeofday", "struct timeval *, struct timezone *", "int gettimeofday(struct timeval *, struct timezone *)"},
  {"int", "getrusage", "int, struct rusage *", "int getrusage(int, struct rusage *)"},
  {"int", "getsockopt", "int, int, int, void *, unsigned int *", "int getsockopt(int, int, int, void *, unsigned int *)"},
  {"ssize_t", "readv", "int, const struct iovec *, int", "ssize_t readv(int, const struct iovec *, int)"},
  {"ssize_t", "writev", "int, const struct iovec *, int", "ssize_t writev(int, const struct iovec *, int)"},
  {"int", "settimeofday", "const struct timeval *, const struct timezone *", "int settimeofday(const struct timeval *, const struct timezone *)"},
  {"int", "fchown", "int, uid_t, gid_t", "int fchown(int, uid_t, gid_t)"},
  {"int", "fchmod", "int, mode_t", "int fchmod(int, mode_t)"},
  {"int", "fchmod", "int, mode_t", "int fchmod(int, mode_t)"},
  {"int", "setreuid", "uid_t, uid_t", "int setreuid(uid_t, uid_t)"},
  {"int", "setregid", "gid_t, gid_t", "int setregid(gid_t, gid_t)"},
  {"int", "rename", "const char *, const char *", "int rename(const char *, const char *)"},
  {"int", "rename", "const char *, const char *", "int rename(const char *, const char *)"},
  {"int", "rename", "const char *, const char *", "int rename(const char *, const char *)"},
  {"int", "flock", "int, int", "int flock(int, int)"},
  {"int", "mkfifo", "const char *, mode_t", "int mkfifo(const char *, mode_t)"},
  {"ssize_t", "sendto", "int, const void *, size_t, int, const struct sockaddr *, unsigned int", "ssize_t sendto(int, const void *, size_t, int, const struct sockaddr *, unsigned int)"},
  {"int", "shutdown", "int, int", "int shutdown(int, int)"},
  {"int", "socketpair", "int, int, int, int *", "int socketpair(int, int, int, int *)"},
  {"int", "mkdir", "const char *, mode_t", "int mkdir(const char *, mode_t)"},
  {"int", "rmdir", "const char *", "int rmdir(const char *)"},
  {"int", "utimes", "const char *, const struct timeval *", "int utimes(const char *, const struct timeval *)"},
  {"int", "adjtime", "const struct timeval *, struct timeval *", "int adjtime(const struct timeval *, struct timeval *)"},
  {"int", "adjtime", "const struct timeval *, struct timeval *", "int adjtime(const struct timeval *, struct timeval *)"},
  {"int", "adjtime", "const struct timeval *, struct timeval *", "int adjtime(const struct timeval *, struct timeval *)"},
  {"int", "adjtime", "const struct timeval *, struct timeval *", "int adjtime(const struct timeval *, struct timeval *)"},
  {"int", "adjtime", "const struct timeval *, struct timeval *", "int adjtime(const struct timeval *, struct timeval *)"},
  {"int", "adjtime", "const struct timeval *, struct timeval *", "int adjtime(const struct timeval *, struct timeval *)"},
  {"int", "adjtime", "const struct timeval *, struct timeval *", "int adjtime(const struct timeval *, struct timeval *)"},
  {"int", "setsid", "", "int setsid()"},
  {"int", "quotactl", "const char *, int, int, caddr_t", "int quotactl(const char *, int, int, caddr_t)"},
  {"int", "quotactl", "const char *, int, int, caddr_t", "int quotactl(const char *, int, int, caddr_t)"},
  {"int", "quotactl", "const char *, int, int, caddr_t", "int quotactl(const char *, int, int, caddr_t)"},
  {"int", "nfssvc", "int, void *", "int nfssvc(int, void *)"},
  {"int", "nfssvc", "int, void *", "int nfssvc(int, void *)"},
  {"int", "statfs", "const char *, struct statfs *", "int statfs(const char *, struct statfs *)"},
  {"int", "fstatfs", "int, struct statfs *", "int fstatfs(int, struct statfs *)"},
  {"int", "getfh", "const char *, fhandle_t *", "int getfh(const char *, fhandle_t *)"},
  {"int", "getfh", "const char *, fhandle_t *", "int getfh(const char *, fhandle_t *)"},
  {"int", "getfh", "const char *, fhandle_t *", "int getfh(const char *, fhandle_t *)"},
  {"int", "getfh", "const char *, fhandle_t *", "int getfh(const char *, fhandle_t *)"},
  {"int", "sysarch", "int, void *", "int sysarch(int, void *)"},
  {"int", "sysarch", "int, void *", "int sysarch(int, void *)"},
  {"int", "sysarch", "int, void *", "int sysarch(int, void *)"},
  {"int", "sysarch", "int, void *", "int sysarch(int, void *)"},
  {"ssize_t", "pread", "int, void *, size_t, int, off_t", "ssize_t pread(int, void *, size_t, int, off_t)"},
  {"ssize_t", "pwrite", "int, const void *, size_t, int, off_t", "ssize_t pwrite(int, const void *, size_t, int, off_t)"},
  {"int", "ntp_gettime", "struct ntptimeval *", "int ntp_gettime(struct ntptimeval *)"},
  {"int", "ntp_adjtime", "struct timex *", "int ntp_adjtime(struct timex *)"},
  {"int", "setgid", "gid_t", "int setgid(gid_t)"},
  {"int", "setegid", "gid_t", "int setegid(gid_t)"},
  {"int", "seteuid", "uid_t", "int seteuid(uid_t)"},
  {"int", "lfs_bmapv", "fsid_t *, struct block_info *, int", "int lfs_bmapv(fsid_t *, struct block_info *, int)"},
  {"int", "lfs_markv", "fsid_t *, struct block_info *, int", "int lfs_markv(fsid_t *, struct block_info *, int)"},
  {"int", "lfs_segclean", "fsid_t *, u_long", "int lfs_segclean(fsid_t *, u_long)"},
  {"int", "lfs_segwait", "fsid_t *, struct timeval *", "int lfs_segwait(fsid_t *, struct timeval *)"},
  {"int", "lfs_segwait", "fsid_t *, struct timeval *", "int lfs_segwait(fsid_t *, struct timeval *)"},
  {"int", "lfs_segwait", "fsid_t *, struct timeval *", "int lfs_segwait(fsid_t *, struct timeval *)"},
  {"int", "lfs_segwait", "fsid_t *, struct timeval *", "int lfs_segwait(fsid_t *, struct timeval *)"},
  {"long", "pathconf", "const char *, int", "long pathconf(const char *, int)"},
  {"long", "fpathconf", "int, int", "long fpathconf(int, int)"},
  {"int", "getrlimit", "int, struct rlimit *", "int getrlimit(int, struct rlimit *)"},
  {"int", "setrlimit", "int, const struct rlimit *", "int setrlimit(int, const struct rlimit *)"},
  {"int", "setrlimit", "int, const struct rlimit *", "int setrlimit(int, const struct rlimit *)"},
  {"void *", "mmap", "void *, size_t, int, int, int, long, off_t", "void * mmap(void *, size_t, int, int, int, long, off_t)"},
  {"quad_t", "__syscall", "quad_t, ...", "quad_t __syscall(quad_t, ...)"},
  {"off_t", "lseek", "int, int, off_t, int", "off_t lseek(int, int, off_t, int)"},
  {"int", "truncate", "const char *, int, off_t", "int truncate(const char *, int, off_t)"},
  {"int", "ftruncate", "int, int, off_t", "int ftruncate(int, int, off_t)"},
  {"int", "__sysctl", "int *, u_int, void *, size_t *, void *, size_t", "int __sysctl(int *, u_int, void *, size_t *, void *, size_t)"},
  {"int", "mlock", "const void *, size_t", "int mlock(const void *, size_t)"},
  {"int", "munlock", "const void *, size_t", "int munlock(const void *, size_t)"},
  {"int", "undelete", "const char *", "int undelete(const char *)"},
  {"int", "futimes", "int, const struct timeval *", "int futimes(int, const struct timeval *)"},
  {"pid_t", "getpgid", "pid_t", "pid_t getpgid(pid_t)"},
  {"int", "reboot", "int, char *", "int reboot(int, char *)"},
  {"int", "poll", "struct pollfd *, u_int, int", "int poll(struct pollfd *, u_int, int)"},
  {"int", "poll", "struct pollfd *, u_int, int", "int poll(struct pollfd *, u_int, int)"},
  {"int", "semget", "key_t, int, int", "int semget(key_t, int, int)"},
  {"int", "semop", "int, struct sembuf *, size_t", "int semop(int, struct sembuf *, size_t)"},
  {"int", "semconfig", "int", "int semconfig(int)"},
  {"int", "semconfig", "int", "int semconfig(int)"},
  {"int", "msgget", "key_t, int", "int msgget(key_t, int)"},
  {"int", "msgsnd", "int, const void *, size_t, int", "int msgsnd(int, const void *, size_t, int)"},
  {"ssize_t", "msgrcv", "int, void *, size_t, long, int", "ssize_t msgrcv(int, void *, size_t, long, int)"},
  {"void *", "shmat", "int, const void *, int", "void * shmat(int, const void *, int)"},
  {"void *", "shmat", "int, const void *, int", "void * shmat(int, const void *, int)"},
  {"int", "shmdt", "const void *", "int shmdt(const void *)"},
  {"int", "shmget", "key_t, size_t, int", "int shmget(key_t, size_t, int)"},
  {"int", "clock_gettime", "clockid_t, struct timespec *", "int clock_gettime(clockid_t, struct timespec *)"},
  {"int", "clock_settime", "clockid_t, const struct timespec *", "int clock_settime(clockid_t, const struct timespec *)"},
  {"int", "clock_getres", "clockid_t, struct timespec *", "int clock_getres(clockid_t, struct timespec *)"},
  {"int", "nanosleep", "const struct timespec *, struct timespec *", "int nanosleep(const struct timespec *, struct timespec *)"},
  {"int", "fdatasync", "int", "int fdatasync(int)"},
  {"int", "mlockall", "int", "int mlockall(int)"},
  {"int", "munlockall", "", "int munlockall()"},
  {"int", "__posix_rename", "const char *, const char *", "int __posix_rename(const char *, const char *)"},
  {"int", "swapctl", "int, const void *, int", "int swapctl(int, const void *, int)"},
  {"int", "getdents", "int, char *, size_t", "int getdents(int, char *, size_t)"},
  {"int", "minherit", "void *, size_t, int", "int minherit(void *, size_t, int)"},
  {"int", "lchmod", "const char *, mode_t", "int lchmod(const char *, mode_t)"},
  {"int", "lchown", "const char *, uid_t, gid_t", "int lchown(const char *, uid_t, gid_t)"},
  {"int", "lutimes", "const char *, const struct timeval *", "int lutimes(const char *, const struct timeval *)"},
  {"int", "__msync13", "void *, size_t, int", "int __msync13(void *, size_t, int)"},
  {"int", "__stat13", "const char *, struct stat *", "int __stat13(const char *, struct stat *)"},
  {"int", "__fstat13", "int, struct stat *", "int __fstat13(int, struct stat *)"},
  {"int", "__lstat13", "const char *, struct stat *", "int __lstat13(const char *, struct stat *)"},
  {"int", "__sigaltstack14", "const struct sigaltstack *, struct sigaltstack *", "int __sigaltstack14(const struct sigaltstack *, struct sigaltstack *)"},
  {"int", "__vfork14", "", "int __vfork14()"},
  {"int", "__posix_chown", "const char *, uid_t, gid_t", "int __posix_chown(const char *, uid_t, gid_t)"},
  {"int", "__posix_fchown", "int, uid_t, gid_t", "int __posix_fchown(int, uid_t, gid_t)"},
  {"int", "__posix_lchown", "const char *, uid_t, gid_t", "int __posix_lchown(const char *, uid_t, gid_t)"},
  {"pid_t", "getsid", "pid_t", "pid_t getsid(pid_t)"},
  {"pid_t", "__clone", "int, void *", "pid_t __clone(int, void *)"},
  {"int", "fktrace", "const int, int, int, int", "int fktrace(const int, int, int, int)"},
  {"ssize_t", "preadv", "int, const struct iovec *, int, int, off_t", "ssize_t preadv(int, const struct iovec *, int, int, off_t)"},
  {"ssize_t", "pwritev", "int, const struct iovec *, int, int, off_t", "ssize_t pwritev(int, const struct iovec *, int, int, off_t)"},
  {"int", "__sigaction14", "int, const struct sigaction *, struct sigaction *", "int __sigaction14(int, const struct sigaction *, struct sigaction *)"},
  {"int", "__sigpending14", "sigset_t *", "int __sigpending14(sigset_t *)"},
  {"int", "__sigprocmask14", "int, const sigset_t *, sigset_t *", "int __sigprocmask14(int, const sigset_t *, sigset_t *)"},
  {"int", "__sigsuspend14", "const sigset_t *", "int __sigsuspend14(const sigset_t *)"},
  {"int", "__sigreturn14", "struct sigcontext *", "int __sigreturn14(struct sigcontext *)"},
  {"int", "__getcwd", "char *, size_t", "int __getcwd(char *, size_t)"},
  {"int", "fchroot", "int", "int fchroot(int)"},
  {"int", "fhopen", "const fhandle_t *, int", "int fhopen(const fhandle_t *, int)"},
  {"int", "fhstat", "const fhandle_t *, struct stat *", "int fhstat(const fhandle_t *, struct stat *)"},
  {"int", "fhstatfs", "const fhandle_t *, struct statfs *", "int fhstatfs(const fhandle_t *, struct statfs *)"},
  {"int", "____semctl13", "int, int, int, ...", "int ____semctl13(int, int, int, ...)"},
  {"int", "__msgctl13", "int, int, struct msqid_ds *", "int __msgctl13(int, int, struct msqid_ds *)"},
  {"int", "__shmctl13", "int, int, struct shmid_ds *", "int __shmctl13(int, int, struct shmid_ds *)"},
  {"int", "lchflags", "const char *, u_long", "int lchflags(const char *, u_long)"},
  {"int", "issetugid", "", "int issetugid()"},
  {"int", "utrace", "const char *, void *, size_t", "int utrace(const char *, void *, size_t)"},
  {0, 0, 0, 0}
};

int   trace_print_sys(t_env *env, t_sym *sym)
{
  int   i;

  for (i = 0; systab[i].name; i++)
  {
    if (! strcmp(systab[i].name, sym->name))
    {
      printf(" sys> %s;\n", systab[i].line);
      return (OK);
    }
  }

  return (ERR);
}
