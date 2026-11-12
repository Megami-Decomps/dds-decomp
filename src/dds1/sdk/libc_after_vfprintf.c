#include "dds1/sdk/libc_internal.h"



/* Declaration-complete historical scanner callees. */

extern size_t fread(void *ptr, size_t size, size_t count, FILE *fp);
extern unsigned long func_00309D00(const char *s, char **end, int base);
extern double func_00305D28(const char *s);
extern long func_00305D40(const char *s);
extern int func_00309E18(int c, FILE *fp);
extern char *func_00305A08(char *tab, char *fmt);

extern char *_setlocale_r(_reent *r, s32 category, const char *locale);

extern void *_localeconv_r(_reent *r);

extern double func_00308B68(_reent *r, const char *s, char **end);

extern double func_00309A80(const char *s, char **end);

extern unsigned long func_00309AD0(_reent *r, const char *s, char **end, s32 base);

typedef struct {
    char *decimal_point;
    char *thousands_sep;
    char *grouping;
    char *int_curr_symbol;
    char *currency_symbol;
    char *mon_decimal_point;
    char *mon_thousands_sep;
    char *mon_grouping;
    char *positive_sign;
    char *negative_sign;
    char int_frac_digits;
    char frac_digits;
    char p_cs_precedes;
    char p_sep_by_space;
    char n_cs_precedes;
    char n_sep_by_space;
    char p_sign_posn;
    char n_sign_posn;
} lconv_t;

extern lconv_t localeconvData __attribute__((section(".data")));

extern void *func_0030A058(_reent *r, void *ptr, size_t size);

extern s32 close(s32 fd);

extern _Bigint *_Balloc(_reent *r, s32 k);

extern s32 write(s32 fd, const void *buf, s32 count);

extern s32 read(s32 fd, void *buf, s32 count);

extern long lseek(s32 fd, long offset, s32 whence);

extern void *_calloc_r(_reent *r, u32 n, u32 size);

extern const double pow10Table[];

s32 __mcmp(_Bigint *a, _Bigint *b);



/* Little-endian IEEE binary64 word view used by the numeric helpers. */
typedef union libcDoubleBits_t {
    f64 value;
    u32 word[2];
    u64 bits;
} libcDoubleBits_t;

void _Bfree(_reent *r, _Bigint *v);
s32 _hi0bits(u32 x);
_Bigint *_d2b(_reent *r, f64 d, s32 *e, s32 *bitsOut);
_Bigint *_i2b(_reent *r, s32 i);
_Bigint *_multadd(_reent *r, _Bigint *b, s32 m, s32 a);
_Bigint *_multiply(_reent *r, _Bigint *a, _Bigint *b);
_Bigint *_pow5mult(_reent *r, _Bigint *b, s32 n);
_Bigint *_lshift(_reent *r, _Bigint *b, s32 shift);
_Bigint *__mdiff(_reent *r, _Bigint *a, _Bigint *b);
s32 quorem(_Bigint *dividend, _Bigint *divisor);
extern const f64 D_003B8498[];

extern const char localeEmptyStr[];
extern const char localeCName[];



/* Native indexed constants span the separately labeled word at 0x0054B928. */
extern const s32 pow5MultTable[];
_Bigint *_multiply(_reent *r, _Bigint *a, _Bigint *b);





f64 _b2d(_Bigint *b, s32 *exponent);

extern const char localeEmptyStr[8] __attribute__((section(".rodata")));

extern const char localeCName[4] __attribute__((section(".rodata")));

extern const s32 pow5MultTable[3] __attribute__((section(".rodata")));

extern const f64 pow10Table[25];

extern const f64 D_003B8498[5];

extern const f64 D_003B84C0[5];

extern const char kernlInternalErrorMsg[40];

extern const char ttyMsgPacketTooLarge[40];

extern const char ttyMsgReceiveError[24];

extern const char ttyMsgSendError[24];

extern const char ttyMsgWlenError[24];

/* Historical newlib c87be3e4d6ae90dcc48b5f26dd87645e7cabff01
 * (2000-04-17) __svfscanf evidence trial. The unsigned-long callback cast
 * of strtol is historical but is not a compatible-return-type ISO C call.
 * This target-ABI experiment requires independent semantic review.
 * This software was developed by the University of California, Berkeley.
 * Header expansions: _LONG_DOUBLE=long double, _POINTER_INT=long;
 * BUF=308+39+3, CHAR_BIT=8; compiler __WCHAR_TYPE__=signed int.
 * Floating point enabled, no multibyte mode; original local state retained.
 */
/* No user fns here. Pesch 15apr92. */

/*
 * Copyright (c) 1990 The Regents of the University of California.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms are permitted
 * provided that the above copyright notice and this paragraph are
 * duplicated in all such forms and that any documentation,
 * advertising materials, and other materials related to such
 * distribution and use acknowledge that the software was developed
 * by the University of California, Berkeley.  The name of the
 * University may not be used to endorse or promote products derived
 * from this software without specific prior written permission.
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */

#ifdef _HAVE_STDC
#else
#endif

#ifndef	NO_FLOATING_POINT
#define FLOATING_POINT
#endif

#ifdef FLOATING_POINT
#define	BUF	(308+39+3)	/* 3 = sign + decimal point + NUL */
/* An upper bound for how long a long prints in decimal.  4 / 13 approximates
   log (2).  Add one char for roundoff compensation and one for the sign.  */
#define MAX_LONG_LEN ((8 * sizeof (long)  - 1) * 4 / 13 + 2)
#else
#define	BUF	40
#endif

/*
 * Flags used during conversion.
 */

#define	LONG		0x01	/* l: long or double */
#define	LONGDBL		0x02	/* L: long double; unimplemented */
#define	SHORT		0x04	/* h: short */
#define	SUPPRESS	0x08	/* suppress assignment */
#define	POINTER		0x10	/* weird %p pointer (`fake hex') */
#define	NOSKIP		0x20	/* do not skip blanks */

/*
 * The following are used in numeric conversions only:
 * SIGNOK, NDIGITS, DPTOK, and EXPOK are for floating point;
 * SIGNOK, NDIGITS, PFXOK, and NZDIGITS are for integral.
 */

#define	SIGNOK		0x40	/* +/- is (still) legal */
#define	NDIGITS		0x80	/* no digits detected */

#define	DPTOK		0x100	/* (float) decimal point is still legal */
#define	EXPOK		0x200	/* (float) exponent (e+3, etc) still legal */

#define	PFXOK		0x100	/* 0x prefix is (still) legal */
#define	NZDIGITS	0x200	/* no zero digits detected */

/*
 * Conversion types.
 */

#define	CT_CHAR		0	/* %c conversion */
#define	CT_CCL		1	/* %[...] conversion */
#define	CT_STRING	2	/* %s conversion */
#define	CT_INT		3	/* integer, i.e., strtol or func_00309D00 */
#define	CT_FLOAT	4	/* floating, i.e., strtod */

#if 0
#define u_char unsigned char
#endif
#define u_char char
#define u_long unsigned long



/*
 * vfscanf
 */

#define BufferEmpty (fp->_r <= 0 && __srefill(fp))

