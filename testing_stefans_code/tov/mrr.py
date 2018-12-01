import matplotlib.pyplot as plt
import numpy as np
import csv

P = 7.55616208*np.power(10., 5.)

R25 = []
M25 = []
e25 = []
p25 = []

with open('mrr_l2_5', 'rb') as csvfile:
    plots = csv.reader(csvfile, delimiter = ',')
    for row in plots:
        R25.append(float(row[0]))
        M25.append(float(row[1]))
        e25.append(float(row[2]))
        p25.append(float(row[3]))

e25n = [a*P for a in e25]
p25n = [a*P for a in p25]

R28 = []
M28 = []
e28 = []
p28 = []

with open('mrr_l2_8', 'rb') as csvfile:
    plots = csv.reader(csvfile, delimiter = ',')
    for row in plots:
        R28.append(float(row[0]))
        M28.append(float(row[1]))
        e28.append(float(row[2]))
        p28.append(float(row[3]))

e28n = [a*P for a in e28]
p28n = [a*P for a in p28]

plt.figure(figsize=(8,4))

plt.subplot(2,2,(1,3))
#plt.xlim(11.5, 14)
#plt.ylim(1.1, 1.2)
#plt.plot(R25, M25, label = 'MRR $\lambda = 0.05$')
plt.plot(R28, M28, label = 'MRR $\lambda = 0.08$')
plt.ylabel('M/M$_\odot$', fontsize=15)
plt.xlabel('R/km', fontsize=15)
plt.legend(loc=2,prop={'size':14})

plt.subplot(2,2,(2,4))
#plt.xlim(0, 200)
#plt.plot(p25n, e25n, label ='EOS $\lambda = 0.05$')
plt.plot(p28n, e28n, label ='EOS $\lambda = 0.08$')
plt.ylabel('$\epsilon$(p)/MeVfm$^{-3}$', fontsize=15)
plt.xlabel('p/MeVfm$^{-3}$', fontsize=15)
plt.legend(loc=2,prop={'size':14})

plt.tight_layout()
plt.show()

