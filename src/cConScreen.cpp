#include <iostream>
#include <windows.h>
#include "cConScreen.h"

#include <conio.h>
#include <stdio.h>

#define CON_STRING_LEN 0xffff

// [0] Top left corner
// [1] Top right corner
// [2] Bottom left corner
// [3] Bottom right corner
// [4] Top horizontal line
// [5] Bottom horizontal line
// [6] Left Vertical line
// [7] Right Vertical Line
// http://www.asciitable.com/
static BYTE puFrame[9][8] =
{
  { 0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 },
  {201, 187, 200, 188, 205, 205, 186, 186},
  {214, 183, 211, 189, 196, 196, 186, 186},
  {213, 184, 212, 190, 205, 205, 179, 179},
  {218, 191, 192, 217, 196, 196, 179, 179},
  {220, 220, 223, 223, 220, 223, 221, 222},
  {219, 219, 219, 219, 223, 220, 221, 222},
  {219, 219, 219, 219, 254, 254, 219, 219},
  {219, 219, 219, 219, 219, 219, 219, 219}
};

conScreen::conScreen()
{
  this->m_hConOut  = GetStdHandle(STD_OUTPUT_HANDLE);
  this->m_hConIn   = GetStdHandle(STD_INPUT_HANDLE);
  this->m_iFrameID = 0;
  this->m_curChar  = 0;
  this->m_uiEvents = 0;
  this->m_xyPos.X  = 0;
  this->m_xyPos.Y  = 0;
  this->m_xySiz.X  = 0;
  this->m_xySiz.Y  = 0;
  this->m_curCol   = 0;
}

conScreen::~conScreen()
{
  //dtor
}

//***************************BUFFER AND FONT******************************************

CONSOLE_SCREEN_BUFFER_INFO* conScreen::getBufferInfo(void)
{
  GetConsoleScreenBufferInfo(this->m_hConOut, &this->m_curBuff);
  return &this->m_curBuff;
}

#if _WIN32_WINNT > 0x0500
  // https://docs.microsoft.com/en-us/windows/console/getcurrentconsolefont
  CONSOLE_FONT_INFO* conScreen::getFontInfo(BOOL wmax)
  {
    GetCurrentConsoleFont(this->m_hConOut, wmax, &this->m_curFont);
    return &this->m_curFont;
  }

#endif

//*****************************POSITION AND SIZE*****************************************

conScreen& conScreen::setPos(SHORT x, SHORT y)
{
  this->m_xyPos.X = x; this->m_xyPos.Y = y;
  SetConsoleCursorPosition(this->m_hConOut, this->m_xyPos);
  return *this;
}

COORD* conScreen::getPos(void)
{
  return &(this->m_xyPos);
}

conScreen& conScreen::setSize(SHORT x, SHORT y)
{
  this->m_xySiz.X = x; this->m_xySiz.Y = y;
  SetConsoleScreenBufferSize(this->m_hConOut, this->m_xySiz);
  return *this;
}

COORD* conScreen::getSize(void)
{
  return &(this->m_xySiz);
}

//***************************HANDLE******************************************

HANDLE conScreen::getHandleOut(void) const
{
  return this->m_hConOut;
}

HANDLE conScreen::getHandleIn(void) const
{
  return this->m_hConIn;
}

//*****************************CHAR AND COLOR**************************************
// https://docs.microsoft.com/en-us/windows/console/console-screen-buffers#_win32_font_attributes

conScreen& conScreen::insByte(WORD chr)
{
  this->m_curChar = chr; return *this;
}

conScreen& conScreen::insColor(WORD clr)
{
  this->m_curCol = clr; return *this;
}

conScreen& conScreen::insColor(WORD fg, WORD bg)
{
  return insColor(16 * bg + fg);
}

conScreen& conScreen::setColor(WORD clr)
{
  SetConsoleTextAttribute(this->m_hConOut, clr); return *this;
}

conScreen& conScreen::setColor(WORD fg, WORD bg)
{
  return this->setColor(16 * bg + fg);
}

//*********************************FILL************************************

conScreen& conScreen::fillCharacter(BYTE pat)
{
  DWORD cCharsWritten, dwConSize; setPos(0,0);
  dwConSize = this->m_xySiz.X * this->m_xySiz.Y;
  FillConsoleOutputCharacter(this->m_hConOut, pat, dwConSize, this->m_xyPos, &cCharsWritten);
  return *this;
}

conScreen& conScreen::fillCharacter(void)
{
  return fillCharacter(this->m_curChar);
}

