/*******************************************************************************

Copyright (c) 2000 Microsoft Corporation.  All rights reserved.

File Name:

    brikmip7.bmp.cpp

Description

    Data file for the resource brikmip7.bmp.

*******************************************************************************/

#include "d3dlocus.h"

static BYTE g_brikmip7[] = {

   0x42, 0x4D, 0x58, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x4E, 0x00, 0x00, 0x00, 0x28, 0x00, 
   0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x01, 0x00, 0x08, 0x00, 0x00, 0x00, 
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC3, 0x0E, 0x00, 0x00, 0xC3, 0x0E, 0x00, 0x00, 0x06, 0x00, 
   0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x4A, 0x8C, 0xAD, 0x00, 0x31, 0xA5, 
   0xBD, 0x00, 0x31, 0xB5, 0xC6, 0x00, 0x21, 0xCE, 0xDE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x01, 
   0x00, 0x00, 0x03, 0x04, 0x00, 0x00, 0x00, 0x00, 
};

static RESOURCEDATA g_rd_brikmip7(TEXT(MODULE_STRING), TEXT("brikmip7.bmp"), g_brikmip7, sizeof(g_brikmip7));

extern "C" LPVOID PREPEND_MODULE(_brikmip7) = (LPVOID)&g_rd_brikmip7;