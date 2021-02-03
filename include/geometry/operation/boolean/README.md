# Overview 

Files in this folder define all of the functions for Boolean operation.

The Boolean operations include:

- Intersection
- Union
- Subtract

Following tables show the functions which have been implemented

- :ballot_box_with_check: : good to go
- :construction: : working in progress
- :negative_squared_cross_mark: : can't be implemented

## Table of Intersection
|          |        Segment          |  Box         | Line         |
|----------|:-----------------------:|--------------|--------------|
| Segment  | :ballot_box_with_check: |:construction:|:construction:|
| Box      |                         |:construction:|:construction:|
| Line     |                         |              |:construction:|
| Polygon  |                         |              |              |


## Table of Union
|          |             Segment          |  Box         | Line     |
|----------|:----------------------------:|--------------|----------|
| Segment  |:negative_squared_cross_mark: |              |          |
| Box      |                              |:construction:|          |
| Line     |                              |              |          |

## Table of Subtract

# File Structure
| file | status |
|------|--------|
|[line_box.hpp](line_box.hpp)| :construction: |

## Line vs. Box

All of related functions can be found in '[line_box.hpp](line_box.hpp)'.

### Intersection
Input: 
- box       : a box
- line      : a line 
- line_flag : a int indicates which region of the line will be used for intersection.

| Value | Instruction |
|:-----:|:-----------:|
| 1     | region on positive side of the line |
|-1     | region on nagative side of the line |
| 0     | the line itself |

Output: a PointChain in right order.

## Segment vs. Segment

All of related functions can be found in '[segment_segment.hpp](segment_segment.hpp)'. 

The basic idea of finding the intersection point of two segment in 2D is simple. A **class IntersectionPairSS_** has been created for saving four points from two segments and intersection point.



## Usage
