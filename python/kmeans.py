
'''
given a set of points
1. select k points (centroids) randomly

2. repeat

    - label points
    - update centroids

'''

#in pythonn, function call is expensive
#also square root is expensive --> use the moltiplycation

from collections import defaultdict
from pprint import pprint
from typing import Tuple, Sequence, Mapping, Callable, Iterable
import matplotlib.pyplot as plt
from random import sample
from collections import defaultdict
from statistics import mean
from functools import partial

Point = Tuple[int, int]
Centroid = Point
Cluster = Sequence[Point]
Dist_func = Callable[[Point,Point], float]

def guess_centroids(dataset: Sequence[Point], k:int) -> Sequence[Centroid]:
    return sample(dataset, k=k)

#it's not important the ^2 --> it's costly
def distance(p: Point, q: Point, /):
    #return sum((xp-xq)*(xp-xq) for xp, xq in zip(p, q))
    return ((p[0]-q[0])*(p[0]-q[0]) + (p[1]-q[1])*(p[1]-q[1]))


def label(dataset: Sequence[Point], centroids: Sequence[Centroid], dist: Dist_func) -> Mapping[Centroid, Cluster]:
    d = defaultdict(list)
    for p in dataset:
        #centroid = min(centroids, key = lambda c: dist(c,p)) #OR
        pdist = partial(dist,p)
        centroid = min(centroids, key=pdist)
        d[centroid].append(p)
    return d

def update_centroids(cluster: Iterable[Cluster]) -> Sequence[Centroid]:
    centroids = []
    for cluster in cluster:
        xc, yc = list(zip(*cluster))
        #xc, yc = list(zip(int(*cluster)))
        centroids.append((mean(xc), mean(yc)))
    return centroids

# def distortion(labeled_dataset: Mapping[Centroid, Cluster], distance: Dist_func) -> float:
#     dist = 0.0
#     for centroid, cluster in labeled_dataset.items():
#         #dist += mean(dist(p,centroid)for p in cluster)
#         pdist = partial(distance,centroid)
#         dist += 




def _kmeans(dataset: Sequence[Point], k: int, n_iter:int, dist:Dist_func) -> Mapping[Centroid, Cluster]:
    centroids = guess_centroids(dataset,k)
    for _ in range(n_iter):
        labeled = label(dataset, centroids, dist)
        centroids = update_centroids(centroids)
    return label(dataset, centroids)

# def kmean(...):
#       
#     for _ in range(outer_iteration):
#         mapping, distorion = _kmean(dataset, k,inner) 
#         if distortion < best_mapping:
        # best_mapping = Mapping
        # best_distortion = best_distortion
        # return best_mapping, best_distortion

points: Sequence[Point] = []

with open("s3.txt") as f:
    points = [tuple(map(float, line.split())) for line in f]
    #for line in f:
    #    x,y = line.split()
    #    points.append((float(x), float(y)))

X,Y = list(zip(*points))

d = _kmeans(points, k=15, n_iter=10, dist=distance)

centroids = d.keys()

Xc, Yc = list(zip(*centroids))

plt.scatter(X,Y, s=0.5)
plt.show()