conScreen& conScreen::fillColor(WORD clr)
{
  DWORD cCharsWritten, dwConSize; setPos(0,0);
  dwConSize = this->m_xySiz.X * this->m_xySiz.Y;
  FillConsoleOutputAttribute(this->m_hConOut, clr, dwConSize, this->m_xyPos, &cCharsWritten);
  return *this;
}

conScreen& conScreen::fillColor(void)
{
  return fillColor(this->m_curCol);
}

//*****************************************************************************

conScreen& conScreen::clrScreen(BYTE pat, WORD clr)
{
  return fillCharacter(pat).fillColor(clr).setPos(0,0);
}

conScreen& conScreen::clrScreen(BYTE pat)
{
  return fillCharacter(pat).fillColor().setPos(0,0);
}

conScreen& conScreen::clrScreen(void)
{
  return fillCharacter().fillColor().setPos(0,0);
}

//*********************************TEXT**********************************

conScreen& conScreen::drawText(SHORT x, SHORT y, const void* txt, WORD clr)
{
  setPos(x, y); setColor(clr); std::cout << (BYTE*)(txt); return *this;
}

conScreen& conScreen::drawText(SHORT x, SHORT y, const void* txt)
{
  return drawText(x,y,txt,this->m_curCol);
}

conScreen& conScreen::drawText(const void* txt, WORD clr)
{
  return drawText(this->m_xyPos.X,this->m_xyPos.X,txt,clr);
}

conScreen& conScreen::drawText(const void* txt)
{
  return drawText(this->m_xyPos.X,this->m_xyPos.X,txt,this->m_curCol);
}

//*********************************INPUT AND EVENTS**********************************

conScreen& conScreen::readInput()
{
  ReadConsoleInput(this->m_hConIn, &this->m_stInput, 1, &this->m_uiEvents);
  return *this;
}

// https://docs.microsoft.com/en-us/windows/console/input-record-str
INPUT_RECORD* conScreen::getInput(void)
{
  return &(this->m_stInput);
}

DWORD conScreen::getEvents(void) const
{
  return this->m_uiEvents;
}

WORD conScreen::getEventType(void)
{
  return (getInput()->EventType);
}

// https://docs.microsoft.com/en-us/windows/console/key-event-record-str
KEY_EVENT_RECORD* conScreen::getEventKey(void)
{
  return &(getInput()->Event.KeyEvent);
}

// https://docs.microsoft.com/en-us/windows/console/mouse-event-record-str
MOUSE_EVENT_RECORD* conScreen::getEventMouse(void)
{
  return &(getInput()->Event.MouseEvent);
}

// https://docs.microsoft.com/en-us/windows/console/mouse-event-record-str
WINDOW_BUFFER_SIZE_RECORD* conScreen::getEventBuffer(void)
{
  return &(getInput()->Event.WindowBufferSizeEvent);
}

// https://docs.microsoft.com/en-us/windows/console/key-event-record-str
MENU_EVENT_RECORD* conScreen::getEventMenu(void)
{
  return &(getInput()->Event.MenuEvent);
}

// https://docs.microsoft.com/en-us/windows/console/mouse-event-record-str
FOCUS_EVENT_RECORD* conScreen::getEventFocus(void)
{
  return &(getInput()->Event.FocusEvent);
}

//*********************************INFO AND MODE*************************************

conScreen& conScreen::setCursorInfo(CONSOLE_CURSOR_INFO *cci)
{
  SetConsoleCursorInfo(this->m_hConOut, cci); return *this;
}

conScreen& conScreen::setCursorMode(DWORD flag)
{
  SetConsoleMode(this->m_hConIn, flag); return *this;
}

//****************************DRAW PIXELS AS CHARS****************************************

conScreen& conScreen::drawPixel(SHORT x, SHORT y, BYTE chr, WORD clr)
{
  setColor(clr); setPos(x, y);
  std::cout << (BYTE)(chr); return *this;
}

conScreen& conScreen::drawPixel(SHORT x, SHORT y, BYTE chr)
{
  return drawPixel(x, y, chr, this->m_curCol);
}

conScreen& conScreen::drawPixel(SHORT x, SHORT y)
{
  return drawPixel(x,y,this->m_curChar,this->m_curCol);
}

conScreen& conScreen::drawPixelAnex(SHORT x, SHORT y, BYTE lev, WORD clr)
{
  lev &= 0x03; if(lev == 0){ lev = 0x20; }else{ lev +=175; }
  return drawPixel(x, y, lev, clr);
}

conScreen& conScreen::drawPixelAnex(SHORT x, SHORT y, BYTE lev)
{
  lev &= 0x03; if(lev == 0){ lev = 0x20; }else{ lev +=175; }
  return drawPixel(x, y, lev);
}

