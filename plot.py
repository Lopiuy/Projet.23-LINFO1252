import sys
import matplotlib.pyplot as plt
import pandas as pd
import numpy as np
import seaborn as sn

sn.set_theme()
plt.rcParams["mathtext.fontset"] = 'cm'
plt.rcParams['font.family'] = 'Times New Roman'

def customization(ax, labels, y, vp, problem):

    # axes and title
    ax.set_xticks(np.arange(1, len(labels) + 1), labels=labels)
    ax.set_xlabel("Nombre de threads " + "$[-]$", fontsize=12)
    ax.set_ylabel("Temps d'exécution " + "$[s]$", fontsize=12)
    ax.set_ylim(0, np.max(y)*1.05)
    title = "Diagramme en violon du problème de synchronisation"
    if problem == 'ph': title += " des philosophes"
    elif problem == 'pc': title += " des producteurs & consommateurs"
    elif problem == 'rw': title += " des lecteurs & écrivains"
    ax.set_title(title, fontsize=14)

    # violon plots
    vp['cmeans'].set_label("moyennes")
    vp['cmeans'].set_color('indianred')
    for pc in vp['bodies']:
        pc.set_facecolor("cornflowerblue")
        pc.set_edgecolor("royalblue")
    vp['cbars'].set_color('royalblue')
    vp['cbars'].set(zorder=1)
    vp['cmins'].set_color('royalblue')
    vp['cmaxes'].set_color('royalblue')

    # legend
    box = ax.get_position()
    ax.set_position([box.x0, box.y0, box.width * 0.9, box.height])
    ax.legend(loc='center left', bbox_to_anchor=(1, 0.5), fancybox=True)

def plot(data, threads, nthreads, problem=''):
    
    _, ax = plt.subplots(figsize=(9, 5))

    labels = [str(threads[i]) for i in range(nthreads)]
    y = data
    means = np.mean(y, axis=1)
    stds = np.std(y, axis=1)

    vp = ax.violinplot(y, showmeans=True)
    ax.scatter(np.arange(1, len(labels) + 1), means+stds, 125*np.ones(nthreads), marker='_', lw=1, color='saddlebrown', label="écarts-types")
    
    customization(ax, labels, y, vp, problem)
    plt.show()


if len(sys.argv) != 2:
    print("Error: Invalid arguments was given")
else :
    try:
        data = pd.read_csv(sys.argv[1], delimiter = ',')
        threads = [1, 2, 4, 8, 16, 32, 64]
        nthreads = len(threads)
        toplot = [list(data[data['nb threads'] == i]['time']) for i in threads]


        if (sys.argv[1].__contains__("ph")): plot(toplot, threads, nthreads, problem='ph')

        elif (sys.argv[1].__contains__("prodcons")): plot(toplot, threads, nthreads, problem='pc')

        elif (sys.argv[1].__contains__("rw")): plot(toplot, threads, nthreads, problem='rw')

        else : print("Error: Invalid arguments was given")
    except:
        print("Error: Invalid arguments was given")
