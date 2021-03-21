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
# mpl.rcParams['figure.dpi'] = 300

fig, (ax1, ax2) = plt.subplots(2)
ax1.set_aspect(1)
ax2.set_aspect(1)

for xs, ys in zip(total_xs, total_ys):
  ax1.plot(xs, ys, linewidth=0.3, color="black")
  ax2.plot(xs, ys, linewidth=0.3, color="black")

import os
colors = ['red', 'blue', 'green', 'yellow', 'purple', 'pink']
color_id = 0
dim_color_map = dict()

for file in os.listdir(sim_path):
  path = sim_path + '/' + file
  if path.endswith("txt") == False: continue
  if file.startswith("matlab") == False: continue
  print("Path:", path)
  data, dim = open_sim_result(path)

  if dim not in dim_color_map.keys():
    dim_color_map[dim] = colors[color_id]
    color_id += 1
  color = dim_color_map[dim]

  print("# of position = %d, dim = %d, color = %s" %(len(data), dim, color))

  for i in range(0, len(data), 20):
    circle = plt.Circle(data[i], dim/2.0, fill=False, linewidth=0.1, color=color)
    ax1.add_patch(circle)

for file in os.listdir(sim_path):
  path = sim_path + "/" + file
  if path.endswith("txt") == False: continue
  if file.startswith("cpp") == False: continue
  print("Path:", path)
  data, dim = open_sim_result(path)

  if dim not in dim_color_map.keys():
    dim_color_map[dim] = colors[color_id]
    color_id += 1
  color = dim_color_map[dim]

  print("# of position = %d, dim = %d, color = %s" %(len(data), dim, color))

  for i in range(0, len(data), 20):
    circle = plt.Circle(data[i], dim/2.0, fill=False, linewidth=0.1, color=color)
    ax2.add_patch(circle)

print("Plot\n")
plt.show()

