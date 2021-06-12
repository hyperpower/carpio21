#!/usr/bin/env python
# coding: utf-8

import matplotlib.pyplot as plt

# %matplotlib inline

cblue = "#4285F4"

ax = plt.axes()
ax.set_xlim([-1.0, 1.0])
ax.set_ylim([-1.0, 1.0])

ax.arrow(0, 0, 0.5,  0.5, width=0.02, head_width=0.08, head_length=0.15, fc=cblue, ec=cblue)
ax.arrow(0, 0, 0.4, -0.5, width=0.02, head_width=0.08, head_length=0.15, fc=cblue, ec=cblue)

x1 = [1.0, -1.5]
y1 = [1.0, -1.5]
plt.plot(x1, y1, "--", c="grey")

x0 = [-0.8,   0.8]
y0 = [ 1.0,  -1.0]
plt.plot(x0, y0, "--", c="grey")

vt0 = [0.25, 0.25]
vt1 = [0.0,  0.6]

plt.plot([vt0[0], vt1[0]], [vt0[1], vt1[1]], ".", c="grey")

ax.arrow(vt0[0], vt0[1], vt1[0] - vt0[0], vt1[1] - vt0[1], width=0.02, head_width=0.08, head_length=0.15, fc=cblue, ec=cblue)


plt.annotate("$e_0$", [0.4, -0.39], fontsize=16, ha='left', va='center', color=cblue)
plt.annotate("$e_1$", [0.45, 0.65], fontsize=16, ha='left', va='center', color=cblue)

plt.annotate("$t_0$", vt0, fontsize=16, ha='left', va='bottom', color="k")
plt.annotate("$t_1$", vt1, fontsize=16, ha='left', va='bottom', color="k")

plt.axis('off')

# plt.show()
# plt.savefig("./source/writing/1-ttintersection/fig3_tt.png")
plt.savefig("./fig3_tt.png")
