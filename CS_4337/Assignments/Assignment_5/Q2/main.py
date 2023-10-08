from math import sqrt

# Global Variables
x, y = 2, 3

def func1(z1):
  def func2(z2):
    def func3(z3):
      def func4(z4):
        def func5(z5):
          distance = sqrt(sum(point ** 2 for point in [x, y, z5])) # Local Variable
          print(f"Distance at subprogram {z5}: {distance}")

        distance = sqrt(sum(point ** 2 for point in [x, y, z4])) # Local Variable
        print(f"Distance at subprogram {z4}: {distance}")
        func5(z4 + 1) # Calling Nested Programs Variables

      distance = sqrt(sum(point ** 2 for point in [x, y, z3])) # Local Variable
      print(f"Distance at subprogram {z3}: {distance}")
      func4(z3 + 1) # Calling Nested Programs Variables

    distance = sqrt(sum(point ** 2 for point in [x, y, z2])) # Local Variable
    print(f"Distance at subprogram {z2}: {distance}")
    func3(z2 + 1) # Calling Nested Programs Variables
  
  distance = sqrt(sum(point ** 2 for point in [x, y, z1])) # Local Variable
  print(f"Distance at subprogram {z1}: {distance}")
  func2(z1 + 1) # Calling Nested Programs Variables

print('Results of subprograms:\n')
func1(1)