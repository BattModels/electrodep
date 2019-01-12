from __future__ import print_function
import netCDF4
import numpy as np
import matplotlib.pyplot as plt

nc = netCDF4.Dataset('liqcrys_K0_other.e')
print('Read exodus file')
eta_on_nodes = nc.variables['vals_nod_var1']
nsteps = eta_on_nodes.shape[0]
X = nc.variables['coordx']
Y = nc.variables['coordy']

Xval = 150 #time of phase transf at x=Xval

for t in range(550, nsteps):
    print('At tstep={}'.format(t))
    eta_on_nodest = eta_on_nodes[t,:]
    if any(eta_on_nodest[X[:]==Xval]>0.5):
        print('Phase transf at: {}'.format(t))
        break