int func_00304D78 (fp, fmt0, ap)
     register FILE *fp;
     char const *fmt0;
     va_list ap;
{
  register u_char *fmt = (u_char *) fmt0;
  register int c;		/* character from format, or conversion */
  register size_t width;	/* field width, or 0 */
  register char *p;		/* points into all kinds of strings */
  register int n;		/* handy integer */
  register int flags;		/* flags as defined above */
  register char *p0;		/* saves original value of p when necessary */
  int nassigned;		/* number of fields assigned */
  int nread;			/* number of characters consumed from fp */
  int base = 0;			/* base argument to strtol/func_00309D00 */
  int nbytes = 1;               /* number of bytes read from fmt string */
  __WCHAR_TYPE__ wc;                   /* wchar to use to read format string */

  u_long (*ccfn) () = 0;	/* conversion function (strtol/func_00309D00) */
  char ccltab[256];		/* character class table for %[...] */
  char buf[BUF];		/* buffer for numeric conversions */
  char *lptr;                   /* literal pointer */
  int state = 0;                /* value to keep track of multibyte state */

  short *sp;
  int *ip;
  float *flp;
  long double *ldp;
  double *dp;
  long *lp;

  /* `basefix' is used to avoid `if' tests in the integer scanner */
  static const short basefix[17] =
    {10, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};

  nassigned = 0;
  nread = 0;
  for (;;)
    {
#ifndef MB_CAPABLE
      wc = *fmt;
#else
      nbytes = _mbtowc_r (_REENT, &wc, fmt, MB_CUR_MAX, &state);
#endif
      fmt += nbytes;
      if (wc == 0)
	return nassigned;
      if (nbytes == 1 && ((_ctype_ + 1)[(unsigned)(wc)] & 8))
	{
	  for (;;)
	    {
	      if (BufferEmpty)
		return nassigned;
	      if (!((_ctype_ + 1)[(unsigned)(*fp->_p)] & 8))
		break;
	      nread++, fp->_r--, fp->_p++;
	    }
	  continue;
	}
      if (wc != '%')
	goto literal;
      width = 0;
      flags = 0;

      /*
       * switch on the format.  continue if done; break once format
       * type is derived.
       */

    again:
      c = *fmt++;

      switch (c)
	{
	case '%':
	literal:
          lptr = fmt - nbytes;
          for (n = 0; n < nbytes; ++n)
            {
	      if (BufferEmpty)
	        goto input_failure;
	      if (*fp->_p != *lptr)
	        goto match_failure;
	      fp->_r--, fp->_p++;
	      nread++;
              ++lptr;
            }
	  continue;

	case '*':
	  flags |= SUPPRESS;
	  goto again;
	case 'l':
	  flags |= LONG;
	  goto again;
	case 'L':
	  flags |= LONGDBL;
	  goto again;
	case 'h':
	  flags |= SHORT;
	  goto again;

	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
	  width = width * 10 + c - '0';
	  goto again;

	  /*
	   * Conversions. Those marked `compat' are for
	   * 4.[123]BSD compatibility.
	   *
	   * (According to ANSI, E and X formats are supposed to
	   * the same as e and x.  Sorry about that.)
	   */

	case 'D':		/* compat */
	  flags |= LONG;
	  /* FALLTHROUGH */
	case 'd':
	  c = CT_INT;
	  ccfn = (u_long (*)())strtol;
	  base = 10;
	  break;

	case 'i':
	  c = CT_INT;
	  ccfn = (u_long (*)())strtol;
	  base = 0;
	  break;

	case 'O':		/* compat */
	  flags |= LONG;
	  /* FALLTHROUGH */
	case 'o':
	  c = CT_INT;
	  ccfn = func_00309D00;
	  base = 8;
	  break;

	case 'u':
	  c = CT_INT;
	  ccfn = func_00309D00;
	  base = 10;
	  break;

	case 'X':		/* compat   XXX */
	case 'x':
	  flags |= PFXOK;	/* enable 0x prefixing */
	  c = CT_INT;
	  ccfn = func_00309D00;
	  base = 16;
	  break;

#ifdef FLOATING_POINT
	case 'E':		/* compat   XXX */
	case 'G':		/* compat   XXX */
/* ANSI says that E,G and X behave the same way as e,g,x */
	  /* FALLTHROUGH */
	case 'e':
	case 'f':
	case 'g':
	  c = CT_FLOAT;
	  break;
#endif

	case 's':
	  c = CT_STRING;
	  break;

	case '[':
	  fmt = func_00305A08 (ccltab, fmt);
	  flags |= NOSKIP;
	  c = CT_CCL;
	  break;

	case 'c':
	  flags |= NOSKIP;
	  c = CT_CHAR;
	  break;

	case 'p':		/* pointer format is like hex */
	  flags |= POINTER | PFXOK;
	  c = CT_INT;
	  ccfn = func_00309D00;
	  base = 16;
	  break;

	case 'n':
	  if (flags & SUPPRESS)	/* ??? */
	    continue;
	  if (flags & SHORT)
	    {
	      sp = __builtin_va_arg (ap, short *);
	      *sp = nread;
	    }
	  else if (flags & LONG)
	    {
	      lp = __builtin_va_arg (ap, long *);
	      *lp = nread;
	    }
	  else
	    {
	      ip = __builtin_va_arg (ap, int *);
	      *ip = nread;
	    }
	  continue;

	  /*
	   * Disgusting backwards compatibility hacks.	XXX
	   */
	case '\0':		/* compat */
	  return (-1);

	default:		/* compat */
	  if (((_ctype_ + 1)[(unsigned)(c)] & 1))
	    flags |= LONG;
	  c = CT_INT;
	  ccfn = (u_long (*)())strtol;
	  base = 10;
	  break;
	}

      /*
       * We have a conversion that requires input.
       */
      if (BufferEmpty)
	goto input_failure;

      /*
       * Consume leading white space, except for formats that
       * suppress this.
       */
      if ((flags & NOSKIP) == 0)
	{
	  while (((_ctype_ + 1)[(unsigned)(*fp->_p)] & 8))
	    {
	      nread++;
	      if (--fp->_r > 0)
		fp->_p++;
	      else
#ifndef CYGNUS_NEC
	      if (__srefill (fp))
#endif
		goto input_failure;
	    }
	  /*
	   * Note that there is at least one character in the
	   * buffer, so conversions that do not set NOSKIP ca
	   * no longer result in an input failure.
	   */
	}

      /*
       * Do the conversion.
       */
      switch (c)
	{

	case CT_CHAR:
	  /* scan arbitrary characters (sets NOSKIP) */
	  if (width == 0)
	    width = 1;
	  if (flags & SUPPRESS)
	    {
	      size_t sum = 0;

	      for (;;)
		{
		  if ((n = fp->_r) < width)
		    {
		      sum += n;
		      width -= n;
		      fp->_p += n;
#ifndef CYGNUS_NEC
		      if (__srefill (fp))
			{
#endif
			  if (sum == 0)
			    goto input_failure;
			  break;
#ifndef CYGNUS_NEC
			}
#endif
		    }
		  else
		    {
		      sum += width;
		      fp->_r -= width;
		      fp->_p += width;
		      break;
		    }
		}
	      nread += sum;
	    }
	  else
	    {
#ifdef CYGNUS_NEC
	      /* Kludge city for the moment */
	      char *dest = __builtin_va_arg (ap, char *);
	      int n = width;
	      if (fp->_r == 0)
		goto input_failure;

	      while (n && fp->_r)
		{
		  *dest++ = *(fp->_p++);
		  n--;
		  fp->_r--;
		  nread++;
		}
#else
	      size_t r = fread ((void *) __builtin_va_arg (ap, char *), 1, width, fp);

	      if (r == 0)
		goto input_failure;
	      nread += r;
#endif
	      nassigned++;
	    }
	  break;

	case CT_CCL:
	  /* scan a (nonempty) character class (sets NOSKIP) */
	  if (width == 0)
	    width = ~0;		/* `infinity' */
	  /* take only those things in the class */
	  if (flags & SUPPRESS)
	    {
	      n = 0;
	      while (ccltab[*fp->_p])
		{
		  n++, fp->_r--, fp->_p++;
		  if (--width == 0)
		    break;
		  if (BufferEmpty)
		    {
		      if (n == 0)
			goto input_failure;
		      break;
		    }
		}
	      if (n == 0)
		goto match_failure;
	    }
	  else
	    {
	      p0 = p = __builtin_va_arg (ap, char *);
	      while (ccltab[*fp->_p])
		{
		  fp->_r--;
		  *p++ = *fp->_p++;
		  if (--width == 0)
		    break;
		  if (BufferEmpty)
		    {
		      if (p == p0)
			goto input_failure;
		      break;
		    }
		}
	      n = p - p0;
	      if (n == 0)
		goto match_failure;
	      *p = 0;
	      nassigned++;
	    }
	  nread += n;
	  break;

	case CT_STRING:
	  /* like CCL, but zero-length string OK, & no NOSKIP */
	  if (width == 0)
	    width = ~0;
	  if (flags & SUPPRESS)
	    {
	      n = 0;
	      while (!((_ctype_ + 1)[(unsigned)(*fp->_p)] & 8))
		{
		  n++, fp->_r--, fp->_p++;
		  if (--width == 0)
		    break;
		  if (BufferEmpty)
		    break;
		}
	      nread += n;
	    }
	  else
	    {
	      p0 = p = __builtin_va_arg (ap, char *);
	      while (!((_ctype_ + 1)[(unsigned)(*fp->_p)] & 8))
		{
		  fp->_r--;
		  *p++ = *fp->_p++;
		  if (--width == 0)
		    break;
		  if (BufferEmpty)
		    break;
		}
	      *p = 0;
	      nread += p - p0;
	      nassigned++;
	    }
	  continue;

	case CT_INT:
	  /* scan an integer as if by strtol/func_00309D00 */
#ifdef hardway
	  if (width == 0 || width > sizeof (buf) - 1)
	    width = sizeof (buf) - 1;
#else
	  /* size_t is unsigned, hence this optimisation */
	  if (--width > sizeof (buf) - 2)
	    width = sizeof (buf) - 2;
	  width++;
#endif
	  flags |= SIGNOK | NDIGITS | NZDIGITS;
	  for (p = buf; width; width--)
	    {
	      c = *fp->_p;
	      /*
	       * Switch on the character; `goto ok' if we
	       * accept it as a part of number.
	       */
	      switch (c)
		{
		  /*
		   * The digit 0 is always legal, but is special.
		   * For %i conversions, if no digits (zero or nonzero)
		   * have been scanned (only signs), we will have base==0.
		   * In that case, we should set it to 8 and enable 0x
		   * prefixing. Also, if we have not scanned zero digits
		   * before this, do not turn off prefixing (someone else
		   * will turn it off if we have scanned any nonzero digits).
		   */
		case '0':
		  if (base == 0)
		    {
		      base = 8;
		      flags |= PFXOK;
		    }
		  if (flags & NZDIGITS)
		    flags &= ~(SIGNOK | NZDIGITS | NDIGITS);
		  else
		    flags &= ~(SIGNOK | PFXOK | NDIGITS);
		  goto ok;

		  /* 1 through 7 always legal */
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		  base = basefix[base];
		  flags &= ~(SIGNOK | PFXOK | NDIGITS);
		  goto ok;

		  /* digits 8 and 9 ok iff decimal or hex */
		case '8':
		case '9':
		  base = basefix[base];
		  if (base <= 8)
		    break;	/* not legal here */
		  flags &= ~(SIGNOK | PFXOK | NDIGITS);
		  goto ok;

		  /* letters ok iff hex */
		case 'A':
		case 'B':
		case 'C':
		case 'D':
		case 'E':
		case 'F':
		case 'a':
		case 'b':
		case 'c':
		case 'd':
		case 'e':
		case 'f':
		  /* no need to fix base here */
		  if (base <= 10)
		    break;	/* not legal here */
		  flags &= ~(SIGNOK | PFXOK | NDIGITS);
		  goto ok;

		  /* sign ok only as first character */
		case '+':
		case '-':
		  if (flags & SIGNOK)
		    {
		      flags &= ~SIGNOK;
		      goto ok;
		    }
		  break;

		  /* x ok iff flag still set & 2nd char */
		case 'x':
		case 'X':
		  if (flags & PFXOK && p == buf + 1)
		    {
		      base = 16;/* if %i */
		      flags &= ~PFXOK;
		      goto ok;
		    }
		  break;
		}

	      /*
	       * If we got here, c is not a legal character
	       * for a number.  Stop accumulating digits.
	       */
	      break;
	    ok:
	      /*
	       * c is legal: store it and look at the next.
	       */
	      *p++ = c;
	      if (--fp->_r > 0)
		fp->_p++;
	      else
#ifndef CYGNUS_NEC
	      if (__srefill (fp))
#endif
		break;		/* (-1) */
	    }
	  /*
	   * If we had only a sign, it is no good; push back the sign.
	   * If the number ends in `x', it was [sign] '0' 'x', so push back
	   * the x and treat it as [sign] '0'.
	   */
	  if (flags & NDIGITS)
	    {
	      if (p > buf)
		(void) func_00309E18 (*(u_char *)-- p, fp);
	      goto match_failure;
	    }
	  c = ((u_char *) p)[-1];
	  if (c == 'x' || c == 'X')
	    {
	      --p;
	      /*(void)*/ func_00309E18 (c, fp);
	    }
	  if ((flags & SUPPRESS) == 0)
	    {
	      u_long res;

	      *p = 0;
	      res = (*ccfn) (buf, (char **) NULL, base);
	      if (flags & POINTER)
		*(__builtin_va_arg (ap, void * *)) = (void *) (unsigned long) res;
	      else if (flags & SHORT)
		{
		  sp = __builtin_va_arg (ap, short *);
		  *sp = res;
		}
	      else if (flags & LONG)
		{
		  lp = __builtin_va_arg (ap, long *);
		  *lp = res;
		}
	      else
		{
		  ip = __builtin_va_arg (ap, int *);
		  *ip = res;
		}
	      nassigned++;
	    }
	  nread += p - buf;
	  break;

#ifdef FLOATING_POINT
	case CT_FLOAT:
	{
	  /* scan a floating point number as if by strtod */
	  /* This code used to assume that the number of digits is reasonable.
	     However, ANSI / ISO C makes no such stipulation; we have to get
	     exact results even when there is an unreasonable amount of
	     leading zeroes.  */
	  long leading_zeroes, zeroes, exp_adjust;
	  char *exp_start;
#ifdef hardway
	  if (width == 0 || width > sizeof (buf) - 1)
	    width = sizeof (buf) - 1;
#else
	  /* size_t is unsigned, hence this optimisation */
	  if (--width > sizeof (buf) - 2)
	    width = sizeof (buf) - 2;
	  width++;
#endif
	  flags |= SIGNOK | NDIGITS | DPTOK | EXPOK;
	  zeroes = 0;
	  exp_adjust = 0;
	  for (p = buf; width; )
	    {
	      c = *fp->_p;
	      /*
	       * This code mimicks the integer conversion
	       * code, but is much simpler.
	       */
	      switch (c)
		{

		case '0':
		  if (flags & NDIGITS)
		    {
		      flags &= ~SIGNOK;
		      zeroes++;
		      goto fskip;
		    }
		  /* Fall through.  */
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
		  flags &= ~(SIGNOK | NDIGITS);
		  goto fok;

		case '+':
		case '-':
		  if (flags & SIGNOK)
		    {
		      flags &= ~SIGNOK;
		      goto fok;
		    }
		  break;
		case '.':
		  if (flags & DPTOK)
		    {
		      flags &= ~(SIGNOK | DPTOK);
		      leading_zeroes = zeroes;
		      goto fok;
		    }
		  break;
		case 'e':
		case 'E':
		  /* no exponent without some digits */
		  if ((flags & (NDIGITS | EXPOK)) == EXPOK
		      || ((flags & EXPOK) && zeroes))
		    {
		      if (! (flags & DPTOK))
			{
			  exp_adjust = zeroes - leading_zeroes;
			  exp_start = p;
			}
		      flags =
			(flags & ~(EXPOK | DPTOK)) |
			SIGNOK | NDIGITS;
		      zeroes = 0;
		      goto fok;
		    }
		  break;
		}
	      break;
	    fok:
	      *p++ = c;
	      width--;
	    fskip:
              ++nread;
	      if (--fp->_r > 0)
		fp->_p++;
	      else
#ifndef CYGNUS_NEC
	      if (__srefill (fp))
#endif
		break;		/* (-1) */
	    }
	  if (zeroes)
	    flags &= ~NDIGITS;
	  /*
	   * If no digits, might be missing exponent digits
	   * (just give back the exponent) or might be missing
	   * regular digits, but had sign and/or decimal point.
	   */
	  if (flags & NDIGITS)
	    {
	      if (flags & EXPOK)
		{
		  /* no digits at all */
		  while (p > buf)
                    {
		      func_00309E18 (*(u_char *)-- p, fp);
                      --nread;
                    }
		  goto match_failure;
		}
	      /* just a bad exponent (e and maybe sign) */
	      c = *(u_char *)-- p;
              --nread;
	      if (c != 'e' && c != 'E')
		{
		  (void) func_00309E18 (c, fp);	/* sign */
		  c = *(u_char *)-- p;
                  --nread;
		}
	      (void) func_00309E18 (c, fp);
	    }
	  if ((flags & SUPPRESS) == 0)
	    {
	      double res;
	      long new_exp;

	      *p = 0;
	      if ((flags & (DPTOK | EXPOK)) == EXPOK)
		{
		  exp_adjust = zeroes - leading_zeroes;
		  new_exp = -exp_adjust;
		  exp_start = p;
		}
	      else if (exp_adjust)
		new_exp = func_00305D40 (exp_start + 1) - exp_adjust;
	      if (exp_adjust)
		{
		  static const char exponent_format[] __attribute__((section(".rodata"))) = "e%ld";

		  /* If there might not be enough space for the new exponent,
		     truncate some trailing digits to make room.  */
		  if (exp_start >= buf + sizeof (buf) - MAX_LONG_LEN)
		    exp_start = buf + sizeof (buf) - MAX_LONG_LEN - 1;
                 sprintf (exp_start, exponent_format, new_exp);
		}
	      res = func_00305D28 (buf);
	      if (flags & LONG)
		{
		  dp = __builtin_va_arg (ap, double *);
		  *dp = res;
		}
	      else if (flags & LONGDBL)
		{
		  ldp = __builtin_va_arg (ap, long double *);
		  *ldp = res;
		}
	      else
		{
		  flp = __builtin_va_arg (ap, float *);
		  *flp = res;
		}
	      nassigned++;
	    }
	  break;
	}
#endif /* FLOATING_POINT */
	}
    }
input_failure:
  return nassigned ? nassigned : -1;
match_failure:
  return nassigned;
}


