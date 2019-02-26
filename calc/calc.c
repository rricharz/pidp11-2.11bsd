/*
   CALC for 2.11BSD on PiDP-11

   Written 2019 by rricharz  s an example
   of a vintage Berkeley C program

   The following operators are allowed:

     +,-       prefix for decimal input
     +,-,*,/   arithmetic operators
     ()        brackets
     |,&       bitwise operators
     <<,>>     bitwise shift operators

     $XX..     hex numeric input
     %XX..     binary numeric input
*/

#include <limits.h>

#define EOL	'\n'

char ch;
double r;
int stop,dotused;

/*********/
int fix(rf)
double rf;
/*********/
{
  if (rf>32767.5)
    {
      printf("Integer value exceeds upper limit, set to 32767\n");
      return 32767;
    }
  else if (rf<-32768.5)
    {
      printf("Integer value exceeds lower limit, set to -32768");
      return -32768;
    }
  else
    return (int)rf;
}

/**************/
void checkfor(c)
char c;
/**************/
{
  if (ch != c)
    {
      stop=0;
      printf("SYNTAX ERROR: ");

      if (ch==EOL) printf("<eol>");
      else if (ch=32) printf("ascii(%d)",ch);
      else printf("%c",ch);

      printf(" not expected\n");

      while (ch != EOL)
        ch=getchar();
    }
}

/**************/
int isnumber(cn)
char cn;
/**************/
{
  return (cn >= '0') && (cn <= '9');
}

/***********************/
void print_binary(number)
unsigned int number;
/***********************/
{
  if (number)
    {
      /* recursive call for each digit */
      print_binary(number >> 1);
      if (number & 1) printf("1");
      else printf("0");
    }
}

/*****************/
void printresult(r)
double r;
/*****************/
{
  long d;
  int i;
  printf("double: %f   ",r);
  if ((r>=LONG_MIN) && (r<=LONG_MAX))
    {
      d=(long)r;
      printf("long: $%08X ",d);
      printf("o%011O",d);
      if ((r>=INT_MIN) && (r<=INT_MAX))
        {
          i=(int)d;
          printf("   int: $%04x ",i);
          printf("o%06o %%",i);
          if (i==0) printf("0");
          else print_binary(i);
        }
    }
  else
    printf("outside long integer range");
}

/***************/
double express();
/***************/

/**********/
int binval()
/**********/
{
  if (ch=='0') return 0;
  else if (ch=='1') return 1;
  else return -1;
}

/********/
int oval()
/********/
{
  if ((ch>='0') && (ch<='7'))
    return (ch-'0');
  else return -1;
}

/**********/
int hexval()
/**********/
{
  if ((ch>='0') && (ch<='9'))
    return (ch-'0');
  else if ((ch>='a') && (ch<='f'))
    return (ch-'a'+10);
  else return (-1);
}

/*************/
double factor()
/*************/
{
  int negative;
  double rf,rt;
  int i,iv;
  negative=0;
  rf=0.;
  ch=getchar();
  if (ch=='(')
    {
      stop=0;
      rf=express();
      checkfor(')');
      ch=getchar();
    }
  else if (ch=='%')
    {
      stop=0;
      ch=getchar();
      iv=0;
      while (binval()>=0)
        { 
          iv = (iv << 1)+binval();
          ch=getchar();
        }
      rf=(double)iv;
    }
    else if (ch=='o')
    {
      stop=0;
      ch=getchar();
      iv=0;
      while (oval()>=0)
        { 
          iv = (iv << 3)+oval();
          ch=getchar();
        }
      rf=(double)iv;
    }
  else if (ch=='$')
    {
      stop=0;
      ch=getchar();
      iv=0;
      while (hexval()>=0)
        {
          iv=(iv << 4)+hexval();
          ch=getchar();
        }
      rf=(double)iv;
    }
 else 
    {
      if (ch != EOL )
        {
          stop=0;
          if (ch=='+') ch=getchar();
          if (ch=='-')
            { 
              negative=1;
              ch=getchar();
            };
          while (isnumber(ch))
            {           
              rt=rf+rf;
              rt=rt+rt;
              rf=rt+rt+rf+rf+
               (double)(ch-'0');
              ch=getchar();
            };
          if (ch=='.')
            {
              dotused=1;
              ch=getchar();
              rt=0.1;
              while (isnumber(ch))
                {
                  rf=rf+(double)(ch-'0')*rt;
                  rt=rt/10.;
                  ch=getchar();
                }
            };
        };
      if (negative) rf=-rf;
    }
  return rf;
}

/*************/
double simexp()
/*************/
{
  double rs;
  rs=factor();
  while ((ch=='*') | (ch=='/')
       | (ch=='&') | (ch=='<')
       | (ch=='>'))
    {
      switch (ch)
        {
        case '*': {
                    rs=rs*factor(); break;
                  };
        case '/': {
                    rs=rs/factor(); break;
                  };
        case '&': {
                    rs=(double)(fix(rs) & fix(factor()));
                    break;
                  };
        case '<': {
                    ch = getchar();
                    checkfor('<');
                    rs=(double)(fix(rs) << fix(factor()));
                    break;
                  };
        case '>': {
                    ch = getchar();
                    checkfor('>');
                    rs=(double)(fix(rs) >> fix(factor()));
                    break;
                  };
        }
    }
  return rs;
}

/**************/
double express()
/**************/
{
  double re;
  re=simexp();
  while ((ch=='+') | (ch=='-') |
            (ch=='|'))
    {
      switch (ch)
        {
        case '+': {
                    re=re+simexp(); break;
                  };
        case '-': {
                    re=re-simexp(); break;
                  };
        case '|': {
                    re=(double)(fix(re) |
                     fix(factor()));
                    break;
                  }
        };
    };
  return re;
}

/******************/
int main(argc, argv)
int argc;
char *argv[];
/******************/
{
  printf("CALC for 2.11BSD Unix on PiDP11\n");
  printf("Enter an expression, examples are:\n");
  printf("32767     input decimal number\n");
  printf("$fff      input hex number\n");
  printf("o2750     input octal number\n");  
  printf("%%11001   input binary number\n");
  printf("-55.35    input negative number\n");
  printf("2*(5+28)\n");
  printf("<return>  exit\n");
  printf("operators: +,-,*,/,(),&,|,<<,>>\n");
  r=0.;
  dotused=0;
  do
    {
      stop=1;
      printresult(r);
      printf("\n? ");
      dotused=0;
      r=express();
      checkfor(EOL);
    }
  while (stop == 0);
}
    
