from pyrecord import Record
import array as arr
import random
import sys

# Python Arrays, Lists, Tuples, and Records
arr = arr.array("i", [1, 2, 3, 4])

list = [1, 2, 3, 4]

tuple = (1, 2, 3, 4)

Customer = Record.create_type("Customer", "name", "email_address", "dob", "credit_card")
bob = Customer("Bob Ross", "bob.ross@hotmail.com", "10/29/1942", "1518458768551234")
print(bob.name)
print

# Reverse Slicing
l = [1, 2, 3, 4]
s = "1234"

print(l[::-1])
print(s[::-1])
print

# Randomize List In-Place
l = [1, 2, 3, 4, 5, 6, 7]
random.shuffle(l)
print(l)
print

# range vs. xrange
a = range(1000)
b = xrange(1000)
print(type(a), "Size:", sys.getsizeof(a))
print(type(b), "Size:", sys.getsizeof(b))
print


# Add items to list
l = [0]
l.append(1)
l.insert(0, -1)
print(l)
print


# split()
s = "I want to break up this string into words as a a list"
l = s.split()
print(l)