#undef FLOATING_POINT
#undef BUF
#undef MAX_LONG_LEN
#undef LONG
#undef LONGDBL
#undef SHORT
#undef SUPPRESS
#undef POINTER
#undef NOSKIP
#undef SIGNOK
#undef NDIGITS
#undef DPTOK
#undef EXPOK
#undef PFXOK
#undef NZDIGITS
#undef CT_CHAR
#undef CT_CCL
#undef CT_STRING
#undef CT_INT
#undef CT_FLOAT
#undef u_char
#undef u_long
#undef BufferEmpty

/* Historical __sccl from newlib 1.9.0 vfscanf.c.
 * This software was developed by the University of California, Berkeley.
 * The donor defines u_char as signed plain char on this target.
 */
/*
 * Copyright (c) 1990 The Regents of the University of California.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms are permitted
 * provided that the above copyright notice and this paragraph are
 * duplicated in all such forms and that any documentation,
 * advertising materials, and other materials related to such
 * distribution and use acknowledge that the software was developed
 * by the University of California, Berkeley.  The name of the
 * University may not be used to endorse or promote products derived
 * from this software without specific prior written permission.
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */

/*static*/
char *
func_00305A08 (tab, fmt)
     register char *tab;
     register char *fmt;
{
  register int c, n, v;

  /* first `clear' the whole table */
  c = *fmt++;			/* first char hat => negated scanset */
  if (c == '^')
    {
      v = 1;			/* default => accept */
      c = *fmt++;		/* get new first char */
    }
  else
    v = 0;			/* default => reject */
  /* should probably use memset here */
  for (n = 0; n < 256; n++)
    tab[n] = v;
  if (c == 0)
    return fmt - 1;		/* format ended before closing ] */

  /*
   * Now set the entries corresponding to the actual scanset to the
   * opposite of the above.
   *
   * The first character may be ']' (or '-') without being special; the
   * last character may be '-'.
   */

  v = 1 - v;
  for (;;)
    {
      tab[c] = v;		/* take character c */
    doswitch:
      n = *fmt++;		/* and examine the next */
      switch (n)
	{

	case 0:		/* format ended too soon */
	  return fmt - 1;

	case '-':
	  /*
	   * A scanset of the form [01+-] is defined as `the digit 0, the
	   * digit 1, the character +, the character -', but the effect of a
	   * scanset such as [a-zA-Z0-9] is implementation defined.  The V7
	   * Unix scanf treats `a-z' as `the letters a through z', but treats
	   * `a-a' as `the letter a, the character -, and the letter a'.
	   *
	   * For compatibility, the `-' is not considerd to define a range if
	   * the character following it is either a close bracket (required by
	   * ANSI) or is not numerically greater than the character we just
	   * stored in the table (c).
	   */
	  n = *fmt;
	  if (n == ']' || n < c)
	    {
	      c = '-';
	      break;		/* resume the for(;;) */
	    }
	  fmt++;
	  do
	    {			/* fill in the range */
	      tab[++c] = v;
	    }
	  while (c < n);
#if 1			/* XXX another disgusting compatibility hack */
	  /*
	   * Alas, the V7 Unix scanf also treats formats such
	   * as [a-c-e] as `the letters a through e'. This too
	   * is permitted by the standard....
	   */
	  goto doswitch;
#else
	  c = *fmt++;
	  if (c == 0)
	    return fmt - 1;
	  if (c == ']')
	    return fmt;
#endif

	  break;


	case ']':		/* end of scanset */
	  return fmt;

	default:		/* just another character */
	  c = n;
	  break;
	}
    }
  /* NOTREACHED */
}


s32 vsprintf(char *str, const char *fmt, va_list ap) {
    s32 result;
    FILE f;

    f._flags = 0x208;
    f._bf._base = f._p = (u8 *)str;
    f._bf._size = f._w = 0x7FFFFFFF;
    f._data = _impure_ptr;
    result = vfprintf(&f, fmt, ap);
    *f._p = '\0';
    return result;
}

s32 vsprintf_r(_reent *r, char *str, const char *fmt, va_list ap) {
    s32 result;
    FILE f;

    f._flags = 0x208;
    f._bf._base = f._p = (u8 *)str;
    f._bf._size = f._w = 0x7FFFFFFF;
    f._data = r;
    result = vfprintf(&f, fmt, ap);
    *f._p = '\0';
    return result;
}

long _write_r(_reent *r, s32 fd, const char *buf, s32 n) {
    long result;

    libcErrno = 0;
    if ((result = write(fd, buf, n)) == -1 && libcErrno != 0) {
        r->_errno = libcErrno;
    }
    return result;
}

s32 __swsetup(FILE *fp)
{
    /* Keep attachment and initialization as one phase; see levers/control-flow.md. */
    do {
        if (fp->_data == NULL) fp->_data = _impure_ptr;
        if (!fp->_data->__sdidinit) __sinit(fp->_data);
    } while (0);
    if (!(fp->_flags & 8)) {
        if (!(fp->_flags & 0x10)) return -1;
        if (fp->_flags & 4) {
            if (fp->_ub._base != NULL) {
                if (fp->_ub._base != fp->_ubuf)
                    _free_r(fp->_data, fp->_ub._base);
                fp->_ub._base = NULL;
            }
            fp->_flags &= ~(4 | 0x20);
            fp->_r = 0;
            fp->_p = fp->_bf._base;
        }
        fp->_flags |= 8;
    }
    if (fp->_bf._base == NULL) __smakebuf(fp);
    if (fp->_flags & 1) {
        fp->_w = 0;
        fp->_lbfsize = -fp->_bf._size;
    } else {
        fp->_w = (fp->_flags & 2) ? 0 : fp->_bf._size;
    }
    return 0;
}

double func_00305D28(const char *s) {
    return func_00309A80(s, NULL);
}

long func_00305D40(const char *s) {
    return strtol(s, NULL, 10);
}

s32 _close_r(_reent *r, s32 fd) {
    s32 result;

    libcErrno = 0;
    result = close(fd);
    if (result == -1 && libcErrno != 0) {
        r->_errno = libcErrno;
    }
    return result;
}

/****************************************************************
 *
 * The author of this software is David M. Gay.
 *
 * Copyright (c) 1991 by AT&T.
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose without fee is hereby granted, provided that this entire notice
 * is included in all copies of any software which is or includes a copy
 * or modification of this software and in all copies of the supporting
 * documentation for such software.
 *
 * THIS SOFTWARE IS BEING PROVIDED "AS IS", WITHOUT ANY EXPRESS OR IMPLIED
 * WARRANTY.  IN PARTICULAR, NEITHER THE AUTHOR NOR AT&T MAKES ANY
 * REPRESENTATION OR WARRANTY OF ANY KIND CONCERNING THE MERCHANTABILITY
 * OF THIS SOFTWARE OR ITS FITNESS FOR ANY PARTICULAR PURPOSE.
 *
 ***************************************************************/


