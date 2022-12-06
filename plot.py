import sys
import argparse
import matplotlib.pyplot as plt
import pandas as pd
import numpy as np
import seaborn as sn

sn.set_theme()
plt.rcParams["mathtext.fontset"] = 'cm'
plt.rcParams['font.family'] = 'Times New Roman'

def problem_matcher(filename):
    if filename.__contains__("ph"):
        return 'ph'
    elif filename.__contains__("prodcons"):
        return 'pc'
    elif filename.__contains__("rw"):
        return 'rw'
    elif filename.__contains__("_tas"):
        return 'tas'
    elif filename.__contains__("_ttas"):
        return 'ttas'
    elif filename.__contains__("backoff"):
        return 'backoff'
    else:
        return None

def name_matcher(problem):
    if problem == 'ph':
        return " des philosophes"
    elif problem == 'pc':
        return" des producteurs & consommateurs"
    elif problem == 'rw':
        return " des lecteurs & écrivains"
    elif problem == 'tas':
        return " par l'algorithme test-and-set"
    elif problem == 'ttas':
        return " par l'algorithme test-and-test-and-set"
    elif problem == 'backoff':
        return " par l'algorithme backoff test-and-test-and-set"
    else:
        return ""

def customization(ax, labels, y, vp, problem):
    # axes and title
    ax.set_xticks(np.arange(1, len(labels) + 1), labels=labels)
    ax.set_xlabel("Nombre de threads " + "$[-]$", fontsize=12)
    ax.set_ylabel("Temps d'exécution " + "$[s]$", fontsize=12)
    ax.set_ylim(0, np.max(y) * 1.05)
    title = "Diagramme en violon du problème de synchronisation\n"
    title += name_matcher(problem)
    ax.set_title(title, fontsize=14)

    # violon plots
    vp['cmeans'].set_label("moyennes")
    vp['cmeans'].set_color('navy')
    for pc in vp['bodies']:
        pc.set_facecolor('cornflowerblue')
        pc.set_edgecolor('royalblue')
    vp['cbars'].set_color('royalblue')
    vp['cbars'].set(zorder=1)
    vp['cmins'].set_color('royalblue')
    vp['cmaxes'].set_color('royalblue')

    # legend
    #box = ax.get_position()
    #ax.set_position([box.x0, box.y0, box.width * 0.9, box.height])
    #ax.legend(loc='center left', bbox_to_anchor=(1.02, 0.5), fancybox=True)
    ax.legend(loc='upper left', fontsize=12)


def customization_dual(ax, labels, vp, color1, color2, problem):
    # axes and title
    ax.set_xticks(np.arange(1, len(labels) + 1), labels=labels)
    ax.set_xlabel("Nombre de threads " + "$[-]$", fontsize=12)
    ax.set_ylabel("Temps d'exécution " + "$[s]$", fontsize=12)
    title = "Diagramme en violon du problème de synchronisation\n"
    title += name_matcher(problem)
    ax.set_title(title, fontsize=14)

    # violon plots
    vp['cmeans'].set_label("moyennes")
    vp['cmeans'].set_color('navy')
    for pc in vp['bodies']:
        pc.set_facecolor(color1)
        pc.set_edgecolor(color2)
    vp['cbars'].set_color(color2)
    vp['cbars'].set(zorder=1)
    vp['cmins'].set_color(color2)
    vp['cmaxes'].set_color(color2)

