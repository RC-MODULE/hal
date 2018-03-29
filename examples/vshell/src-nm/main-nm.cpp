/* 
 * h264bitstream - a library for reading and writing H.264 video
 * Copyright (C) 2005-2007 Auroras Entertainment, LLC
 * 
 * Written by Alex Izvorski <aizvorski@gmail.com>
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */


#include <stdlib.h>
#include <stdio.h>
#include "hal_target.h"
#include "hal.h"
#include "string.h"


int main(int argc, char *argv[])
{
	
	unsigned sync;

	sync = halHostSync(0x6406);			// Handshake 
	int imWidth = halHostSync(123);
	int imHeight= halHostSync(124);
	int imSize = imWidth*imHeight;
	int* src = (int*)halMalloc32(imSize / 4);
	int* dst = (int*)halMalloc32(imSize / 4);
	
	sync = halHostSync((int)src);			// Handshake 
	sync = halHostSync((int)dst);			// Handshake 
	
	if (src == 0 || dst == 0)
		halLedSOS(3);
		//return -1;
	

	int counter = 0;
	while (1) {
		sync = halHostSync(111);
		//nmppsSubC_8s(src, 128, dst, imSize);
		int *s = (int*)src;
		int *d = (int*)dst;
		
		for (int i=0; i < imSize / 4; i++) {
			*d = ~(*s);
			d++;
			s++;
		}
		//memcpy(dst, src, imSize / 4);
		sync = halHostSync(counter++);
	}
	halFree(src);
	halFree(dst);
    return 0;
}
