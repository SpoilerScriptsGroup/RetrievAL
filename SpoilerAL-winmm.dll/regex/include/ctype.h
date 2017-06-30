#ifndef _CTYPE_H
#define _CTYPE_H

#define isascii(c)  ((c) >= 0 && (c) <= CHAR_MAX)
#define isprint(c)  ((c) >= ' ' && (c) <= '\x7E')
#define isspace(c)  ((c) == ' ' || ((c) >= '\t' && (c) <= '\r'))
#define isupper(c)  ((c) >= 'A' && (c) <= 'Z')
#define islower(c)  ((c) >= 'a' && (c) <= 'z')
#define isalpha(c)  (isupper(c) || islower(c))
#define isdigit(c)  ((c) >= '0' && (c) <= '9')
#define isalnum(c)  (isalpha(c) || isdigit(c))
#define isxdigit(c) (isdigit(c) || ((c) >= 'A' && (c) <= 'F') || ((c) >= 'a' && (c) <= 'f'))
#define iscntrl(c)  (((c) >= '\0' && (c) <= '\x1F') || (c) == '\x7F')
#define isgraph(c)  ((c) >= '\x21' && (c) <= '\x7E')
#define ispunct(c)  (((c) >= '\x21' && (c) <= '\x2F') || ((c) >= '\x3A' && (c) <= '\x40') || ((c) >= '\x5B' && (c) <= '\x60') || ((c) >= '\x7B' && (c) <= '\x7E'))
#define iscsymf(c)  (isupper(c) || (c) == '_' || islower(c))
#define iscsym(c)   (isdigit(c) || iscsymf(c))
#define tolower(c)  (isupper(c) ? ((c) + ('a' - 'A')) : (c))
#define toupper(c)  (islower(c) ? ((c) - ('a' - 'A')) : (c))

#endif
