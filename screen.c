/**
 * PLATOTerm64 - A PLATO Terminal for the Commodore 64
 * Based on Steve Peltz's PAD
 *
 * Author: Thomas Cherryhomes <thom.cherryhomes at gmail dot com>
 *
 * screen.c - Display output functions
 */

#include <unistd.h>
#include <stdlib.h>
#include <windows.h>
#include "screen.h"
#include "protocol.h"
#include "font.h"
#include "io.h"

unsigned char CharHigh=16;
unsigned char CharWide=8;
padPt TTYLoc;
HDC hDC;

extern int ySize;

unsigned char fontm23[2048];
COLORREF foreground, background;
padRGB foregroundColor, backgroundColor;
HWND hwnd;

#define true 1
#define false 0

extern padBool FastText;

unsigned short scaley[]={
463,462,461,460,459,458,457,456,
455,454,454,453,452,451,450,449,
448,447,446,445,445,444,443,442,
441,440,439,438,437,436,435,435,
434,433,432,431,430,429,428,427,
426,426,425,424,423,422,421,420,
419,418,417,416,416,415,414,413,
412,411,410,409,408,407,407,406,
405,404,403,402,401,400,399,398,
397,397,396,395,394,393,392,391,
390,389,388,388,387,386,385,384,
383,382,381,380,379,378,378,377,
376,375,374,373,372,371,370,369,
369,368,367,366,365,364,363,362,
361,360,360,359,358,357,356,355,
354,353,352,351,350,350,349,348,
347,346,345,344,343,342,341,341,
340,339,338,337,336,335,334,333,
332,331,331,330,329,328,327,326,
325,324,323,322,322,321,320,319,
318,317,316,315,314,313,312,312,
311,310,309,308,307,306,305,304,
303,303,302,301,300,299,298,297,
296,295,294,293,293,292,291,290,
289,288,287,286,285,284,284,283,
282,281,280,279,278,277,276,275,
275,274,273,272,271,270,269,268,
267,266,265,265,264,263,262,261,
260,259,258,257,256,256,255,254,
253,252,251,250,249,248,247,246,
246,245,244,243,242,241,240,239,
238,237,237,236,235,234,233,232,
231,230,229,228,227,227,226,225,
224,223,222,221,220,219,218,218,
217,216,215,214,213,212,211,210,
209,208,208,207,206,205,204,203,
202,201,200,199,199,198,197,196,
195,194,193,192,191,190,189,189,
188,187,186,185,184,183,182,181,
180,180,179,178,177,176,175,174,
173,172,171,171,170,169,168,167,
166,165,164,163,162,161,161,160,
159,158,157,156,155,154,153,152,
152,151,150,149,148,147,146,145,
144,143,142,142,141,140,139,138,
137,136,135,134,133,133,132,131,
130,129,128,127,126,125,124,123,
123,122,121,120,119,118,117,116,
115,114,114,113,112,111,110,109,
108,107,106,105,104,104,103,102,
101,100,99,98,97,96,95,95,
94,93,92,91,90,89,88,87,
86,86,85,84,83,82,81,80,
79,78,77,76,76,75,74,73,
72,71,70,69,68,67,67,66,
65,64,63,62,61,60,59,58,
57,57,56,55,54,53,52,51,
50,49,48,48,47,46,45,44,
43,42,41,40,39,38,38,37,
36,35,34,33,32,31,30,29,
29,28,27,26,25,24,23,22,
21,20,19,19,18,17,16,15,
14,13,12,11,10,10,9,8,
7,6,5,4,3,2,1,0,
};

/**
 * screen_init() - Set up the screen
 */
void screen_init(HWND _hwnd)
{
    hwnd = _hwnd;
    foregroundColor.red=foregroundColor.green=foregroundColor.blue=255;
    backgroundColor.red=backgroundColor.green=backgroundColor.blue=0;
    foreground=RGB(255,255,255);
    background=RGB(0,0,0);
    screen_clear();
}

