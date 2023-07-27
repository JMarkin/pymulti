N = 10000


def get_sum():
    s = 0
    for i in range(N):
        for j in range(N):
            s += i * j
    return s
