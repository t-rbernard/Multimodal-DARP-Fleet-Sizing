import matplotlib.pyplot as plt

#visualisation params
eps = 0.2 #decalage du label par rapport au point


class Edge:
    def __init__(self):
        self.start = None
        self.end = None
        self.length = None

    def __init__(self, start: int, end: int, length: float):
        self.start = start
        self.end = end
        self.length = length


class Node:
    def __init__(self):
        self.status = None
        self.x = None
        self.y = None

    def __init__(self, x: float, y: float, status: str = "work"):
        self.status = status
        self.x = x
        self.y = y


#TODO : get those from cpp
nodes = []
edges = []
i = 0
N = len(nodes)
instanceFolderPath = "resources/test/outputs/basic_debug_instance/"

#Read nodes
fp = open(instanceFolderPath + 'nodes.txt')
lines = fp.read().split("\n")
fp.close()
for nodeLine in lines:
    splitLine = nodeLine.split(" ")
    if len(splitLine) == 2:
        nodes.append(Node(float(splitLine[0]), float(splitLine[1])))

#Read edges
fp = open(instanceFolderPath + 'edges.txt')
lines = fp.read().split("\n")
fp.close()
for edgeLine in lines:
    splitLine = edgeLine.split(" ")
    if len(splitLine) == 3:
        edges.append(Edge(int(splitLine[0]), int(splitLine[1]), float(splitLine[2])))


#print points from nodes vectors
plt.scatter([node.x for node in nodes], [node.y for node in nodes], color="red", s=50)

#print labels
while i < N:
    suiv = (i+1)%N
    for i in range(len(nodes)):
        plt.text(nodes[i].x+eps, nodes[i].y-eps, i)
    i=i+1


#Print edges
for edge in edges:
    plt.annotate(text='', xy=(nodes[edge.start].x, nodes[edge.start].y),
                 xytext=(nodes[edge.end].x, nodes[edge.end].y),
                 arrowprops=dict(arrowstyle='<-'))


#on agrandit un peu les limites sur X pour que les
# labels ne dépassent pas le cadre
plt.xlim(min([node.x for node in nodes])-eps, max([node.x for node in nodes])+2*eps)
plt.show()