import pandas as pd
import numpy as np

values = pd.read_csv('../../measures/measure_ttastest.csv')
maxes = values['max'].values
maxes = maxes[maxes != 0]
mean = np.mean(maxes)

print(int(mean))