def customization_merged(ax, labels, rects1, rects2, problem):
    # axes and title
    ax.set_xticks(np.arange(len(labels)), labels=labels)
    ax.set_xlabel("Nombre de threads " + "$[-]$", fontsize=12)
    ax.set_ylabel("Temps d'exécution moyens" + "$[s]$", fontsize=12)
    title = "Histogrammes comparatifs des performances\ndu problème de synchronisation"
    title += name_matcher(problem)
    ax.set_title(title, fontsize=14)
    
    # bar plots
    for r1, r2 in zip(rects1.patches, rects2.patches):
        r1.set(facecolor='cornflowerblue', edgecolor='royalblue', alpha=0.85)
        r2.set(facecolor='mediumpurple', edgecolor='purple', alpha=0.85)

        height = r1.get_height()
        ax.annotate('{:.2f}'.format(height),
                    xy=(r1.get_x() + r1.get_width() / 2, height),
                    xytext=(0, 3),  # 3 points vertical offset
                    textcoords="offset points",
                    ha='center', va='bottom')
        height = r2.get_height()
        ax.annotate('{:.2f}'.format(height),
                    xy=(r2.get_x() + r2.get_width() / 2, height),
                    xytext=(0, 3),  # 3 points vertical offset
                    textcoords="offset points",
                    ha='center', va='bottom')

    # legend
    #box = ax.get_position()
    #ax.set_position([box.x0, box.y0, box.width * 0.9, box.height])
    #ax.legend(loc='center left', bbox_to_anchor=(1.02, 0.5), fancybox=True)
    ax.legend(loc='upper right')

def customization_merged3(ax, labels, rects1, rects2, rects3, title, pos):
    sz = 12
    # axes and title
    ax.set_xticks(np.arange(len(labels))*gap, labels=labels)
    ax.set_xlabel("Nombre de threads " + "$[-]$", fontsize=sz)
    ax.set_ylabel("Temps d'exécution moyens " + "$[s]$", fontsize=sz)
    ax.set_title(title, fontsize=sz+2)
    
    # bar plots
    for r1, r2, r3 in zip(rects1.patches, rects2.patches, rects3.patches):
        r1.set(facecolor='skyblue', edgecolor='deepskyblue', alpha=0.9)
        r2.set(facecolor='mediumslateblue', edgecolor='slateblue', alpha=0.85)
        r3.set(facecolor='blueviolet', edgecolor='darkviolet', alpha = 0.85)

        height = r1.get_height()
        ax.annotate('{:.2f}'.format(height),
                    xy=(r1.get_x() + r1.get_width() / 2, height),
                    xytext=(0, 3),  # 3 points vertical offset
                    textcoords="offset points",
                    ha='center', va='bottom')
        height = r2.get_height()
        ax.annotate('{:.2f}'.format(height),
                    xy=(r2.get_x() + r2.get_width() / 2, height),
                    xytext=(0, 3),  # 3 points vertical offset
                    textcoords="offset points",
                    ha='center', va='bottom')
        height = r3.get_height()
        ax.annotate('{:.2f}'.format(height),
                    xy=(r3.get_x() + r3.get_width() / 2, height),
                    xytext=(0, 3),  # 3 points vertical offset
                    textcoords="offset points",
                    ha='center', va='bottom')

    # legend
    #box = ax.get_position()
    #ax.set_position([box.x0, box.y0, box.width * 0.9, box.height])
    #ax.legend(loc='center left', bbox_to_anchor=(1.02, 0.5), fancybox=True)
    ax.legend(loc=pos, fontsize=sz-1)

