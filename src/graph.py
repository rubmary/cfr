import matplotlib
matplotlib.use('Agg')
import matplotlib.pylab as plt
import sys
MAX_VAL = 2e7

def make_plot(path_input, path_graph, name):
    file = open(path_input)
    i = 0
    regret1 = []
    regret2 = []
    iterations = []
    while i < MAX_VAL:
        line = file.readline()
        if not line:
            break
        line = line.split()
        iterations.append(int(line[0]))
        regret1.append(float(line[1]))
        regret2.append(float(line[2]))
        i = i+1
    file.close()
    plt.figure()
    plt.title(name)
    plt.xscale('log')
    plt.plot(iterations, regret1, label='Jugador 1')
    plt.plot(iterations, regret2, label='Jugador 2')
    plt.ylabel('Regret (max)')
    plt.xlabel('Iteraciones')
    plt.legend()
    plt.savefig(path_graph + name + ".png")

if __name__ == "__main__":
    if(len(sys.argv) < 2):
        print("Debes introducir el path del juego")
        exit(0);
    subpath = sys.argv[1]
    game = subpath.split('/')[0]
    path_input = "regret/" + subpath + "/regret.txt"
    path_graph = "graphics/" + game + "/"
    name = subpath.replace('/', '(').replace('_', ',')
    if '(' in name:
        name = name + ')'
    make_plot(path_input, path_graph, name)
