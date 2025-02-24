# Define a function to read the text file and return the degree and coefficients of the polynomial
def read_file(filename):
# Open the file in read mode
f = open(filename, "r")
# Read the first line and convert it to an integer
n = int(f.readline())
# Read the second line and split it by spaces
line = f.readline()
# Convert each element to a float and store them in a list
a = [float(x) for x in line.split()]
# Close the file
f.close()
# Return the degree and coefficients of the polynomial
return n, a

# Define a function to print the polynomial in a formatted way
def print_poly(n, a):
# Initialize an empty string to store the polynomial
poly = ""
# Loop through each coefficient from the highest degree to the lowest
for i in range(n, -1, -1):
# If the coefficient is zero, skip it
if a[i] == 0:
continue
# If the coefficient is positive and not the first term, add a plus sign
if a[i] > 0 and i != n:
poly += " + "
# If the coefficient is negative, add a minus sign
if a[i] < 0:
poly += " - "
# Add the absolute value of the coefficient and a space
poly += str(abs(a[i])) + " "
# If the degree is not zero, add x and the power
if i != 0:
poly += "x^" + str(i)
# Print the polynomial
print("f(x) =", poly)

# Define a function to evaluate the polynomial at a given value of x
def eval_poly(n, a, x):
# Initialize a variable to store the result
result = 0
# Loop through each coefficient from the lowest degree to the highest
for i in range(n + 1):
# Multiply the coefficient by x raised to the power of i and add it to the result
result += a[i] * (x ** i)
# Return the result
return result

# Ask the user to input the filename of the text file
filename = input("Please enter the filename of the text file: ")
# Read the file and get the degree and coefficients of the polynomial
n, a = read_file(filename)
# Print the polynomial in a formatted way
print_poly(n, a)
# Keep asking the user to input the value of x until zero is entered
while True:
# Ask the user to input the value of x
x = float(input("Please enter the value of x (enter 0 to stop): "))
# If zero is entered, print the constant term and stop
if x == 0:
print("f(0) =", a[0])
break
# Otherwise, evaluate and print the value of f(x)
else:
print("f({}) = {}".format(x, eval_poly(n, a, x)))