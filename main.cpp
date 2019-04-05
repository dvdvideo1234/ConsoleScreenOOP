#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <windows.h>

#include "include/cConScreen.h"

int main()
{
  cConScreen scr;

  scr.setPos(0,0).setSize(80, 25).drawPixel(0,0,' ');

  scr.insByte(176).insColor(4,9).drawLine(5,5,75,20);

  scr.insColor(12,2).drawOval(20,12,16,8,'A');

  scr.setFrameID(4).insColor(3,11).insByte(' ').drawFrame(1,1,78,23);

  scr.insColor(14,2).drawText(2,2,"Some test here");

  scr.insColor(2,6).drawRect(45,3,9,5,176);

  scr.clrScreen(' ', 0);

  while(1);
}

