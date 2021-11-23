#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "helpers/helpers.h"

void validate_args(int argc, char** argv);

int main(int argc, char** argv) {
  validate_args(argc, argv);
  unsigned int port = (unsigned int) atoi(argv[2]);

  return 0;
}

void validate_args(int argc, char** argv) {
  if (argc != 3) {
    fprintf(stderr, "Usage: %s <v4|v6> <PORT>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  if (strcmp(argv[1], "v4") != 0 && strcmp(argv[1], "v6") != 0) {
    fprintf(stderr, "Protocol must be \"v4\" or \"v6\"!\n");
    fprintf(stderr, "Usage: %s <v4|v6> <PORT>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  if (!is_numeric_string(argv[2])) {
    fprintf(stderr, "Port must be a positive number!\n");
    fprintf(stderr, "Usage: %s <v4|v6> <PORT>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  unsigned int port = (unsigned int) atoi(argv[2]);
  if (port < 1024 || port > 65535) {
    fprintf(stderr, "Port is not in a valid range!\n");
    fprintf(stderr, "Usage: %s <v4|v6> <PORT>\n", argv[0]);
    exit(EXIT_FAILURE);
  }
}
