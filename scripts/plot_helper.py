import os, math
import numpy as np


Colors = {
    "yellow" : "#FBBD0C",
    "blue"   : "#4285F4",
    "red"    : "#F35325",
    "green"  : "#81BC06",
    "black"  : "#1D1D1D",
}

def generate_0_svg(p):
    fn = "0.svg"
    fn_path = os.path.abspath(os.path.join(p, fn))
    if os.path.exists(fn_path):
        return  # already exists, do nothing
    else:
        f = open(fn_path, "w")
        f.write("<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"0\" height=\"0\"/>")
        f.close()
def add(p1, p0):
    """
    Add two 3D points.
    :param p1: First point as a list or tuple of [x, y, z].
    :param p0: Second point as a list or tuple of [x, y, z].
    :return: Resulting point as a list [x', y', z'].
    """
    p = [p1[0]+p0[0],
         p1[1]+p0[1],
         p1[2]+p0[2]]
    return p

def sub(p1, p0):
    p = [p1[0]-p0[0],
         p1[1]-p0[1],
         p1[2]-p0[2]]
    return p

def cross(p1, p0):
    """
    Calculate the cross product of two 3D vectors.
    :param p1: First vector as a list or tuple of [x, y, z].
    :param p0: Second vector as a list or tuple of [x, y, z].
    :return: Cross product as a list [x', y', z'].
    """
    x = p1[1] * p0[2] - p1[2] * p0[1]
    y = p1[2] * p0[0] - p1[0] * p0[2]
    z = p1[0] * p0[1] - p1[1] * p0[0]
    return [x, y, z]

def scale(p, s):
    """
    Scale a 3D vector by a scalar.
    :param p: Input vector as a list or tuple of [x, y, z].
    :param s: Scalar value to scale the vector.
    :return: Scaled vector as a list [x', y', z'].
    """
    return [p[0] * s, p[1] * s, p[2] * s]

def mid(p1, p0):
    p = [(p1[0]+p0[0])*0.5,
         (p1[1]+p0[1])*0.5,
         (p1[2]+p0[2])*0.5]
    return p

def rotate_90(index, vec):
    """
    Rotate a vector 90 degrees around the x-axis.
    :param v: Input vector [x, y, z].
    :return: Rotated vector [x', y', z'].
    """
    R_x = np.array([
        [1, 0, 0],
        [0, 0, -1],
        [0, 1, 0]
    ])
    R_y = np.array([
        [0, 0, 1],
        [0, 1, 0],
        [-1, 0, 0]
    ])
    R_z = np.array([
        [0, -1, 0],
        [1, 0, 0],
        [0, 0, 1]
    ])
    R_rotation = [R_x, R_y, R_z]
    if index < 0 or index >= len(R_rotation):
        raise ValueError("Index must be between 0 and 2 (inclusive).")
    return np.dot(R_rotation[index], vec)

def absmin_v_and_i(arr):
    """
    Find the minimum absolute value in a 3D vector and its index.
    :param arr: A list or tuple of [x, y, z].
    :return: Tuple (min_value, index) where min_value is the minimum absolute value and index is its position in the vector.
    """
    abs_arr = [abs(x) for x in arr]
    min_value = min(abs_arr)
    index = abs_arr.index(min_value)
    return min_value, index

def avg_coordinate(points):
    """
    Calculate the average coordinate of a list of points.
    :param points: List of points, each point is a list or tuple of [x, y, z].
    :return: Average point as a list [x_avg, y_avg, z_avg].
    """
    x_avg = sum(p[0] for p in points) / len(points)
    y_avg = sum(p[1] for p in points) / len(points)
    z_avg = sum(p[2] for p in points) / len(points)
    
    return [x_avg, y_avg, z_avg]

def length(p):
    return math.sqrt(p[0]*p[0]+p[1]*p[1]+p[2]*p[2])

def normalize_vector(p):
    """
    Normalize a 3D vector.
    :param p: A list or tuple of [x, y, z].
    :return: Normalized vector as a list [x', y', z'].
    """
    l = length(p)
    if l == 0:
        return [0, 0, 0]  # Avoid division by zero
    return [p[0] / l, p[1] / l, p[2] / l]

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
    l  = length(p)
    return [p[0]/l, p[1]/l, p[2]/l]

def rectangle_from_center_and_normal(pc, n, scale):
    """
    Create a rectangle in 3D space from a center point and a normal vector.
    :param pc: Center point of the rectangle as a list or tuple of [x, y, z].
    :param n: Normal vector of the rectangle as a list or tuple of [nx, ny, nz].
    :param scale: Scale factor for the rectangle size.
    :return: A list of points representing the corners of the rectangle.
    """
    c = np.array(pc)
    nz = np.array(normalize(n))  # normalize the normal vector
    minv, mini = absmin_v_and_i(nz)
    
    # vertical vector of n
    nx = np.array(rotate_90(mini, nz))   # rotate normal vector to be horizontal
    ny = np.array(cross(nx, nz))   # cross product to get vertical vector

    p0 = c - 0.5 * nx * scale - 0.5 * ny * scale
    p1 = c + 0.5 * nx * scale - 0.5 * ny * scale
    p2 = c + 0.5 * nx * scale + 0.5 * ny * scale
    p3 = c - 0.5 * nx * scale + 0.5 * ny * scale

    return [p0, p1, p2, p3, p0]

def point_projection_to_plane(point, plane):
    """
    Calculate the projection of a point onto a plane.

    :param point: The point as a list or tuple [x0, y0, z0].
    :param plane: The plane equation coefficients [A, B, C, D].
    :return: The projected point [x', y', z'].
    """
    x0, y0, z0 = point
    A, B, C, D = plane

    # Calculate the parameter t
    t = -(A * x0 + B * y0 + C * z0 + D) / (A**2 + B**2 + C**2)

    # Calculate the projected point
    x_prime = x0 + A * t
    y_prime = y0 + B * t
    z_prime = z0 + C * t

    return [x_prime, y_prime, z_prime]