def customization_merged3s(ax, labels, rects1, rects2, rects3, srects1, srects2, srects3, pos):
    sz = 12
    # axes and title
    axes = ax
    ax = axes[0]
    ax.set_xticks((np.arange(len(labels))*gap)[:-1], labels=labels[:-1])
    ax.set_xlabel("Nombre de threads " + "$[-]$", fontsize=sz)
    ax.set_ylabel("Temps d'exécution moyens " + "$[s]$", fontsize=sz)
    
    # bar plots
    for r1, r2, r3 in zip(rects1.patches, rects2.patches, rects3.patches):
        r1.set(facecolor='skyblue', edgecolor='deepskyblue', alpha=0.9)
        r2.set(facecolor='mediumslateblue', edgecolor='slateblue', alpha=0.85)
        r3.set(facecolor='blueviolet', edgecolor='darkviolet', alpha = 0.85)

        height = r1.get_height()
        ax.annotate('{:.2f}'.format(height),
                    xy=(r1.get_x() + r1.get_width() / 2, height),
                    xytext=(0, 3),  # 3 points vertical offset
                    textcoords="offset points",
                    ha='center', va='bottom')
        height = r2.get_height()
        ax.annotate('{:.2f}'.format(height),
                    xy=(r2.get_x() + r2.get_width() / 2, height),
                    xytext=(0, 3),  # 3 points vertical offset
                    textcoords="offset points",
                    ha='center', va='bottom')
        height = r3.get_height()
        ax.annotate('{:.2f}'.format(height),
                    xy=(r3.get_x() + r3.get_width() / 2, height),
                    xytext=(0, 3),  # 3 points vertical offset
                    textcoords="offset points",
                    ha='center', va='bottom')

    # legend
    ax.legend(loc=pos, fontsize=sz-1)

    ax = axes[1]
    ax.set_xticks([(np.arange(len(labels))*gap)[-1]], labels=[labels[-1]])
    for r1, r2, r3 in zip(srects1.patches, srects2.patches, srects3.patches):
        r1.set(facecolor='skyblue', edgecolor='deepskyblue', alpha=0.9)
        r2.set(facecolor='mediumslateblue', edgecolor='slateblue', alpha=0.85)
        r3.set(facecolor='blueviolet', edgecolor='darkviolet', alpha = 0.85)

        height = r1.get_height()
        ax.annotate('{:.2f}'.format(height),
                    xy=(r1.get_x() + r1.get_width() / 2, height),
                    xytext=(0, 3),  # 3 points vertical offset
                    textcoords="offset points",
                    ha='center', va='bottom')
        height = r2.get_height()
        ax.annotate('{:.2f}'.format(height),
                    xy=(r2.get_x() + r2.get_width() / 2, height),
                    xytext=(0, 3),  # 3 points vertical offset
                    textcoords="offset points",
                    ha='center', va='bottom')
        height = r3.get_height()
        ax.annotate('{:.2f}'.format(height),
                    xy=(r3.get_x() + r3.get_width() / 2, height),
                    xytext=(0, 3),  # 3 points vertical offset
                    textcoords="offset points",
                    ha='center', va='bottom')


def plot(y, threads, problem=''):
    fig, ax = plt.subplots(figsize=(6, 7.2), dpi=125, num="simple_plot_"+problem)
    fig.tight_layout(pad=3.0)

    nthreads = len(threads)
    labels = [str(threads[i]) for i in range(nthreads)]
    means = np.mean(y, axis=1)
    stds = np.std(y, axis=1)

    ax.errorbar(np.arange(1, len(labels) + 1), means, yerr=stds, fmt='none', ecolor= 'red', linewidth=2, alpha=0.66, label="écarts types")
    vp = ax.violinplot(y, showmeans=True)
    customization(ax, labels, y, vp, problem)

    plt.show()

def plot_dual(y1, y2, threads1, threads2, problem1='', problem2=''):
    _, ax = plt.subplots(nrows=1, ncols=2, figsize=(14, 6), sharey=True, num="dual_plot_"+problem1+problem2)

    nthreads1 = len(threads1)
    nthreads2 = len(threads2)
    labels1 = [str(threads1[i]) for i in range(nthreads1)]
    labels2 = [str(threads2[i]) for i in range(nthreads2)]
    means1 = np.mean(y1, axis=1)
    means2 = np.mean(y2, axis=1)
    stds1 = np.std(y1, axis=1)
    stds2 = np.std(y2, axis=1)

    vp1 = ax[0].violinplot(y1, showmeans=True)
    ax[0].scatter(np.arange(1, len(labels1) + 1), means1 + stds1, 125 * np.ones(nthreads1), marker='_', lw=1,
                  color='saddlebrown', label="écarts-types")

    vp2 = ax[1].violinplot(y2, showmeans=True)
    ax[1].scatter(np.arange(1, len(labels2) + 1), means2 + stds2, 125 * np.ones(nthreads2), marker='_', lw=1,
                  color='saddlebrown', label="écarts-types")

    customization_dual(ax[0], labels1, vp1, 'cornflowerblue', 'royalblue', problem=problem1)
    customization_dual(ax[1], labels2, vp2, 'mediumpurple', 'purple', problem=problem2)
    for axes in ax:
        axes.set_ylim(0, max(float(np.max(y1)), float(np.max(y2))) * 1.05)

    # legend
    box = ax[1].get_position()
    ax[1].set_position([box.x0, box.y0, box.width * 0.9, box.height])
    ax[1].legend(loc='center left', bbox_to_anchor=(1.05, 0.5), fancybox=True)

    plt.show()

