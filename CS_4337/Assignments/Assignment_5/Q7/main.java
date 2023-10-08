import static java.lang.System.*;

class Main 
{
  static int multiply(int x, int y)
  {
    return x * y;
  }

  public static void main(String[] args) 
  { 
    int a = 2, b = 7, c = 8, d = 9;

    int result = a + b * multiply(c, d);

    out.println(result);
  }
}