/* Historical newlib 1.9 dtoa.c Pack_32 form; target limbs are 32-bit. */
s32 quorem(_Bigint *b, _Bigint *S)
{
  int n;
  s32 borrow, y;
  u32 carry, q, ys;
  u32 *bx, *bxe, *sx, *sxe;
  s32 z;
  u32 si, zs;

  n = S->wds;
  if (b->wds < n)
    return 0;
  sx = S->x;
  sxe = sx + --n;
  bx = b->x;
  bxe = bx + n;
  q = *bxe / (*sxe + 1);	/* ensure q <= true quotient */
  if (q)
    {
      borrow = 0;
      carry = 0;
      do
	{
	  si = *sx++;
	  ys = (si & 0xffff) * q + carry;
	  zs = (si >> 16) * q + (ys >> 16);
	  carry = zs >> 16;
	  y = (*bx & 0xffff) - (ys & 0xffff) + borrow;
	  borrow = y >> 16;
	  z = (*bx >> 16) - (zs & 0xffff) + borrow;
	  borrow = z >> 16;
	  (((unsigned short *)bx)[1] = (unsigned short)z,
           ((unsigned short *)bx)[0] = (unsigned short)y, bx++);
	}
      while (sx <= sxe);
      if (!*bxe)
	{
	  bx = b->x;
	  while (--bxe > bx && !*bxe)
	    --n;
	  b->wds = n;
	}
    }
  if (__mcmp (b, S) >= 0)
    {
      q++;
      borrow = 0;
      carry = 0;
      bx = b->x;
      sx = S->x;
      do
	{
	  si = *sx++;
	  ys = (si & 0xffff) + carry;
	  zs = (si >> 16) + (ys >> 16);
	  carry = zs >> 16;
	  y = (*bx & 0xffff) - (ys & 0xffff) + borrow;
	  borrow = y >> 16;
	  z = (*bx >> 16) - (zs & 0xffff) + borrow;
	  borrow = z >> 16;
	  (((unsigned short *)bx)[1] = (unsigned short)z,
           ((unsigned short *)bx)[0] = (unsigned short)y, bx++);
	}
      while (sx <= sxe);
      bx = b->x;
      bxe = bx + n;
      if (!*bxe)
	{
	  while (--bxe > bx && !*bxe)
	    --n;
	  b->wds = n;
	}
    }
  return q;
}

/****************************************************************
 *
 * The author of this software is David M. Gay.
 *
 * Copyright (c) 1991 by AT&T.
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose without fee is hereby granted, provided that this entire notice
 * is included in all copies of any software which is or includes a copy
 * or modification of this software and in all copies of the supporting
 * documentation for such software.
 *
 * THIS SOFTWARE IS BEING PROVIDED "AS IS", WITHOUT ANY EXPRESS OR IMPLIED
 * WARRANTY.  IN PARTICULAR, NEITHER THE AUTHOR NOR AT&T MAKES ANY
 * REPRESENTATION OR WARRANTY OF ANY KIND CONCERNING THE MERCHANTABILITY
 * OF THIS SOFTWARE OR ITS FITNESS FOR ANY PARTICULAR PURPOSE.
 *
 ***************************************************************/


/* Historical newlib 1.9 dtoa.c, IEEE binary64 little-endian / Pack_32. */
/* Immutable native strings remain in their original .rodata objects. */
extern const char D_003B8320[] __attribute__((section(".rodata")));
extern const char D_003B8330[] __attribute__((section(".rodata")));
extern const char D_003B8338[] __attribute__((section(".rodata")));

INCLUDE_RODATA(const s32, "sdk/libc_after_vfprintf", D_003B8320);

INCLUDE_RODATA(const s32, "sdk/libc_after_vfprintf", D_003B8330);

INCLUDE_RODATA(const s32, "sdk/libc_after_vfprintf", D_003B8338);

char *_dtoa_r(_reent *ptr, f64 _d, s32 mode, s32 ndigits,
              s32 *decpt, s32 *sign, char **rve)
{
  int bbits, b2, b5, be, dig, i, ieps, ilim, ilim0, ilim1, j, j1, k, k0,
    k_check, leftright, m2, m5, s2, s5, spec_case, try_quick;
  libcDoubleBits_t d, d2, eps;
  s32 L;
  int denorm;
  u32 x;
  _Bigint *b, *b1, *delta, *mlo, *mhi, *S;
  double ds;
  char *s, *s0;
  d.value = _d;
  if (ptr->_result)
    {
      ptr->_result->k = ptr->_result_k;
      ptr->_result->maxwds = 1 << ptr->_result_k;
      _Bfree (ptr, ptr->_result);
      ptr->_result = 0;
    }
  if ((d.word[1]) & ((u32)0x80000000L))
    {
      *sign = 1;
      (d.word[1]) &= ~((u32)0x80000000L);
    }
  else
    *sign = 0;
  if (((d.word[1]) & ((u32)0x7ff00000L)) == ((u32)0x7ff00000L))
    {
      *decpt = 9999;
      s =
 !(d.word[0]) && !((d.word[1]) & 0xfffff) ? D_003B8320 :
 D_003B8330;
      if (rve)
 *rve =
   s[3] ? s + 8 :
   s + 3;
      return s;
    }
  if (!d.value)
    {
      *decpt = 1;
      s = D_003B8338;
      if (rve)
 *rve = s + 1;
      return s;
    }
  b = _d2b (ptr, d.value, &be, &bbits);
  if ((i = (int) ((d.word[1]) >> 20 & (((u32)0x7ff00000L) >> 20))) != 0)
    {
      d2.value = d.value;
      (d2.word[1]) &= ((u32)0xfffffL);
      (d2.word[1]) |= ((u32)0x3ff00000L);
      i -= 1023;
      denorm = 0;
    }
  else
    {
      i = bbits + be + (1023 + (53 - 1) - 1);
      x = (i > 32) ? ((d.word[1]) << (64 - i)) | ((d.word[0]) >> (i - 32))
       : ((d.word[0]) << (32 - i));
      d2.value = x;
      (d2.word[1]) -= 31 * ((u32)0x100000L);
      i -= (1023 + (53 - 1) - 1) + 1;
      denorm = 1;
    }
  ds = (d2.value - 1.5) * 0.289529654602168 + 0.1760912590558 + i * 0.301029995663981;
  k = (int) ds;
  if (ds < 0. && ds != k)
    k--;
  k_check = 1;
  if (k >= 0 && k <= 22)
    {
      if (d.value < pow10Table[k])
 k--;
      k_check = 0;
    }
  j = bbits - i - 1;
  if (j >= 0)
    {
      b2 = 0;
      s2 = j;
    }
  else
    {
      b2 = -j;
      s2 = 0;
    }
  if (k >= 0)
    {
      b5 = 0;
      s5 = k;
      s2 += k;
    }
  else
    {
      b2 -= k;
      b5 = -k;
      s5 = 0;
    }
  if (mode < 0 || mode > 9)
    mode = 0;
  try_quick = 1;
  if (mode > 5)
    {
      mode -= 4;
      try_quick = 0;
    }
  leftright = 1;
  ilim = ilim1 = -1;
  switch (mode)
    {
    case 0:
    case 1:
      i = 18;
      ndigits = 0;
      break;
    case 2:
      leftright = 0;
    case 4:
      if (ndigits <= 0)
 ndigits = 1;
      ilim = ilim1 = i = ndigits;
      break;
    case 3:
      leftright = 0;
    case 5:
      i = ndigits + k + 1;
      ilim = i;
      ilim1 = i - 1;
      if (i <= 0)
 i = 1;
    }
  j = sizeof (u32);
  for (ptr->_result_k = 0; sizeof (_Bigint) - sizeof (u32) + j <= i;
       j <<= 1)
    ptr->_result_k++;
  ptr->_result = _Balloc (ptr, ptr->_result_k);
  s = s0 = (char *) ptr->_result;
  if (ilim >= 0 && ilim <= 14 && try_quick)
    {
      i = 0;
      d2.value = d.value;
      k0 = k;
      ilim0 = ilim;
      ieps = 2;
      if (k > 0)
 {
   ds = pow10Table[k & 0xf];
   j = k >> 4;
   if (j & 0x10)
     {
       j &= 0x10 - 1;
       d.value /= D_003B8498[5 - 1];
       ieps++;
     }
   for (; j; j >>= 1, i++)
     if (j & 1)
       {
  ieps++;
  ds *= D_003B8498[i];
       }
   d.value /= ds;
 }
      else if ((j1 = -k) != 0)
 {
   d.value *= pow10Table[j1 & 0xf];
   for (j = j1 >> 4; j; j >>= 1, i++)
     if (j & 1)
       {
  ieps++;
  d.value *= D_003B8498[i];
       }
 }
      if (k_check && d.value < 1. && ilim > 0)
 {
   if (ilim1 <= 0)
     goto fast_failed;
   ilim = ilim1;
   k--;
   d.value *= 10.;
   ieps++;
 }
      eps.value = ieps * d.value + 7.;
      (eps.word[1]) -= (53 - 1) * ((u32)0x100000L);
      if (ilim == 0)
 {
   S = mhi = 0;
   d.value -= 5.;
   if (d.value > eps.value)
     goto one_digit;
   if (d.value < -eps.value)
     goto no_digits;
   goto fast_failed;
 }
      if (leftright)
 {
   eps.value = 0.5 / pow10Table[ilim - 1] - eps.value;
   for (i = 0;;)
     {
       L = d.value;
       d.value -= L;
       *s++ = '0' + (int) L;
       if (d.value < eps.value)
  goto ret1;
       if (1. - d.value < eps.value)
  goto bump_up;
       if (++i >= ilim)
  break;
       eps.value *= 10.;
       d.value *= 10.;
     }
 }
      else
 {
   eps.value *= pow10Table[ilim - 1];
   for (i = 1;; i++, d.value *= 10.)
     {
       L = d.value;
       d.value -= L;
       *s++ = '0' + (int) L;
       if (i == ilim)
  {
    if (d.value > 0.5 + eps.value)
      goto bump_up;
    else if (d.value < 0.5 - eps.value)
      {
        while (*--s == '0');
        s++;
        goto ret1;
      }
    break;
  }
     }
 }
    fast_failed:
      s = s0;
      d.value = d2.value;
      k = k0;
      ilim = ilim0;
    }
  if (be >= 0 && k <= 14)
    {
      ds = pow10Table[k];
      if (ndigits < 0 && ilim <= 0)
 {
   S = mhi = 0;
   if (ilim < 0 || d.value <= 5 * ds)
     goto no_digits;
   goto one_digit;
 }
      for (i = 1;; i++)
 {
   L = d.value / ds;
   d.value -= L * ds;
   *s++ = '0' + (int) L;
   if (i == ilim)
     {
       d.value += d.value;
             if ((d.value > ds) || ((d.value == ds) && (L & 1)))
  {
  bump_up:
    while (*--s == '9')
      if (s == s0)
        {
   k++;
   *s = '0';
   break;
        }
    ++*s++;
  }
       break;
     }
   if (!(d.value *= 10.))
     break;
 }
      goto ret1;
    }
  m2 = b2;
  m5 = b5;
  mhi = mlo = 0;
  if (leftright)
    {
      if (mode < 2)
 {
   i =
     denorm ? be + (1023 + (53 - 1) - 1 + 1) :
     1 + 53 - bbits;
 }
      else
 {
   j = ilim - 1;
   if (m5 >= j)
     m5 -= j;
   else
     {
       s5 += j -= m5;
       b5 += j;
       m5 = 0;
     }
   if ((i = ilim) < 0)
     {
       m2 -= i;
       i = 0;
     }
 }
      b2 += i;
      s2 += i;
      mhi = _i2b (ptr, 1);
    }
  if (m2 > 0 && s2 > 0)
    {
      i = m2 < s2 ? m2 : s2;
      b2 -= i;
      m2 -= i;
      s2 -= i;
    }
  if (b5 > 0)
    {
      if (leftright)
 {
   if (m5 > 0)
     {
       mhi = _pow5mult (ptr, mhi, m5);
       b1 = _multiply (ptr, mhi, b);
       _Bfree (ptr, b);
       b = b1;
     }
         if ((j = b5 - m5) != 0)
     b = _pow5mult (ptr, b, j);
 }
      else
 b = _pow5mult (ptr, b, b5);
    }
  S = _i2b (ptr, 1);
  if (s5 > 0)
    S = _pow5mult (ptr, S, s5);
  spec_case = 0;
  if (mode < 2)
    {
      if (!(d.word[0]) && !((d.word[1]) & ((u32)0xfffffL))
   && (d.word[1]) & ((u32)0x7ff00000L)
 )
 {
   b2 += 1;
   s2 += 1;
   spec_case = 1;
 }
    }
  if ((i = ((s5 ? 32 - _hi0bits (S->x[S->wds - 1]) : 1) + s2) & 0x1f) != 0)
    i = 32 - i;
  if (i > 4)
    {
      i -= 4;
      b2 += i;
      m2 += i;
      s2 += i;
    }
  else if (i < 4)
    {
      i += 28;
      b2 += i;
      m2 += i;
      s2 += i;
    }
  if (b2 > 0)
    b = _lshift (ptr, b, b2);
  if (s2 > 0)
    S = _lshift (ptr, S, s2);
  if (k_check)
    {
      if (__mcmp (b, S) < 0)
 {
   k--;
   b = _multadd (ptr, b, 10, 0);
   if (leftright)
     mhi = _multadd (ptr, mhi, 10, 0);
   ilim = ilim1;
 }
    }
  if (ilim <= 0 && mode > 2)
    {
      if (ilim < 0 || __mcmp (b, S = _multadd (ptr, S, 5, 0)) <= 0)
 {
 no_digits:
   k = -1 - ndigits;
   goto ret;
 }
    one_digit:
      *s++ = '1';
      k++;
      goto ret;
    }
  if (leftright)
    {
      if (m2 > 0)
 mhi = _lshift (ptr, mhi, m2);
      mlo = mhi;
      if (spec_case)
 {
   mhi = _Balloc (ptr, mhi->k);
   memcpy((char *)&mhi->sign, (char *)&mlo->sign, mlo->wds*sizeof(s32) + 2*sizeof(int));
   mhi = _lshift (ptr, mhi, 1);
 }
      for (i = 1;; i++)
 {
   dig = quorem (b, S) + '0';
   j = __mcmp (b, mlo);
   delta = __mdiff (ptr, S, mhi);
   j1 = delta->sign ? 1 : __mcmp (b, delta);
   _Bfree (ptr, delta);
   if (j1 == 0 && !mode && !((d.word[0]) & 1))
     {
       if (dig == '9')
  goto round_9_up;
       if (j > 0)
  dig++;
       *s++ = dig;
       goto ret;
     }
         if ((j < 0) || ((j == 0) && !mode
       && !((d.word[0]) & 1)
           ))
     {
       if (j1 > 0)
  {
    b = _lshift (ptr, b, 1);
    j1 = __mcmp (b, S);
                 if (((j1 > 0) || ((j1 == 0) && (dig & 1)))
        && dig++ == '9')
      goto round_9_up;
  }
       *s++ = dig;
       goto ret;
     }
   if (j1 > 0)
     {
       if (dig == '9')
  {
  round_9_up:
    *s++ = '9';
    goto roundoff;
  }
       *s++ = dig + 1;
       goto ret;
     }
   *s++ = dig;
   if (i == ilim)
     break;
   b = _multadd (ptr, b, 10, 0);
   if (mlo == mhi)
     mlo = mhi = _multadd (ptr, mhi, 10, 0);
   else
     {
       mlo = _multadd (ptr, mlo, 10, 0);
       mhi = _multadd (ptr, mhi, 10, 0);
     }
 }
    }
  else
    for (i = 1;; i++)
      {
 *s++ = dig = quorem (b, S) + '0';
 if (i >= ilim)
   break;
 b = _multadd (ptr, b, 10, 0);
      }
  b = _lshift (ptr, b, 1);
  j = __mcmp (b, S);
  if ((j > 0) || ((j == 0) && (dig & 1)))
    {
    roundoff:
      while (*--s == '9')
 if (s == s0)
   {
     k++;
     *s++ = '1';
     goto ret;
   }
      ++*s++;
    }
  else
    {
      while (*--s == '0');
      s++;
    }
ret:
  _Bfree (ptr, S);
  if (mhi)
    {
      if (mlo && mlo != mhi)
 _Bfree (ptr, mlo);
      _Bfree (ptr, mhi);
    }
ret1:
  _Bfree (ptr, b);
  *s = 0;
  *decpt = k + 1;
  if (rve)
    *rve = s;
  return s0;
}