def plot_merged(y1, y2, threads, problem1='', problem2=''):
    fig, ax = plt.subplots(figsize=(10, 10), num="merged_plot_"+problem1+problem2)

    nthreads = len(threads)
    labels = [str(threads[i]) for i in range(nthreads)]
    means1 = np.mean(y1, axis=1)
    means2 = np.mean(y2, axis=1)
    stds1 = np.std(y1, axis=1)
    stds2 = np.std(y2, axis=1)

    x = np.arange(nthreads) # label locations
    width = 0.35 # widths of the bars
    eargs = {'ecolor':'red', 'alpha':0.3}
    rects1 = ax.bar(x - width/2, means1, width, yerr=stds1, error_kw=eargs, label='POSIX')
    rects2 = ax.bar(x + width/2, means2, width, yerr=stds2, error_kw=eargs, label='Attente active')

    customization_merged(ax, labels, rects1, rects2, problem1)
    ax.set_ylim(0, max(float(np.max(means1+stds1)), float(np.max(means2+stds2))) * 1.05)
    plt.show()

gap = 3.2

def plot_merged3(y1, y2, y3, threads, problem=''):
    fig, ax = plt.subplots(figsize=(9.6, 4), dpi=125, num="trimerged_plot_"+problem)
    fig.tight_layout(pad=3.0)

    nthreads = len(threads)
    labels = [str(threads[i]) for i in range(nthreads)]
    means1 = np.mean(y1, axis=1)
    means2 = np.mean(y2, axis=1)
    means3 = np.mean(y3, axis=1)
    stds1 = np.std(y1, axis=1)
    stds2 = np.std(y2, axis=1)
    stds3 = np.std(y3, axis=1)

    x = np.arange(nthreads)*gap # label locations
    width = 0.75 # widths of the bars
    eargsl = {'ecolor':'red','label':'écarts types','alpha':0.4}
    eargs = {'ecolor':'red','alpha':0.4}
    if 0 == len(problem): 
        label = ['TAS', 'TTAS', 'backoff-TTAS']
        title = "Histogrammes des performances des algorithmes d'attente active"
        pos = 'lower right'
    else:
        label = ['POSIX', 'Attente active (TAS)', 'Attente active (TTAS)']
        title = "Histogrammes des performances du problème de synchronisation\n" + name_matcher(problem).strip()
        pos = 'upper left'
    rects1 = ax.bar(x - width*5/4, means1, width, yerr=stds1, error_kw=eargsl, label=label[0])
    rects2 = ax.bar(x, means2, width, yerr=stds2, error_kw=eargs, label=label[1])
    rects3 = ax.bar(x + width*5/4, means3, width, yerr=stds3, error_kw=eargs, label=label[2])

    customization_merged3(ax, labels, rects1, rects2, rects3, title, pos)
    ax.set_ylim(0, max(max(float(np.max(means1+stds1)), float(np.max(means2+stds2))), max(float(np.max(means1+stds1)), float(np.max(means3+stds3)))) * 1.05)
    plt.show()

