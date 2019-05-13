/* Copyright (C) 
 * 2018 - quintin
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 * 
 */
/**
 * @file log.h
 * @brief 
 * @author quintin
 * @version 1.0
 * @date 2018-08-04
 */
#ifndef __LOG_H__
#define __LOG_H__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdarg.h>
#include <unistd.h>

#define MAXLEN (2048)
#define MAXFILEPATH (512)
#define MAXFILENAME (50)

typedef enum{
    ERROR_1=-1,
    ERROR_2=-2,
    ERROR_3=-3

}ERROR0;


typedef enum{
    NONE=0,
    INFO=1,
    DEBUG=2,
    WARN=3,
    ERROR=4,
    ALL=255

}LOGLEVEL;

typedef struct log{
    char logtime[20];
    char filepath[MAXFILEPATH];
    FILE *logfile;

}LOG;

typedef struct logseting{
    char filepath[MAXFILEPATH];
    unsigned int maxfilelen;
    unsigned char loglevel;

}LOGSET;

int LogWrite(unsigned char loglevel,const char *fromat,...);

#endif