/* newlib stdio/flags.c: __sflags; binary/text flags are absent on this target. */
s32 func_00307250(_reent *r, const char *mode, s32 *openFlags) {
    s32 flags, access, options;

    switch (mode[0]) {
    case 'r':
        flags = 0x4;
        access = 0;
        options = 0;
        break;
    case 'w':
        flags = 0x8;
        access = 1;
        options = 0x600;
        break;
    case 'a':
        flags = 0x108;
        access = 1;
        options = 0x208;
        break;
    default:
        r->_errno = 22;
        return 0;
    }
    if (mode[1] && (mode[1] == '+' || mode[2] == '+')) {
        flags = 0x10;
        access = 2;
    }
    *openFlags = access | options;
    return flags;
}

/* Historical newlib buffered read; __srefill resets the consumed count. */
size_t fread(void *buf, size_t size, size_t count, FILE *fp) {
    size_t resid;
    char *p;
    int r;
    size_t total;

    if ((resid = count * size) == 0)
        return 0;
    if (fp->_r < 0)
        fp->_r = 0;
    total = resid;
    p = buf;
    while (resid > (r = fp->_r)) {
        memcpy(p, fp->_p, (size_t)r);
        fp->_p += r;
        p += r;
        resid -= r;
        if (__srefill(fp))
            return (total - resid) / size;
    }
    memcpy(p, fp->_p, resid);
    fp->_r -= resid;
    fp->_p += resid;
    return count;
}

s32 __sfvwrite(FILE *fp, __suio *uio)
{
    u32 length;
    char *p;
    __siov *iov;
    s32 written;
    s32 available;
    s32 newlineKnown;
    s32 newlineDistance;
    char *newline;

    /* One byte count spans the entry and vector phases; see levers/cross-block-locals.md. */
    length = uio->uio_resid;
    if (length == 0)
        return 0;
    if ((fp->_flags & 8) == 0 || fp->_bf._base == 0)
        if (__swsetup(fp) != 0)
            return -1;
    iov = uio->uio_iov;
    length = 0;
    if (fp->_flags & 2) {
        do {
            while (length == 0) {
                p = iov->iov_base;
                length = iov->iov_len;
                iov++;
            }
            written = fp->_write(fp->_cookie, p, length > 1024 ? 1024 : length);
            if (written <= 0)
                goto error;
            p += written;
            length -= written;
        } while ((uio->uio_resid -= written) != 0);
    } else if ((fp->_flags & 1) == 0) {
        do {
            while (length == 0) {
                p = iov->iov_base;
                length = iov->iov_len;
                iov++;
            }
            written = fp->_w;
            if (fp->_flags & 0x200) {
                if (length < written)
                    written = length;
                memmove(fp->_p, p, written);
                fp->_w -= written;
                fp->_p += written;
                written = length;
            } else if (fp->_p > fp->_bf._base && length > written) {
                memmove(fp->_p, p, written);
                fp->_p += written;
                if (fflush(fp) != 0)
                    goto error;
            } else if (length >= (written = fp->_bf._size)) {
                written = fp->_write(fp->_cookie, p, written);
                if (written <= 0)
                    goto error;
            } else {
                written = length;
                memmove(fp->_p, p, written);
                fp->_w -= written;
                fp->_p += written;
            }
            p += written;
            length -= written;
        } while ((uio->uio_resid -= written) != 0);
    } else {
        newlineKnown = 0;
        do {
            if (length == 0) {
                newlineKnown = 0;
                while (length == 0) {
                    p = iov->iov_base;
                    length = iov->iov_len;
                    iov++;
                }
            }
            if (!newlineKnown) {
                newline = memchr(p, '\n', length);
                newlineDistance = newline ? newline + 1 - p : length + 1;
                newlineKnown = 1;
            }
            available = length < newlineDistance ? length : newlineDistance;
            written = fp->_w + fp->_bf._size;
            if (fp->_p > fp->_bf._base && written < available) {
                memmove(fp->_p, p, written);
                fp->_p += written;
                if (fflush(fp) != 0)
                    goto error;
            } else if (available >= (written = fp->_bf._size)) {
                written = fp->_write(fp->_cookie, p, written);
                if (written <= 0)
                    goto error;
            } else {
                written = available;
                memmove(fp->_p, p, written);
                fp->_w -= written;
                fp->_p += written;
            }
            if ((newlineDistance -= written) == 0) {
                if (fflush(fp) != 0)
                    goto error;
                newlineKnown = 0;
            }
            p += written;
            length -= written;
        } while ((uio->uio_resid -= written) != 0);
    }
    return 0;
error:
    fp->_flags |= 0x40;
    return -1;
}

char *_setlocale_r(_reent *r, s32 category, const char *locale)
{
    if (locale != NULL) {
        if (strcmp(locale, localeCName) != 0 && strcmp(locale, localeEmptyStr) != 0)
            return NULL;
        r->_current_category = category;
        r->_current_locale = locale;
    }
    return localeCName;
}

void *_localeconv_r(_reent *r) {
    return &localeconvData;
}

char *setlocale(s32 category, const char *locale) {
    return _setlocale_r(_impure_ptr, category, locale);
}

void *localeconv(void) {
    return _localeconv_r(_impure_ptr);
}

long _lseek_r(_reent *r, s32 fd, long offset, s32 whence) {
    long result;

    libcErrno = 0;
    result = lseek(fd, offset, whence);
    if (result == -1 && libcErrno != 0) {
        r->_errno = libcErrno;
    }
    return result;
}

s32 _mbtowc_r(_reent *r, s32 *pwc, const char *s, u32 n, s32 *state) {
    s32 dummy;
    s32 *out = pwc;

    if (pwc == NULL) {
        out = &dummy;
    }
    if (s == NULL) {
        return 0;
    }
    if (n == 0) {
        return -1;
    }
    *out = *(u8 *)s;
    if (*(u8 *)s == 0) {
        return 0;
    }
    return 1;
}

INCLUDE_ASM(const s32, "sdk/libc_after_vfprintf", memchr);

_Bigint *_Balloc(_reent *r, s32 k) {
    _Bigint *rv;
    s32 x;

    if (r->_freelist == NULL) {
        r->_freelist = _calloc_r(r, 4, 16);
        if (r->_freelist == NULL) {
            return NULL;
        }
    }
    rv = r->_freelist[k];
    if (rv != NULL) {
        r->_freelist[k] = rv->next;
    } else {
        x = 1 << k;
        rv = _calloc_r(r, 1, 0x14 + x * sizeof(u32));
        if (rv == NULL) {
            return NULL;
        }
        rv->k = k;
        rv->maxwds = x;
    }
    rv->sign = rv->wds = 0;
    return rv;
}

