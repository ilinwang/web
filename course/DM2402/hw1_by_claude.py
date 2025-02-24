# -------------------------
# This code can run without problem. 
#
# How to run:
# 
# 1. input the file that contains 2 lines, where line 1 is an integer n; and line 2 contains n+1 real numbers a[0], a[1],..., a[n]  
# 2. it will print out a text like f(x)= a[0] + a[1] x^1 + a[2] x^2 + ... + a[n] x^n
# 3. it will ask the user to input a value of x, say, b. Then it calculates and outputs f(b)  
# 4. it repeats step 3, unless b=0, which prints out f(0) and STOP
#
# This code is written by 王大頭, email XXX@gs.ncku.edu.tw, on 2021/09/18
# -------------------------

# Read input file
filename = input("Enter file name: ")
with open(filename) as f:
    n = int(f.readline())
    coeffs = [float(x) for x in f.readline().split()]

# Print polynomial
print("f(x)=", end=" ") 
for i in range(n+1):
    coeff = coeffs[i]
    if coeff != 0:
        print(f"{coeff:+} ", end="")
        if i > 0:
            print(f"x^{i} ", end="")
print()
        
# Evaluate polynomial 
while True:
    b = float(input("Enter x value (0 to quit): "))
    if b == 0:
        print(coeffs[0])
        break
        
    result = coeffs[0]
    for i in range(1, n+1):
        result += coeffs[i] * (b ** i)
    print(f"f({b}) = {result}")
