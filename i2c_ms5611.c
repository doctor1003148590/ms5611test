
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include "i2c_ms5611.h"

//Default timeout value for read operations. Set to 0 to disable timeout detection.
uint16_t readTimeout = 0;

int8_t readBytes(uint8_t devAddr, uint8_t length, uint8_t *data)
{
    int8_t count = 0;
#ifdef DEBUG
    printf("read %#x %u\n",devAddr,length);
#endif
    int fd = open("/dev/i2c-0", O_RDWR);

    if (fd < 0) {
        fprintf(stderr, "Failed to open device: %s\n", strerror(errno));
        return(-1);
    }
    if (ioctl(fd, I2C_SLAVE, devAddr) < 0) {
        fprintf(stderr, "Failed to select device: %s\n", strerror(errno));
        close(fd);
        return(-1);
    }
    count = read(fd, data, length);
    if (count < 0) {
        fprintf(stderr, "Failed to read device(%d): %s\n", count, strerror(errno));
        close(fd);
        return(-1);
    } else if (count != length) {
        fprintf(stderr, "Short read  from device, expected %d, got %d\n", length, count);
        close(fd);
        return(-1);
    }
    close(fd);

    return count;
}


int writeBytes(uint8_t devAddr, uint8_t* data)
{
    int8_t count = 0;
    int fd;

    fd = open("/dev/i2c-0", O_RDWR);
    if (fd < 0) {
        fprintf(stderr, "Failed to open device: %s\n", strerror(errno));
        return -1;
    }
    if (ioctl(fd, I2C_SLAVE, devAddr) < 0) {
        fprintf(stderr, "Failed to select device: %s\n", strerror(errno));
        close(fd);
        return -1;
    }

    count = write(fd, data, 1);
    if (count < 0) {
        fprintf(stderr, "Failed to write device(%d): %s\n", count, strerror(errno));
        close(fd);
        return -1;
    } else if (count != 1) {
        fprintf(stderr, "Short write to device, expected %d, got %d\n", 1, count);
        close(fd);
        return -1;
    }
    close(fd);

    return 0;
}
