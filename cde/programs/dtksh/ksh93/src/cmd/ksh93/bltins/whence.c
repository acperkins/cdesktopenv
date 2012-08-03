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
 * License along with these librararies and programs; if not, write
 * to the Free Software Foundation, Inc., 51 Franklin Street, Fifth
 * Floor, Boston, MA 02110-1301 USA
 */
/* $XConsortium: whence.c /main/3 1995/11/01 16:31:17 rswiston $ */
/***************************************************************
*                                                              *
*                      AT&T - PROPRIETARY                      *
*                                                              *
*        THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF        *
*                    AT&T BELL LABORATORIES                    *
*         AND IS NOT TO BE DISCLOSED OR USED EXCEPT IN         *
*            ACCORDANCE WITH APPLICABLE AGREEMENTS             *
*                                                              *
*                Copyright (c) 1995 AT&T Corp.                 *
*              Unpublished & Not for Publication               *
*                     All Rights Reserved                      *
*                                                              *
*       The copyright notice above does not evidence any       *
*      actual or intended publication of such source code      *
*                                                              *
*               This software was created by the               *
*           Advanced Software Technology Department            *
*                    AT&T Bell Laboratories                    *
*                                                              *
*               For further information contact                *
*                    {research,attmail}!dgk                    *
*                                                              *
***************************************************************/

/* : : generated by proto : : */

#if !defined(__PROTO__)
#if defined(__STDC__) || defined(__cplusplus) || defined(_proto) || defined(c_plusplus)
#if defined(__cplusplus)
#define __MANGLE__	"C"
#else
#define __MANGLE__
#endif
#define __STDARG__
#define __PROTO__(x)	x
#define __OTORP__(x)
#define __PARAM__(n,o)	n
#if !defined(__STDC__) && !defined(__cplusplus)
#if !defined(c_plusplus)
#define const
#endif
#define signed
#define void		int
#define volatile
#define __V_		char
#else
#define __V_		void
#endif
#else
#define __PROTO__(x)	()
#define __OTORP__(x)	x
#define __PARAM__(n,o)	o
#define __MANGLE__
#define __V_		char
#define const
#define signed
#define void		int
#define volatile
#endif
#if defined(__cplusplus) || defined(c_plusplus)
#define __VARARG__	...
#else
#define __VARARG__
#endif
#if defined(__STDARG__)
#define __VA_START__(p,a)	va_start(p,a)
#else
#define __VA_START__(p,a)	va_start(p)
#endif
#endif
#include	"defs.h"
#include	<error.h>
#include	"shtable.h"
#include	"name.h"
#include	"path.h"
#include	"shlex.h"
#include	"builtins.h"

#define P_FLAG	1
#define V_FLAG	2
#define A_FLAG	4
#define F_FLAG	010
#define X_FLAG	020

static int whence __PROTO__((char**, int));

/*
 * command is called with argc==0 when checking for -V or -v option
 * In this case return 0 when -v or -V or unknown option, otherwise
 *   the shift count to the command is returned
 */
int	b_command __PARAM__((register int argc,char *argv[],__V_ *extra), (argc, argv, extra)) __OTORP__(register int argc;char *argv[];__V_ *extra;){
	register int n, flags=0;
	NOT_USED(extra);
	opt_index = opt_char = 0;
	while((n = optget(argv,sh_optcommand))) switch(n)
	{
	    case 'p':
		sh_onstate(SH_DEFPATH);
		break;
	    case 'v':
		flags |= X_FLAG;
		break;
	    case 'V':
		flags |= V_FLAG;
		break;
	    case ':':
		if(argc==0)
			return(0);
		error(2, opt_arg);
		break;
	    case '?':
		if(argc==0)
			return(0);
		error(ERROR_usage(2), opt_arg);
		break;
	}
	if(argc==0)
		return(flags?0:opt_index);
	argv += opt_index;
	if(error_info.errors || !*argv)
		error(ERROR_usage(2),optusage((char*)0));
	return(whence(argv, flags));
}

/*
 *  for the whence command
 */
