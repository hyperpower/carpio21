import os, math
import numpy as np


Colors = {
    "yellow" : "#FBBD0C",
    "blue"   : "#4285F4",
    "red"    : "#F35325",
    "green"  : "#81BC06",
}

def generate_0_svg(p):
    fn = "0.svg"
    f = open(os.path.abspath(os.path.join(p, fn)), "w")
    f.write("<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"0\" height=\"0\"/>")
    f.close()


def sub(p1, p0):
    p = [p1[0]-p0[0],
         p1[1]-p0[1],
         p1[2]-p0[2]]
    return p

def mid(p1, p0):
    p = [(p1[0]+p0[0])*0.5,
         (p1[1]+p0[1])*0.5,
         (p1[2]+p0[2])*0.5]
    return p

def length(p):
    return math.sqrt(p[0]*p[0]+p[1]*p[1]+p[2]*p[2])

def transpose(arr2d):
    """
    Transpose a 2D array to a column format.
    """
    res = []
    for i in range(len(arr2d[0])):
        col = [arr2d[j][i] for j in range(len(arr2d))]
        res.append(col)
    return res    

def plane_equation(p0, p1, p2):
    v1 = np.array(p1) - np.array(p0)
    v2 = np.array(p2) - np.array(p0)
    n = np.cross(v1, v2)
    A, B, C = n
    D = -np.dot(n, p0)
    return A, B, C, D

def plane_intersection_line(plane1, plane2):
    # plane1, plane2: (A, B, C, D)
    n1 = np.array(plane1[:3])
    n2 = np.array(plane2[:3])
    direction = np.cross(n1, n2)
    if np.allclose(direction, 0):
        return None  # no intersection, planes are parallel or coincident

    # slove for the intersection point
    A = np.array([n1, n2])
    b = -np.array([plane1[3], plane2[3]])
    try:
        # z=0
        A2 = A[:, :2]
        sol = np.linalg.solve(A2, b)
        point = [sol[0], sol[1], 0]
    except np.linalg.LinAlgError:
        A2 = A[:, [0,2]]
        sol = np.linalg.solve(A2, b)
        point = [sol[0], 0, sol[1]]
    return point, direction


def line_equation_3d(p0, p1):
    # p0, p1: [x, y, z]
    direction = [p1[i] - p0[i] for i in range(3)]
    return p0, direction

def line_plane_intersection(plane, p0, direction):
    # plane: (A, B, C, D)
    # p0: [x0, y0, z0]，直线上的点
    # direction: [a, b, c]，直线方向向量
    A, B, C, D = plane
    x0, y0, z0 = p0
    a, b, c = direction
    denom = A*a + B*b + C*c
    if denom == 0:
        return None  # 平行或在平面上
    t = -(A*x0 + B*y0 + C*z0 + D) / denom
    x = x0 + a*t
    y = y0 + b*t
    z = z0 + c*t
    return [x, y, z]

def point_at_distance(point, direction, dist):
    point = np.array(point)
    direction = np.array(direction)
    direction_unit = direction / np.linalg.norm(direction)
    return (point + direction_unit * dist).tolist()

def normalize(p):
    l = length(p)
    s3 = math.sqrt(3.0)
    return [p[0]/l*s3, p[1]/l*s3, p[2]/l*s3]