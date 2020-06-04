#include        <stdio.h>
#include        <fcntl.h>
#include        <string.h>
#include        <time.h>
#include        <sys/types.h>
#include        <sys/mman.h>
#include        <sys/utsname.h>

static int  _sys_sync_for_cpu_file = -1;
static int  _sys_sync_for_dev_file = -1;
static int  _sys_sync_enable       =  0;

static int  _sys_sync_attr(char* attr, unsigned int offset, unsigned int size, int direction)
{
    return sprintf(attr, "0x%08X%08X\n", offset, ((size & 0xFFFFFFF0) |
                                                  (direction << 2) | 1));
}

void print_diff_time(struct timeval start_time, struct timeval end_time)
{
    struct timeval diff_time;
    if (end_time.tv_usec < start_time.tv_usec) {
        diff_time.tv_sec  = end_time.tv_sec  - start_time.tv_sec  - 1;
        diff_time.tv_usec = end_time.tv_usec - start_time.tv_usec + 1000*1000;
    } else {
        diff_time.tv_sec  = end_time.tv_sec  - start_time.tv_sec ;
        diff_time.tv_usec = end_time.tv_usec - start_time.tv_usec;
    }
    printf("time = %ld.%06ld sec\n", diff_time.tv_sec, diff_time.tv_usec);
}

void memory_write_test(unsigned int size, int o_sync)
{
    int            fd;
    unsigned char  attr[1024];
    int            attr_len;
    struct timeval start_time, phase1_time, phase2_time, end_time;
    int            error_count;
    unsigned char* udmabuf;
    unsigned long* tmp_buf;
    unsigned long  tmp_word  = (O_SYNC == 0)? 0xCC : 0xCD;
    size_t         tmp_words = size/sizeof(unsigned long);

    if (o_sync == 0)
        attr_len = _sys_sync_attr(attr, 0, size, 0);
      
    tmp_buf = (unsigned long*)malloc(size);
    if (tmp_buf == NULL) {
        printf("Can not alloc tmp_buf\n");
        return;
    }
    for (int i = 0; i < tmp_words; i++)
        tmp_buf[i] = 0;

    if ((fd  = open("/dev/udmabuf0", O_RDWR | o_sync)) != -1) {
        udmabuf = mmap(NULL, size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
        gettimeofday(&start_time, NULL);
        if ((o_sync == 0) && (_sys_sync_enable != 0))
            write(_sys_sync_for_cpu_file, attr, attr_len);
        memcpy(udmabuf, tmp_buf, size);
        if ((o_sync == 0) && (_sys_sync_enable != 0))
            write(_sys_sync_for_dev_file, attr, attr_len);
        gettimeofday(&end_time  , NULL);
        print_diff_time(start_time, end_time);
        close(fd);
    }
    free(tmp_buf);
}

void memory_read_test(unsigned int size, int o_sync)
{
    int            fd;
    unsigned char  attr[1024];
    int            attr_len;
    struct timeval start_time, phase1_time, phase2_time, end_time;
    int            error_count;
    unsigned char* udmabuf;
    unsigned long* tmp_buf;
    size_t         tmp_words = size/sizeof(unsigned long);

    tmp_buf = (unsigned long*)malloc(size);
    if (tmp_buf == NULL) {
        printf("Can not alloc tmp_buf\n");
        return;
    }

    if (o_sync == 0)
        attr_len = _sys_sync_attr(attr, 0, size, 0);
      
    if ((fd  = open("/dev/udmabuf0", O_RDWR | o_sync)) != -1) {
        udmabuf = mmap(NULL, size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
        printf("MMAP=0x%08X_%08X ", ((unsigned long)udmabuf >> 32) & 0xFFFFFFFF,
                                    ((unsigned long)udmabuf >>  0) & 0xFFFFFFFF);
        gettimeofday(&start_time, NULL);
        if ((o_sync == 0) && (_sys_sync_enable != 0))
            write(_sys_sync_for_cpu_file, attr, attr_len);
        memcpy(tmp_buf, udmabuf, size);
        if ((o_sync == 0) && (_sys_sync_enable != 0))
            write(_sys_sync_for_dev_file, attr, attr_len);
        gettimeofday(&end_time  , NULL);
        print_diff_time(start_time, end_time);
        close(fd);
    }
    free(tmp_buf);
}

void main()
{
    int            fd;
    unsigned char  attr[1024];
    unsigned char* buf;
    unsigned int   buf_size;
    unsigned long  debug_vma = 0;
    unsigned long  sync_mode = 2;
    int            error_count;
    struct timeval start_time, end_time;

    if ((fd  = open("/sys/class/u-dma-buf/udmabuf0/size"     , O_RDONLY)) != -1) {
        read(fd, attr, 1024);
        sscanf(attr, "%d", &buf_size);
        close(fd);
    }
    printf("size=%d\n", buf_size);

    if ((_sys_sync_for_cpu_file = open("/sys/class/u-dma-buf/udmabuf0/sync_for_cpu", O_RDWR)) < 0) {
        printf("Can not open /sys/class/u-dma-buf/udmabuf0/sync_for_cpu\n");
        goto done;
    }
    
    if ((_sys_sync_for_dev_file = open("/sys/class/u-dma-buf/udmabuf0/sync_for_device", O_RDWR)) < 0) {
        printf("Can not open /sys/class/u-dma-buf/udmabuf0/sync_for_device\n");
        goto done;
    }
    
    if (1) {
        printf("memory_write_test(SIZE=%d, O_SYNC=0) : ", buf_size);
        memory_write_test(buf_size, 0);

        printf("memory_read_test (SIZE=%d, O_SYNC=0) : ", buf_size);
        memory_read_test(buf_size, 0);
    }

    if (0) {
        printf("memory_write_test(SIZE=%d, O_SYNC=1) : ", buf_size);
        memory_write_test(buf_size, O_SYNC);

        printf("memory_read_test (SIZE=%d, O_SYNC=1) : ", buf_size);
        memory_read_test(buf_size, O_SYNC);
    }
 done:
    if (_sys_sync_for_cpu_file >= 0)
        close(_sys_sync_for_cpu_file);
    if (_sys_sync_for_dev_file >= 0)
        close(_sys_sync_for_dev_file);
}
