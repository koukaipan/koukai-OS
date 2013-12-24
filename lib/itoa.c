

/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111, USA.
 */

/* Notes: No warranty expressed or implied. Use at own risk. */

#include "lib/itoa.h"

char *itoa (int val, int base)
{
	int i ;
	static char _l_itoa_buf[32] = {0};
	if(val == 0){
		_l_itoa_buf[30] = '0';
		return &_l_itoa_buf[30];
	}
	
	for(i = 30; i ; --i, val /= base){
		_l_itoa_buf[i] = "0123456789abcdef"[val % base];
		if(val == 0)	break;
	}
	
	return &_l_itoa_buf[i+1];
}
