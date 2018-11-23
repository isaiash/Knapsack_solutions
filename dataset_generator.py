import random
import numpy as np
from sklearn.metrics.pairwise import euclidean_distances
import kmedoids
from math import log10
from scipy.stats.stats import pearsonr
import os
import sys
import time

N_LEVELS = 10

TESTS_PER_LEVEL = 1000

MAX_N_ELEMENTS = 1000000
MIN_N_ELEMENTS = 1

MAX_BAG_SIZE = 1000000
MIN_BAG_SIZE = 1
# NOTA: El valor de los pesos tendra como maximo el valor de la mochila
MAX_VALUE = 1000000
MIN_VALUE = 1

PATH_TO_DATASET = "Uniform/DataSet/"
def index_list_by_level():
    uniform_level = []
    for level in range(N_LEVELS):
        uniform_level.append([])
        
    for instance in os.listdir(PATH_TO_DATASET):
        index, num = open(PATH_TO_DATASET + (str)(instance),'r').read().split()[:2]
        indice = (int)((int)(num)*(N_LEVELS-1)/MAX_N_ELEMENTS)
        print(index, num, indice)
        uniform_level[indice].append(index)
    return uniform_level
        
def get_instance(index):
    return list(map(int, open(PATH_TO_DATASET + index,'r').read().split()))

def get_features(weights, values):
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
    p_coef, p_value = pearsonr(weights, values)

    if (mean_w == 0 or median_w == 0 or desv_w == 0 or mean_v == 0 or median_v == 0 or desv_v ==0):
        return -1
    
    return [num_elem, # n
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
            p_coef]

uniform_data = index_list_by_level()
count = 0
for level in range(N_LEVELS):
    print("______LEVEL",level+1,"______")
    print("Sampling...")
    n_tests = (int)(TESTS_PER_LEVEL/(level+1))
    level_dataset = []
    level_features = []
    min_lev = (int)(MAX_N_ELEMENTS*(level/N_LEVELS))
    max_lev = (int)(MAX_N_ELEMENTS*((level + 1)/N_LEVELS))
    t0 = time.clock()
    for test in range(n_tests):
        weights = []
        values = []
        cap = random.randint(MIN_BAG_SIZE, MAX_BAG_SIZE)
        num_elem = random.randint(1+min_lev, max_lev)
        for el in range(num_elem):
            weights.append(random.randint(MIN_BAG_SIZE, cap))
            values.append(random.randint(MIN_VALUE, MAX_VALUE))
        
        features = get_features(weights, values)
        if features == -1: continue
            
        level_dataset.append([num_elem, cap, * weights, * values])        
        level_features.append(features)

        if time.clock() - t0 > 60: 
            print("Test number: ", test, "/", n_tests)
            t0 = time.clock()
    print("Appending "+(str)(len(uniform_data[level]))+" uniform instances...")
    for instances in uniform_data[level]:
        inst = get_instance(instances)
        mid = (int)(len(inst[3:])/2)
        features = get_features(inst[3:mid+3], inst[mid+3:])
        if features == -1: continue
        level_dataset.append(inst[1:])
        level_features.append(features)
    pairwise_matrix = euclidean_distances(level_features, level_features)
    print("kMedoids...")
    M, C = kmedoids.kMedoids(pairwise_matrix, (int)(n_tests/10) + (int)(len(uniform_data[level])/10))
    print("Saving...")
    for index in M:
        d = open('Cluster_dataset/dataset/'+str(count),'w')
        print(count, * level_dataset[index], sep=' ', file=d)

        f = open('Cluster_dataset/features/'+str(count),'w')
        print(count, * level_features[index], sep=' ', file=f)
        
        count+=1

    print(count, " instances saved on level ", level+1)