/**
 * screen_set_hdc() - set handle to display context
 */
void screen_set_hdc(HDC _hdc)
{
    hDC = _hdc;
}

/**
 * screen_main() - render/preserve screen
 */
void screen_main(void)
{
}

/**
 * screen_wait(void) - Sleep for approx 16.67ms
 */
void screen_wait(void)
{
}

/**
 * screen_beep(void) - Beep the terminal
 */
void screen_beep(void)
{
    MessageBeep(0);
}

/**
 * screen_clear - Clear the screen
 */
void screen_clear(void)
{
    RECT r;
    HBRUSH brush;

    r.top=0;
    r.bottom=512;
    r.left=0;
    r.right=512;

    io_replay_clear();

    brush = CreateSolidBrush(background);
    SelectObject(hDC,brush);
    FillRect(hDC,&r,brush);
    SelectObject(hDC,GetStockObject(NULL_BRUSH));
    DeleteObject(brush);
}

/**
 * screen_block_draw(Coord1, Coord2) - Perform a block fill from Coord1 to Coord2
 */
void screen_block_draw(padPt* Coord1, padPt* Coord2)
{
  short x1;
  short y1;
  short x2;
  short y2;
    RECT r;
    HBRUSH brush;

  x1=min(Coord1->x,Coord2->x);
  x2=max(Coord1->x,Coord2->x);
  if (ySize < 512)
  {
      y1=min(scaley[Coord1->y],scaley[Coord2->y]);
      y2=max(scaley[Coord1->y],scaley[Coord2->y]);
  }
  else
  {
    y1=min((Coord1->y^0x1FF)&0x1FF,(Coord2->y^0x1FF)&0x1FF);
    y2=max((Coord1->y^0x1FF)&0x1FF,(Coord2->y^0x1FF)&0x1FF);
  }
    r.left = x1;
    r.right = x2;
    r.top = y1;
    r.bottom = y2;

    if (CurMode==ModeInverse || CurMode==ModeErase)
    {
        brush = CreateSolidBrush(background);
    }
    else
    {
        brush = CreateSolidBrush(foreground);
    }

    SelectObject(hDC,brush);
    FillRect(hDC,&r,brush);

    SelectObject(hDC,GetStockObject(NULL_BRUSH));
    DeleteObject(brush);
}

/**
 * screen_dot_draw(Coord) - Plot a mode 0 pixel
 */
void screen_dot_draw(padPt* Coord)
{
    if (CurMode == ModeErase || CurMode == ModeInverse)
    {
        if (ySize < 512)
        {
            SetPixel(hDC,Coord->x,scaley[Coord->y],background);
        }
        else
            SetPixel(hDC,Coord->x,Coord->y^0x1FF,background);
    }
    else
    {
        if (ySize < 512)
        {
            SetPixel(hDC,Coord->x,scaley[Coord->y],foreground);
        }
        else
            SetPixel(hDC,Coord->x,Coord->y^0x1FF,foreground);
    }
}

/**
 * screen_line_draw(Coord1, Coord2) - Draw a mode 1 line
 */
void screen_line_draw(padPt* Coord1, padPt* Coord2)
{
    short x1,y1,x2,y2;
    HPEN pen;

    x1=Coord1->x;
    x2=Coord2->x;

    if (ySize < 512)
    {
        y1=scaley[Coord1->y];
        y2=scaley[Coord2->y];
    }
    else
    {
        y1=(Coord1->y^0x1FF)&0x1FF;
        y2=(Coord2->y^0x1FF)&0x1FF;
    }

    if (CurMode == ModeErase || CurMode == ModeInverse)
    {
        pen = CreatePen(PS_SOLID,1,background);
    }
    else
    {
        pen = CreatePen(PS_SOLID,1,foreground);
    }

    SelectObject(hDC,pen);
    MoveToEx(hDC,x1,y1,NULL);
    LineTo(hDC,x2,y2);
    SelectObject(hDC,GetStockObject(NULL_PEN));
    DeleteObject(pen);
}

