#pragma once

#include "memory/FixedAllocator.h"

#include <iostream>

#include <eigen/Dense>

template<typename Type>
struct SimplexSolver {
	struct Variable {
		enum class EnumType {
			NAMEME, // TODO< name me >
			LASTLINE, // TODO< give proper name >
			UNUSED
		};

		Variable() {
			this->type = EnumType::UNUSED;
		}

		Variable(EnumType type, int identifier) {
			this->type = type;
			this->identifier = identifier;
		}

		int identifier;
		EnumType type;
	};

	struct Result {
		enum class EnumSolverState {
			FOUNDSOLUTION,
			UNBOUNDEDSOLUTION,
			TOOMANYITERATIONS
		};

		Result(Result::EnumSolverState state) {
			this->state = state;
		}

		Result::EnumSolverState state;
		
	};

	Eigen::Matrix<Type, Eigen::Dynamic, Eigen::Dynamic> matrix;

	static const unsigned SizeOfVariableArray = 256;

	// FIXME< not fixed size allocator for special cases >
	FixedAllocator<SizeOfVariableArray, Variable> variables;

	SimplexSolver() {};

	// see https://www.youtube.com/watch?v=Axg9OhJ4cjg (german)           bottom row is negative unlike in the video
	// see http://de.slideshare.net/itsmedv91/special-cases-in-simplex    special cases

	// TODO< handle other special cases >


	Result iterate() {
		setupVariables();



		// for now we use a iteration counter to protect us from infinite loops
		// TODO< implement loop detection scheme, basic idea
		//       * we manage a fixed sized array as a sliding window or the hashes of the operations
		//       * if we detect a loop we apply bland's rule to resolve it (is the rule detection realy neccesary?)
		//
		//
		//       * every operation (pivot column, pivot row, number of pivot element itself) gets an hash
		//       * at each iteration we subtract the previous hash and add the current hash
		//       * if the hash doesn't change in n iteration(where n == 2 for the simple loop) we are looping
		// >

		// TODO< implement https://en.wikipedia.org/wiki/Bland's_rule >

		unsigned iterationCounter = 0;
		const unsigned MaximalIterationCounter = 128;

		for(;;) {
			iterationCounter++;
			if( iterationCounter > MaximalIterationCounter ) {
				return Result(Result::EnumSolverState::TOOMANYITERATIONS);
			}

			bool foundMaximumColumn;

			const int pivotColumnIndex = searchMinimumColumn(foundMaximumColumn);
			// all values in the target value row are < 0.0, done
			if( !foundMaximumColumn ) {
				return Result(Result::EnumSolverState::FOUNDSOLUTION);
			}

			//std::cout << "min column " << pivotColumnIndex << std::endl;

			if( areAllEntriesOfPivotColumnNegativeOrZero(pivotColumnIndex) ) {
				// solution is unbounded

				return Result(Result::EnumSolverState::UNBOUNDEDSOLUTION);
			}

			// divide numbers of pivot column with right side and store in temporary vector
			Eigen::Matrix<Type, Eigen::Dynamic, 1> minRatioVector = divideRightSideWithPivotColumnWhereAboveZero(pivotColumnIndex);

			//std::cout << "temporary vector" << std::endl;
			//std::cout << minRatioVector << std::endl;

			const int minIndexOfTargetFunctionCoefficient = getMinIndexOfMinRatioVector(minRatioVector);
			const bool positiveMinRatioExists = doesPositiveMinRatioExist(minRatioVector);
			if( !positiveMinRatioExists ) {
				// solution is unbounded

				return Result(Result::EnumSolverState::UNBOUNDEDSOLUTION);
			}

			const int pivotRowIndex = minIndexOfTargetFunctionCoefficient;

			//std::cout << "pivotRowIndex " << pivotRowIndex << std::endl;

			Type pivotElement = matrix(pivotRowIndex, pivotColumnIndex);

			// divide the pivot row with the pivot element
			matrix.block(pivotRowIndex,0, 1, matrix.cols()) /= pivotElement;

			// TODO< assert that pivot elemnt is roughtly 1.0 >



			// do pivot operation

			for(int pivotRowIteration = 0; pivotRowIteration < matrix.rows(); pivotRowIteration++ ) {
				if( pivotRowIteration == pivotRowIndex ) {
					continue;
				}

				Type iterationElementAtPivotColumn = matrix(pivotRowIteration, pivotColumnIndex);
				matrix.block(pivotRowIteration, 0, 1, matrix.cols()) -= (matrix.block(pivotRowIndex, 0, 1, matrix.cols()) * iterationElementAtPivotColumn);
			}

			// set the variable identifier that we know which row of the matrix is for which variable
			variables[pivotRowIndex] = Variable(Variable::EnumType::NAMEME, pivotColumnIndex);

			//std::cout << matrix << std::endl;
			
		}
	}

protected:
	void setupVariables() {
		// TODO
	}

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

	bool areAllEntriesOfPivotColumnNegativeOrZero(const int pivotColumnIndex) {
		for( int rowIndex = 0; rowIndex < matrix.rows()-1; rowIndex++ ) {
			if( matrix(rowIndex, pivotColumnIndex) > static_cast<Type>(0.0) ) {
				return false;
			}
		}

		return true;
	}

	static int getMinIndexOfMinRatioVector(const Eigen::Matrix<Type, Eigen::Dynamic, 1> &vector) {
		int index;
		vector.minCoeff(&index);
		return index;
	}

	static bool doesPositiveMinRatioExist(const Eigen::Matrix<Type, Eigen::Dynamic, 1> &vector) {
		for( int i = 0; i < vector.rows(); i++ ) {
			if( vector(i) > static_cast<Type>(0.0) ) {
				return true;
			}
		}

		return false;
	}
};
