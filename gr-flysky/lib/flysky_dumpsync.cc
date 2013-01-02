/* -*- c++ -*- */
/* 
 * Copyright 2012 <+YOU OR YOUR COMPANY+>.
 * 
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 * 
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gr_io_signature.h>
#include <flysky_dumpsync.h>
#include <iostream>
#include <stdio.h>

#define PKSIZEBI  184
#define PKSIZEBY  PKSIZEBI/8


flysky_dumpsync_sptr
flysky_make_dumpsync ()
{
	return flysky_dumpsync_sptr (new flysky_dumpsync ());
}


flysky_dumpsync::flysky_dumpsync ()
	: gr_sync_block ("dumpsync",
		gr_make_io_signature (1, 1, sizeof(char)),
		gr_make_io_signature (0, 0, 0))
{
    set_history(PKSIZEBI);
}


flysky_dumpsync::~flysky_dumpsync ()
{
}


int
flysky_dumpsync::work (int noutput_items,
			gr_vector_const_void_star &input_items,
			gr_vector_void_star &output_items)
{
	char *in = (char*) input_items[0];
    unsigned char packet[PKSIZEBY];

    if (in[0] & (char)0x02){
        

        for(int x=0;x<PKSIZEBY;x++){
            for(int y=0;y<8;y++){
               packet[x] = packet[x]<<1 | (in[x*8+y]);
            }
        }
        //printf("\nFrame=");
        printf("\n");
        printf("%02x %02x%02x%02x%02x",packet[0], packet[1], packet[2], packet[3], packet[4]);
        for(int x=5; x<PKSIZEBY; x+=2){
            printf(" %02x%02x",packet[x], packet[x+1]);
        }

        //for(int x=0; x<PKSIZEBY; x+=2){
        //    printf(" %02x%02x",packet[x], packet[x+1]);
        //}
        return PKSIZEBI;
    } 

	return 1;
}





