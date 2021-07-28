/*   SCE CONFIDENTIAL                                       */
/*   PlayStation(R)3 Programmer Tool Runtime Library 310.001 */
/*   Copyright (C) 2008 Sony Computer Entertainment Inc.    */
/*   All Rights Reserved.                                   */

#ifndef __CELL_FS_CELL_FS_FILE_API_H__
#define __CELL_FS_CELL_FS_FILE_API_H__

#include <sys/fs.h>

typedef struct CellFsDirectoryEntry
{
    CellFsStat attribute;
    CellFsDirent entry_name;
} CellFsDirectoryEntry;

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef enum CellFsDiscReadRetryType {
    CELL_FS_DISC_READ_RETRY_NONE,
    CELL_FS_DISC_READ_RETRY_DEFAULT
} CellFsDiscReadRetryType;

CellFsErrno cellFsOpen(const char *path, int flags, int *fd, const void *arg, uint64_t size);
CellFsErrno cellFsRead(int fd, void *buf, uint64_t nbytes, uint64_t *nread);
CellFsErrno cellFsWrite(int fd, const void *buf, uint64_t nbytes, uint64_t *nwrite);
CellFsErrno cellFsClose(int fd);
CellFsErrno cellFsOpendir(const char *path, int *fd);
CellFsErrno cellFsReaddir(int fd, CellFsDirent *dir, uint64_t *nread);
CellFsErrno cellFsClosedir(int fd);
CellFsErrno cellFsStat(const char *path, CellFsStat *sb);
CellFsErrno cellFsFstat(int fd, CellFsStat *sb);
CellFsErrno cellFsMkdir(const char *path, CellFsMode mode);
CellFsErrno cellFsRename(const char *from, const char *to);
CellFsErrno cellFsRmdir(const char *path);
CellFsErrno cellFsUnlink(const char *path);
CellFsErrno cellFsLseek(int fd, int64_t offset, int whence, uint64_t *pos);
CellFsErrno cellFsFsync(int fd);

CellFsErrno cellFsFGetBlockSize(int fd, uint64_t *sector_size,
                uint64_t *block_size);
CellFsErrno cellFsGetBlockSize(const char *path, uint64_t *sector_size,
                uint64_t *block_size);
CellFsErrno cellFsGetFreeSize(
        const char *directory_path,
        uint32_t *fs_block_size,
        uint64_t *free_block_count);

CellFsErrno cellFsTruncate(const char *path, uint64_t size);
CellFsErrno cellFsFtruncate(int fd, uint64_t size);

CellFsErrno cellFsUtime(const char *path, const CellFsUtimbuf *timep);
CellFsErrno cellFsChmod(const char *path, CellFsMode mode);

CellFsErrno cellFsGetDirectoryEntries(
        int fd,
        CellFsDirectoryEntry *entries,
        uint32_t entries_size,
        uint32_t *data_count);
/* from SDK200 */
CellFsErrno cellFsAllocateFileAreaWithoutZeroFill(
        const char *path,
        const uint64_t size);
/* from SDK220 */
CellFsErrno cellFsSetIoBuffer(
        int fd,
        size_t buffer_size_limit,
        int page_type,
        sys_memory_container_t container);
CellFsErrno cellFsSetDefaultContainer(
        sys_memory_container_t container,
        size_t total_limit);
CellFsErrno cellFsSetIoBufferFromDefaultContainer(
        int fd,
        size_t buffer_size_limit,
        int page_type);
/* from SDK240 */
CellFsErrno cellFsReadWithOffset(
        int fd,
        uint64_t offset,
        void *buffer,
        uint64_t buffer_size,
        uint64_t *read_bytes);
CellFsErrno cellFsWriteWithOffset(
        int fd,
        uint64_t offset,
        const void *data,
        uint64_t data_size,
        uint64_t *written_bytes);
CellFsErrno cellFsSdataOpen(
        const char *path,
        int flags, /* must be CELL_FS_O_RDONLY now */
        int *fd,
        const void *arg,
        uint64_t size);
CellFsErrno cellFsSdataOpenByFd(
        int mself_fd,
        int flags, /* must be CELL_FS_O_RDONLY now */
        int *sdata_fd,
        uint64_t offset, 
        const void *arg,
        uint64_t size);

CellFsErrno cellFsSetDiscReadRetrySetting(const CellFsDiscReadRetryType retry_type);
#ifdef __cplusplus
}
#endif /* __cplusplus */

#define CELL_FS_IO_BUFFER_PAGE_SIZE_64KB 0x0002
#define CELL_FS_IO_BUFFER_PAGE_SIZE_1MB  0x0004

/* CellFsRingBuffer.copy */
#define CELL_FS_ST_COPY             0
#define CELL_FS_ST_COPYLESS         1

/* CellFsRingBuffer */
typedef struct CellFsRingBuffer
{
  uint64_t ringbuf_size;
  uint64_t block_size;
  uint64_t transfer_rate;
  int copy;
} CellFsRingBuffer;

/* return value of status cellFsSt{Read,Write}GetStatus */
#define CELL_FS_ST_INITIALIZED      0x0001
#define CELL_FS_ST_NOT_INITIALIZED  0x0002
#define CELL_FS_ST_STOP             0x0100
#define CELL_FS_ST_PROGRESS         0x0200

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

CellFsErrno cellFsStReadInit(int fd, const CellFsRingBuffer *ringbuf);
CellFsErrno cellFsStReadFinish(int fd);
CellFsErrno cellFsStReadGetRingBuf(int fd, CellFsRingBuffer *ringbuf);
CellFsErrno cellFsStReadGetStatus(int fd, uint64_t *status);
CellFsErrno cellFsStReadGetRegid(int fd, uint64_t *regid);
CellFsErrno cellFsStReadStart(int fd, uint64_t offset, uint64_t size);
CellFsErrno cellFsStReadStop(int fd);
CellFsErrno cellFsStRead(int fd, char *buf, uint64_t size, uint64_t *rsize);
CellFsErrno cellFsStReadGetCurrentAddr(int fd, char **addr, uint64_t *size);
CellFsErrno cellFsStReadPutCurrentAddr(int fd, char *addr, uint64_t size);
CellFsErrno cellFsStReadWait(int fd, uint64_t size);
CellFsErrno cellFsStReadWaitCallback(int fd, uint64_t size, void (*func)(int xfd, uint64_t xsize));

#ifdef __cplusplus
}
#endif /* __cplusplus */

/* max count for cellFsAioInit */
#define CELL_FS_AIO_MAX_FS      10
/* max count for cellFsAioRead par mount_point */
#define CELL_FS_AIO_MAX_REQUEST 32
/* CellFsAio */
typedef struct CellFsAio {
  int fd;
  uint64_t offset;
  void *buf;
  uint64_t size;
  uint64_t user_data;
} CellFsAio;

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
CellFsErrno cellFsAioInit(const char *mount_point);
CellFsErrno cellFsAioFinish(const char *mount_point);
CellFsErrno cellFsAioRead(CellFsAio *aio, int *id,
                          void (*func)(CellFsAio *aio, CellFsErrno err,
                                       int id, uint64_t size));
CellFsErrno cellFsAioCancel(int id);
CellFsErrno cellFsAioWrite(CellFsAio *aio, int *id,
                           void (*func)(CellFsAio *aio, CellFsErrno err,
                                        int id, uint64_t size));
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __CELL_FS_CELL_FS_FILE_API_H__ */

