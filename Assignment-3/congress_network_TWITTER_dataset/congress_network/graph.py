import networkx as nx
import matplotlib.pyplot as plt

G = nx.Graph()
with open('minimum_spanning_tree_prim.txt') as f:
     for line in f:
         u, v = map(int, line.strip().split(' - '))
         G.add_edge(u, v)

nx.draw(G, with_labels=True, node_size=1000, node_color='skyblue', font_size=10, font_weight='bold')
plt.show()