void _Bfree(_reent *r, _Bigint *v) {
    if (v != NULL) {
        v->next = r->_freelist[v->k];
        r->_freelist[v->k] = v;
    }
}

_Bigint *_multadd(_reent *r, _Bigint *b, s32 m, s32 a)
{
    s32 wds = b->wds;
    u32 *x = b->x;
    s32 i = 0;
    u32 lo;
    u32 hi;
    _Bigint *b1;

    do {
        lo = (*x & 0xffff) * m + a;
        hi = (*x >> 16) * m + (lo >> 16);
        a = hi >> 16;
        *x++ = (hi << 16) + (lo & 0xffff);
    } while (++i < wds);
    if (a != 0) {
        if (wds >= b->maxwds) {
            b1 = _Balloc(r, b->k + 1);
            memcpy(&b1->sign, &b->sign, b->wds * sizeof(u32) + 2 * sizeof(s32));
            _Bfree(r, b);
            b = b1;
        }
        b->x[wds++] = a;
        b->wds = wds;
    }
    return b;
}

_Bigint *_s2b(_reent *r, const char *s, s32 nd0, s32 nd, u32 y9)
{
    _Bigint *b;
    s32 n;
    s32 k;
    s32 i;
    s32 capacity;

    n = (nd + 8) / 9;
    k = 0;
    capacity = 1;
    while (capacity < n) {
        capacity <<= 1;
        k++;
    }

    b = _Balloc(r, k);
    b->x[0] = y9;
    b->wds = 1;

    i = 9;
    if (nd0 > 9) {
        s += 9;
        do {
            b = _multadd(r, b, 10, *s++ - '0');
        } while (++i < nd0);
        s++;
    } else {
        s += 10;
    }

    while (i < nd) {
        b = _multadd(r, b, 10, *s++ - '0');
        ++i;
    }
    return b;
}

s32 _hi0bits(u32 x) {
    s32 k = 0;

    if ((x & 0xFFFF0000) == 0) {
        k = 16;
        x <<= 16;
    }
    if ((x & 0xFF000000) == 0) {
        k += 8;
        x <<= 8;
    }
    if ((x & 0xF0000000) == 0) {
        k += 4;
        x <<= 4;
    }
    if ((x & 0xC0000000) == 0) {
        k += 2;
        x <<= 2;
    }
    if ((x & 0x80000000) == 0) {
        k++;
        if ((x & 0x40000000) == 0) {
            return 32;
        }
    }
    return k;
}

s32 _lo0bits(u32 *y) {
    s32 k;
    u32 x = *y;

    if (x & 7) {
        if (x & 1) {
            return 0;
        }
        if (x & 2) {
            *y = x >> 1;
            return 1;
        }
        *y = x >> 2;
        return 2;
    }
    k = 0;
    if ((x & 0xFFFF) == 0) {
        k = 16;
        x >>= 16;
    }
    if ((x & 0xFF) == 0) {
        k += 8;
        x >>= 8;
    }
    if ((x & 0xF) == 0) {
        k += 4;
        x >>= 4;
    }
    if ((x & 3) == 0) {
        k += 2;
        x >>= 2;
    }
    if ((x & 1) == 0) {
        k++;
        x >>= 1;
        if (x == 0) {
            return 32;
        }
    }
    *y = x;
    return k;
}

_Bigint *_i2b(_reent *r, s32 i) {
    _Bigint *b = _Balloc(r, 1);

    b->x[0] = i;
    b->wds = 1;
    return b;
}

/* newlib mprec multiplication, packed 32-bit limbs and little-endian Storeinc. */
_Bigint *_multiply(_reent *r, _Bigint *a, _Bigint *b)
{
    _Bigint *c;
    s32 k, wa, wb, wc;
    u32 carry, y, z;
    u32 *x, *xa, *xae, *xb, *xbe, *xc, *xc0;
    u32 z2;

    if (a->wds < b->wds) {
        c = a;
        a = b;
        b = c;
    }
    k = a->k;
    wa = a->wds;
    wb = b->wds;
    wc = wa + wb;
    if (wc > a->maxwds)
        k++;
    c = _Balloc(r, k);
    for (x = c->x, xa = x + wc; x < xa; x++)
        *x = 0;
    xa = a->x;
    xae = xa + wa;
    xb = b->x;
    xbe = xb + wb;
    xc0 = c->x;
    for (; xb < xbe; xb++, xc0++) {
        if ((y = *xb & 0xffff) != 0) {
            x = xa;
            xc = xc0;
            carry = 0;
            do {
                z = (*x & 0xffff) * y + (*xc & 0xffff) + carry;
                carry = z >> 16;
                z2 = (*x++ >> 16) * y + (*xc >> 16) + carry;
                carry = z2 >> 16;
                ((u16 *)xc)[1] = z2;
                ((u16 *)xc)[0] = z;
                xc++;
            } while (x < xae);
            *xc = carry;
        }
        if ((y = *xb >> 16) != 0) {
            x = xa;
            xc = xc0;
            carry = 0;
            z2 = *xc;
            do {
                z = (*x & 0xffff) * y + (*xc >> 16) + carry;
                carry = z >> 16;
                ((u16 *)xc)[1] = z;
                ((u16 *)xc)[0] = z2;
                xc++;
                z2 = (*x++ >> 16) * y + (*xc & 0xffff) + carry;
                carry = z2 >> 16;
            } while (x < xae);
            *xc = z2;
        }
    }
    for (xc0 = c->x, xc = xc0 + wc; wc > 0 && !*--xc; --wc)
        ;
    c->wds = wc;
    return c;
}

INCLUDE_RODATA(const s32, "sdk/libc_after_vfprintf", localeconvData);

INCLUDE_RODATA(const s32, "sdk/libc_after_vfprintf", localeEmptyStr);

INCLUDE_RODATA(const s32, "sdk/libc_after_vfprintf", D_003B83B0);

INCLUDE_RODATA(const s32, "sdk/libc_after_vfprintf", localeCName);

_Bigint *_pow5mult(_reent *r, _Bigint *b, s32 n)
{
    _Bigint *power;
    _Bigint *nextPower;
    _Bigint *product;
    s32 remainder;

    remainder = n & 3;
    if (remainder != 0) {
        b = _multadd(r, b, pow5MultTable[remainder - 1], 0);
    }
    n >>= 2;
    if (n == 0) {
        return b;
    }
    power = r->_p5s;
    if (power == NULL) {
        power = r->_p5s = _i2b(r, 625);
        power->next = NULL;
    }
    for (;;) {
        if (n & 1) {
            product = _multiply(r, b, power);
            _Bfree(r, b);
            b = product;
        }
        n >>= 1;
        if (n == 0) {
            break;
        }
        if ((nextPower = power->next) == NULL) {
            nextPower = power->next = _multiply(r, power, power);
            nextPower->next = NULL;
        }
        power = nextPower;
    }
    return b;
}

_Bigint *_lshift(_reent *r, _Bigint *b, s32 shift) {
    _Bigint *b1;
    u32 *src;
    u32 *dst;
    u32 *end;
    u32 carry;
    s32 n;
    s32 wds;
    s32 bitCount;
    s32 count;

    n = shift >> 5;
    count = b->maxwds;
    wds = n + b->wds + 1;
    bitCount = b->k;
    if (count < wds) {
        do {
            count *= 2;
            bitCount++;
        } while (count < wds);
    }
    b1 = _Balloc(r, bitCount);
    dst = b1->x;
    count = 0;
    while (count < n) {
        *dst++ = 0;
        count++;
    }
    src = b->x;
    shift &= 0x1F;
    end = src + b->wds;
    if (shift != 0) {
        bitCount = 0x20 - shift;
        carry = 0;
        do {
            *dst++ = (*src << shift) | carry;
            carry = *src++ >> bitCount;
        } while (src < end);
        *dst = carry;
        if (carry != 0) {
            wds++;
        }
    } else {
        do {
            *dst++ = *src++;
        } while (src < end);
    }
    b1->wds = wds - 1;
    _Bfree(r, b);
    return b1;
}

s32 __mcmp(_Bigint *a, _Bigint *b) {
    u32 *xa, *xa0, *xb, *xb0;
    s32 i, j;

    i = a->wds;
    j = b->wds;
    if ((i -= j) != 0) {
        return i;
    }
    xa0 = a->x;
    xa = xa0 + j;
    xb0 = b->x;
    xb = xb0 + j;
    for (;;) {
        if (*--xa != *--xb) {
            return *xa < *xb ? -1 : 1;
        }
        if (xa <= xa0) {
            break;
        }
    }
    return 0;
}

_Bigint *__mdiff(_reent *r, _Bigint *a, _Bigint *b) {
    s32 sign = __mcmp(a, b);
    _Bigint *result;
    u32 *srcA;
    u32 *srcB;
    u32 *dst;
    u32 *endA;
    u32 *endB;
    s32 countA;
    s32 countB;
    s32 borrow;
    s32 low;
    u32 limb;

    if (sign == 0) {
        result = _Balloc(r, 0);
        result->wds = 1;
        result->x[0] = 0;
        return result;
    }
    if (sign < 0) {
        result = a;
        a = b;
        b = result;
        sign = 1;
    } else {
        sign = 0;
    }

    result = _Balloc(r, a->k);
    result->sign = sign;
    srcA = a->x;
    srcB = b->x;
    dst = result->x;
    countA = a->wds;
    countB = b->wds;
    endA = srcA + countA;
    endB = srcB + countB;
    borrow = 0;
    do {
        u32 wordB;
        limb = *srcA++;
        wordB = *srcB++;
        low = (limb & 0xFFFF) - (wordB & 0xFFFF) + borrow;
        borrow = low >> 16;
        limb = (limb >> 16) - (wordB >> 16);
        limb += borrow;
        ((u16 *)dst)[0] = low;
        ((u16 *)dst)[1] = limb;
        dst++;
        borrow = (s32)limb >> 16;
    } while (srcB < endB);
    while (srcA < endA) {
        u32 word = *srcA++;
        low = (word & 0xFFFF) + borrow;
        borrow = low >> 16;
        limb = (word >> 16) + borrow;
        ((u16 *)dst)[0] = low;
        ((u16 *)dst)[1] = limb;
        dst++;
        borrow = (s32)limb >> 16;
    }
    dst--;
    while (*dst == 0) {
        dst--;
        countA--;
    }
    result->wds = countA;
    return result;
}

f64 _ulp(f64 x)
{
    libcDoubleBits_t input;
    libcDoubleBits_t result;
    s32 exponent;

    input.value = x;
    exponent = (input.word[1] & 0x7FF00000) - 0x03400000;
    if (exponent > 0) {
        result.word[1] = exponent;
        result.word[0] = 0;
    } else {
        exponent = -exponent >> 20;
        if (exponent < 20) {
            result.word[1] = 0x80000 >> exponent;
            result.word[0] = 0;
        } else {
            result.word[1] = 0;
            exponent -= 20;
            result.word[0] = exponent >= 31 ? 1 : 1 << (31 - exponent);
        }
    }
    return result.value;
}

