# -------------------------
# This code can run without problem.
#
# How to run:
# 1. input the file that contains 2 lines, where line 1 is an integer n; and line 2 contains n+1 real numbers a[0], a[1],..., a[n]
# 2. it will print out a text like f(x)= a[0] + a[1] x^1 + a[2] x^2 + ... + a[n] x^n
# 3. it will ask the user to input a value of x, say, b. Then it calculates and outputs f(b)
# 4. it repeats step 3, unless b=0, which prints out f(0) and STOP
#
# This code is written by 王大頭, email XXX@gs.ncku.edu.tw, on 2021/09/18
# -------------------------
# Function to evaluate f(x) at given x = b
def evaluate(a, b):
    result = 0
    for i, coef in enumerate(a):
        result += coef * (b ** i)
    return result

# Read the filename from user input
filename = input("Enter the filename: ")

try:
    # Open the file for reading
    with open(filename, "r") as f:
        n = int(f.readline().strip())  # Read n from the first line
        a = list(map(float, f.readline().strip().split()))  # Read coefficients from the second line

    # Print f(x)
    print("f(x)=", end="")
    for i, coef in enumerate(a):
        if coef == 0:
            continue
        elif i == 0:
            print(f" {coef}", end="")
        elif coef > 0:
            print(f" + {coef} x^{i}", end="")
        else:
            print(f" - {-coef} x^{i}", end="")
    print()

    while True:
        b = float(input("Enter the value of x: "))
        if b == 0:
            print(a[0])
            break
        else:
            print("f(b) =", evaluate(a, b))

except FileNotFoundError:
    print("File not found.")
except ValueError:
    print("Invalid file format.")
