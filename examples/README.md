# Examples for Carpio Project

This folder include all the examples for Carpio project.

## Table of Content

1 Geometry

 1-1 Basic Geometry Objects

   1-1-1 :ballot_box_with_check: Point Position to Segment  

   1-1-2 :ballot_box_with_check: Segments Intersection

   1-1-3 :ballot_box_with_check: Line Box Intersection 

   1-1-4 :construction:          Segment Triangle Intersection  

   1-1-5 :ballot_box_with_check: Point in Polygon      

   1-1-6 :construction:          Affine Transformation  

   1-1-7 Triangles Intersection 

   1-1-8 Distance and Nearest Point 

   1-1-9 Polygon Partition

   1-1-10 Polygon collision detection

   1-1-11 Polygon boolean

 1-2 Curve
   
   1-2-1 Bezier

   1-2-2 B Spine

 1-3 Triangle Surface

   1-3-1 Build triangle surface

   1-3-2 Delauney Triangulation

2 Algebra

 2-1 Array

   2-1-1 Array operations

   2-1-2 :ballot_box_with_check: Array OpenMP

   2-1-3 :ballot_box_with_check: MPI

 2-2 Matrix

   2-2-1 Matrix operations

 2-3 Solver

   2-3-1 :construction: Jacobi Solver

3 Domain
 
 3-1 Structure

   3-1-1 :ballot_box_with_check: Field Define

   3-1-2 :ballot_box_with_check: Vector Center and Face Define

   3-1-3 Corner Define

   3-1-4 Field Interpolation

   3-1-5 Cut Cell

 3-2 Octree

4 Equation

 4-1 Convection

   4-1-1 :ballot_box_with_check: One Step 1D

   4-1-2 :ballot_box_with_check: Gate 1D

   4-1-3 :ballot_box_with_check: One Step 2D

   4-1-4 :ballot_box_with_check: Gate 2D

   4-1-5 :ballot_box_with_check: Sine Square

   4-1-6 :ballot_box_with_check: Semi Ellipse

   4-1-7 Rotational One Gate

   4-1-8 Rotational Two Gate

   4-1-9 Rotational Smooth

 4-2 Poisson

   4-2-1 :ballot_box_with_check: Time Scheme 1D

   4-2-2 :ballot_box_with_check: Unit Sine Square

   4-2-3 :ballot_box_with_check: Unit Sine Cube

   4-2-4 flower shape 

 4-3 Navier-Stokes

   4-3-1 Lid Driven Cavity

   4-3-2 Backward Facing Step

   4-3-3 Flow pass circle

   4-3-4 Flow pass cylinder

   4-3-5 Flow pass sphere

 4-4 VOF

   4-4-1 Reconstruct eclipse

   4-4-2 Reconstruct rectangle

   4-4-3 Zalesak Slotted Disk Rotation

   4-4-4 Reversed Single Vortex Flow

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
