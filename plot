#!/bin/python
import matplotlib.pyplot as plt

nodes = []

with open('vert_positions') as f:
    for l in f:
        comma_pos = l.find(",")
        x         = int(l[0:comma_pos])
        y         = int(l[comma_pos+1:]) 
        nodes.append((x, y)) 
        plt.plot(x, y, "o")

with open('graph_in') as f:
    for l in f:
        comma_pos = l.find(",")
        vert_a    = int(l[0:comma_pos])
        vert_b    = int(l[comma_pos+1:]) 
        x         = nodes[vert_a][0]
        y         = nodes[vert_a][1] 
        x2        = nodes[vert_b][0]
        y2        = nodes[vert_b][1] 
        plt.plot([x, x2], [y, y2], 'k-')

plt.show()
