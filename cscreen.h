/**
 * 
 * cscreen.h version 1.01
 * 
 * UPDATE 1.01:
 * Improved performance when calculating pixel coordinates and drawing rectangles
 * Added functions 'drawBitmap' and 'runSync', for drawing bitmaps and running other
 * executables synchronized with the main process(wait the other executable to end),
 * respectively. If you want to create bitmaps, you can use this online tool:
 * http://descubre.inf.um.es/editor-codigo.php?id=f5c7d05034345634502f66e4c6e373d
 *  
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#ifndef SCREEN_WIDTH
#define SCREEN_WIDTH 80
#endif
#ifndef SCREEN_HEIGHT
#define SCREEN_HEIGHT 25
#endif
#define UP_ARROW 17
#define DOWN_ARROW 18
#define LEFT_ARROW 19
#define RIGHT_ARROW 20
#define NULL_CHAR (char)0
#define BLACK 0
#define BLUE 1
#define GREEN 2
#define CYAN 3
#define RED 4
#define MAGENTA 5
#define BROWN 6
#define LIGHT_GRAY 7
#define DARK_GRAY 8
#define LIGHT_BLUE 9
#define LIGHT_GREEN 10
#define LIGHT_CYAN 11
#define LIGHT_RED 12
#define LIGHT_MAGENTA 13
#define YELLOW 14
#define WHITE 15
#define CPI 3.14159265
#ifdef _WIN32
#include <windows.h>
#include <dos.h>
#include <io.h>
#include <conio.h>
#else
#define _POSIX_SOURCE
#include <unistd.h>
#endif
long int ticks = 0;
int multicolor = 0;
void setTextColor(FILE *stream, int color);
void wait(long int ms);
unsigned char getASCII();
void tone() {
  printf("\007");
}
long int seconds() {
  return floor((((double)ticks) / ((double)CLOCKS_PER_SEC))*60);
}
long int millis() {
  return floor(((((double)ticks) / ((double)CLOCKS_PER_SEC))*60)*1000);
}
#ifdef _WIN32
void wait(long int ms) {
  Sleep(ms);
}
unsigned char getASCII() {
  unsigned char res1 = getch();
  if (res1 == (unsigned char)224) {
    unsigned char res2 = getch();
    if (res2 == (unsigned char)72) {
      return (unsigned char)UP_ARROW;
    } else if (res2 == (unsigned char)80) {
      return (unsigned char)DOWN_ARROW;
    } else if (res2 == (unsigned char)75) {
      return (unsigned char)LEFT_ARROW;
    } else if (res2 == (unsigned char)77) {
      return (unsigned char)RIGHT_ARROW;
    }
    return (unsigned char)NULL_CHAR;
  }
  return res1;
}
void setTextColor(FILE *stream, int color) {
  int outfd = fileno(stream);
  HANDLE out = (HANDLE)_get_osfhandle(outfd);
  DWORD outType = GetFileType(out);
  DWORD mode;
  if (outType == FILE_TYPE_CHAR && GetConsoleMode(out, &mode)) {
    SetConsoleTextAttribute(out, color);
  }
}
void gotoXY(int x, int y) {  
  HANDLE hcon;  
  hcon = GetStdHandle(STD_OUTPUT_HANDLE);  
  COORD dwPos;  
  dwPos.X = x;  
  dwPos.Y= y;  
  SetConsoleCursorPosition(hcon, dwPos);  
}  
#else
void gotoXY(int x, int y) {
  printf("%c[%d;%df", 0x1B, y, x);
}
unsigned char getASCII() {
  system("/bin/stty raw");
  unsigned char res1 = getchar();
  if (res1 == (unsigned char)224) {
    unsigned char res2 = getchar();
    if (res2 == (unsigned char)72) {
      return (unsigned char)UP_ARROW;
    } else if (res2 == (unsigned char)80) {
      return (unsigned char)DOWN_ARROW;
    } else if (res2 == (unsigned char)75) {
      return (unsigned char)LEFT_ARROW;
    } else if (res2 == (unsigned char)77) {
      return (unsigned char)RIGHT_ARROW;
    }
    return (unsigned char)NULL_CHAR;
  }
  system("/bin/ssty cooked");
  return res1;
}
void wait(long int ms) {
  usleep(ms*1000);
}
static const char *ansiColorSequences[] = {
  "\x1B[0;30m",
  "\x1B[0;34m",
  "\x1B[0;32m",
  "\x1B[0;36m",
  "\x1B[0;31m",
  "\x1B[0;35m",
  "\x1B[0;33m",
  "\x1B[0;37m",
  "\x1B[1;30m",
  "\x1B[1;34m",
  "\x1B[1;32m",
  "\x1B[1;36m",
  "\x1B[1;31m",
  "\x1B[1;35m",
  "\x1B[1;33m",
  "\x1B[1;37m"
};
static const char *ansiColorTerms[] = {
  "xterm",
  "rxvt",
  "vt100",
  "linux",
  "screen",
  0
};
static int isAnsiColorTerm(void) {
  char *term = getenv("TERM");
  for (const char **ansiTerm = &ansiColorTerms[0]; *ansiTerm; ++ansiTerm) {
    int match = 1;
    const char *t = term;
    const char *a = *ansiTerm;
    while (*a && *t) {
      if (*a++ != *t++) {
        match = 0;
        break;
      }
    }
    if (match) return 1;
  }
  return 0;
}

void setTextColor(FILE *stream, int color) {
  int outfd = fileno(stream);
  if (isatty(outfd) && isAnsiColorTerm()) {
    fputs(ansiColorSequences[color], stream);
  }
}

#endif
unsigned char writingColor = 15;
char screen[SCREEN_WIDTH*SCREEN_HEIGHT+1];
unsigned char color[SCREEN_WIDTH*SCREEN_HEIGHT];
const double DEG2RAD = CPI/180.0;

void setColor(int c) {
  writingColor = c;
}

int randomInt(int min, int max) {
  return round((max-min)*((double)rand()/(double)RAND_MAX))+min;
}

float randomFloat(int min, int max) {
  return ((max-min)*((double)rand()/(double)RAND_MAX))+min;
}

void initLib() {
  for (int i = 0; i < SCREEN_WIDTH*SCREEN_HEIGHT; i++) {
    screen[i] = (char)0;
    color[i] = (unsigned char)0;
  }
  srand(time(0));
  randomInt(0, 15);
}

void background(int val) {
  for (int i = 0; i < SCREEN_WIDTH*SCREEN_HEIGHT; i++) {
    screen[i] = (char)(val*219);
  }
}

void clrScr() {
  #ifdef WIN32
  system("cls");
  #else
  system("clear");
  #endif
}

void updScr(int draw) {
  if (draw > 0) {
    gotoXY(0, 0); 
    for (int y = 0; y < SCREEN_HEIGHT; y++) {
      for (int x = 0; x < SCREEN_WIDTH; x++) {
        int ci = (SCREEN_WIDTH*y)+x;
        setTextColor(stdout, color[0]);
        printf("%c", screen[ci]);
      }
      if (y < SCREEN_HEIGHT-1) {
        printf("\n");
      }
    }
  } else if (draw < 0) {
    gotoXY(0, 0);
    for (int y = 0; y < SCREEN_HEIGHT; y++) {
      for (int x = 0; x < SCREEN_WIDTH; x++) {
        int ci = (SCREEN_WIDTH*y)+x;
        setTextColor(stdout, color[ci]);
        printf("%c", screen[ci]);
      }
      if (y < SCREEN_HEIGHT-1) {
        printf("\n");
      }
    }
  }
  ticks++;
}

void setRaw(char s, int x, int y) {
  int ci = x+(floor(y/2)*SCREEN_WIDTH);
  screen[ci] = s;
  color[ci] = writingColor;
}
long int size(char *str) {
    char *s;
    for (s = str; *s; ++s);
    return(s - str);
}
void gprint(char *text, int x, int y) {
  for (int i = 0; i < size(text); i++) {
    setRaw(text[i], x+i, y);
  }
}
void gprintC(char text[], int x, int y) {
  for (int i = 0; i < sizeof(text); i++) {
    setRaw(text[i], x+i, y);
  }
}
void setPixel(int x, int y, int val) {
  int i = x+(floor(y/2)*SCREEN_WIDTH);
  color[i] = writingColor;
  if (x >= 0 && x < SCREEN_WIDTH) {
    if (y >= 0 && y < (SCREEN_HEIGHT*2)) {
      char scrval = screen[i];
      if (y%2==0) {
        if (val > 0) {
          if (scrval == (char)0) {
            screen[i] = (char)223;
          } else if (scrval == (char)220) {
            screen[i] = (char)219;
          }
        } else {
          if (scrval == (char)219) {
            screen[i] = (char)220;
          } else if (scrval == (char)223) {
            screen[i] = (char)0;
          }
        }
      } else {
        if (val > 0) {
          if (scrval == (char)0) {
            screen[i] = (char)220;
          } else if (scrval == (char)223) {
            screen[i] = (char)219;
          }
        } else {
          if (scrval == (char)219) {
            screen[i] = (char)223;
          } else if (scrval == (char)220) {
            screen[i] = (char)0;
          }
        }
      }
    }
  }
}
double dist(int x1, int y1, int x2, int y2) {
  return sqrt(pow(x1-x2, 2)+pow(y1-y2, 2));
}
void line(double x1, double y1, double x2, double y2, int val) {
  double ang = atan2(abs(y1-y2), abs(x1-x2));
  double dst = dist(x1, y1, x2, y2);
  double nx = x1;
  double ny = y1;
  setPixel(x1, y1, val);
  setPixel(x2, y2, val);
  for (double i = 0; i < dst; i += 1) {
    setPixel(nx, ny, val);
    if ((x1-x2) > 0) {
      nx -= cos(ang);
    } else {
      nx += cos(ang);
    }
    if ((y1-y2) > 0) {
      ny -= sin(ang);
    } else {
      ny += sin(ang);
    }
  }
}

void rect(int x, int y, int w, int h, int val) {
  line(x, y, x+w, y, val);
  line(x, y, x, y+h, val);
  line(x+w, y, x+w, y+h, val);
  line(x, y+h, x+w, y+h, val);
  setPixel(x+w, y+h, val);
}

void fillRect(int x, int y, int w, int h, int val) {
  int dx = x+w;
  int dy = y+h;
  for (int nx = x; nx < dx; x++) {
    for (int ny = y; ny < dy; y++) {
      setPixel(nx, ny, val);
    }
  }
}

void quad(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, int val) {
  line(x1, y1, x2, y2, val);
  line(x2, y2, x3, y3, val);
  line(x3, y3, x4, y4, val);
  line(x4, y4, x1, y1, val);
}

void triangle(int x1, int y1, int x2, int y2, int x3, int y3, int val) {
  line(x1, y1, x2, y2, val);
  line(x2, y2, x3, y3, val);
  line(x3, y3, x1, y1, val);
}

double maxVal(double a, double b) {
  return a>b?a:b;
}

void ellipse(int x, int y, int RX, int RY, int val) {
	double i, inc;
	inc = (CPI+0.1) / maxVal(RX,RY) / 4;
	i=0;
  int nx = (int)((RX*cos(i)+y)+.5);
  int ny = (int)((RY*sin(i)+x)+.5);
  int px = 0;
  int py = 0;
	while(i <= (CPI+0.1)*2) {
    px = nx;
    py = ny;
    nx = (int)((RX*cos(i)+y)+.5);
    ny = (int)((RY*sin(i)+x)+.5);
    line(px, py, nx, ny, val);
		i=i+inc;
	}
}

void drawBitmap(int **bitmap, int rx, int ry, int bmpw, int bmph) {
  for (int x = 0; x < bmpw; x++) {
    for (int y = 0; y < bmph; y++) {
      if (bitmap[y][x] > 0) {
        setColor(bitmap[y][x]);
      }
      setPixel(x+rx, y+ry, moneyAdd[y][x]>0?1:0);
    }
  }
}

void runSync(char *exec) {
  system(exec);
  wait(15);
}

// COMMING SOON: 

/**
void fileSet(char *fname, char *content) {

}

void fileAdd(char *fname, char *content) {

}

void fileDelete(char *fname) {

}

void fileCreate(char *fname) {

}

int fileExists(char *fname) {

}

void fileRename(char *fname, char *newfname) {

}

void fileMove(char *fname, char *newfname) {

}

void fileCopy(char *fname, char *newfname) {

}
*/