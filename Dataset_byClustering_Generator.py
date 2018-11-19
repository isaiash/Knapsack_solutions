import random
import numpy as np
from sklearn.metrics.pairwise import euclidean_distances
import kmedoids
import matplotlib.pyplot as plt
from math import log10

N_LEVELS = 5

TESTS_PER_LEVEL = 100000

MAX_N_ELEMENTS = 10000
MIN_N_ELEMENTS = 1

MAX_BAG_SIZE = 50000
MIN_BAG_SIZE = 1
# NOTA: El valor de los pesos tendra como maximo el valor de la mochila
MAX_VALUE = 10000
MIN_VALUE = 1

DIV_FACTOR = 10

all_dataset = []
all_features = []
for level in range(N_LEVELS):
    n_tests = (int)(TESTS_PER_LEVEL/(level+1))
    factor = (level+1)/N_LEVELS
    for test in range(n_tests):
        weights = []
        values = []
        cap = random.randint(MIN_BAG_SIZE, MAX_BAG_SIZE)
        num_elem = random.randint(MIN_N_ELEMENTS, MAX_N_ELEMENTS*factor)
        for el in range(num_elem):
            weights.append(random.randint(MIN_BAG_SIZE, cap))
            values.append(random.randint(MIN_VALUE, MAX_VALUE))
        all_dataset.append([num_elem, cap, *weights, *values])
        
        mean_w = np.mean(weights)
        median_w = np.median(weights) 
        desv_w = np.std(weights)
        min_w = min(weights)
        max_w = max(weights)
        mean_v = np.mean(values)
        median_v = np.median(values) 
        desv_v = np.std(values)
        min_v = min(values)
        max_v = max(values) 
        
        if (mean_w == 0 or median_w == 0 or desv_w == 0 or mean_v == 0 or median_v == 0 or desv_v ==0):
            continue
        
        all_features.append([
            num_elem, # n
            cap, # cap
            cap/mean_w,
            cap/median_w,
            cap/desv_w,
            mean_w/mean_v,
            median_w/median_v,
            desv_w/desv_v,
            max_w - min_w,
            max_v - min_v,
            mean_w, 
            median_w, 
            desv_w, 
            min_w, 
            max_w, 
            mean_v, 
            median_v, 
            desv_v, 
            min_v, 
            max_v,
        ])
    print(n_tests, "tests for level", level+1)
    
pairwise_matrix = euclidean_distances(all_features, all_features)

i = 10000
M, C = kmedoids.kMedoids(pairwise_matrix, i)

for id_, index in enumerate(M):
    d = open('Cluster_dataset/dataset/'+str(id_),'w')
    print(id_, *all_dataset[index], sep=' ', file=d)
    
    f = open('Cluster_dataset/features/'+str(id_),'w')
    print(id_, *all_features[index], sep=' ', file=f)