//*********************************OVAL/ELIPSE************************************

conScreen& conScreen::drawOval(SHORT x, SHORT y, SHORT sx, SHORT sy, BYTE chr, WORD clr)
{
  int wx, wy, th, xa, ya;
  int asq = sx * sx;
  int bsq = sy * sy;

  drawPixel(x, y+sy, chr, clr);
  drawPixel(x, y-sy, chr, clr);
  wx = 0; wy = sy;
  xa = 0; ya = asq * 2 * sy;
  th = asq / 4 - asq * sy;
  for (;;) {
    th += xa + bsq;
    if (th >= 0)
    {
      ya -= asq * 2;
      th -= ya;
      wy--;
    }
    xa += bsq * 2; wx++;
    if (xa >= ya) break;
    drawPixel(x+wx, y-wy, chr, clr);
    drawPixel(x-wx, y-wy, chr, clr);
    drawPixel(x+wx, y+wy, chr, clr);
    drawPixel(x-wx, y+wy, chr, clr);
  }
  drawPixel(x+sx, y, chr, clr);
  drawPixel(x-sx, y, chr, clr);
  wx = sx; wy = 0;
  xa = bsq * 2 * sx;
  ya = 0; th = bsq / 4 - bsq * sx;
  for (;;) {
    th += ya + asq;
    if (th >= 0) {
        xa -= bsq * 2;
        th -= xa;
        wx--;
    }
    ya += asq * 2; wy++;
    if (ya > xa) break;
    drawPixel(x+wx, y-wy, chr, clr);
    drawPixel(x-wx, y-wy, chr, clr);
    drawPixel(x+wx, y+wy, chr, clr);
    drawPixel(x-wx, y+wy, chr, clr);
  }; return *this;
}

conScreen& conScreen::drawOval(SHORT x, SHORT y, SHORT sx, SHORT sy, BYTE chr)
{
  return drawOval(x,y,sx,sy,chr,this->m_curCol);
}

conScreen& conScreen::drawOval(SHORT x, SHORT y, SHORT sx, SHORT sy)
{
  return drawOval(x,y,sx,sy,this->m_curChar,this->m_curCol);
}

//*****************************************************************************

conScreen& conScreen::drawLine(SHORT x0, SHORT y0, SHORT x1, SHORT y1, BYTE chr, WORD clr)
{
  int dy = y1 - y0;
  int dx = x1 - x0;
  int sx, sy;

  if (dy < 0) { dy = -dy;  sy = -1; } else { sy = 1; }
  if (dx < 0) { dx = -dx;  sx = -1; } else { sx = 1; }
  dy <<= 1; dx <<= 1; // Multiply with 2

  drawPixel(x0, y0, chr, clr);
  if(dx > dy)
  {
    int frac = dy - (dx >> 1);                         // same as 2*dy - dx
    while (x0 != x1)
    {
      if (frac >= 0)
      {
        y0 += sy;
        frac -= dx;                                // same as fraction -= 2*dx
      }
      x0 += sx;
      frac += dy;                                    // same as fraction -= 2*dy
      drawPixel(x0, y0, chr, clr);
    }
  }
  else
  {
    int frac = dx - (dy >> 1);
    while(y0 != y1)
    {
      if(frac >= 0)
      {
        x0 += sx;
        frac -= dy;
      }
      y0 += sy;
      frac += dx;
      drawPixel(x0, y0, chr, clr);
    }
  }; return *this;
}

conScreen& conScreen::drawLine(SHORT x0, SHORT y0, SHORT x1, SHORT y1, BYTE chr)
{
  return drawLine(x0,y0,x1,y1,chr,this->m_curCol);
}

conScreen& conScreen::drawLine(SHORT x0, SHORT y0, SHORT x1, SHORT y1)
{
  return drawLine(x0,y0,x1,y1,this->m_curChar,this->m_curCol);
}


//*****************************************************************************

conScreen& conScreen::setFrameID(DWORD fid)
{
  this->m_iFrameID = fid; return *this;
}

DWORD conScreen::getFrameID(void) const
{
  return this->m_iFrameID;
}

//*****************************************************************************