f64 _b2d(_Bigint *b, s32 *exponent)
{
    libcDoubleBits_t result;
    u32 *first;
    u32 *last;
    u32 top;
    u32 previous;
    s32 leading;

    first = b->x;
    last = first + b->wds;
    top = *--last;
    leading = _hi0bits(top);
    *exponent = 32 - leading;

    if (leading < 11) {
        u32 below;
        result.word[1] = (top >> (11 - leading)) | 0x3FF00000;
        below = first < last ? *--last : 0;
        result.word[0] = (top << (leading + 21)) | (below >> (11 - leading));
    } else {
        if (first < last) {
            last--;
            previous = *last;
        } else {
            previous = 0;
        }
        leading -= 11;
        if (leading != 0) {
            result.word[1] = (top << leading) | (previous >> (32 - leading)) |
                             0x3FF00000;
            if (first < last) {
                last--;
                top = *last;
            } else {
                top = 0;
            }
            result.word[0] = (previous << leading) | (top >> (32 - leading));
        } else {
            result.word[1] = top | 0x3FF00000;
            result.word[0] = previous;
        }
    }
    return result.value;
}

_Bigint *_d2b(_reent *r, f64 d, s32 *e, s32 *bitsOut) {
    libcDoubleBits_t value;
    _Bigint *b;
    u32 highMantissa;
    u32 *limbs;
    s32 exponent;
    s32 trailingBits;
    s32 wordCount;

    b = _Balloc(r, 1);
    value.value = d;
    limbs = b->x;
    highMantissa = value.word[1] & 0xFFFFF;
    value.word[1] &= 0x7FFFFFFF;
    exponent = value.word[1] >> 20;
    if (exponent != 0) {
        highMantissa |= 0x100000;
    }
    /* Target-specific allocation aid: the real raw low-word snapshot uses
     * $5 only through the test and stack copy, before _lo0bits can clobber it.
     * This constrains reconstruction, not the historical source spelling. */
    {
        FAKEMATCH_ASM_REG(u32, rawLowWord, "$5") = value.word[0];
        FAKEMATCH_ASM_LIVE(rawLowWord);
        if (rawLowWord != 0) {
            u32 lowWord = rawLowWord;
            trailingBits = _lo0bits(&lowWord);
            if (trailingBits != 0) {
                limbs[0] = lowWord | (highMantissa << (32 - trailingBits));
                highMantissa >>= trailingBits;
            } else {
                limbs[0] = lowWord;
            }
            wordCount = b->wds = (limbs[1] = highMantissa) != 0 ? 2 : 1;
        } else {
            trailingBits = _lo0bits(&highMantissa) + 32;
            limbs[0] = highMantissa;
            wordCount = b->wds = 1;
        }
    }
    if (exponent != 0) {
        *e = (exponent + trailingBits) - 0x433;
        *bitsOut = 0x35 - trailingBits;
    } else {
        *e = trailingBits - 0x432;
        *bitsOut = (wordCount << 5) - _hi0bits(limbs[wordCount - 1]);
    }
    return b;
}

f64 _ratio(_Bigint *a, _Bigint *b)
{
    libcDoubleBits_t numerator;
    libcDoubleBits_t denominator;
    s32 aExponent;
    s32 bExponent;
    s32 difference;

    numerator.value = _b2d(a, &aExponent);
    denominator.value = _b2d(b, &bExponent);
    difference = aExponent - bExponent + 32 * (a->wds - b->wds);
    if (difference > 0) {
        numerator.word[1] += difference * 0x100000U;
    } else {
        denominator.word[1] -= difference * 0x100000U;
    }
    return numerator.value / denominator.value;
}

double _mprec_log10(s32 dig) {
    double v = 1.0;

    if (dig < 24) {
        return pow10Table[dig];
    }
    while (dig > 0) {
        v *= 10.0;
        dig--;
    }
    return v;
}

long _read_r(_reent *r, s32 fd, char *buf, s32 n) {
    long result;

    libcErrno = 0;
    if ((result = read(fd, buf, n)) == -1 && libcErrno != 0) {
        r->_errno = libcErrno;
    }
    return result;
}

INCLUDE_ASM(const s32, "sdk/libc_after_vfprintf", isinf);

INCLUDE_ASM(const s32, "sdk/libc_after_vfprintf", isnan);

const s32 pow5MultTable[3] = {5, 25, 125};

const f64 pow10Table[25] = {
    1e0, 1e1, 1e2, 1e3, 1e4, 1e5, 1e6, 1e7, 1e8, 1e9,
    1e10, 1e11, 1e12, 1e13, 1e14, 1e15, 1e16, 1e17, 1e18, 1e19,
    1e20, 1e21, 1e22, 1e23, 1e24
};

const f64 D_003B8498[5] = {1e16, 1e32, 1e64, 1e128, 1e256};

const f64 D_003B84C0[5] = {1e-16, 1e-32, 1e-64, 1e-128, 1e-256};

INCLUDE_ASM(const s32, "sdk/libc_after_vfprintf", func_00308B68);

INCLUDE_ASM(const s32, "sdk/libc_after_vfprintf", func_00308BD8);

INCLUDE_ASM(const s32, "sdk/libc_after_vfprintf", func_00308BF8);

INCLUDE_ASM(const s32, "sdk/libc_after_vfprintf", func_00309A20);

double func_00309A80(const char *s, char **end) {
    return func_00308B68(_impure_ptr, s, end);
}

float func_00309AA8(const char *s, char **end) {
    return func_00309A80(s, end);
}

/* Historical newlib _strtoul_r: unsigned long is 64-bit on the target.
 * Preserve the legacy ctype input domain and caller-supplied base contract.
 * The early base exit uses the matched _strtol_r loop/finalization shape. */
unsigned long func_00309AD0(_reent *r, const char *input, char **end, s32 base)
{
    const char *s = input;
    unsigned long acc;
    s32 c;
    unsigned long cutoff;
    s32 negative = 0;
    s32 any;
    s32 cutlim;

    do {
        c = *s++;
    } while ((_ctype_ + 1)[c] & 8);
    if (c == '-') {
        negative = 1;
        c = *s++;
    } else if (c == '+') {
        c = *s++;
    }
    if ((base == 0 || base == 16) && c == '0' &&
        (*s == 'x' || *s == 'X')) {
        c = s[1];
        s += 2;
        base = 16;
    }
    if (base == 0)
        base = c == '0' ? 8 : 10;
    cutoff = ~0UL / (unsigned long)base;
    cutlim = ~0UL % (unsigned long)base;
    for (acc = 0, any = 0;; c = *s++) {
        if ((_ctype_ + 1)[c] & 4)
            c -= '0';
        else if ((_ctype_ + 1)[c] & 3)
            c -= ((_ctype_ + 1)[c] & 1) ? 'A' - 10 : 'a' - 10;
        else
            break;
        if (c >= base)
            goto done;
        if (any < 0 || acc > cutoff || (acc == cutoff && c > cutlim)) {
            any = -1;
        } else {
            any = 1;
            acc *= base;
            acc += c;
        }
    }
done:
    if (any < 0) {
        acc = ~0UL;
        r->_errno = 34;
    } else if (negative) {
        acc = -acc;
    }
    if (end != 0)
        *end = (char *)(any ? s - 1 : input);
    return acc;
}

unsigned long func_00309D00(const char *s, char **end, s32 base) {
    return func_00309AD0(_impure_ptr, s, end, base);
}

/* Newlib-family ungetc storage growth, using the stream's real reserve buffer. */
s32 func_00309D30(FILE *fp)
{
    s32 i;
    u8 *p;

    if (fp->_ub._base == fp->_ubuf) {
        if ((p = _malloc_r(fp->_data, 1024)) == NULL)
            return -1;
        fp->_ub._base = p;
        fp->_ub._size = 1024;
        p += 1024 - sizeof(fp->_ubuf);
        for (i = sizeof(fp->_ubuf); --i >= 0;)
            p[i] = fp->_ubuf[i];
        fp->_p = p;
        return 0;
    }
    i = fp->_ub._size;
    p = func_0030A058(fp->_data, fp->_ub._base, i << 1);
    if (p == NULL)
        return -1;
    memcpy(p + i, p, i);
    fp->_p = p + i;
    fp->_ub._base = p;
    fp->_ub._size = i << 1;
    return 0;
}

s32 func_00309E18(s32 c, FILE *fp)
{
    if (c == -1)
        return -1;
    /* Historical CHECK_INIT, also used by the matched stream functions. */
    do {
        if (fp->_data == NULL)
            fp->_data = _impure_ptr;
        if (!fp->_data->__sdidinit)
            __sinit(fp->_data);
    } while (0);
    fp->_flags &= ~0x20;
    if ((fp->_flags & 4) == 0) {
        if ((fp->_flags & 0x10) == 0)
            return -1;
        if (fp->_flags & 8) {
            if (fflush(fp))
                return -1;
            fp->_flags &= ~8;
            fp->_w = 0;
            fp->_lbfsize = 0;
        }
        fp->_flags |= 4;
    }
    c = (u8)c;
    if (fp->_ub._base != NULL) {
        if (fp->_r >= fp->_ub._size && func_00309D30(fp))
            return -1;
        *--fp->_p = c;
        fp->_r++;
        return c;
    }
    if (fp->_bf._base != NULL && fp->_p > fp->_bf._base && fp->_p[-1] == c) {
        fp->_p--;
        fp->_r++;
        return c;
    }
    fp->_ur = fp->_r;
    fp->_up = fp->_p;
    fp->_ub._base = fp->_ubuf;
    fp->_ub._size = sizeof(fp->_ubuf);
    fp->_ubuf[sizeof(fp->_ubuf) - 1] = c;
    fp->_p = &fp->_ubuf[sizeof(fp->_ubuf) - 1];
    fp->_r = 1;
    return c;
}

/* newlib mallocr.c: allocated chunks borrow the next chunk's previousSize
 * word while its PREV_INUSE bit is set. The usable extent is size - 4,
 * not size - 8. The historical MALLOC_ZERO macro handles 3/5/7/9 words;
 * this allocator rounds chunks to 16 bytes (minimum 16), so fresh small
 * chunks use 3 or 7 words. Preserve the historical one-pass macro block.
 * This is fresh allocator payload, not traversal of a typed record. */
void *_calloc_r(_reent *r, u32 n, u32 size) {
    mallocChunkPrefix_t *chunk;
    u32 chunkSize;
    u32 bytes = n * size;
    void *memory = _malloc_r(r, bytes);

    if (memory == NULL) {
        return NULL;
    } else {
        chunk = (mallocChunkPrefix_t *)((u8 *)memory - 8);
        chunkSize = chunk->size & ~3U;
        do {
            u32 clearSize = chunkSize - sizeof(u32);
            if (clearSize <= 9 * sizeof(clearSize)) {
                u32 *words = (u32 *)memory;
                if (clearSize >= 5 * sizeof(clearSize)) {
                    *words++ = 0;
                    *words++ = 0;
                    if (clearSize >= 7 * sizeof(clearSize)) {
                        *words++ = 0;
                        *words++ = 0;
                        if (clearSize >= 9 * sizeof(clearSize)) {
                            *words++ = 0;
                            *words++ = 0;
                        }
                    }
                }
                *words++ = 0;
                *words++ = 0;
                *words = 0;
            } else {
                memset(memory, 0, clearSize);
            }
        } while (0);
        return memory;
    }
}

