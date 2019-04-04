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

  scr.setFrameID(1).insColor(3,11).insByte(0).drawFrame(1,1,78,23);

  while(1);
}

