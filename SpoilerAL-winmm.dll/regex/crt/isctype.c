#include <ctype.h>

static const unsigned char __ctype[256] = {
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
	_PUNCT,            // 0x22 '"'
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
	0,                 // 0x80
	0,                 // 0x81
	0,                 // 0x82
	0,                 // 0x83
	0,                 // 0x84
	0,                 // 0x85
	0,                 // 0x86
	0,                 // 0x87
	0,                 // 0x88
	0,                 // 0x89
	0,                 // 0x8A
	0,                 // 0x8B
	0,                 // 0x8C
	0,                 // 0x8D
	0,                 // 0x8E
	0,                 // 0x8F
	0,                 // 0x90
	0,                 // 0x91
	0,                 // 0x92
	0,                 // 0x93
	0,                 // 0x94
	0,                 // 0x95
	0,                 // 0x96
	0,                 // 0x97
	0,                 // 0x98
	0,                 // 0x99
	0,                 // 0x9A
	0,                 // 0x9B
	0,                 // 0x9C
	0,                 // 0x9D
	0,                 // 0x9E
	0,                 // 0x9F
	0,                 // 0xA0
	0,                 // 0xA1
	0,                 // 0xA2
	0,                 // 0xA3
	0,                 // 0xA4
	0,                 // 0xA5
	0,                 // 0xA6
	0,                 // 0xA7
	0,                 // 0xA8
	0,                 // 0xA9
	0,                 // 0xAA
	0,                 // 0xAB
	0,                 // 0xAC
	0,                 // 0xAD
	0,                 // 0xAE
	0,                 // 0xAF
	0,                 // 0xB0
	0,                 // 0xB1
	0,                 // 0xB2
	0,                 // 0xB3
	0,                 // 0xB4
	0,                 // 0xB5
	0,                 // 0xB6
	0,                 // 0xB7
	0,                 // 0xB8
	0,                 // 0xB9
	0,                 // 0xBA
	0,                 // 0xBB
	0,                 // 0xBC
	0,                 // 0xBD
	0,                 // 0xBE
	0,                 // 0xBF
	0,                 // 0xC0
	0,                 // 0xC1
	0,                 // 0xC2
	0,                 // 0xC3
	0,                 // 0xC4
	0,                 // 0xC5
	0,                 // 0xC6
	0,                 // 0xC7
	0,                 // 0xC8
	0,                 // 0xC9
	0,                 // 0xCA
	0,                 // 0xCB
	0,                 // 0xCC
	0,                 // 0xCD
	0,                 // 0xCE
	0,                 // 0xCF
	0,                 // 0xD0
	0,                 // 0xD1
	0,                 // 0xD2
	0,                 // 0xD3
	0,                 // 0xD4
	0,                 // 0xD5
	0,                 // 0xD6
	0,                 // 0xD7
	0,                 // 0xD8
	0,                 // 0xD9
	0,                 // 0xDA
	0,                 // 0xDB
	0,                 // 0xDC
	0,                 // 0xDD
	0,                 // 0xDE
	0,                 // 0xDF
	0,                 // 0xE0
	0,                 // 0xE1
	0,                 // 0xE2
	0,                 // 0xE3
	0,                 // 0xE4
	0,                 // 0xE5
	0,                 // 0xE6
	0,                 // 0xE7
	0,                 // 0xE8
	0,                 // 0xE9
	0,                 // 0xEA
	0,                 // 0xEB
	0,                 // 0xEC
	0,                 // 0xED
	0,                 // 0xEE
	0,                 // 0xEF
	0,                 // 0xF0
	0,                 // 0xF1
	0,                 // 0xF2
	0,                 // 0xF3
	0,                 // 0xF4
	0,                 // 0xF5
	0,                 // 0xF6
	0,                 // 0xF7
	0,                 // 0xF8
	0,                 // 0xF9
	0,                 // 0xFA
	0,                 // 0xFB
	0,                 // 0xFC
	0,                 // 0xFD
	0,                 // 0xFE
	0,                 // 0xFF
};

int __cdecl _isctype(int c, int desc)
{
	return (unsigned int)c < 256 && (__ctype[c] & desc);
}
