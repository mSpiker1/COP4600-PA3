// Small test program created to test functionality of write/read between modules.



#include <fcntl.h>

#include <stdio.h>

#include <stdlib.h>

#include <unistd.h>



#define MESSAGE "Hello, world!" // Enter your message to test write/read



int main() {

    // Open the writer device file

    int writer_fd = open("/dev/lkmasg2_writer", O_WRONLY);

    if (writer_fd < 0) {

        perror("Failed to open writer device");

        exit(EXIT_FAILURE);

    }



    // Write a message to the writer device

    ssize_t bytes_written = write(writer_fd, MESSAGE, sizeof(MESSAGE) - 1);

    if (bytes_written < 0) {

        perror("Failed to write to writer device");

        close(writer_fd);

        exit(EXIT_FAILURE);

    }



    printf("Wrote %zd bytes to the writer device\n", bytes_written);



    close(writer_fd);



    // Open the reader device file

    int reader_fd = open("/dev/lkmasg2_reader", O_RDONLY);

    if (reader_fd < 0) {

        perror("Failed to open reader device");

        exit(EXIT_FAILURE);

    }



    // Read from the reader device

    char buffer[1025];

    ssize_t bytes_read = read(reader_fd, buffer, sizeof(buffer));

    if (bytes_read < 0) {

        perror("Failed to read from reader device");

        close(reader_fd);

        exit(EXIT_FAILURE);

    }



    printf("Read %zd bytes from the reader device: %.*s\n", bytes_read, (int)bytes_read, buffer); // Prints read output 



    close(reader_fd);



    return 0;

}