def plot_merged3s(y1, y2, y3, threads, problem=''):
    fig, ax = plt.subplots(nrows=1, ncols=2, gridspec_kw={'width_ratios':[5,1]}, figsize=(9.6, 4), dpi=125, num="trimerged_plot_"+problem)
    fig.tight_layout(pad=3.0)

    nthreads = len(threads)
    labels = [str(threads[i]) for i in range(nthreads)]
    means1 = np.mean(y1, axis=1)
    means2 = np.mean(y2, axis=1)
    means3 = np.mean(y3, axis=1)
    stds1 = np.std(y1, axis=1)
    stds2 = np.std(y2, axis=1)
    stds3 = np.std(y3, axis=1)

    x = np.arange(nthreads)*gap # label locations
    width = 0.75 # widths of the bars
    eargsl = {'ecolor':'red','label':'écarts types','alpha':0.5}
    eargs = {'ecolor':'red','alpha':0.5}
    if 0 == len(problem): 
        label = ['TAS', 'TTAS', 'backoff-TTAS']
        title = "Histogrammes des performances des algorithmes d'attente active"
        pos = 'lower right'
    else:
        label = ['POSIX', 'Attente active (TAS)', 'Attente active (TTAS)']
        title = "Histogrammes des performances du problème de synchronisation" + name_matcher(problem)
        if problem == 'ph':
            pos = 'upper left'
        else:
            pos = 'upper right'
    rects1 = ax[0].bar(x[:-1] - width*5/4, means1[:-1], width, yerr=stds1[:-1], error_kw=eargsl, label=label[0])
    rects2 = ax[0].bar(x[:-1], means2[:-1], width, yerr=stds2[:-1], error_kw=eargs, label=label[1])
    rects3 = ax[0].bar(x[:-1] + width*5/4, means3[:-1], width, yerr=stds3[:-1], error_kw=eargs, label=label[2])

    srects1 = ax[1].bar(x[-1] - width*5/4, means1[-1], width, yerr=stds1[-1], error_kw=eargsl, label=label[0])
    srects2 = ax[1].bar(x[-1], means2[-1], width, yerr=stds2[-1], error_kw=eargs, label=label[1])
    srects3 = ax[1].bar(x[-1] + width*5/4, means3[-1], width, yerr=stds3[-1], error_kw=eargs, label=label[2])

    customization_merged3s(ax, labels, rects1, rects2, rects3, srects1, srects2, srects3, pos)
    ax[0].set_ylim(0, max(max(float(np.max(means1[:-1]+stds1[:-1])), float(np.max(means2[:-1]+stds2[:-1]))), max(float(np.max(means1[:-1]+stds1[:-1])), float(np.max(means3[:-1]+stds3[:-1])))) * 1.1)
    ax[1].set_ylim(0, max(max(float(np.max(means1[-1]+stds1[-1])), float(np.max(means2[-1]+stds2[-1]))), max(float(np.max(means1[-1]+stds1[-1])), float(np.max(means3[-1]+stds3[-1])))) * 1.1)
    fig.suptitle(title)
    plt.show()

