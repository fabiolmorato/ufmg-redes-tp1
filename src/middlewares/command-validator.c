#include <stdbool.h>
#include <ctype.h>
#include <string.h>

bool command_validator_middleware(char** params, unsigned int size, char** response) {
  for (unsigned int i = 0; i < size; i++) {
    char* word = params[i];
    size_t word_len = strlen(word);

    for (unsigned int j = 0; j < word_len; j++) {
      char c = word[j];

      if (!(c >= 'a' && c <= 'z')) {
        strncpy(*response, "invalid message", 16);
        return false;
      }
    }
  }

  return true;
}
