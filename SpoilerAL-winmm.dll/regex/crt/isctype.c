#include <ctype.h>

static const unsigned char __ctype[128] = {
	_CONTROL,          // 0x00 '\0'
	_CONTROL,          // 0x01
	_CONTROL,          // 0x02
	_CONTROL,          // 0x03
	_CONTROL,          // 0x04
	_CONTROL,          // 0x05
	_CONTROL,          // 0x06
	_CONTROL,          // 0x07 '\a'
	_CONTROL,          // 0x08 '\b'
	_CONTROL | _SPACE, // 0x09 '\t'
	_CONTROL | _SPACE, // 0x0A '\n'
	_CONTROL | _SPACE, // 0x0B '\v'
	_CONTROL | _SPACE, // 0x0C '\f'
	_CONTROL | _SPACE, // 0x0D '\r'
	_CONTROL,          // 0x0E
	_CONTROL,          // 0x0F
	_CONTROL,          // 0x10
	_CONTROL,          // 0x11
	_CONTROL,          // 0x12
	_CONTROL,          // 0x13
	_CONTROL,          // 0x14
	_CONTROL,          // 0x15
	_CONTROL,          // 0x16
	_CONTROL,          // 0x17
	_CONTROL,          // 0x18
	_CONTROL,          // 0x19
	_CONTROL,          // 0x1A
	_CONTROL,          // 0x1B
	_CONTROL,          // 0x1C
	_CONTROL,          // 0x1D
	_CONTROL,          // 0x1E
	_CONTROL,          // 0x1F
	_SPACE | _BLANK,   // 0x20 ' '
	_PUNCT,            // 0x21 '!'
	_PUNCT,            // 0x22 '\"'
	_PUNCT,            // 0x23 '#'
	_PUNCT,            // 0x24 '$'
	_PUNCT,            // 0x25 '%'
	_PUNCT,            // 0x26 '&'
	_PUNCT,            // 0x27 '\''
	_PUNCT,            // 0x28 '('
	_PUNCT,            // 0x29 ')'
	_PUNCT,            // 0x2A '*'
	_PUNCT,            // 0x2B '+'
	_PUNCT,            // 0x2C ','
	_PUNCT,            // 0x2D '-'
	_PUNCT,            // 0x2E '.'
	_PUNCT,            // 0x2F '/'
	_DIGIT | _HEX,     // 0x30 '0'
	_DIGIT | _HEX,     // 0x31 '1'
	_DIGIT | _HEX,     // 0x32 '2'
	_DIGIT | _HEX,     // 0x33 '3'
	_DIGIT | _HEX,     // 0x34 '4'
	_DIGIT | _HEX,     // 0x35 '5'
	_DIGIT | _HEX,     // 0x36 '6'
	_DIGIT | _HEX,     // 0x37 '7'
	_DIGIT | _HEX,     // 0x38 '8'
	_DIGIT | _HEX,     // 0x39 '9'
	_PUNCT,            // 0x3A ':'
	_PUNCT,            // 0x3B ';'
	_PUNCT,            // 0x3C '<'
	_PUNCT,            // 0x3D '='
	_PUNCT,            // 0x3E '>'
	_PUNCT,            // 0x3F '?'
	_PUNCT,            // 0x40 '@'
	_UPPER | _HEX,     // 0x41 'A'
	_UPPER | _HEX,     // 0x42 'B'
	_UPPER | _HEX,     // 0x43 'C'
	_UPPER | _HEX,     // 0x44 'D'
	_UPPER | _HEX,     // 0x45 'E'
	_UPPER | _HEX,     // 0x46 'F'
	_UPPER,            // 0x47 'G'
	_UPPER,            // 0x48 'H'
	_UPPER,            // 0x49 'I'
	_UPPER,            // 0x4A 'J'
	_UPPER,            // 0x4B 'K'
	_UPPER,            // 0x4C 'L'
	_UPPER,            // 0x4D 'M'
	_UPPER,            // 0x4E 'N'
	_UPPER,            // 0x4F 'O'
	_UPPER,            // 0x50 'P'
	_UPPER,            // 0x51 'Q'
	_UPPER,            // 0x52 'R'
	_UPPER,            // 0x53 'S'
	_UPPER,            // 0x54 'T'
	_UPPER,            // 0x55 'U'
	_UPPER,            // 0x56 'V'
	_UPPER,            // 0x57 'W'
	_UPPER,            // 0x58 'X'
	_UPPER,            // 0x59 'Y'
	_UPPER,            // 0x5A 'Z'
	_PUNCT,            // 0x5B '['
	_PUNCT,            // 0x5C '\\'
	_PUNCT,            // 0x5D ']'
	_PUNCT,            // 0x5E '^'
	_PUNCT,            // 0x5F '_'
	_PUNCT,            // 0x60 '`'
	_LOWER | _HEX,     // 0x61 'a'
	_LOWER | _HEX,     // 0x62 'b'
	_LOWER | _HEX,     // 0x63 'c'
	_LOWER | _HEX,     // 0x64 'd'
	_LOWER | _HEX,     // 0x65 'e'
	_LOWER | _HEX,     // 0x66 'f'
	_LOWER,            // 0x67 'g'
	_LOWER,            // 0x68 'h'
	_LOWER,            // 0x69 'i'
	_LOWER,            // 0x6A 'j'
	_LOWER,            // 0x6B 'k'
	_LOWER,            // 0x6C 'l'
	_LOWER,            // 0x6D 'm'
	_LOWER,            // 0x6E 'n'
	_LOWER,            // 0x6F 'o'
	_LOWER,            // 0x70 'p'
	_LOWER,            // 0x71 'q'
	_LOWER,            // 0x72 'r'
	_LOWER,            // 0x73 's'
	_LOWER,            // 0x74 't'
	_LOWER,            // 0x75 'u'
	_LOWER,            // 0x76 'v'
	_LOWER,            // 0x77 'w'
	_LOWER,            // 0x78 'x'
	_LOWER,            // 0x79 'y'
	_LOWER,            // 0x7A 'z'
	_PUNCT,            // 0x7B '{'
	_PUNCT,            // 0x7C '|'
	_PUNCT,            // 0x7D '}'
	_PUNCT,            // 0x7E '~'
	_CONTROL,          // 0x7F
};

int __cdecl _isctype(int c, int desc)
{
	return (unsigned int)c < 0x80 && (__ctype[c] & desc);
}