/* Newlib 1.10 mallocr.c realloc with target-selected 16-byte alignment,
 * 32-bit INTERNAL_SIZE_T, and no mmap/zero-size-free paths. The size_t-to-long
 * comparisons intentionally retain the historical target's 64-bit long.
 * MALLOC_COPY's real do/while blocks copy allocator payload, not chunk fields:
 * the payload starts eight bytes after the typed header and may borrow the
 * next chunk's previousSize word while PREV_INUSE is set. All header accesses
 * use mallocChunkPrefix_t. The ascending copy supports backward coalescing.
 * Historical source: mirror/newlib-cygwin, newlib-1_10_0,
 * newlib/libc/stdlib/mallocr.c, realloc and MALLOC_COPY.
 */
void *func_0030A058(_reent *r, void *oldmem, size_t bytes)
{
    u32 nb;
    mallocChunkPrefix_t * oldp;
    u32 oldsize;
    mallocChunkPrefix_t * newp;
    u32 newsize;
    void* newmem;
    mallocChunkPrefix_t * next;
    u32 nextsize;
    mallocChunkPrefix_t * prev;
    u32 prevsize;
    mallocChunkPrefix_t * remainder;
    u32 remainder_size;
    mallocChunkPrefix_t * bck;
    mallocChunkPrefix_t * fwd;
    if (oldmem == 0) return _malloc_r(r, bytes);
    __malloc_lock(r);
    newp = oldp = ((mallocChunkPrefix_t *)((char*)(oldmem) - 2*4U));
    newsize = oldsize = ((oldp)->size & ~(3U));
    nb = (((long)((bytes) + (4U + 15U)) < (long)(16U + 15U)) ? ((16U + 15U) & ~(15U)) : (((bytes) + (4U + 15U)) & ~(15U)));
    ;
    if ((long)(oldsize) < (long)(nb))
    {
        next = ((mallocChunkPrefix_t *)(((char*)(oldp)) + (oldsize)));
        if (next == __malloc_av_.top || !((((mallocChunkPrefix_t *)(((char*)(next))+((next)->size & ~1U)))->size) & 1U))
        {
            nextsize = ((next)->size & ~(3U));
            if (next == __malloc_av_.top)
            {
                if ((long)(nextsize + newsize) >= (long)(nb + 16U))
                {
                    newsize += nextsize;
                    __malloc_av_.top = ((mallocChunkPrefix_t *)(((char*)(oldp)) + (nb)));
                    ((__malloc_av_.top)->size = ((newsize - nb) | 1U));
                    ((oldp)->size = (((oldp)->size & 1U) | (nb)));
                    __malloc_unlock(r);
                    return ((void*)((char*)(oldp) + 2*4U));
                }
            }
            else if (((long)(nextsize + newsize) >= (long)(nb)))
            {
                {
                    bck = next->backward;
                    fwd = next->forward;
                    fwd->backward = bck;
                    bck->forward = fwd;
                }
                ;
                newsize += nextsize;
                goto split;
            }
        }
        else
        {
            next = 0;
            nextsize = 0;
        }
        if (!((oldp)->size & 1U))
        {
            prev = ((mallocChunkPrefix_t *)( ((char*)(oldp)) - ((oldp)->previousSize) ));
            prevsize = ((prev)->size & ~(3U));
            if (next != 0)
            {
                if (next == __malloc_av_.top)
                {
                    if ((long)(nextsize + prevsize + newsize) >= (long)(nb + 16U))
                    {
                        {
                            bck = prev->backward;
                            fwd = prev->forward;
                            fwd->backward = bck;
                            bck->forward = fwd;
                        }
                        ;
                        newp = prev;
                        newsize += prevsize + nextsize;
                        newmem = ((void*)((char*)(newp) + 2*4U));
                        do {
                            u32 mcsz = (oldsize - 4U);
                            if(mcsz <= 9*sizeof(mcsz)) {
                                u32* mcsrc = (u32*) (oldmem);
                                u32* mcdst = (u32*) (newmem);
                                if(mcsz >= 5*sizeof(mcsz)) {
                                    *mcdst++ = *mcsrc++;
                                    *mcdst++ = *mcsrc++;
                                    if(mcsz >= 7*sizeof(mcsz)) {
                                        *mcdst++ = *mcsrc++;
                                        *mcdst++ = *mcsrc++;
                                        if(mcsz >= 9*sizeof(mcsz)) {
                                            *mcdst++ = *mcsrc++;
                                            *mcdst++ = *mcsrc++;
                                        }
                                    }
                                }
                                *mcdst++ = *mcsrc++;
                                *mcdst++ = *mcsrc++;
                                *mcdst = *mcsrc ;
                            }
                            else memcpy(newmem, oldmem, mcsz);
                        }
                        while(0);
                        __malloc_av_.top = ((mallocChunkPrefix_t *)(((char*)(newp)) + (nb)));
                        ((__malloc_av_.top)->size = ((newsize - nb) | 1U));
                        ((newp)->size = (((newp)->size & 1U) | (nb)));
                        __malloc_unlock(r);
                        return newmem;
                    }
                }
                else if (((long)(nextsize + prevsize + newsize) >= (long)(nb)))
                {
                    {
                        bck = next->backward;
                        fwd = next->forward;
                        fwd->backward = bck;
                        bck->forward = fwd;
                    }
                    ;
                    {
                        bck = prev->backward;
                        fwd = prev->forward;
                        fwd->backward = bck;
                        bck->forward = fwd;
                    }
                    ;
                    newp = prev;
                    newsize += nextsize + prevsize;
                    newmem = ((void*)((char*)(newp) + 2*4U));
                    do {
                        u32 mcsz = (oldsize - 4U);
                        if(mcsz <= 9*sizeof(mcsz)) {
                            u32* mcsrc = (u32*) (oldmem);
                            u32* mcdst = (u32*) (newmem);
                            if(mcsz >= 5*sizeof(mcsz)) {
                                *mcdst++ = *mcsrc++;
                                *mcdst++ = *mcsrc++;
                                if(mcsz >= 7*sizeof(mcsz)) {
                                    *mcdst++ = *mcsrc++;
                                    *mcdst++ = *mcsrc++;
                                    if(mcsz >= 9*sizeof(mcsz)) {
                                        *mcdst++ = *mcsrc++;
                                        *mcdst++ = *mcsrc++;
                                    }
                                }
                            }
                            *mcdst++ = *mcsrc++;
                            *mcdst++ = *mcsrc++;
                            *mcdst = *mcsrc ;
                        }
                        else memcpy(newmem, oldmem, mcsz);
                    }
                    while(0);
                    goto split;
                }
            }
            if (prev != 0 && (long)(prevsize + newsize) >= (long)nb)
            {
                {
                    bck = prev->backward;
                    fwd = prev->forward;
                    fwd->backward = bck;
                    bck->forward = fwd;
                }
                ;
                newp = prev;
                newsize += prevsize;
                newmem = ((void*)((char*)(newp) + 2*4U));
                do {
                    u32 mcsz = (oldsize - 4U);
                    if(mcsz <= 9*sizeof(mcsz)) {
                        u32* mcsrc = (u32*) (oldmem);
                        u32* mcdst = (u32*) (newmem);
                        if(mcsz >= 5*sizeof(mcsz)) {
                            *mcdst++ = *mcsrc++;
                            *mcdst++ = *mcsrc++;
                            if(mcsz >= 7*sizeof(mcsz)) {
                                *mcdst++ = *mcsrc++;
                                *mcdst++ = *mcsrc++;
                                if(mcsz >= 9*sizeof(mcsz)) {
                                    *mcdst++ = *mcsrc++;
                                    *mcdst++ = *mcsrc++;
                                }
                            }
                        }
                        *mcdst++ = *mcsrc++;
                        *mcdst++ = *mcsrc++;
                        *mcdst = *mcsrc ;
                    }
                    else memcpy(newmem, oldmem, mcsz);
                }
                while(0);
                goto split;
            }
        }
        newmem = _malloc_r (r, bytes);
        if (newmem == 0)
        {
            __malloc_unlock(r);
            return 0;
        }
        if ( (newp = ((mallocChunkPrefix_t *)((char*)(newmem) - 2*4U))) == ((mallocChunkPrefix_t *)( ((char*)(oldp)) + ((oldp)->size & ~1U) )))
        {
            newsize += ((newp)->size & ~(3U));
            newp = oldp;
            goto split;
        }
        do {
            u32 mcsz = (oldsize - 4U);
            if(mcsz <= 9*sizeof(mcsz)) {
                u32* mcsrc = (u32*) (oldmem);
                u32* mcdst = (u32*) (newmem);
                if(mcsz >= 5*sizeof(mcsz)) {
                    *mcdst++ = *mcsrc++;
                    *mcdst++ = *mcsrc++;
                    if(mcsz >= 7*sizeof(mcsz)) {
                        *mcdst++ = *mcsrc++;
                        *mcdst++ = *mcsrc++;
                        if(mcsz >= 9*sizeof(mcsz)) {
                            *mcdst++ = *mcsrc++;
                            *mcdst++ = *mcsrc++;
                        }
                    }
                }
                *mcdst++ = *mcsrc++;
                *mcdst++ = *mcsrc++;
                *mcdst = *mcsrc ;
            }
            else memcpy(newmem, oldmem, mcsz);
        }
        while(0);
        _free_r(r, oldmem);
        __malloc_unlock(r);
        return newmem;
    }
    split:
    remainder_size = ((long)((newsize)-(nb)));
    if (remainder_size >= (long)16U)
    {
        remainder = ((mallocChunkPrefix_t *)(((char*)(newp)) + (nb)));
        ((newp)->size = (((newp)->size & 1U) | (nb)));
        ((remainder)->size = (remainder_size | 1U));
        (((mallocChunkPrefix_t *)(((char*)(remainder)) + (remainder_size)))->size |= 1U);
        _free_r(r, ((void*)((char*)(remainder) + 2*4U)));
    }
    else
    {
        ((newp)->size = (((newp)->size & 1U) | (newsize)));
        (((mallocChunkPrefix_t *)(((char*)(newp)) + (newsize)))->size |= 1U);
    }
    ;
    __malloc_unlock(r);
    return ((void*)((char*)(newp) + 2*4U));
}

INCLUDE_RODATA(const s32, "sdk/libc_after_vfprintf", D_003B85A8);

INCLUDE_RODATA(const s32, "sdk/libc_after_vfprintf", D_003B85B0);

INCLUDE_RODATA(const s32, "sdk/libc_after_vfprintf", D_003B85B8);

INCLUDE_RODATA(const s32, "sdk/libc_after_vfprintf", D_003B85C0);

INCLUDE_RODATA(const s32, "sdk/libc_after_vfprintf", kernlInternalErrorMsg);

INCLUDE_RODATA(const s32, "sdk/libc_after_vfprintf", ttyMsgPacketTooLarge);

INCLUDE_RODATA(const s32, "sdk/libc_after_vfprintf", ttyMsgReceiveError);

INCLUDE_RODATA(const s32, "sdk/libc_after_vfprintf", ttyMsgSendError);

INCLUDE_RODATA(const s32, "sdk/libc_after_vfprintf", ttyMsgWlenError);

