/*
 * CDE - Common Desktop Environment
 *
 * Copyright (c) 1993-2012, The Open Group. All rights reserved.
 *
 * These libraries and programs are free software; you can
 * redistribute them and/or modify them under the terms of the GNU
 * Lesser General Public License as published by the Free Software
 * Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 * These libraries and programs are distributed in the hope that
 * they will be useful, but WITHOUT ANY WARRANTY; without even the
 * implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE. See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with these libraries and programs; if not, write
 * to the Free Software Foundation, Inc., 51 Franklin Street, Fifth
 * Floor, Boston, MA 02110-1301 USA
 */
/* $XConsortium: basic.h /main/3 1995/11/08 09:46:43 rswiston $ */
/* Copyright (c) 1986, 1987, 1988, 1989 Hewlett-Packard Co. */
/* Basic.h has a few very primitive #define's used by all MARKUP programs */
#include <stdlib.h>

/* hpux protects stdio.h from multiple includes thru stdlib.h . */
#include <stdio.h>

#define TRUE 1
#define FALSE 0
typedef unsigned char LOGICAL;

/* End of String marker */
#define M_EOS '\0'

/* Integer null value marker */
#define M_NULLVAL 0

/* 8-bit data characters */
typedef unsigned char M_CHAR8;

/* wide characters */
typedef wchar_t M_WCHAR;

/* For function prototypes */
typedef char * M_POINTER;

#define M_NOPAR void

char *MakeMByteString(const M_WCHAR *from);

M_WCHAR *MakeWideCharString(const char *from);

int w_strcmp(const M_WCHAR *string1, const M_WCHAR *string2);

M_WCHAR *w_strcpy(M_WCHAR *string1, const M_WCHAR *string2);

M_WCHAR *w_strncpy(M_WCHAR *string1, const M_WCHAR *string2, int max);

int w_strlen(const M_WCHAR *string);

M_WCHAR *w_strchr(M_WCHAR *string, const M_WCHAR chr);

M_WCHAR *w_strstr(M_WCHAR *string1, M_WCHAR *string2);

void *m_malloc(int size, char *msg);

void m_free(void *block, char *msg);

void m_err2(const char *text, const M_WCHAR *arg1, const M_WCHAR *arg2);

int mb_getwc(void *m_ptr);
