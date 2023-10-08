#include <stdio.h>

// Narrowing Conversion
void narrowTest()
{
  double a = 398473847387438;
  float b = a;
  if(a != b) {
    printf("Narrowing conversion occured from double %f to float %f\n", a, b);
  }
  
  float x = 420.69;
  int y = x;
  if(x != y) {
    printf("Narrowing conversion occured from float %f to int %d\n", x, y);
  }

  int p = 56;
  char q = p;
  if(x != y) {
    printf("Narrowing conversion occured from int %d to char %c\n", p, q);
  }
}

// Widenining Conversion
void wideTest()
{
  short a = 20;
  int b = a;
  if(sizeof(b) > sizeof(a)){
    printf("Widening conversion occured from short %d to int %d\n", a, b);
  }

  float x = 39;
  double y = x;
  if(sizeof(y) > sizeof(x)){
    printf("Widening conversion occured from float %f to double %f\n", x, y);
  }

  char c = 'A';
  char s[2] = {c , '\0'};
  if(sizeof(s) > sizeof(c)){
    printf("Widening conversion occured from char %c to string %s\n", c, s);
  }
}

// Explicit Conversion
void explicitTest()
{
  // If any error is thrown from this function, then explicit conversion was not successful
  int a = 11;
  float b = (float) a;
  printf("Explicit conversion occured from int %d to float %f\n", a, b);

  float x = 69.420;
  int y = (int) x;
   printf("Explicit conversion occured from float %f to int %d\n", x, y);

  int p = 65;
  char q = (char) p;
   printf("Explicit conversion occured from int %d to char %c\n", p, q);
}

// Implicit Conversion
void implicitTest()
{
  // If any error is thrown from this function, then implicit conversion was not successful
  int a = 42;
  float b = 24;
  b = 24 * 42;
  printf("Implicit conversion occured for int %d times float %f to float\n", a, b);


  float x = 420.69;
  int y = x;
  printf("Implicit conversion occured from float %f to int %d\n", x, y);
}

int main(void) {
  narrowTest();
  wideTest();
  explicitTest();
  implicitTest();
}