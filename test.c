#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_LENGTH 4             ///< The buffer length (crude but fine)
static char receive[BUFFER_LENGTH]; ///< The receive buffer from the LKM

int main()
{
    int ret, fd;

    printf("\n------TEST------\n");
    fd = open("/dev/SuperRand", O_RDWR); // Open the device with read/write access
    if (fd < 0)
    {
        perror("Failed to open the device...");
        return errno;
    }

    printf("Reading from super_rand device...\n");
    ret = read(fd, receive, BUFFER_LENGTH); // Read the response from the LKM
    if (ret < 0)
    {
        printf("Failed %d\n", ret);
        return errno;
    }
    else
    {
        printf("Output number: %s\n", receive);
    }
    printf("Done\n");
    return 1;
}