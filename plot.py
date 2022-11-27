import sys
import matplotlib.pyplot as plt
import pandas as pd
import numpy as np
import seaborn as sn

sn.set_theme()
plt.rcParams["mathtext.fontset"] = 'cm'
plt.rcParams['font.family'] = 'Times New Roman'

def plot(data, nthreads, problem='ph'):
    
    _, ax = plt.subplots(figsize=(9, 5))

    labels = [str(data["threads"][5 * i]) for i in range(nthreads)]
    y = [list(data[i:i+5]["t"].values) for i in range(nthreads)]
    means = np.mean(y, axis=1)
    stds = np.std(y, axis=1)

    # customization
    ax.set_xticks(np.arange(1, len(labels) + 1), labels=labels)
    ax.set_xlabel("Nombre de threads " + "$[-]$", fontsize=12)
    ax.set_ylabel("Temps d'exécution " + "$[s]$", fontsize=12)
    ax.set_ylim(np.min(y)*0, np.max(y)*1.11)
    ax.set_title("Diagramme en violon du problème de synchronisation", fontsize=14)

    vp = ax.violinplot(y, showmeans=True)
    vp['cmeans'].set_label("moyennes")
    vp['cmeans'].set_color('indianred')
    for pc in vp['bodies']:
        pc.set_facecolor("cornflowerblue")
        pc.set_edgecolor("royalblue")
    vp['cbars'].set_color('royalblue')
    vp['cbars'].set(zorder=1)
    vp['cmins'].set_color('royalblue')
    vp['cmaxes'].set_color('royalblue')

    
    ax.scatter(np.arange(1, len(labels) + 1), means+stds, 125*np.ones(nthreads), marker='_', lw=1, color='saddlebrown', label="écarts-types")
    ax.scatter(np.arange(1, len(labels) + 1), means-stds, 125*np.ones(nthreads), marker='_', lw=1, color='saddlebrown')

    plt.legend(loc='lower right')
    plt.show()


if len(sys.argv) != 2:
    print("Error: Invalid arguments was given")
else :
    data = pd.read_csv(sys.argv[1], delimiter = ',')
    nthreads = int(len(data)/5)
    if (sys.argv[1].__contains__("philo")):
        data.columns = ["threads", "i", "t"]
        plot(data, nthreads, problem='ph')
    elif (sys.argv[1].__contains__("prodcons")):
        pass
    elif (sys.argv[1].__contains__("rw")):
        pass
    else :
        print("Error: Invalid arguments was given")
