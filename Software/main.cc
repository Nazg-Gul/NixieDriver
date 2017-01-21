#include <errno.h>
#include <signal.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include <libusb.h>

//#define VERSION "0.1.0"
#define VENDOR_ID 0x04d8
#define PRODUCT_ID 0x003f

//#define VENDOR_ID 0x05ac
//#define PRODUCT_ID 0x0221

const static int PACKET_INT_LEN = 65;
const static int INTERFACE = 0;
const static int ENDPOINT_INT_IN = 0x81; /* endpoint 0x81 address for IN */
const static int ENDPOINT_INT_OUT = 0x01; /* endpoint 1 address for OUT */
const static int TIMEOUT = 5000; /* timeout in ms */

#define COMMAND_SET_DIGITS 0x80

static void bad(const char *why) {
  fprintf(stderr, "Fatal error> %s\n", why);
  exit(17);
}

static libusb_device_handle *devh = NULL;
static libusb_context *ctx = NULL;

static int find_lvr_hidusb(void) {
  devh = libusb_open_device_with_vid_pid(ctx, VENDOR_ID, PRODUCT_ID);
  return devh ? 0 : -EIO;
}

static int send_command(int command, int arg1, int arg2, int arg3) {
  int r, i;
  int transferred;
  unsigned char buffer[PACKET_INT_LEN];

  //buffer[0] = 0;
  buffer[0] = command;
  buffer[1] = arg1;
  buffer[2] = arg2;
  buffer[3] = arg3;

  r = libusb_interrupt_transfer(devh,
                                ENDPOINT_INT_OUT,
                                buffer,
                                PACKET_INT_LEN,
                                &transferred,
                                TIMEOUT);

  if (r < 0) {
    fprintf(stderr, "Interrupt write error %s\n", libusb_error_name(r));
  }

  return 0;
}

static int read_answer(unsigned char *buffer) {
  int r;
  int transferred;

  r = libusb_interrupt_transfer(devh,
                                ENDPOINT_INT_IN,
                                buffer,
                                PACKET_INT_LEN,
                                &transferred,
                                TIMEOUT);
  if (r < 0) {
    fprintf(stderr, "Interrupt read error %s\n", libusb_error_name(r));
    return r;
  }
}

static void send_nixie_value(int value) {
  send_command(COMMAND_SET_DIGITS,
               value % 10,
               value / 10 % 10,
               value / 100 % 10);
}

int main(int argc, char **argv) {
  int r = 1;

  if (argc < 2) {
    fprintf(stderr, "Usage: %s <value_to_display>\n", argv[0]);
    return EXIT_FAILURE;
  }

  r = libusb_init(&ctx);
  if (r < 0) {
    fprintf(stderr, "Failed to initialise libusb\n");
    return EXIT_FAILURE;
  }

  /* Set verbosity level to 3, as suggested in the documentation. */
  libusb_set_debug(ctx, 3);

  r = find_lvr_hidusb();
  if (r < 0) {
    fprintf(stderr, "Could not find/open LVR Generic HID device\n");
    return EXIT_FAILURE;
  }

  printf("Successfully find the LVR Generic HID device\n");

  libusb_detach_kernel_driver(devh, 0);

#if 0
  r = libusb_set_configuration(devh, 1);
  if (r < 0) {
    fprintf(stderr, "libusb_set_configuration error %d\n", r);
    goto out;
  }

  printf("Successfully set usb configuration 1\n");
#endif

  r = libusb_claim_interface(devh, 0);
   if (r < 0) {
    fprintf(stderr, "libusb_claim_interface error %d\n", r);
    goto out;
  }

  printf("Successfully claimed interface\n");

  libusb_release_interface(devh, 0);

  send_nixie_value(atoi(argv[1]));

out:
  /* libusb_reset_device(devh); */
  libusb_close(devh);
  libusb_exit(ctx);
  return r == 0 ? EXIT_SUCCESS : -EXIT_FAILURE;
}
