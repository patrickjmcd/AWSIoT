/***************************************************************************
 *   Copyright (C) 2016 by OmanTek                                         *
 *   Author Kyle Hayes  kylehayes@omantek.com                              *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Library General Public License as       *
 *   published by the Free Software Foundation; either version 2 of the    *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU Library General Public     *
 *   License along with this program; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef __EXAMPLE_UTILS_H__
#define __EXAMPLE_UTILS_H__

#ifdef _WIN32
    #include <windows.h>
    #define strcasecmp _stricmp
    #define strdup _strdup
	#define snprintf_platform sprintf_s
	#define sscanf_platform sscanf_s
#else
    #include <unistd.h>
    #include <strings.h>
	#define snprintf_platform snprintf
	#define sscanf_platform sscanf
#endif


#include <stdint.h>
#include <iostream>
#include <sstream>
#include <vector>
using namespace std;

// Define different data types
#define CIP_DATA_TYPE_BOOL      1
#define CIP_DATA_TYPE_SINT      2
#define CIP_DATA_TYPE_USINT     3
#define CIP_DATA_TYPE_INT       4
#define CIP_DATA_TYPE_UINT      5
#define CIP_DATA_TYPE_DINT      6
#define CIP_DATA_TYPE_UDINT     7
#define CIP_DATA_TYPE_REAL      8
#define CIP_DATA_TYPE_STRING    9

#define DATA_TIMEOUT            5000

typedef vector<float> float_vec_t;
typedef vector<int> int_vec_t;


static string intToStr (int vnumber)
{
    ostringstream convert;
    convert << vnumber;
    return convert.str();
};

static int getTypeSize(int type)
{
    int result;
    switch (type)
    {
        case 1: // for BOOL
            result = 1;
            break;
        case 2: // for SINT
            result = 1;
            break;
        case 3: // for USINT
            result = 1;
            break;
        case 4: // for INT
            result = 2;
            break;
        case 5: // for UINT
            result = 2;
            break;
        case 6: // for DINT
            result = 4;
            break;
        case 7: // for UDINT
            result = 4;
            break;
        case 8: // for REAL
            result = 4;
            break;
        case 9: // for string
            result = 88;
            break;
        default:
            result = 0;
    }; // switch end
    return result;
};

#ifdef __cplusplus
extern "C" {
#endif

extern int sleep_ms(int ms);
extern int64_t time_ms(void);


#ifdef __cplusplus
}
#endif

#endif

