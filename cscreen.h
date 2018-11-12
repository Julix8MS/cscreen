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
#define NULL_CHAR 0
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
#define CPI 3.14159269
#ifdef _WIN32
#warning Al usarlo con windows se sustituira la libreria unistd.h con conio.h, windows.h, io.h y dos.h
#include <windows.h>
#include <dos.h>
#include <io.h>
#include <conio.h>
#else
#define _POSIX_SOURCE
#include <unistd.h>
#endif
long int ticks = 0;
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

void setColor(int c) {
  setTextColor(stdout, c);
}

int randomInt(int min, int max) {
  return round((max-min)*((double)rand()/(double)RAND_MAX))+min;
}

float randomFloat(int min, int max) {
  return ((max-min)*((double)rand()/(double)RAND_MAX))+min;
}

char screen[SCREEN_WIDTH*SCREEN_HEIGHT];
const double DEG2RAD = CPI/180.0;

void initLib() {
  for (int i = 0; i < SCREEN_WIDTH*SCREEN_HEIGHT; i++) {
    screen[i] = (char)0;
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
    //clrScr();
    for (int y = 0; y < SCREEN_HEIGHT; y++) {
      for (int x = 0; x < SCREEN_WIDTH; x++) {
        printf("%c", screen[(SCREEN_WIDTH*y)+x]);
      }
      if (y < SCREEN_HEIGHT-1) {
        printf("\n");
      }
    }
  } else if (draw < 0) {
    gotoXY(0, 0);
    //clrScr();
    for (int y = 0; y < SCREEN_HEIGHT; y++) {
      for (int x = 0; x < SCREEN_WIDTH; x++) {
        setColor(randomInt(1, 15));
        printf("%c", screen[(SCREEN_WIDTH*y)+x]);
      }
      if (y < SCREEN_HEIGHT-1) {
        printf("\n");
      }
    }
  }
  ticks++;
}

void setRaw(char s, int x, int y) {
  int ny = floor(y/2);
  screen[x+(ny*SCREEN_WIDTH)] = s;
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
void setPixel(int x, int y, int val) {
  if (x >= 0 && x < SCREEN_WIDTH) {
    if (y >= 0 && y < (SCREEN_HEIGHT*2)) {
      int i = x+(floor(y/2)*SCREEN_WIDTH);
      if (y%2==0) {
        if (val > 0) {
          if (screen[i] == (char)0) {
            screen[i] = (char)223;
          } else if (screen[i] == (char)220) {
            screen[i] = (char)219;
          }
        } else {
          if (screen[i] == (char)219) {
            screen[i] = (char)220;
          } else if (screen[i] == (char)223) {
            screen[i] = (char)0;
          }
        }
      } else {
        if (val > 0) {
          if (screen[i] == (char)0) {
            screen[i] = (char)220;
          } else if (screen[i] == (char)223) {
            screen[i] = (char)219;
          }
        } else {
          if (screen[i] == (char)219) {
            screen[i] = (char)223;
          } else if (screen[i] == (char)220) {
            screen[i] = (char)0;
          }
        }
      }
    }
  }
}

void rawLine(int x0, int y0, int x1, int y1, int val) {
  int dy, dx, incrE, incrNE, d, x, y;
  dx = x1 - x0;
  dy = y1 - y0;
  d = 2 * dy - dx;
  incrE = 2*dy;
  incrNE = 2*(dy - dx);
  x = x0;
  y = y0;
  setPixel(x, y, val);
  while(x < x1) {
    if (d <= 0) {
	    d += incrE;
	    x++;
	  } else {
      d += incrNE;
      x++;
      y++;
	  }
    setPixel(x, y, val);
  } 
}

void line(int x1, int y1, int x2, int y2, int val) {
  if (y1 > y2) {
    rawLine(x2, y2, x1, y1, val);
  } else if (y1 < y2) {
    rawLine(x1, y1, x2, y2, val);
  } else if (y1 == y2) {
    if (x1 > x2) {
      rawLine(x2, y2, x1, y1, val);
    } else if (x1 < x2) {
      rawLine(x1, y1, x2, y2, val);
    } else if (x1 == x2) {
      setPixel(x1, y1, val);
    }
  }
}

void rect(int x, int y, int w, int h, int val) {
  line(x, y, x+w, y, val);
  line(x, y, x, y+h, val);
  line(x+w, y, x+w, y+h, val);
  line(x, y+h, x+w, y+h, val);
}

void fillRect(int x, int y, int w, int h, int val) {
  for (int ny = y; ny < y+h; ny++) {
    line(x, ny, x+w, ny, val);
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
	inc = CPI / maxVal(RX,RY) / 2;
	i=0;
  int nx = (int)((RX*cos(i)+y)+.5);
  int ny = (int)((RY*sin(i)+x)+.5);
  int px = 0;
  int py = 0;
	while(i <= CPI*2) {
    px = nx;
    py = ny;
    nx = (int)((RX*cos(i)+y)+.5);
    ny = (int)((RY*sin(i)+x)+.5);
    setPixel(nx, ny, val);
    line(px, py, nx, ny, val);
		i=i+inc;
	}
}