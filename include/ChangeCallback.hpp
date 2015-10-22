#pragma once

// templated abstraction for a coupled action to a changed value
// can be used for
// * recalculating a hash
// * invalidating cache
// * recalculating normals, inverse of matrix, etc
template<typename Type>
class ChangeCallback {
	public: ChangeCallback(const Type &value) {
		set(value);
	}

	public: void set(const Type &value) {
		currentValue = value;
		changed();
	}

	public: Type get() {
		return currentValue;
	}
	
	protected: virtual void changed() = 0;

	private: Type currentValue;
};