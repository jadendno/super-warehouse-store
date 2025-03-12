#include <sqlite3.h>

sqlite3* OpenDb(const char* name);

namespace init {
  void Init(const char* name);
}
