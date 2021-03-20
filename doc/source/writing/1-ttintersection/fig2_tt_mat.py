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

plt.annotate("0 (0,0)", [0.0,0.05], fontsize=16, ha='left', va='bottom', color="black",
              bbox=dict(boxstyle='round,pad=0.2', fc="white",alpha=1.0))
plt.annotate("1 (+,0)", [0.6,0.7], fontsize=16,ha='left', va='center', color="black",
              bbox=dict(boxstyle='round,pad=0.2', fc="white",alpha=1.0))
plt.annotate("2 (+,+)", [0.0,0.6], fontsize=16,ha='left', va='center', color="black",
              bbox=dict(boxstyle='round,pad=0.2', fc="white",alpha=1.0))
plt.annotate("3 (0,+)", [-0.5,0.6], fontsize=16,ha='center', va='center', color="black",
              bbox=dict(boxstyle='round,pad=0.2', fc="white",alpha=1.0))
plt.annotate("4 (-,+)", [-0.5,-0.0], fontsize=16,ha='center', va='center', color="black",
              bbox=dict(boxstyle='round,pad=0.2', fc="white",alpha=1.0))
plt.annotate("5 (-,0)", [-0.5,-0.5], fontsize=16,ha='center', va='center', color="black",
              bbox=dict(boxstyle='round,pad=0.2', fc="white",alpha=1.0))
plt.annotate("6 (-,-)", [-0.0,-0.5], fontsize=16,ha='left', va='center', color="black",
              bbox=dict(boxstyle='round,pad=0.2', fc="white",alpha=1.0))
plt.annotate("7 (0,-)", [0.7,-0.85], fontsize=16,ha='center', va='center', color="black",
              bbox=dict(boxstyle='round,pad=0.2', fc="white",alpha=1.0))
plt.annotate("8 (+,-)", [0.7,0.0], fontsize=16,ha='center', va='center', color="black",
              bbox=dict(boxstyle='round,pad=0.2', fc="white",alpha=1.0))

plt.annotate("$e_0$", [0.4, -0.39], fontsize=16, ha='left', va='center', color=cblue)
plt.annotate("$e_1$", [0.45, 0.65], fontsize=16, ha='left', va='center', color=cblue)

plt.axis('off')

# plt.show()
plt.savefig("./source/writing/1-ttintersection/fig2_tt.png")
