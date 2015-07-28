#!/bin/python
import numpy             as np
import matplotlib.pyplot as plt

np.random.seed(5)
x = np.arange(1, 101)
y = 20 + 3 * x + np.random.normal(0, 60, 100)
plt.plot(x, y, "o")

plt.plot([70, 70], [100, 250], 'k-', lw=2)
plt.plot([70, 90], [90, 200], 'k-')

plt.show()
