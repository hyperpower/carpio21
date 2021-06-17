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


# ax.arrow(vt0[0], vt0[1], vt1[0] - vt0[0], vt1[1] - vt0[1], width=0.02, head_width=0.08, head_length=0.15, fc=cblue, ec=cblue)


plt.annotate("$e_0$", [0.4, -0.39], fontsize=16, ha='left', va='center', color=cblue)
plt.annotate("$e_1$", [0.45, 0.65], fontsize=16, ha='left', va='center', color=cblue)



def plot_vt(plt, vt1, vt2):
    plt.plot([vt1[0], vt2[0]], [vt1[1], vt2[1]], "--", c="k", alpha = 0.8)
    plt.scatter([vt1[0], vt2[0]], [vt1[1], vt2[1]], s=40, c="b", alpha=0.9)
    plt.annotate("$t_0$", vt1, fontsize=16, ha='left', va='bottom', color="k")
    plt.annotate("$t_1$", vt2, fontsize=16, ha='left', va='bottom', color="k")
# plt.annotate("1", [0.55, -0.49], fontsize=16, ha='left', va='bottom', color="k")
# region label
def plot_region_label(plt):
    labels = [
        [-0.1, 0], 
        [0.75, -0.95],
        [0.75, 0.0], 
        [0.75, 0.75],
        [0.1,  0.75],
        [-0.65, 0.75],
        [-0.65, 0], 
        [-0.65, -0.65],
        [0.0, -0.65]
    ]
    count = 0 
    for coord in labels:
        plt.text(coord[0], coord[1], str(count), size=15, rotation=0.,
            ha="center", va="center",
            bbox=dict(boxstyle="round",
                    ec=(1., 0.5, 0.5),
                    fc=(1., 0.8, 0.8),
                    )
            )
        count += 1


plot_vt(plt, vt0, vt1)
plot_region_label(plt)

plt.axis('off')
plt.tight_layout()

# plt.show()
# plt.savefig("./source/writing/1-ttintersection/fig3_tt.png")
plt.savefig("./fig3_tt.png")
