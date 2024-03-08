import os
import numpy as np

def run():
    file_path = './4TEST.txt'
    with open(file_path, 'w') as f:
        n = 5
        f.write(f'{n}\n')
        for i in range(n):
            N = np.random.randint(1, 20)
            K = np.random.randint(1, N)
            f.write(f'{N} {K}\n')
            # Set v
            v = []
            while (len(v) < N):
                v.append(np.random.randint(1, 100))
                v = list(set(v))
            v.sort()
            for j in range(len(v)):
                f.write(f'{v[j]} ')
            f.write('\n')


    print('Tests realizados!\n')

if __name__ == '__main__':
    np.random.seed(9999)
    run()