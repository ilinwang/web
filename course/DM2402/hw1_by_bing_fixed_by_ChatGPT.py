# The provided Python code seems to have a few issues that need to be addressed for it to work correctly. Here is a fixed version of the code:

# Indentation: Python relies on indentation to recognize code blocks.
# Opening files safely: It's a good practice to open files using the with statement, which ensures that the file is closed properly even if an error occurs.
# String concatenation: Fixed the concatenation logic to properly format the polynomial.

# Define a function to read the text file and return the degree and coefficients of the polynomial
def read_file(filename):
    with open(filename, "r") as f:
        n = int(f.readline())
        line = f.readline()
        a = [float(x) for x in line.split()]
    return n, a

# Define a function to print the polynomial in a formatted way
def print_poly(n, a):
    poly = ""
    for i in range(n, -1, -1):
        if a[i] == 0:
            continue
        if a[i] > 0 and i != n:
            poly += " + "
        if a[i] < 0:
            poly += " - "
        poly += str(abs(a[i]))
        if i != 0:
            poly += " x^" + str(i)
    print("f(x) =", poly)

# Define a function to evaluate the polynomial at a given value of x
def eval_poly(n, a, x):
    result = 0
    for i in range(n + 1):
        result += a[i] * (x ** i)
    return result

# User interaction
filename = input("Please enter the filename of the text file: ")
n, a = read_file(filename)
print_poly(n, a)

while True:
    x = float(input("Please enter the value of x (enter 0 to stop): "))
    if x == 0:
        print("f(0) =", a[0])
        break
    else:
        print("f({}) = {}".format(x, eval_poly(n, a, x)))