/**
 * screen_char_draw(Coord, ch, count) - Output buffer from ch* of length count as PLATO characters
 */
void screen_char_draw(padPt* Coord, unsigned char* ch, unsigned char count)
{
  short offset; /* due to negative offsets */
  unsigned short x;      /* Current X and Y coordinates */
  unsigned short y;
  unsigned short* px;   /* Pointers to X and Y coordinates used for actual plotting */
  unsigned short* py;
  unsigned char i; /* current character counter */
  unsigned char a; /* current character byte */
  unsigned char j,k; /* loop counters */
  char b; /* current character row bit signed */
  char width=8;
  char height=16;
  short deltaX=1;
  short deltaY=1;
  unsigned char *p;
  unsigned char* curfont;
  COLORREF mainColor, altColor;

  if (CurMode==ModeRewrite)
    {
      altColor = RGB(backgroundColor.red,backgroundColor.green,backgroundColor.blue);
    }
  else if (CurMode==ModeInverse)
    {
      altColor = RGB(foregroundColor.red,foregroundColor.green,foregroundColor.blue);
    }

  if (CurMode==ModeErase || CurMode==ModeInverse)
    {
      mainColor = RGB(backgroundColor.red,backgroundColor.green,backgroundColor.blue);
    }
  else
    {
      mainColor = RGB(foregroundColor.red,foregroundColor.green,foregroundColor.blue);
    }

  switch(CurMem)
    {
    case M0:
      curfont=font;
      offset=-32;
      break;
    case M1:
      curfont=font;
      offset=64;
      break;
    case M2:
      curfont=fontm23;
      offset=-32;
      break;
    case M3:
      curfont=fontm23;
      offset=32;
      break;
    }

  x=Coord->x;
  if (ModeBold)
    y=((Coord->y+30)^0x1FF)&0x1FF;
  else
    y=((Coord->y+15)^0x1FF)&0x1FF;

  if (FastText==padF)
    {
      goto chardraw_with_fries;
    }

  /* the diet chardraw routine - fast text output. */

  for (i=0;i<count;++i)
    {
      a=*ch;
      ++ch;
      a+=offset;
      p=&curfont[FONTPTR(a)];

      for (j=0;j<16;++j)
        {
          b=*p;

          for (k=0;k<8;++k)
            {
              if (b&0x80) /* check sign bit. */
                {
                    if (ySize < 512)
                        SetPixel(hDC,x,scaley[y^0x1FF],mainColor);
                    else
                        SetPixel(hDC,x,y,mainColor);
                }

              ++x;
              b<<=1;
            }

          ++y;
          x-=width;
          ++p;
        }

      x+=width;
      y-=height;
    }

  return;

 chardraw_with_fries:
  if (Rotate)
    {
      deltaX=-abs(deltaX);
      width=-abs(width);
      px=&y;
      py=&x;
    }
    else
    {
      px=&x;
      py=&y;
    }

  if (ModeBold)
    {
      deltaX = deltaY = 2;
      width<<=1;
      height<<=1;
    }

  for (i=0;i<count;++i)
    {
      a=*ch;
      ++ch;
      a+=offset;
      p=&curfont[FONTPTR(a)];
      for (j=0;j<16;++j)
        {
          b=*p;

          if (Rotate)
            {
              px=&y;
              py=&x;
            }
          else
            {
              px=&x;
              py=&y;
            }

          for (k=0;k<8;++k)
            {
              if (b&0x80) /* check sign bit. */
                {
                  if (ModeBold)
                    {
                        if (ySize<512)
                        {
                            SetPixel(hDC,*px+1,scaley[*py^0x1FF],mainColor);
                            SetPixel(hDC,*px,scaley[*py+1^0x1FF],mainColor);
                            SetPixel(hDC,*px+1,scaley[*py+1^0x1FF],mainColor);
                        }
                        else
                        {
                            SetPixel(hDC,*px+1,*py,mainColor);
                            SetPixel(hDC,*px,*py+1,mainColor);
                            SetPixel(hDC,*px+1,*py+1,mainColor);
                        }
                    }
                    if (ySize<512)
                        SetPixel(hDC,*px,scaley[*py^0x1FF],mainColor);
                    else
                        SetPixel(hDC,*px,*py,mainColor);
                }
              else
                {
                  if (CurMode==ModeInverse || CurMode==ModeRewrite)
                    {
                      if (ModeBold)
                        {
                            if (ySize<512)
                            {
                                SetPixel(hDC,*px+1,scaley[*py^0x1FF],altColor);
                                SetPixel(hDC,*px,scaley[*py+1^0x1FF],altColor);
                                SetPixel(hDC,*px+1,scaley[*py+1^0x1FF],altColor);
                            }
                            else
                            {
                                SetPixel(hDC,*px+1,*py,altColor);
                                SetPixel(hDC,*px,*py+1,altColor);
                                SetPixel(hDC,*px+1,*py+1,altColor);
                            }
                        }
                        if (ySize<512)
                            SetPixel(hDC,*px,scaley[*py^0x1FF],altColor);
                        else
                            SetPixel(hDC,*px,*py,altColor);
                    }
                }

              x += deltaX;
              b<<=1;
            }

          y+=deltaY;
          x-=width;
          ++p;
        }

      Coord->x+=width;
      x+=width;
      y-=height;
    }

  return;
}

