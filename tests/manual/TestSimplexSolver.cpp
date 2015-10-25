#include "include/solver/SimplexSolver.hpp"

int main() {
	SimplexSolver<double> solver;

	/* first example from video tutorial, seems to work, bottom row is negative unlike in the video
	*/
	
	/*
	solver.matrix = Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic>(3, 5);

	solver.matrix << 4, 8, 1, 0, 24,
	2, 1, 0, 1, 10,
	-3,-4,0,0,0;
	*/


	/* own example with dual solution
	*/
	
solver.matrix = Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic>(3, 5);

	solver.matrix << -10, -3, 1, 0, -5,
	5, 8, 0, 1, 20,
	0 ,-1, 0, 0, 2;
	

	
	/* this has no solution


	check online here: http://simplexrechner.matthias-priebe.de/

	Zielfunktion:
Maximiere F = 0.34 · x1 + 0.1 · x2 + 5 · x3 + 7
 
System der Nebenbedingungen:
 
1  	24 · x1 + 56 · x2 + 67 · x3	 ≥  7
2  	5 · x1 + 53 · x2 + 52 · x3	 ≤  4
3  	57 · x1 + 24 · x2 + 53 · x3	 ≤  9

System der Nebenbedingungen in Gleichungsform (vorbereitet für den Algorithmus):

1  	-24 · x1-56 · x2-67 · x3 + 1 · x4	= -7
2  	5 · x1 + 53 · x2 + 52 · x3 + 1 · x5	= 4
3  	57 · x1 + 24 · x2 + 53 · x3 + 1 · x6	= 9

	*/



	solver.iterate();

}