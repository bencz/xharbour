#ifndef _IO_H
#define _IO_H

/* io.h - private header for low-level I/O definitions */

#ifndef _WINCE

/* type definitions */
#ifndef _TIME_T_DEFINED
#define _TIME_T_DEFINED
typedef unsigned long time_t;
#endif

#ifdef __FSIZE_T_64
typedef unsigned long long _fsize_t;
#else
typedef unsigned long _fsize_t;
#endif

struct _finddata_t {
    unsigned int attrib;
    time_t time_create;
    time_t time_access;
    time_t time_write;
    _fsize_t size;
    char name[260];
};

/* file attributes for _findfirst() */
#define _A_NORMAL   0x00
#define _A_RDONLY   0x01
#define _A_HIDDEN   0x02
#define _A_SYSTEM   0x04
#define _A_SUBDIR   0x10
#define _A_ARCH     0x20

/* file sharing modes for _sopen() */
#define _SH_DENYRW  0x10
#define _SH_DENYWR  0x20
#define _SH_DENYRD  0x30
#define _SH_DENYNO  0x40

/* flags for _open() and _chmod() */
#define _S_IREAD    0x100
#define _S_IWRITE   0x80

/* locking modes for _locking() */
#define _LK_UNLCK   0
#define _LK_LOCK    1
#define _LK_NBLCK   2
#define _LK_RLCK    3
#define _LK_NBRLCK  4

/* declarations */
int __cdecl _access(const char *, int);
int __cdecl _chmod(const char *, int);
int __cdecl _chsize(int, long);
int __cdecl _close(int);
int __cdecl _commit(int);
int __cdecl _creat(const char *, int);
int __cdecl _dup(int);
int __cdecl _dup2(int, int);
int __cdecl _eof(int);
long __cdecl _filelength(int);
long __cdecl _findfirst(const char *, struct _finddata_t *);
int __cdecl _findnext(long, struct _finddata_t *);
int __cdecl _findclose(long);
long __cdecl _get_osfhandle(int);
int __cdecl _isatty(int);
int __cdecl _locking(int, int, long);
long __cdecl _lseek(int, long, int);
int __cdecl _open(const char *, int, ...);
int __cdecl _pipe(int *, unsigned int, int);
int __cdecl _read(int, void *, unsigned int);
int __cdecl _setmode(int, int);
int __cdecl _sopen(const char *, int, int, ...);
long __cdecl _tell(int);
int __cdecl _unlink(const char *);
int __cdecl _write(int, const void *, unsigned int);

/* macros */
#define _tell(fh)  _lseek((fh),0L,/*SEEK_CUR*/1)

/* compatibility names */
#ifndef _NO_OLDNAMES
#define SH_DENYRW  _SH_DENYRW
#define SH_DENYWR  _SH_DENYWR
#define SH_DENYRD  _SH_DENYRD
#define SH_DENYNO  _SH_DENYNO
#define S_IREAD  _S_IREAD
#define S_IWRITE  _S_IWRITE
#define LK_UNLCK  _LK_UNLCK
#define LK_LOCK  _LK_LOCK
#define LK_NBLCK  _LK_NBLCK
#define LK_RLCK  _LK_RLCK
#define LK_NBRLCK  _LK_NBRLCK

#define access(p, i)         _access(p, i)
#define chmod(p, i)          _chmod(p, i)
#define chsize(i, l)         _chsize(i, l)
#define close(i)             _close(i)
#define creat(p, i)          _creat(p, i)
#define dup(i)               _dup(i)
#define dup2(i, i2)          _dup2(i,i2)
#define eof(i)               _eof(i)
#define filelength(i)        _filelength(i)
#define isatty(i)            _isatty(i)
#define locking(i, i2, l)    _locking(i, i2, l)
#define lseek(i, l, i2)      _lseek(i, l, i2)
#define open(p, i, ...)      _open(p, i, __VA_ARGS__)
#define read(i, p, ui)       _read(i, p, ui)
#define setmode(i, i2)       _setmode(i, i2)
#define sopen(p, i, i2, ...) _sopen(p, i, i2, __VA_ARGS__)
#define tell(i)              _tell(i)
#define unlink(p)            _unlink(p)
#define write(i, p, ui)      _write(i, p, ui)
#endif /* _NO_OLDNAMES */

#endif /* _WINCE */

#endif /* _IO_H */

