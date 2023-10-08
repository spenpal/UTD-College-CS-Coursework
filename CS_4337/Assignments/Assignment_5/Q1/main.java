import static java.lang.System.*;

// Static Scope Example
class Main 
{
  static int x = 420;

  public static void main(String[] args) 
  { 
    out.println(second());
  }

  static int first() 
  { 
    return x; 
  } 
  
  static int second() 
  { 
    int x = 69; 
    return first(); 
  }
}