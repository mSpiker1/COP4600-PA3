#include <stdio.h>

#include <stdlib.h>

#include <fcntl.h>

#include <string.h>

#include <unistd.h>



#define DEVICE_PATH "/dev/lkmasg1_input"



int main() {

    int fd;

    char buf[256];



    fd = open(DEVICE_PATH, O_RDONLY);

    if (fd < 0) {

        perror("Failed to open the device...");

        return -1;

    }



    memset(buf, 0, sizeof(buf));

    ssize_t bytesRead = read(fd, buf, sizeof(buf) - 1);



    if (bytesRead < 0) {

        perror("Failed to read from the device...");

        close(fd);

        return -1;

    }



    printf("Read from the device: %s\n", buf);



    close(fd);

    return 0;

}

