# Super Rand

Super Rand is a kernel module which allows user to open and read to get random number.

## Installation

#### Install
Use terminal to install

```terminal
# make from project folder
make
# install device
sudo insmod super_rand.ko
# log device message
sudo dmesg -c
```

Result

```terminal
[ 1959.363877] SuperRand: super_rand unregistered
[ 2722.393458] SuperRand: device class created correctly
```


## Test

After make, there will be a test file built from **test.c**

```c
// test.c
#define BUFFER_LENGTH 4;

fd = open("/dev/SuperRand", O_RDWR); // Open the device with read/write 
//...
ret = read(fd, receive, BUFFER_LENGTH);
// "receive" return a random number
```

#### Run test

Use terminal to run test

```terminal
sudo ./test
```

#### Result
```terminal
sudo ./test
```

## Uninstall

Use terminal to uninstall

```terminal
# Remove device
sudo rmmod super_rand
# Clean all module files and test file
make clean
```

