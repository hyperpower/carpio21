# Examples for Carpio Project

This folder include all the examples for Carpio project.

## Table of Content

1. Geometry
   1. Basic Geometry Objects
      1. :ballot_box_with_check: Point Location 
      2. :ballot_box_with_check: Segments Intersection
      3. :ballot_box_with_check: Segment Box Intersection
      4. :construction: Segment Triangle Intersection
      5. :ballot_box_with_check: Point in Polygon
      6. :construction: affine transformation
      7. :construction: Triangles intersection
      8. Distance and Closest Point
      9. Polygon collision detection
      10. Polygon boolean
   2. Triangle Surface
      1. Build triangle surface
      2. Delauney Triangulation
2. Algebra
   1. Array
      1. Array operations
      2. :ballot_box_with_check: Array OpenMP
   2. Matrix
      1. Matrix operations
   3. Solver
      1. :construction: Jacobi Solver
3. Domain
   1. Structure
      1. :ballot_box_with_check: Field Define
      2. :ballot_box_with_check: Vector Center and Face Define
      3. Corner Define
      4. Field Interpolation
      5. Cut Cell
   2. Octree
4. Equation
   1. Convection
      1. :ballot_box_with_check: One Step 1D
      2. :ballot_box_with_check: Gate 1D
      3. :ballot_box_with_check: One Step 2D
      4. :ballot_box_with_check: Gate 2D
      5. :ballot_box_with_check: Sine Square
      6. :ballot_box_with_check: Semi Ellipse
      7. Rotational One Gate
      8. Rotational Two Gate
      9. Rotational Smooth
   2. Poisson
      1. :ballot_box_with_check: Time Scheme 1D
      2. Flower Shape 
   3. Navier-Stokes
      1. Lid Driven Cavity
      2. Backward Facing Step
      3. Flow pass circle
      4. Flow pass cylinder
      5. Flow pass sphere
   4. VOF
      1. Reconstruct eclipse
      2. Reconstruct rectangle
      3. Zalesak Slotted Disk Rotation
      4. Reversed Single Vortex Flow

## Prerequisites

Compliation of the cases depends on multiple projects. They are CMake, Python 3 and Multiple packages of Python.


## Usage

Each case can work independently. Follwing instruction shows how to these cases.

If you want to run case "1-1-1-point_location" (project_name), you can chang your working directory to this folder.

```
cd path_to_project
```

And run the 'run.py'

```
python3 run.py
```

The case will automatically run for you. The figures will put in fig folder, and the data will put in 'data' folder.

## To do list

- delete report.py in 1-1-2

## License

This project is licensed under the MIT License - see the [LICENSE](../LICENSE) file for details
