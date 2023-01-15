
/********************************************************
*  declare SFR union                                    *
********************************************************/
struct	bit_def_byte {
	unsigned char	b0:1;
	unsigned char	b1:1;
	unsigned char	b2:1;
	unsigned char	b3:1;
	unsigned char	b4:1;
	unsigned char	b5:1;
	unsigned char	b6:1;
	unsigned char	b7:1;
};

struct	bit_def_word {
	unsigned char	b0:1;
	unsigned char	b1:1;
	unsigned char	b2:1;
	unsigned char	b3:1;
	unsigned char	b4:1;
	unsigned char	b5:1;
	unsigned char	b6:1;
	unsigned char	b7:1;

	unsigned char	b8:1;
	unsigned char	b9:1;
	unsigned char	b10:1;
	unsigned char	b11:1;
	unsigned char	b12:1;
	unsigned char	b13:1;
	unsigned char	b14:1;
	unsigned char	b15:1;
};

struct	bit_def_dword {
	unsigned char	b0:1;
	unsigned char	b1:1;
	unsigned char	b2:1;
	unsigned char	b3:1;
	unsigned char	b4:1;
	unsigned char	b5:1;
	unsigned char	b6:1;
	unsigned char	b7:1;

	unsigned char	b8:1;
	unsigned char	b9:1;
	unsigned char	b10:1;
	unsigned char	b11:1;
	unsigned char	b12:1;
	unsigned char	b13:1;
	unsigned char	b14:1;
	unsigned char	b15:1;

	unsigned char	b16:1;
	unsigned char	b17:1;
	unsigned char	b18:1;
	unsigned char	b19:1;
	unsigned char	b20:1;
	unsigned char	b21:1;
	unsigned char	b22:1;
	unsigned char	b23:1;

	unsigned char	b24:1;
	unsigned char	b25:1;
	unsigned char	b26:1;
	unsigned char	b27:1;
	unsigned char	b28:1;
	unsigned char	b29:1;
	unsigned char	b30:1;
	unsigned char	b31:1;
};

union	byte_def{
	struct	bit_def_byte bit;
	unsigned char	byte;
};

union	word_def{
	struct	bit_def_word bit;
	struct{
		unsigned char	low;					/* low  8 bit */
		unsigned char	high;					/* high 8 bit */
	} byte;
	unsigned short	word;
};

union	dword_def{
	struct	bit_def_dword bit;
	struct{
		unsigned char	low;					/* low  8 bit */
		unsigned char	mid;					/* mid  8 bit */
		unsigned char	high;					/* high 8 bit */
		unsigned char	up;					/* up	8 bit */
	} byte;
	unsigned long	dword;
};


