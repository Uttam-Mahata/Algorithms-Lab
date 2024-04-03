from matplotlib import pyplot as plt
import networkx as nx

G = nx.Graph()
with open('mst_data.txt', 'r') as f:
        for line in f:
            src, dest = map(int, line.strip().split())
            G.add_edge(src, dest)

pos = nx.spring_layout(G)
nx.draw(G, pos, with_labels=True, node_size=700, node_color='skyblue', font_size=20, font_weight='bold', edge_color='gray', width=2, edge_cmap=plt.cm.Blues)
plt.show();