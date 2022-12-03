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
    ax.set_ylim(0, np.max(y) * 1.05)
    title = "Diagramme en violon du problème de synchronisation"
    if problem == 'ph':
        title += " des philosophes"
    elif problem == 'pc':
        title += " des producteurs & consommateurs"
    elif problem == 'rw':
        title += " des lecteurs & écrivains"
    elif problem == 'tas':
        title += " par l'algorithme test-and-set"
    elif problem == 'ttas':
        title += " par l'algorithme test-and-test-and-set"
    elif problem == 'backoff':
        title += " par l'algorithme backoff"
    ax.set_title(title, fontsize=14)

    # violon plots
    vp['cmeans'].set_label("moyennes")
    vp['cmeans'].set_color('indianred')
    for pc in vp['bodies']:
        pc.set_facecolor('cornflowerblue')
        pc.set_edgecolor('royalblue')
    vp['cbars'].set_color('royalblue')
    vp['cbars'].set(zorder=1)
    vp['cmins'].set_color('royalblue')
    vp['cmaxes'].set_color('royalblue')

    # legend
    box = ax.get_position()
    ax.set_position([box.x0, box.y0, box.width * 0.9, box.height])
    ax.legend(loc='center left', bbox_to_anchor=(1.02, 0.5), fancybox=True)


def customization_dual(ax, labels, vp, color1, color2, problem=''):
    # axes and title
    ax.set_xticks(np.arange(1, len(labels) + 1), labels=labels)
    ax.set_xlabel("Nombre de threads " + "$[-]$", fontsize=12)
    ax.set_ylabel("Temps d'exécution " + "$[s]$", fontsize=12)
    title = "Diagramme en violon du problème de synchronisation\n"
    if problem == 'tas':
        title += " par l'algorithme test-and-set"
    elif problem == 'ttas':
        title += " par l'algorithme test-and-test-and-set"
    ax.set_title(title, fontsize=14)

    # violon plots
    vp['cmeans'].set_label("moyennes")
    vp['cmeans'].set_color('indianred')
    for pc in vp['bodies']:
        pc.set_facecolor(color1)
        pc.set_edgecolor(color2)
    vp['cbars'].set_color(color2)
    vp['cbars'].set(zorder=1)
    vp['cmins'].set_color(color2)
    vp['cmaxes'].set_color(color2)


def plot(data, threads, nthreads, problem=''):
    _, ax = plt.subplots(figsize=(9, 5), num="LINFO1252 : plot")

    labels = [str(threads[i]) for i in range(nthreads)]
    y = data
    means = np.mean(y, axis=1)
    stds = np.std(y, axis=1)

    vp = ax.violinplot(y, showmeans=True)
    ax.scatter(np.arange(1, len(labels) + 1), means + stds, 125 * np.ones(nthreads), marker='_', lw=1,
               color='saddlebrown', label="écarts-types")

    customization(ax, labels, y, vp, problem)
    plt.show()


def plot_dual(data1, data2, threads, nthreads):
    _, ax = plt.subplots(nrows=1, ncols=2, figsize=(14, 6), sharey=True, num="LINFO1252 : dual plot")

    labels = [str(threads[i]) for i in range(nthreads)]
    y1 = data1;
    y2 = data2
    means1 = np.mean(y1, axis=1);
    means2 = np.mean(y2, axis=1)
    stds1 = np.std(y1, axis=1);
    stds2 = np.std(y2, axis=1)

    vp1 = ax[0].violinplot(y1, showmeans=True)
    ax[0].scatter(np.arange(1, len(labels) + 1), means1 + stds1, 125 * np.ones(nthreads), marker='_', lw=1,
                  color='saddlebrown', label="écarts-types")

    vp2 = ax[1].violinplot(y2, showmeans=True)
    ax[1].scatter(np.arange(1, len(labels) + 1), means2 + stds2, 125 * np.ones(nthreads), marker='_', lw=1,
                  color='saddlebrown', label="écarts-types")

    customization_dual(ax[0], labels, vp1, 'cornflowerblue', 'royalblue', problem='tas')
    customization_dual(ax[1], labels, vp2, 'mediumpurple', 'purple', problem='ttas')
    for axes in ax:
        axes.set_ylim(0, max(float(np.max(y1)), float(np.max(y2))) * 1.05)

    # legend
    box = ax[1].get_position()
    ax[1].set_position([box.x0, box.y0, box.width * 0.9, box.height])
    ax[1].legend(loc='center left', bbox_to_anchor=(1.05, 0.5), fancybox=True)

    plt.show()


if len(sys.argv) == 2:
    try:
        data = pd.read_csv(sys.argv[1], delimiter=',')
        threads = [1, 2, 4, 8, 16, 32, 64]
        nthreads = len(threads)
        toplot = [list(data[data['nb threads'] == i]['time']) for i in threads]

        if sys.argv[1].__contains__("ph"):
            plot(toplot, threads, nthreads, problem='ph')

        elif sys.argv[1].__contains__("prodcons"):
            plot(toplot, threads, nthreads, problem='pc')

        elif sys.argv[1].__contains__("rw"):
            plot(toplot, threads, nthreads, problem='rw')

        elif sys.argv[1].__contains__("_tas"):
            plot(toplot, threads, nthreads, problem='tas')

        elif sys.argv[1].__contains__("ttas"):
            plot(toplot, threads, nthreads, problem='ttas')

        elif sys.argv[1].__contains__("backoff"):
            plot(toplot, threads, nthreads, problem='backoff')

        else:
            print("Error: Invalid arguments were given")
    except:
        print("Error: Invalid arguments were given")

elif len(sys.argv) == 3:

    try:
        data1 = pd.read_csv(sys.argv[1], delimiter=',')
        data2 = pd.read_csv(sys.argv[2], delimiter=',')
        threads = [1, 2, 4, 8, 16, 32, 64]
        nthreads = len(threads)
        toplot1 = [list(data1[data1['nb threads'] == i]['time']) for i in threads]
        toplot2 = [list(data2[data2['nb threads'] == i]['time']) for i in threads]

        plot_dual(toplot1, toplot2, threads, nthreads)
    except:
        print("Error:Invalid arguments were given")

else:
    print(
        "Error: Invalid arguments were given\n\n### How to use ? ###\n\n>>> $python3 plot.py <file.csv> \tdisplays a simple plot\n\n>>> $python3 plot.py <file1.csv> <file2.csv> \tdisplays a dual plot\n\n@author: victor l.")
