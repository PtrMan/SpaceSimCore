#pragma once

#include "ChangeCallback.hpp"

template<typename Type>
class ChangeCallbackCalculateInverse : public ChangeCallback<Type> {
	public: ChangeCallbackCalculateInverse(const Type &value) : ChangeCallback<Type>(value) {
	}

	public: Type &getInverse() {
		return inverse;
	}

	protected: virtual void changed() {
		inverse = this->get().inverse();
	}

	private: Type inverse;
};
