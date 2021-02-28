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

def plot_design(total_xs, total_ys):
  for xs, ys in zip(total_xs, total_ys):
    plt.plot(xs, ys)
  plt.show()

def dump_plain(total_xs, total_ys, path):
  num_poly = len(total_xs)
  with open(path, "w") as f:
    f.write("%d\n" % num_poly)

    for i in range(num_poly):
      f.write( "%d\n" % len(total_xs[i]))

      for x, y in zip(total_xs[i], total_ys[i]):
        f.write("%f %f " % (x, y))
        f.write("\n")

if len(sys.argv) < 2:
  print("usage: python %s <design folder> <design> <output folder>" %
    sys.argv[0])
  quit()

design_folder = sys.argv[1]
design = sys.argv[2]
output_folder = sys.argv[3]

testcases = []
testcases.append(design)

for design in testcases:
  design_path = design_folder + "/" + design + ".mat"
  output_path = output_folder + "/" + design + ".txt"
  print("[+]", design)
  print("deisgn path:", design_path)
  print("output path:", output_path)

  (xs, ys) = process(design_path)
  dump_plain(xs, ys, output_path)

  if len(testcases) == 1:
    plot_design(xs, ys)


