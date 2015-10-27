#pragma once

#include "ChangeCallback.hpp"

template<typename Type>
class ChangeCallbackCalculateInverse : public ChangeCallback<Type> {
    public: ChangeCallbackCalculateInverse() {
    }

    /*
	public: ChangeCallbackCalculateInverse(const Type &value) : ChangeCallback<Type>(value) {
	}
    */

	public: Type &getInverse() {
		return inverse;
	}

    protected: virtual void changed() override {
		inverse = this->get().inverse();
	}

	private: Type inverse;
};
