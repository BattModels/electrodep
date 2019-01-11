import yt
import numpy as np
import matplotlib.pyplot as plt

Xline = 150
#calcualting time when the transformation happens at this x=Xline

for tstep in range(550,700):
    print('At time step:{}\n'.format(tstep))
    ds = yt.load('liqcrys_K0_other.e',tstep)
    ad = ds.all_data()
    print(ad['connect1', 'eta'].shape)
    etam = np.mean(ad['connect1', 'eta'], axis=1)
    etam = etam.reshape((200,200))

    x = np.arange(0, 200)
    y = np.arange(0, 200)
#    print(etam[Xline,:])
    if np.any(etam[:,Xline]>0.5):
        print('Transformed at time step: {}\n'.format(tstep))
        print('Transformed at actual time: {}\n'.format(ds.current_time))
        # Making a contour plot to confirm
        X, Y = np.meshgrid(x, y)
        plt.contourf(X, Y, etam, 20, cmap='RdGy')
        plt.colorbar()
        plt.show()
        break
