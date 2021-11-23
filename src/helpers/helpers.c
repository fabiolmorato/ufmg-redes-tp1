#include <stdbool.h>
#include <string.h>

bool is_numeric_string(char* str) {
  if (str == NULL) return false;
  size_t len = strlen(str);

  for (size_t i = 0; i < len; i++) {
    char c = str[i];
    if (!(c >= '0' && c <= '9')) return false;
  }

  return true;
}