conScreen& conScreen::drawFrame(SHORT x, SHORT y, SHORT sx, SHORT sy, BYTE chr, WORD col, BYTE* tit, CHAR cen)
{
  BYTE *arr = puFrame[this->m_iFrameID];
  drawPixel(x     ,y     ,chr > 0 ? chr : arr[0],col); // Top left corner of drawframe
  drawPixel(x+sx-1,y     ,chr > 0 ? chr : arr[1],col); // Top right corner of drawframe
  drawPixel(x     ,y+sy-1,chr > 0 ? chr : arr[2],col); // Bottom left corner of drawframe
  drawPixel(x+sx-1,y+sy-1,chr > 0 ? chr : arr[3],col); // Bottom right corner of drawframe
  for(int ix = x+1; ix < x+sx-1; ix++)
  {
    drawPixel(ix,     y,chr > 0 ? chr : arr[4],col);   // Top horizontal line
    drawPixel(ix,y+sy-1,chr > 0 ? chr : arr[5],col);   // Bottom Horizontal line
  }
  for(int iy = y+1; iy < y+sy-1; iy++)
  {
    drawPixel(x     ,iy,chr > 0 ? chr : arr[6],col);   // Left Vertical line
    drawPixel(x+sx-1,iy,chr > 0 ? chr : arr[7],col);   // Right Vertical Line
  }
  if(tit != NULL)
  {
    WORD len = 0; while(tit[len] && len < CON_STRING_LEN){ len++; }
    if(cen < 0){ setPos(x,y); }                // Left   = -
    else if(cen > 0){ setPos(x+(sx-len),y); }  // Right  = +
    else{ setPos(x+(sx-len)/2,y); }            // Center = 0
    std::cout << tit;                          //put Title
  }
  return *this;
}

conScreen& conScreen::drawFrame(SHORT x, SHORT y, SHORT sx, SHORT sy, BYTE chr, WORD col, BYTE* tit)
{
  return drawFrame(x, y, sx, sy, chr, col, tit, 0);
}

conScreen& conScreen::drawFrame(SHORT x, SHORT y, SHORT sx, SHORT sy, BYTE chr, WORD col)
{
  return drawFrame(x, y, sx, sy, chr, col, NULL, 0);
}

conScreen& conScreen::drawFrame(SHORT x, SHORT y, SHORT sx, SHORT sy, BYTE chr)
{
  return drawFrame(x, y, sx, sy, chr, this->m_curCol, NULL, 0);
}

conScreen& conScreen::drawFrame(SHORT x, SHORT y, SHORT sx, SHORT sy)
{
  return drawFrame(x, y, sx, sy, this->m_curChar, this->m_curCol, NULL, 0);
}

 //*****************************RECTANGLE***********************************

conScreen& conScreen::drawRect(SHORT x, SHORT y, SHORT sx, SHORT sy, BYTE pat, WORD col)
{
  setColor(col);               //Apply the color provided
  for(int iy=y;iy<y+sy;iy++)   //Fill Y Region Loop
  {
    for(int ix=x;ix<x+sx;ix++) //Fill X region Loop
    {
      drawPixel(ix,iy,pat);
    }
  }; return *this;
}

conScreen& conScreen::drawRect(SHORT x, SHORT y, SHORT sx, SHORT sy, BYTE pat)
{
  return drawRect(x, y, sx, sy, pat, this->m_curCol);
}

conScreen& conScreen::drawRect(SHORT x, SHORT y, SHORT sx, SHORT sy)
{
  return drawRect(x, y, sx, sy, this->m_curChar, this->m_curCol);
}

//**********************************WINDOW********************************

conScreen& conScreen::drawWindow(SHORT x, SHORT y, SHORT sx, SHORT sy, BYTE chef, BYTE cher, WORD colf, WORD colr, BYTE* tit, CHAR cen)
{
  drawFrame(x,y,sx,sy,chef,colf,tit,cen);
  drawRect(x+1,y+1,sx-2,sy-2,cher,colf);
  return *this;
}

conScreen& conScreen::drawWindow(SHORT x, SHORT y, SHORT sx, SHORT sy, BYTE cher, WORD colr, BYTE* tit)
{
  drawFrame(x,y,sx,sy,this->m_curChar,this->m_curCol,tit);
  drawRect(x+1,y+1,sx-2,sy-2,cher,colr);
  return *this;
}

//*******************************PALETTE****************************************

conScreen& conScreen::drawPalette(SHORT x, SHORT y, const void* txt)
{
  WORD len = ((BYTE*)memchr(txt, 0, CON_STRING_LEN) - (BYTE*)txt);
  for(int i=0;i<16;i++)
  {
    for(int j=0;j<16;j++)
    {
      insColor(i, j);
      drawText((i*len)+x, j+y, txt);
    }
  }; return *this;
}

conScreen& conScreen::drawPalette(SHORT x, SHORT y)
{
  return drawPalette(x,y,"#");
}

conScreen& conScreen::drawPalette(void)
{
  return drawPalette(0,0,"#");
}
