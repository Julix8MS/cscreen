# cscreen.h library
A cross-platform version of conio.h and curses.h.

To initialize the library, you can use <code>initLib()</code> function.
<code>
#include "cscreen.h"
  
int main() {
  initLib(); // Initialize the library
  line(0, 0, 79, 49, 1);
  updScr(1);
  while (1);
  return 0;
}
</code>
