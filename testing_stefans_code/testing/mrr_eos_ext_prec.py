import matplotlib.pyplot as plt
import numpy as np
import csv

P = 7.55616208*np.power(10., 5.)

Rd = []
Md = []
ed = []
pd = []

with open('mr_eos_com.out', 'rb') as csvfile:
    plots = csv.reader(csvfile, delimiter = ',')
    for row in plots:
        Rd.append(float(row[0]))
        Md.append(float(row[1]))
        ed.append(float(row[2]))
        pd.append(float(row[3]))

edn = [a*P for a in ed]
pdn = [a*P for a in pd]

R11 = []
M11 = []
e11 = []
p11 = []

with open('results_el1_5_p', 'rb') as csvfile:
    plots = csv.reader(csvfile, delimiter = ' ')
    for row in plots:
        R11.append(float(row[1]))
        M11.append(float(row[3]))
        e11.append(float(row[5]))
        p11.append(float(row[4]))

e11n = [a*P for a in e11]
p11n = [a*P for a in p11]

R25 = []
M25 = []
e25 = []
p25 = []

with open('results_el2_5_p', 'rb') as csvfile:
    plots = csv.reader(csvfile, delimiter = ' ')
    for row in plots:
        R25.append(float(row[1]))
        M25.append(float(row[3]))
        e25.append(float(row[5]))
        p25.append(float(row[4]))

e25n = [a*P for a in e25]
p25n = [a*P for a in p25]

R26 = []
M26 = []
e26 = []
p26 = []

with open('results_el2_6_p', 'rb') as csvfile:
    plots = csv.reader(csvfile, delimiter = ' ')
    for row in plots:
        R26.append(float(row[1]))
        M26.append(float(row[3]))
        e26.append(float(row[5]))
        p26.append(float(row[4]))

e26n = [a*P for a in e26]
p26n = [a*P for a in p26]

R27 = []
M27 = []
e27 = []
p27 = []

with open('results_el2_7_p', 'rb') as csvfile:
    plots = csv.reader(csvfile, delimiter = ' ')
    for row in plots:
        R27.append(float(row[1]))
        M27.append(float(row[3]))
        e27.append(float(row[5]))
        p27.append(float(row[4]))

e27n = [a*P for a in e27]
p27n = [a*P for a in p27]

R28 = []
M28 = []
e28 = []
p28 = []

plt.figure(figsize=(7,12))

plt.subplot(211)
plt.xlim(10.6, 13)
plt.ylim(1.6, 2.1)
plt.plot(Rd, Md, label = 'MRR input')
plt.plot(R11, M11, label = 'MRR $\lambda = 0.1$')
#plt.plot(R21, M21, label = 'MRR $\lambda =0.01$')
#plt.plot(R24, M24, label = 'MRR $\lambda =0.04$')
plt.plot(R25, M25, label = 'MRR $\lambda =0.05$')
#plt.plot(R255, M255, label = 'MRR $\lambda =0.055$')
#plt.plot(R257, M257, label = 'MRR $\lambda =0.057$')
plt.plot(R26, M26, label = 'MRR $\lambda =0.06$')
plt.plot(R27, M27, label = 'MRR $\lambda =0.07$')
#plt.plot(R28, M28, label = 'MRR $\lambda =0.08$')
#plt.plot(R31, M31, label = 'MRR $\lambda =0.001$')
plt.ylabel('M/M$_\odot$', fontsize=15)
plt.xlabel('R/km', fontsize=15)
plt.legend(loc=3,prop={'size':12})

plt.subplot(212)
plt.xlim(0, 650)
plt.ylim(400,1300)
plt.plot(pdn, edn, label ='EOS input')
plt.plot(p11n, e11n, label ='EOS $\lambda = 0.1$')
#plt.plot(p21n, e21n, label ='EOS $\lambda = 0.01$')
#plt.plot(p24n, e24n, label ='EOS $\lambda = 0.04$')
plt.plot(p25n, e25n, label ='EOS $\lambda = 0.05$')
#plt.plot(p255n, e255n, label ='EOS $\lambda = 0.055$')
#plt.plot(p257n, e257n, label ='EOS $\lambda = 0.057$')
plt.plot(p26n, e26n, label ='EOS $\lambda = 0.06$')
plt.plot(p27n, e27n, label ='EOS $\lambda = 0.07$')
#plt.plot(p28n, e28n, label ='EOS $\lambda = 0.08$')
#plt.plot(p31n, e31n, label ='EOS $\lambda = 0.001$')
plt.ylabel('$\epsilon$(p)/MeVfm$^{-3}$', fontsize=15)
plt.xlabel('p/MeVfm$^{-3}$', fontsize=15)
plt.legend(loc=4,prop={'size':12})

plt.subplots_adjust(hspace = 0.5)
plt.tight_layout()
plt.show()
#plt.savefig('mrr_eos_ext_upright.pdf')
