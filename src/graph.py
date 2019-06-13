import matplotlib.pylab as plt
import sys
MAX_VAL = 2e7

def make_plot(path_input, path_graph, name):
    file = open(path_input)
    i = 0
    regret1 = []
    regret2 = []
    while i < MAX_VAL:
        line = file.readline()
        if not line:
            break
        regret1.append(float(line.split()[0]))
        regret2.append(float(line.split()[1]))
        i = i+1
    file.close()
    plt.figure()
    plt.title(name)
    plt.xscale('log')
    plt.plot(regret1, label='Jugador 1')
    plt.plot(regret2, label='Jugador 2')
    plt.ylabel('Regret (max)')
    plt.xlabel('Iteraciones')
    plt.legend()
    plt.savefig(path_graph)

if __name__ == "__main__":
    if(len(sys.argv) < 2):
        print("Debes introducir el nombre del juego")
        exit(0);
    name = sys.argv[1]
    path_input = "regret/" + name + "/regret.txt"
    path_graph = "graphics/" + name + "/" + name + ".png"
    make_plot(path_input, path_graph, name)