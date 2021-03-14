import matplotlib.pyplot as plt
import matplotlib as mpl
import scipy.io
import h5py
import os
import sys

if len(sys.argv) < 2:
  print("usage: python %s <file>" % sys.argv[0])
  quit()

mpl.rcParams['figure.dpi'] = 300

plt.gca().set_aspect(1)
with open(sys.argv[1]) as fp:
  while True:
    line = fp.readline()
    if line == "": break

    xs = []
    ys = []
    for i in range(int(line)):
      x, y = [float(d) for d in fp.readline().split() ]
      xs.append(x)
      ys.append(y)

    plt.plot(xs, ys, linewidth=0.1)

plt.show()

