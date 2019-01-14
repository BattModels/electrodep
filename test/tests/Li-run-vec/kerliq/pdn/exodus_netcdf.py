from __future__ import print_function
import netCDF4
import numpy as np
#import matplotlib.pyplot as plt
from glob import glob


def time_transf(f, tstart=550):
    nc = netCDF4.Dataset(f)
    print('Read exodus file '+f+'\n')
    eta_on_nodes = nc.variables['vals_nod_var3']
    nsteps = eta_on_nodes.shape[0]
    if(nsteps<550):
        print('No data for t>550')
        return -2.0, -2.0, -2.0
    print('nsteps={}\n'.format(nsteps))
    X = nc.variables['coordx']
    Y = nc.variables['coordy']
    time_whole = nc.variables['time_whole'][:]
    Xval = 150 #time of phase transf at x=Xval                                                                   
    for t in range(550, nsteps):
        print('At tstep={}'.format(t))
        line_inds = np.where(X[:]==Xval)[0]
        if any(eta_on_nodes[t, line_inds]>0.5):
            print('Phase transf at: {}'.format(t))
            #Calculating exact time assuming constant velocity
            inds = np.extract(eta_on_nodes[t, line_inds]>0.5,line_inds)
            t_exact_min = time_whole[-1]
            for ind in inds:
                tw = time_whole[t]
                tw_old = time_whole[t-1]
                t_exact = tw_old + (0.5-eta_on_nodes[t-1,ind])/(eta_on_nodes[t,ind]-eta_on_nodes[t-1,ind])*(tw-tw_old)
                if(t_exact<t_exact_min):
                    t_exact_min = t_exact           
            return t,time_whole[t],t_exact_min
    return -1.0,-1.0,-1.0

def time_transf_old(f, tstart=550):
    nc = netCDF4.Dataset(f)
    print('Read exodus file '+f+'\n')
    eta_on_nodes = nc.variables['vals_nod_var1']
    nsteps = eta_on_nodes.shape[0]
    if(nsteps<550):
        print('No data for t>550')
        return (0.0, 0.0)
    print('nsteps={}\n'.format(nsteps))
    X = nc.variables['coordx']
    Y = nc.variables['coordy']
    time_whole = nc.variables['time_whole'][:]
    Xval = 150 #time of phase transf at x=Xval
    
    for t in range(550, nsteps):
        print('At tstep={}'.format(t))
        eta_on_nodest = eta_on_nodes[t,:]
        if any(eta_on_nodest[X[:]==Xval]>0.5):
            print('Phase transf at: {}'.format(t))
            
            return t,time_whole[t]
#            break

files = glob('*.e')
times = []
ps = []
cs = []
for f in files:
    ps.append(float(f[13:20]))
    cs.append(float(f[23:30]))
    t, t_whole, t_exact = time_transf(f)
    times.append(t_exact)

print(ps)
print(cs)
print(times)

with open('exod_res.pkl', 'wb') as f:  
    pickle.dump([ps, cs, times], f)
