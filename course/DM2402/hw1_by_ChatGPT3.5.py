# This code can run without problem.
# How to run:
# 1. Input the file that contains 2 lines, where line 1 is an integer n;
#    and line 2 contains n+1 real numbers a[0], a[1],..., a[n]
# 2. It will print out a text like f(x)= a[0] + a[1] x^1 + a[2] x^2 + ... + a[n] x^n
# 3. It will ask the user to input a value of x, say, b. Then it calculates and outputs f(b)
# 4. It repeats step 3, unless b=0, which prints out f(0) and STOP
# This code is written by 王大頭, email XXX@gs.ncku.edu.tw, on 2021/09/18

def format_coefficient(coefficient, power):
    if coefficient == 0:
        return ""
    elif power == 0:
        return f"{coefficient}"
    elif power == 1:
        return f"{coefficient}x"
    else:
        return f"{coefficient}x^{power}"

def main():
    filename = input("Enter the filename: ")
    with open(filename, 'r') as file:
        n = int(file.readline())
        coefficients = list(map(float, file.readline().split()))

    formatted_terms = [format_coefficient(coefficients[i], i) for i in range(n + 1)]
    formatted_terms = [term for term in formatted_terms if term != ""]

    polynomial = " + ".join(formatted_terms)
    polynomial = polynomial.replace(" + -", " - ")

    print(f"f(x) = {polynomial}")

    while True:
        b = float(input("Enter a value for x (0 to stop): "))
        if b == 0:
            break
        result = sum(coeff * (b ** power) for power, coeff in enumerate(coefficients))
        print(f"f({b}) = {result}")

if __name__ == "__main__":
    main()
