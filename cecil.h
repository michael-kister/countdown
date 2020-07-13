

#define __NCOL__ 33

#define __CLEAR__ for (i = 0; i < __NCOL__; ++i) { printf(" "); }

#define __C_ROW__(x,y) { printf("\033[" x #y ";0H"); __CLEAR__; }

#define __C_PAR__(x) {					\
	__C_ROW__( #x , 0); __C_ROW__( #x , 1);		\
	__C_ROW__( #x , 2); __C_ROW__( #x , 3);		\
	__C_ROW__( #x , 4); __C_ROW__( #x , 5);		\
	__C_ROW__( #x , 6); __C_ROW__( #x , 7);		\
	__C_ROW__( #x , 8); __C_ROW__( #x , 9); }   
    
#define __C_PAG__ { __C_PAR__(0); __C_PAR__(1) }

#define LINE(x) { printf("\033[" #x ";0H"); }



#define _t2 0x1
#define _m1 0x2
#define _m2 0x4
#define _m3 0x8
#define _b1 0x10
#define _b2 0x20
#define _b3 0x40

#define __s0 (_t2+_m1+_m3+_b1+_b2+_b3)
#define __s1 (_m3+_b3)
#define __s2 (_t2+_m2+_m3+_b1+_b2)
#define __s3 (_t2+_m2+_m3+_b2+_b3)
#define __s4 (_m1+_m2+_m3+_b3)
#define __s5 (_t2+_m1+_m2+_b2+_b3)
#define __s6 (_t2+_m1+_m2+_b1+_b2+_b3)
#define __s7 (_t2+_m3+_b3)
#define __s8 (_t2+_m1+_m2+_m3+_b1+_b2+_b3)
#define __s9 (_t2+_m1+_m2+_m3+_b3)

int __S[] = {__s0, __s1, __s2, __s3, __s4,
	     __s5, __s6, __s7, __s8, __s9};

void print_top(int n) {
    printf(" ");
    if (_t2 & __S[n])
	printf("_");
    else
	printf(" ");
    printf(" ");
}

void print_mid(int n) {
    if (_m1 & __S[n])
	printf("|");
    else
	printf(" ");
    if (_m2 & __S[n])
	printf("_");
    else
	printf(" ");
    if (_m3 & __S[n])
	printf("|");
    else
	printf(" ");
}

void print_bot(int n) {
    if (_b1 & __S[n])
	printf("|");
    else
	printf(" ");
    if (_b2 & __S[n])
	printf("_");
    else
	printf(" ");
    if (_b3 & __S[n])
	printf("|");
    else
	printf(" ");
}

void print_number(int h, int t, int o) {

    LINE(1);
    printf("      ----------------  \n");
    printf("     |  ");
    print_top(h); printf(" ");
    print_top(t); printf(" ");
    print_top(o); printf("   |\n");
    
    printf("     |  ");
    print_mid(h); printf(" ");
    print_mid(t); printf(" ");
    print_mid(o); printf("   |\n");
    
    printf("     |  ");
    print_bot(h); printf(" ");
    print_bot(t); printf(" ");
    print_bot(o); printf("   |\n");
    printf("     |                | \n");
    printf("      ----------------  \n");
    
    
}

int cecil(void) {
    LINE(13);
    printf("And your target is...");
    int hhh,ttt,ooo,i;
    for (i = 0; i < 20; ++i) {
	hhh = rand() % 9 + 1;
	ttt = rand() % 10;
	ooo = rand() % 10;
	print_number(hhh,ttt,ooo);
	usleep(100000);
    }
    return 100*hhh + 10*tt + 1*ooo;
}
