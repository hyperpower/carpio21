# Examples for Carpio Project

This folder include all the examples for Carpio project.

## Table of Content

### :one: Geometry

#### 1-1 Basic Geometry Objects

   1-1-1 :white_check_mark: Point Position to Segment  

   1-1-2 :white_check_mark: Segments Intersection

   1-1-3 :white_check_mark: Line Box Intersection 

   1-1-4 :construction:          Segment Triangle Intersection  

   1-1-5 :white_check_mark: Point in Polygon      

   1-1-6 :construction:          Affine Transformation  

   1-1-7 Triangles Intersection 

   1-1-8 Distance and Nearest Point 

   1-1-9 Polygon Partition

   1-1-10 Polygon collision detection

   1-1-11 Polygon boolean

#### 1-2 Curve
   
   1-2-1 Bezier

   1-2-2 B Spine

#### 1-3 Triangle Surface

   1-3-1 Build triangle surface

   1-3-2 Delauney Triangulation

### :two: Algebra

#### 2-1 Array

   2-1-1 Array operations

   2-1-2 :white_check_mark: Array OpenMP

   2-1-3 :white_check_mark: MPI

#### 2-2 Matrix

   2-2-1 Matrix operations

#### 2-3 Solver

   2-3-1 :white_check_mark: Jacobi Solver

### :three: Domain
 
#### 3-1 Structure

   3-1-1 :white_check_mark: Field Define

   3-1-2 :white_check_mark: Boundary Value

   3-1-3 Interpolation 

   3-1-4 Cut Cell

#### 3-2 Octree

### :four: Equation

#### 4-1 Convection

   4-1-1 :white_check_mark: One Step 1D

   4-1-2 :white_check_mark: Gate 1D

   4-1-3 :white_check_mark: One Step 2D

   4-1-4 :white_check_mark: Gate 2D

   4-1-5 :white_check_mark: Sine Square

   4-1-6 :white_check_mark: Semi Ellipse

   4-1-7 Rotational One Gate

   4-1-8 Rotational Two Gate

   4-1-9 Rotational Smooth

#### 4-2 Poisson

   4-2-1 :white_check_mark: One Boundary

   4-2-2 :white_check_mark: Sine Boundary Cube

   4-2-3 :white_check_mark: Unit Sine Square

   4-2-4 :white_check_mark: Unit Sine Cube

   4-2-4 flower shape 

#### 4-3 Navier-Stokes

   4-3-1 Lid Driven Cavity

   4-3-2 Backward Facing Step

   4-3-3 Flow pass circle

   4-3-4 Flow pass cylinder

   4-3-5 Flow pass sphere

#### 4-4 VOF

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
