import math


a = float(input())
b = float(input())
c = float(input())
d = b * b - 4 * a * c
if d > 0:
    x1 = (-b - math.sqrt(d)) / (2 * a)
    x2 = (-b + math.sqrt(d)) / (2 * a)
    if(x1 > x2):
        print(x2, x1)
    else:
        print(x1, x2)
elif d == 0:
    print(-b / (2 * a))