if __name__ == "__main__":

    # parsing command-line arguments
    parser = argparse.ArgumentParser()
    parser.add_argument('file_csv', type=str, help='a csv file to plot')
    parser.add_argument('-d', '--dual_plot', metavar='file_csv2', type=str, required=False, help='a second csv file to plot as dual plot')
    parser.add_argument('-m', '--merged_plot', metavar='file_csv2', type=str, required=False, help='a second csv file to plot as merged plot')
    parser.add_argument('-m3', '--merged_plot3', metavar=('file_csv2', 'file_csv3'), nargs=2, type=str, required=False, help='two other csv files to plot as merged plot')
    parser.add_argument('-m3s', '--merged_plot3split', metavar=('file_csv2', 'file_csv3'), nargs=2, type=str, required=False, help='two other csv files to plot as merged plot with a splitted component')

    args = parser.parse_args()
 
    # launching plots
    try:
        csv1 = pd.read_csv(args.file_csv)
    except:
        print("Error: Cannot open csv file")
        quit()
    threads1 = sorted(list(set(csv1['nb threads'])))
    toplot1 = [list(csv1[csv1['nb threads'] == i]['time']) for i in threads1]
    prob1 = problem_matcher(args.file_csv)
    if args.dual_plot is not None:
        try:
            csv2 = pd.read_csv(args.dual_plot)
        except:
           print("Error: Cannot open csv file") 
           quit()
        # do dual plot
        threads2 = sorted(list(set(csv2['nb threads'])))
        toplot2 = [list(csv2[csv2['nb threads'] == i]['time']) for i in threads2]
        prob2 = problem_matcher(args.dual_plot)
        if prob1 is not None and prob2 is not None:    
            plot_dual(toplot1, toplot2, threads1, threads2, problem1=prob1, problem2=prob2)
        elif prob1 is not None:
            plot_dual(toplot1, toplot2, threads1, threads2, problem1=prob1)
        elif prob2 is not None:
            plot_dual(toplot1, toplot2, threads1, threads2, problem2=prob2)
        else:
            plot_dual(toplot1, toplot2, threads1, threads2)
    elif args.merged_plot is not None:
        try:
            csv3 = pd.read_csv(args.merged_plot)
        except:
           print("Error: Cannot open csv file") 
           quit() 
        # do merged plot
        threads3 = sorted(list(set(csv3['nb threads'])))
        if threads1 != threads3:
            print("Error: Number of threads must be the same")
            quit()
        else:
            toplot3 = [list(csv3[csv3['nb threads'] == i]['time']) for i in threads3]
            prob3 = problem_matcher(args.merged_plot)
            if prob1 is not None and prob3 is not None:    
                plot_merged(toplot1, toplot3, threads1, problem1=prob1, problem2=prob3)
            elif prob1 is not None:
                plot_merged(toplot1, toplot3, threads1, problem1=prob1)
            elif prob3 is not None:
                plot_merged(toplot1, toplot3, threads1, problem2=prob3)
            else:
                plot_merged(toplot1, toplot3, threads1)
    elif args.merged_plot3 is not None:
        try:
            csv3 = pd.read_csv(args.merged_plot3[0])
            csv4 = pd.read_csv(args.merged_plot3[1])
        except:
            print("Error: Cannot open csv file") 
            quit() 
        # do trimerged plot
        threads3 = sorted(list(set(csv3['nb threads'])))
        threads4 = sorted(list(set(csv4['nb threads'])))
        if threads1 != threads3 or threads1 != threads4:
            print("Error: Number of threads must be the same")
            quit()
        else:
            toplot3 = [list(csv3[csv3['nb threads'] == i]['time']) for i in threads3]
            prob3 = problem_matcher(args.merged_plot3[0])
            toplot4 = [list(csv4[csv4['nb threads'] == i]['time']) for i in threads4]
            prob4 = problem_matcher(args.merged_plot3[1])
            if prob1 == prob3 and prob1 == prob4:
                # let's assume csv's are in good order i.e. TAS then TTAS then BACKOFF
                plot_merged3(toplot1, toplot3, toplot4, threads1, problem=prob1)
            else:
                # same assumption
                plot_merged3(toplot1, toplot3, toplot4, threads1)

    elif args.merged_plot3split is not None:
        try:
            csv3 = pd.read_csv(args.merged_plot3split[0])
            csv4 = pd.read_csv(args.merged_plot3split[1])
        except:
            print("Error: Cannot open csv file") 
            quit() 
        # do splitted trimerged plot
        threads3 = sorted(list(set(csv3['nb threads'])))
        threads4 = sorted(list(set(csv4['nb threads'])))
        if threads1 != threads3 or threads1 != threads4:
            print("Error: Number of threads must be the same")
            quit()
        else:
            toplot3 = [list(csv3[csv3['nb threads'] == i]['time']) for i in threads3]
            prob3 = problem_matcher(args.merged_plot3split[0])
            toplot4 = [list(csv4[csv4['nb threads'] == i]['time']) for i in threads4]
            prob4 = problem_matcher(args.merged_plot3split[1])
            if prob1 == prob3 and prob1 == prob4:
                # let's assume csv's are in good order i.e. TAS then TTAS then BACKOFF
                plot_merged3s(toplot1, toplot3, toplot4, threads1, problem=prob1)
            else:
                # same assumption
                plot_merged3s(toplot1, toplot3, toplot4, threads1)
    else:
        # do simple plot
        if prob1 is not None:
            plot(toplot1, threads1, problem=prob1)
        else:
            plot(toplot1, threads1)