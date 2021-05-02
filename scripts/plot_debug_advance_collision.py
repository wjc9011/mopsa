import matplotlib.pyplot as plt
import matplotlib as mpl
import os
import sys

if len(sys.argv) < 2:
  print("usage: python %s <file>" % sys.argv[0])
  quit()

mpl.rcParams['figure.dpi'] = 300

plt.gca().set_aspect(1)

with open(sys.argv[1]) as fp:

  # plot particle
  num = int(fp.readline())
  xs = []
  ys = []
  for i in range(num):
    x, y = [float(d) for d in fp.readline().split() ]
    xs.append(x)
    ys.append(y)

  plt.plot(xs, ys, '--', linewidth=0.6, color='black')

  # plot after particle
  num = int(fp.readline())
  xs = []
  ys = []
  for i in range(num):
    x, y = [float(d) for d in fp.readline().split() ]
    xs.append(x)
    ys.append(y)

  plt.plot(xs, ys, linewidth=0.4, color='green')

  # plot after golden particle
  num = int(fp.readline())
  xs = []
  ys = []
  for i in range(num):
    x, y = [float(d) for d in fp.readline().split() ]
    xs.append(x)
    ys.append(y)

  plt.plot(xs, ys, linewidth=0.2, color='red')


  # plot obstacle nodes
  num = int(fp.readline())
  xs = []
  ys = []
  for i in range(num):
    x, y = [float(d) for d in fp.readline().split() ]
    xs.append(x)
    ys.append(y)
  plt.scatter(xs, ys, s=0.1)


  # plot Golden A, B
  golden_collision = int(fp.readline())
  Ax, Ay = [float(d) for d in fp.readline().split()]
  Bx, By = [float(d) for d in fp.readline().split()]
  if golden_collision:
    plt.scatter(Ax, Ay, s=0.5, color = "red")
    plt.scatter(Bx, By, s=0.5, color = "red")

  # plot Adv A, B
  adv_collision = int(fp.readline())
  Ax, Ay = [float(d) for d in fp.readline().split()]
  Bx, By = [float(d) for d in fp.readline().split()]
  if adv_collision:
    plt.scatter(Ax, Ay, s=0.5, color = "green")
    plt.scatter(Bx, By, s=0.5, color = "green")

plt.show()

