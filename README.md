# cscreen.h library
A cross-platform version of conio.h and curses.h.

To initialize the library, you can use <code>initLib()</code> function.
<br><code>initLib(); // Initialize the library</code><br>

To update the screen, you can use <code>updScr(Mode)</code> function. There are 3 modes: No update(0), Update monocolor(1) or Update color dependent pair of pixels(2). When you are in monocolor mode and you have a multicolor bitmap, all the pixels will be of the color of the first pixel. Also, if you are in multicolor mode and you put a monocolor bitmap, all the pixels except the first will have a random color.
<br><code>updScr(1); // Update the screen in monocolor mode</code><br>
  
