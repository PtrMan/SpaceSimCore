#pragma once

#include <vector>

using namespace std;

template<typename Type>
class Array {
public:
	int getCount() const {
		return data.size();
	}

	Type &operator[](int i) {
		return data[i];
	}

	void add(Type &element) {
		data.push_back(element);
	}

	void remove(Type &element) {
		for(auto it = data.begin(); it != data.end(); it++) {
			if( *it == element ) {
				data.erase(it);
				return;
			}
		}

		// TODO< throw something >
	}

private:
	vector<Type> data;
};