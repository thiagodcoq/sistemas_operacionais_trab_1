#include <sys/types.h>

struct ioinfo {
  pid_t pid_requester;
  char type;
};
typedef struct ioinfo IOinfo;
