#pragma once

#include <iostream>

#include <eigen/Dense>

template<typename Type>
struct SimplexSolver {
	Eigen::Matrix<Type, Eigen::Dynamic, Eigen::Dynamic> matrix;

	// see https://www.youtube.com/watch?v=Axg9OhJ4cjg (german)   bottom row is negative unlike in the video

	// TODO< handle special cases >

	void iterate() {
		for(;;) {
			bool foundMaximumColumn;

			const int pivotColumnIndex = searchMinimumColumn(foundMaximumColumn);
			// all values in the target value row are < 0.0, done
			if( !foundMaximumColumn ) {
				std::cout << "no found!" << std::endl;
				break;
			}

			std::cout << "min column " << pivotColumnIndex << std::endl;

			// divide numbers of pivot column with right side and store in temporary vector
			Eigen::Matrix<Type, Eigen::Dynamic, 1> temporaryVector = divideRightSideWithPivotColumnWhereAboveZero(pivotColumnIndex);
			const int minIndexOfTargetFunctionCoefficient = getMinIndexOfVector(temporaryVector);
			const int pivotRowIndex = minIndexOfTargetFunctionCoefficient;

			std::cout << "pivotRowIndex " << pivotRowIndex << std::endl;

			Type pivotElement = matrix(pivotRowIndex, pivotColumnIndex);

			// divide the pivot row with the pivot element
			matrix.block(pivotRowIndex,0, 1, matrix.cols()) /= pivotElement;

			//std::cout << "after   divide the pivot row with the pivot element " << std::endl;
			//std::cout << matrix << std::endl;

			// TODO< assert that pivot elemnt is roughtly 1.0 >



			// do pivot operation

			for(int pivotRowIteration = 0; pivotRowIteration < matrix.rows(); pivotRowIteration++ ) {
				if( pivotRowIteration == pivotRowIndex ) {
					continue;
				}

				Type iterationElementAtPivotColumn = matrix(pivotRowIteration, pivotColumnIndex);
				matrix.block(pivotRowIteration,0, 1, matrix.cols()) -= (matrix.block(pivotRowIndex, 0, 1, matrix.cols()) * iterationElementAtPivotColumn);
			}

			// TODO< set the variable identifier for the pivot element >


			std::cout << matrix << std::endl;
			
		}
	}

protected:
	int searchMinimumColumn(bool &foundMinimumColumn) {
		foundMinimumColumn = false;

		const int matrixRowCount = matrix.rows();

		Type minValue = static_cast<Type>(0.0);
		int minColumn = -1;

		for( int iterationColumn = 0; iterationColumn < matrix.cols()-1; iterationColumn++ ) {
			if( matrix(matrixRowCount-1, iterationColumn) < minValue ) {
				minValue = matrix(matrixRowCount-1, iterationColumn);
				minColumn = iterationColumn;
				foundMinimumColumn = true;
			}
		}

		return minColumn;
	}

	Eigen::Matrix<Type, Eigen::Dynamic, 1> divideRightSideWithPivotColumnWhereAboveZero(int pivotColumnIndex) {
		const int matrixRowCount = matrix.rows();
		const int matrixColumnCount = matrix.cols();

		Eigen::Matrix<Type, Eigen::Dynamic, 1> result(matrix.rows()-1, 1);

		for( int rowIndex = 0; rowIndex < matrixRowCount-1; rowIndex++ ) {
			if( matrix(rowIndex, pivotColumnIndex) > static_cast<Type>(0.0) ) {
				result(rowIndex) = matrix(rowIndex, matrixColumnCount-1) / matrix(rowIndex, pivotColumnIndex);
			}
			else {
				// ASK< we set it just to the right side >
				result(rowIndex) = matrix(rowIndex, matrixColumnCount-1);
			}
		}

		return result;
	}

	static int getMinIndexOfVector(const Eigen::Matrix<Type, Eigen::Dynamic, 1> &vector) {
		Type min = vector(0);
		int minIndex = 0;

		for( int index = 1; index < vector.rows(); index++ ) {
			if( vector(index) < min ) {
				min = vector(index);
				minIndex = 0;
			}
		}

		return minIndex;
	}
};
