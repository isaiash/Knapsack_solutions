import random

N_ELEMENTOS = [100, 200, 300, 500, 800, 1000, 1200, 1500]
CAPACIDADES = [1100, 1500, 1700, 2000, 5000, 10000, 14000, 16000]
CANTIDAD = 50

f = open('dataset_' + str(CANTIDAD),'w')
for el, cap in zip(N_ELEMENTOS, CAPACIDADES):
    for i in range(CANTIDAD):
        weights = []
        values = []
        print(el, cap, file=f, end = ' ')
        for j in range(el):
            weights.append(random.randint(5, 20))
            values.append(random.randint(50, 100))
        print(*weights, sep=' ', file=f)
        print(*values, sep=' ', file=f)
