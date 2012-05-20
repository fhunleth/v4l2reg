#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <linux/videodev2.h>

void usage(const char *progname) 
{
  fprintf(stderr, "%s [-d <device file>] [-i <i2c address>]  <register> [value]\n", progname);
}

int main(int argc, char *argv[])
{
  struct v4l2_dbg_register v4l2reg;
  const char *device_file = "/dev/video0";
  int opt;
  int fd;

  memset(&v4l2reg, 0, sizeof(v4l2reg));

  while ((opt = getopt(argc, argv, "d:i:")) != -1) {
    switch (opt) {
    case 'd':
      device_file = optarg;
      break;
    case 'i':
      v4l2reg.match.type = V4L2_CHIP_MATCH_I2C_ADDR;
      v4l2reg.match.addr = strtol(optarg, 0, 0);
      break;
    default:
      usage(argv[0]);
      return -1;
    }
  }

  if (optind >= argc) {
    usage(argv[0]);
    return -1;
  }

  v4l2reg.reg = strtol(argv[optind], 0, 0);
  optind++;

  fd = open(device_file, O_RDWR);
  if (fd < 0) {
    perror("Can't open device");
    return -1;
  }


  if (optind >= argc) {
    /* Get */
    if (ioctl(fd, VIDIOC_DBG_G_REGISTER, &v4l2reg) < 0) {
      perror("VIDIOC_DBG_G_REGISTER");
      return -1;
    }

  } else {
    /* Set */
    v4l2reg.val = strtol(argv[optind], 0, 0);
    if (ioctl(fd, VIDIOC_DBG_S_REGISTER, &v4l2reg) < 0) {
      perror("VIDIOC_DBG_S_REGISTER");
      return -1;
    }
  } 

  printf("0x%llx = 0x%llx\n", v4l2reg.reg, v4l2reg.val);
  close(fd);
  return 0;
}