/**
 * screen_tty_char - Called to plot chars when in tty mode
 */
void screen_tty_char(padByte theChar)
{
  if ((theChar >= 0x20) && (theChar < 0x7F)) {
    screen_char_draw(&TTYLoc, &theChar, 1);
    TTYLoc.x += CharWide;
  }
  else if (theChar == 0x0b) /* Vertical Tab */
    {
      TTYLoc.y += CharHigh;
    }
  else if ((theChar == 0x08) && (TTYLoc.x > 7))/* backspace */
    {
      TTYLoc.x -= CharWide;
      // screen_block_draw(&scalex[TTYLoc.x],&scaley[TTYLoc.y],&scalex[TTYLoc.x+CharWide],&scaley[TTYLoc.y+CharHigh]);
    }
  else if (theChar == 0x0A)/* line feed */
    TTYLoc.y -= CharHigh;
  else if (theChar == 0x0D)/* carriage return */
    TTYLoc.x = 0;

  if (TTYLoc.x + CharWide > 511) {/* wrap at right side */
    TTYLoc.x = 0;
    TTYLoc.y -= CharHigh;
  }

  if (TTYLoc.y < 0) {
    screen_clear();
    TTYLoc.y=495;
  }

}

/**
 * screen_foreground - Called to set foreground color.
 */
void screen_foreground(padRGB* theColor)
{
  foregroundColor.red = theColor->red;
  foregroundColor.green = theColor->green;
  foregroundColor.blue = theColor->blue;
  foreground = GetNearestColor(hDC,RGB(theColor->red, theColor->green, theColor->blue));
}

/**
 * screen_background - Called to set foreground color.
 */
void screen_background(padRGB* theColor)
{
  backgroundColor.red = theColor->red;
  backgroundColor.green = theColor->green;
  backgroundColor.blue = theColor->blue;
  background = GetNearestColor(hDC,RGB(theColor->red, theColor->green, theColor->blue));
}


/**
 * screen_paint - Called to paint at location.
 */
void screen_paint(padPt* Coord)
{
    int x = Coord->x;
    int y;
    HBRUSH brush;

    if (ySize<512)
        y = scaley[Coord->y];
    else
        y = Coord->y^0x1FF;

    brush = CreateSolidBrush(foreground);
    SelectObject(hDC,brush);
    FloodFill(hDC,x,y,foreground);
    SelectObject(hDC,GetStockObject(NULL_BRUSH));
    DeleteObject(brush);
}

/**
 * screen_done()
 * Close down TGI
 */
void screen_done(void)
{
}
