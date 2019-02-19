/*  octal converter				*/
/*  Input decimal, octal or binary numbers	*/
/*  Output decimal, octal and binary		*/
/*  Decimal output is signed and unsigned word  */
/*  Octal and binary output up to 22 bit	*/
/*  For syntax type octal without arguments	*/
/*						*/
/*  rricharz 2019 for 2.11BSD Unix on PiDP-11	*/
/*  Uses Berkley C syntax!			*/

long scanstr(str)
char* str;
{
	int i = 0;
	int base = 10;
	int topdigit = '9';	
	int negative = 0;
	long result = 0;
	while (str[i]== ' ') {
		i++;
        }
	if (str[i]=='+')
		i++;
	else if (str[i] == '-') {
		negative = 1;
		i++;
        }
	else if (str[i] == 'o') {
		base = 8;
		topdigit = '7';
		i++;
	}
	else if (str[i] == 'b') {
		base = 2;
		topdigit = '1';
		i++;
	}
	while ((str[i]>='0') && (str[i]<=topdigit)) {
		result = base * result + str[i++] -'0';
        }
	if (str[i] != 0) {
		printf("Illegal input char %c\n", str[i]);
		exit(1);
	}
	if (negative)
		result = -result;
	return result;
}

void printOctal(r)
long r;
{
	long mask = 0x200000;
	long m;

	int n;
	if (r & mask)
		printf("1");
	else
		printf("0");
	mask = 0x1c0000;
	while (mask!=0) {
		n = (r & mask) / (mask / 7);
		printf("   %d", n);
		mask = mask / 8;
	}
}

void printBinary(r)
long r;
{
	long mask = 0x200000;
	while (mask !=0) {
          	if (r & mask)
			printf("1");
		else
			printf("0");
		mask = mask / 2;
		if (mask == 0x100000) printf(" ");
		if (mask == 0x20000) printf(" ");
		if (mask == 0x4000) printf(" ");
		if (mask == 0x800) printf(" ");
		if (mask == 0x100) printf(" ");
		if (mask == 0x20) printf(" ");
		if (mask == 0x4) printf(" ");
	}
}

int main(argc, argv)
int argc;
char *argv[];
{
	long r;

	if (argc != 2) {
		printf("Examples:\n");
		printf("octal 255\n");
		printf("octal -99\n");
		printf("octal o2777\n");
		printf("octal b1011101\n");
		exit(1);
	}

  	r = scanstr(argv[1]);
	printf("16-bit integer:\n");
	if ((r<=32767) && (r>=-32768))
		printf("  signed decimal:   %d\n", (int)r);
        else
		printf("  signed decimal:   outside range!\n");
        if ((r<=65535) && (r>=0))
		printf("  unsigned decimal: %ld\n", r);
	else
		printf("  unsigned decimal: outside range!\n");
        printf("octal:   ");
	printOctal(r);
	printf("\n");
        printf("binary:  ");
	printBinary(r);
	printf("\n");
	exit(0);
}
