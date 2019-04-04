#ifndef CONSCREEN_H
#define CONSCREEN_H

// https://docs.microsoft.com/en-us/windows/desktop/winprog/windows-data-types
//     Colors for the console screen:
//     0 = Black
//     1 = Blue
//     2 = Green
//     3 = Cyan
//     4 = Red
//     5 = Magenta
//     6 = Yellow
//     7 = LightGray
//     8 = DarkGray
//     9 = LightBlue
//     10 = LightGreen
//     11 = LightCyan
//     12 = LightRed
//     13 = LightMagenta
//     14 = LightYellow
//     15 = White

typedef class conScreen
{
  public:
    HANDLE getHandleIn(void) const;
    HANDLE getHandleOut(void) const;
    INPUT_RECORD* getInput(void);
    DWORD getEvents(void) const;
    DWORD getFrameID(void) const;
    COORD* getPos(void);
    COORD* getSize(void);
    WORD getEventType(void);
    conScreen& setPos(SHORT x, SHORT y);
    conScreen& setSize(SHORT x, SHORT y);
    conScreen& Clear(void);
    conScreen& readInput(void);
    KEY_EVENT_RECORD* getEventKey(void);
    MOUSE_EVENT_RECORD* getEventMouse(void);
    MENU_EVENT_RECORD* getEventMenu(void);
    FOCUS_EVENT_RECORD* getEventFocus(void);
    WINDOW_BUFFER_SIZE_RECORD* getEventBuffer(void);
    conScreen& setColor(WORD clr);
    conScreen& setColor(WORD fg, WORD bg);
    conScreen& insByte(WORD chr);
    conScreen& insColor(WORD clr);
    conScreen& insColor(WORD fg, WORD bg);
    conScreen& setCursorInfo(CONSOLE_CURSOR_INFO *cci);
    conScreen& setCursorMode(DWORD flag);
    conScreen& setFrameID(DWORD fid);
    conScreen& drawPalette(void);
    conScreen& drawPalette(SHORT x, SHORT y);
    conScreen& drawPalette(SHORT x, SHORT y, const void* txt);
    conScreen& drawText(const void* txt);
    conScreen& drawText(const void* txt, WORD clr);
    conScreen& drawText(SHORT x, SHORT y, const void* txt);
    conScreen& drawText(SHORT x, SHORT y, const void* txt, WORD clr);
    conScreen& drawPixel(SHORT x, SHORT y);
    conScreen& drawPixel(SHORT x, SHORT y, BYTE chr);
    conScreen& drawPixel(SHORT x, SHORT y, BYTE chr, WORD clr);
    conScreen& drawPixelAnex(SHORT x, SHORT y, BYTE lev);
    conScreen& drawPixelAnex(SHORT x, SHORT y, BYTE lev, WORD clr);
    conScreen& drawOval(SHORT x, SHORT y, SHORT sx, SHORT sy);
    conScreen& drawOval(SHORT x, SHORT y, SHORT sx, SHORT sy, BYTE chr);
    conScreen& drawOval(SHORT x, SHORT y, SHORT sx, SHORT sy, BYTE chr, WORD clr);
    conScreen& drawLine(SHORT x0, SHORT y0, SHORT x1, SHORT y1);
    conScreen& drawLine(SHORT x0, SHORT y0, SHORT x1, SHORT y1, BYTE chr);
    conScreen& drawLine(SHORT x0, SHORT y0, SHORT x1, SHORT y1, BYTE chr, WORD clr);
    conScreen& drawRect(SHORT x, SHORT y, SHORT sx, SHORT sy);
    conScreen& drawRect(SHORT x, SHORT y, SHORT sx, SHORT sy, BYTE pat);
    conScreen& drawRect(SHORT x, SHORT y, SHORT sx, SHORT sy, BYTE pat, WORD col);
    conScreen& drawFrame(SHORT x, SHORT y, SHORT sx, SHORT sy);
    conScreen& drawFrame(SHORT x, SHORT y, SHORT sx, SHORT sy, BYTE chr);
    conScreen& drawFrame(SHORT x, SHORT y, SHORT sx, SHORT sy, BYTE chr, WORD col);
    conScreen& drawFrame(SHORT x, SHORT y, SHORT sx, SHORT sy, BYTE chr, WORD col, BYTE* tit);
    conScreen& drawFrame(SHORT x, SHORT y, SHORT sx, SHORT sy, BYTE chr, WORD col, BYTE* tit, CHAR cen);
    conScreen& drawWindow(SHORT x, SHORT y, SHORT sx, SHORT sy, BYTE pat, WORD col, BYTE* tit);
    conScreen& drawWindow(SHORT x, SHORT y, SHORT sx, SHORT sy, BYTE pout, BYTE pin, WORD coli, WORD colo, BYTE* tit, CHAR cen);
    conScreen();
    virtual ~conScreen();
  protected:
    BYTE m_iFrameID;
    BYTE m_cChar;
    HANDLE m_hConOut;
    HANDLE m_hConIn;
    INPUT_RECORD m_stInput;
    DWORD m_uiEvents;
    COORD m_xyDpos;
    COORD m_xyDsiz;
    WORD m_curCol;
  private:
} cConScreen;

#endif // CONSCREEN_H
