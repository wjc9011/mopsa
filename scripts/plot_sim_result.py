import matplotlib.pyplot as plt
import scipy.io
import h5py
import os
import sys

def open_by_h5py(design_path):
  mat = h5py.File(design_path)
  poly = mat['polylines'][()][0]

  # print(type(poly))
  # print(poly.shape)

  data = []
  for i in range(poly.shape[0]):
    data.append(mat[poly[i]].value)

  total_xs = []
  total_ys = []
  for i in range(len(data)):
    tmp = data[i]
    xs = tmp[1, :]
    ys = tmp[2, :]
    # print(tmp, xs, ys)
    total_xs.append(xs)
    total_ys.append(ys)

  return total_xs, total_ys

def open_by_scipy(design_path):

  # print("Open %s by scipy" % design_path)
  data = scipy.io.loadmat(design_path)['polylines']
  # print(data)
  # print(data.shape)

  total_xs = []
  total_ys = []
  for poly in data:
    # print(poly)
    total_xs.append(poly[0][:, 1])
    total_ys.append(poly[0][:, 2])

  return total_xs, total_ys

def process(design_path):

  try:
    return open_by_h5py(design_path)
  except:
    return open_by_scipy(design_path)

def open_sim_result(path):

  data = []
  dima = 0
  with open(path, "r") as fp:
    num, dim = [float(x) for x in fp.readline().split()]
    num = int(num)
    print(num, dim)
    for i in range(num):
      line = fp.readline()
      data.append([float(x) for x in line.split()])

  return data, dim


if len(sys.argv) < 2:
  print("usage: python %s design" %
    sys.argv[0])
  quit()

design_path = "../converted_geometry/" + sys.argv[1] + ".mat"
sim_path    = "../sim_output/" + sys.argv[1]
print("Design:", sys.argv[1])
print("Design path:", design_path)
print("sim output folder", sim_path)

(total_xs, total_ys) = process(design_path)

import matplotlib as mpl
mpl.rcParams['figure.dpi'] = 300

for xs, ys in zip(total_xs, total_ys):
  plt.plot(xs, ys, linewidth=0.1)

import os
colors = ['red', 'blue', 'green', 'yellow']
path_cnt = 0
for file in os.listdir(sim_path + "/matlab/."):
  path = sim_path + "/matlab/" + file
  if path.endswith("txt") == False: continue
  print("Path:", path)
  data, dim = open_sim_result(path)
  print("# of position = %d, dim = %d" %(len(data), dim))

  color = colors[path_cnt]
  for i in range(len(data)):
    circle = plt.Circle(data[i], dim/2.0, fill=False, linewidth=0.1, color=color)
    plt.gca().add_patch(circle)
  path_cnt += 1

# print("Plot\n")
# plt.show()

# path_cnt = 0
for file in os.listdir(sim_path + "/cpp/."):
  path = sim_path + "/cpp/" + file
  if path.endswith("txt") == False: continue
  print("Path:", path)
  data, dim = open_sim_result(path)
  print("# of position = %d, dim = %d" %(len(data), dim))

  color = colors[path_cnt]
  for i in range(len(data)):
    circle = plt.Circle(data[i], dim/2.0, fill=False, linewidth=0.1, color=color)
    plt.gca().add_patch(circle)
  path_cnt += 1

print("Plot\n")
plt.show()