int	b_whence __PARAM__((int argc,char *argv[],__V_ *extra), (argc, argv, extra)) __OTORP__(int argc;char *argv[];__V_ *extra;){
	register int flags=0, n;
	NOT_USED(argc);
	NOT_USED(extra);
	if(*argv[0]=='t')
		flags = V_FLAG;
	while((n = optget(argv,sh_optwhence))) switch(n)
	{
	    case 'a':
		flags |= A_FLAG;
		/* FALL THRU */
	    case 'v':
		flags |= V_FLAG;
		break;
	    case 'f':
		flags |= F_FLAG;
		break;
	    case 'p':
		flags |= P_FLAG;
		break;
	    case ':':
		error(2, opt_arg);
		break;
	    case '?':
		error(ERROR_usage(2), opt_arg);
		break;
	}
	argv += opt_index;
	if(error_info.errors || !*argv)
		error(ERROR_usage(2),optusage((char*)0));
	return(whence(argv, flags));
}

static int whence __PARAM__((char **argv, register int flags), (argv, flags)) __OTORP__(char **argv; register int flags;){
	register const char *name;
	register Namval_t *np;
	register const char *cp;
	register aflag,r=0;
	register const char *msg;
	int notrack = 1;
	while(name= *argv++)
	{
		aflag = ((flags&A_FLAG)!=0);
		cp = 0;
		np = 0;
		if(flags&P_FLAG)
			goto search;
		/* reserved words first */
		if(sh_lookup(name,shtab_reserved))
		{
			sfprintf(sfstdout,"%s%s\n",name,(flags&V_FLAG)?ERROR_translate(is_reserved,1):"");
			if(!aflag)
				continue;
			aflag++;
		}
		/* non-tracked aliases */
		if((np=nv_search(name,sh.alias_tree,0))
			&& !nv_isnull(np) && !(notrack=nv_isattr(np,NV_TAGGED))
			&& (cp=nv_getval(np))) 
		{
			if(flags&V_FLAG)
			{
				if(nv_isattr(np,NV_EXPORT))
					msg = is_xalias;
				else
					msg = is_alias;
				sfprintf(sfstdout,ERROR_translate(msg,1),name);
			}
			sfputr(sfstdout,sh_fmtq(cp),'\n');
			if(!aflag)
				continue;
			cp = 0;
			aflag++;
		}
		/* built-ins and functions next */
		if((np=nv_search(name,sh.fun_tree,0)) && (!nv_isnull(np) || nv_isattr(np,NV_FUNCTION)))
		{
			if(is_abuiltin(np) && (cp=np->nvenv))
				goto search;
			if((flags&F_FLAG) && nv_isattr(np,NV_FUNCTION))
				if(!(np=nv_search(name,sh.bltin_tree,0)) || nv_isnull(np))
					goto search;
			cp = "";
			if(flags&V_FLAG)
			{
				if(nv_isnull(np))
				{
					if(!nv_isattr(np,NV_FUNCTION))
						goto search;
					cp = is_ufunction;
				}
				else if(is_abuiltin(np))
					cp = is_builtin;
				else if(nv_isattr(np,NV_EXPORT))
					cp = is_xfunction;
				else
					cp = is_function;
			}
			sfprintf(sfstdout,"%s%s\n",name,ERROR_translate(cp,1));
			if(!aflag)
				continue;
			cp = 0;
			aflag++;
		}
	search:
		if(sh_isstate(SH_DEFPATH))
		{
			cp=0;
			notrack=1;
		}
		if(path_search(name,cp,2))
			cp = name;
		else
			cp = sh.lastpath;
		sh.lastpath = 0;
		if(cp)
		{
			if(flags&V_FLAG)
			{
				if(*cp!= '/')
				{
					if(!np || nv_isnull(np))
						sfprintf(sfstdout,"%s%s\n",name,ERROR_translate(is_ufunction,1));
					continue;
				}
				sfputr(sfstdout,sh_fmtq(name),' ');
				/* built-in version of program */
				if(np && is_abuiltin(np) && np->nvenv && strcmp(np->nvenv,cp)==0)
					msg = is_builtver;
				/* tracked aliases next */
				else if(!notrack && *name == '/')
					msg = "is";
				else
					msg = is_talias;
				sfputr(sfstdout,ERROR_translate(msg,1),' ');
			}
			sfputr(sfstdout,sh_fmtq(cp),'\n');
		}
		else if(aflag<=1) 
		{
			r |= 1;
			if(flags&V_FLAG)
			{
				sfprintf(sfstdout,ERROR_translate(e_found,1),sh_fmtq(name));
				sfputc(sfstdout,'\n');
			}
		}
	}
	return(r);
}
