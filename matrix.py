import numpy, sys, time

def calculate(n):
    a = numpy.zeros((n, n)) # Matrix A
    b = numpy.zeros((n, n)) # Matrix B
    c = numpy.zeros((n, n)) # Matrix C

    # Initialize the matrices to some values.
    for i in range(n):
        for j in range(n):
            a[i, j] = i * n + j
            b[i, j] = j * n + i
            c[i, j] = 0

    begin = time.time()

    # Write code to calculate C = A * B. #
    for i in range(n):
        for k in range(n):
            for j in range(n):
                c[i, k] += a[i, j] * b[j, k]

    end = time.time()
#     print("time: %.6f sec" % (end - begin)) 
    print("%d %.6f" % (n, (end - begin)))
    # Print C for debugging. Comment out the print before measuring the execution time.
    total = 0
    for i in range(n):
        for j in range(n):
            # print c[i, j]
            total += c[i, j]
    # Print out the sum of all values in C.
    # This should be 450 for N=3, 3680 for N=4, and 18250 for N=5.
#     print("sum: %.6f" % total)

# if (len(sys.argv) != 2):
#     print("usage: python %s N" % sys.argv[0])
#     quit()
for i in range(500):
    calculate(i)
