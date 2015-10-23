#pragma once

#include <memory>

using namespace std;

template<typename Type>
class SharedPointer {
public:
	SharedPointer(Type *pointer) {
		ptr = shared_ptr<Type>(pointer);
	}

	SharedPointer &operator=(const SharedPointer &rhs) {
		ptr = rhs.ptr;
	}

	Type *operator->() {
		return ptr.operator->();
	}

	const Type *operator->() const {
		return ptr.operator->();
	}
	

	bool operator==(const SharedPointer &rhs) const {
		return ptr.operator==(rhs.ptr);
	}

	bool operator==(SharedPointer &rhs) const {
		return ptr == rhs.ptr;
	}
	
protected:
	shared_ptr<Type> ptr;
};
