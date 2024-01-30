import matplotlib.pyplot as plt

#visualisation params
eps = 0.2 #decalage du label par rapport au point

class edge:
    def __init__(self):
        self.start = None
        self.end = None

    def __init__(self, start, end):
        self.start = start
        self.end = end

#TODO : get those from cpp
X = []
Y = []
i = 0
N = len(X)

#print points
plt.scatter(X, Y, color = "red", s = 50)

#print labels
while i < N:
    suiv = (i+1)%N
    for i in range(len(X)):
        plt.text(X[i]+eps, Y[i]-eps, i)
    i=i+1


#Print edges
for edge in edges :
    plt.annotate(text='', xy=(X[edge.start],Y[edge.start]), xytext = (X[edge.end],Y[edge.end]), arrowprops=dict(arrowstyle='<-'))


#on agrandit un peu les limites sur X pour que les
# labels ne dépassent pas le cadre
plt.xlim(min(X)-eps, max(X)+2*eps)
plt.show()