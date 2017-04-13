#ifndef BAG_H
#define BAG_H

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "../utils.h"
#include "math.h"

enum BAG_ELEMENT_MARK {
	BAG_MARK_NULL, BAG_MARK_DELETE, BAG_MARK_PRESENT
};

#define MAX_ITERATORS 1

#define LARGE_PRIME_A 10007
#define LARGE_PRIME_B 11003
#define LARGE_PRIME_C 12007

int nextLargestPrime(unsigned int x);  // defined in utils.C

// ======================================================================
// A bag is implemented with a hash table to allow efficient access and removal

using llint = long long int;

template<class BAG_ELEMENT>
class Bag {

public:

	class Iterator;

	// ========================
	// CONSTRUCTOR & DESTRUCTOR

	Bag(int s, void (*e_func)(BAG_ELEMENT, llint&, llint&, llint&));

	virtual ~Bag();

	// =========
	// ACCESSORS
	llint count() const {
		return _count;
	}
	llint member(const BAG_ELEMENT e) const;

	BAG_ELEMENT chooseRandom() const;

	BAG_ELEMENT getReorder(llint a, llint b, llint c) const;

	BAG_ELEMENT getReorder(llint a, llint b) const;

	BAG_ELEMENT get(llint a, llint b) const;

	// =========
	// MODIFIERS
	void add(const BAG_ELEMENT e);

	void addNoDuplicates(const BAG_ELEMENT e);

	void remove(const BAG_ELEMENT e);

	void deleteAllElements();

	void clear();

	class Iterator {

	public:

		Iterator(Bag<BAG_ELEMENT> *b, llint i = 0);
		virtual ~Iterator() {
		}

		bool operator==(const Iterator& it2);
		bool operator!=(const Iterator& it2);
		Iterator& operator++();

		BAG_ELEMENT& operator*();

		// ACCESSOR
	protected:

		Iterator() = delete;

		// ==============
		// REPRESENTATION
		llint i;
		Bag<BAG_ELEMENT> *bag;

		friend class Bag;

	};

	Iterator begin();
	Iterator end();

private:
	void resize(llint s);
public:
	static void extract_int(llint e, llint &a, llint &b, llint &c);

protected:

	llint hash(llint a, llint b, llint c) const;

// ==============
// REPRESENTATION
	llint _size;
	llint _count;
	llint del_count;
	BAG_ELEMENT *data;
	enum BAG_ELEMENT_MARK *marks;
	unsigned int num_iterators;

// extract function (an argument to the constructor)
	void (*extract_func)(BAG_ELEMENT, llint&, llint&, llint&);

// skip function
	inline llint skip(llint orig, llint current) const;

	friend class Iterator;

};

template<class BAG_ELEMENT>
Bag<BAG_ELEMENT>::Bag(int s,
		void (*e_func)(BAG_ELEMENT, llint&, llint&, llint&)) {
	extract_func = e_func;
	num_iterators = 0;
	_size = nextLargestPrime(s);
	marks = new enum BAG_ELEMENT_MARK[_size];
	data = new BAG_ELEMENT[_size];
	for (llint i = 0; i < _size; i++)
		marks[i] = BAG_MARK_NULL;
	_count = 0;
	del_count = 0;
}

template<class BAG_ELEMENT>
Bag<BAG_ELEMENT>::~Bag() {
	assert(num_iterators == 0);
	delete[] data;
	delete[] marks;
}

// =========
// ACCESSORS
template<class BAG_ELEMENT>
llint Bag<BAG_ELEMENT>::member(const BAG_ELEMENT e) const {
	llint a, b, c;
	assert(e != (BAG_ELEMENT )0);
	extract_func(e, a, b, c);
	auto orig = hash(a, b, c);
	auto x = orig;
	while (1) {
		if (marks[x] == BAG_MARK_NULL)
			return 0;
		if (marks[x] == BAG_MARK_PRESENT && data[x] == e)
			return 1;
		x = skip(orig, x);
	}
}

template<class BAG_ELEMENT>
BAG_ELEMENT Bag<BAG_ELEMENT>::chooseRandom() const {
	assert(count() > 0);
	while (1) {
		llint random_int = llint(floor(drand48() * _size));
		if (marks[random_int] == BAG_MARK_PRESENT)
			return data[random_int];
	}
}

template<class BAG_ELEMENT>
BAG_ELEMENT Bag<BAG_ELEMENT>::getReorder(llint a, llint b, llint c) const {
	assert(a != b && b != c && a != c);
	if (a < b && a < c)
		return get(a, b, c);
	else if (b < a && b < c)
		return get(b, c, a);
	assert(c < a && c < b);
	return get(c, a, b);
}

template<class BAG_ELEMENT>
BAG_ELEMENT Bag<BAG_ELEMENT>::getReorder(llint a, llint b) const {
	assert(a != b);
	if (a < b)
		return get(a, b);
	return get(b, a);
}

template<class BAG_ELEMENT>
BAG_ELEMENT Bag<BAG_ELEMENT>::get(llint a, llint b) const {
	assert(a != b);
	auto orig = hash(a, b, 0);
	auto x = orig;
	while (1) {
		assert(x >= 0 && x < _size);
		if (marks[x] == BAG_MARK_NULL)
			return 0;
		if (marks[x] == BAG_MARK_PRESENT) {
			llint _a, _b, _c;
			assert(data[x] != (BAG_ELEMENT )0);
			extract_func(data[x], _a, _b, _c);
			if (_a == a && _b == b) {
				return data[x];
			}
		}
		x = skip(orig, x);
	}
}

// =========
// MODIFIERS
template<class BAG_ELEMENT>
void Bag<BAG_ELEMENT>::add(const BAG_ELEMENT e) {
	assert(!member(e));
	if (_count + del_count > _size / 2)
		resize(max2(_count * 4, _size));
	llint a, b, c;
	assert(e != (BAG_ELEMENT )0);
	extract_func(e, a, b, c);
	auto orig = hash(a, b, c);
	auto x = orig;
	while (marks[x] == BAG_MARK_PRESENT)
		x = skip(orig, x);
	if (marks[x] == BAG_MARK_DELETE)
		del_count--;
	marks[x] = BAG_MARK_PRESENT;
	data[x] = e;
	_count++;
}

template<class BAG_ELEMENT>
void Bag<BAG_ELEMENT>::addNoDuplicates(const BAG_ELEMENT e) {
	if (!member(e))
		add(e);
}

template<class BAG_ELEMENT>
void Bag<BAG_ELEMENT>::remove(const BAG_ELEMENT e) {
	llint a, b, c;
	assert(e != (BAG_ELEMENT )0);
	extract_func(e, a, b, c);
	auto orig = hash(a, b, c);
	auto x = orig;
	while (1) {
		assert(marks[x] != BAG_MARK_NULL);
		if (marks[x] == BAG_MARK_PRESENT && data[x] == e) {
			marks[x] = BAG_MARK_DELETE;
			del_count++;
			_count--;
			break;
		}
		x = skip(orig, x);
	}
}

template<class BAG_ELEMENT>
void Bag<BAG_ELEMENT>::deleteAllElements() {
	assert(num_iterators == 0);
	for (llint i = 0; i < _size; i++) {
		if (marks[i] == BAG_MARK_PRESENT)
			delete data[i];
		marks[i] = BAG_MARK_NULL;
	}
	del_count = 0;
	_count = 0;
}

template<class BAG_ELEMENT>
void Bag<BAG_ELEMENT>::clear() {
	assert(num_iterators == 0);
	for (llint i = 0; i < _size; i++) {
		marks[i] = BAG_MARK_NULL;
	}
	del_count = 0;
	_count = 0;
}

template<class BAG_ELEMENT>
void Bag<BAG_ELEMENT>::resize(llint s) {
	assert(s > 0 && s > _count);
// save old stuff
	llint old_size = _size;
	BAG_ELEMENT *old_data = data;
	enum BAG_ELEMENT_MARK *old_marks = marks;
	// make new space
	_size = nextLargestPrime(s);
	marks = new enum BAG_ELEMENT_MARK[_size];
	data = new BAG_ELEMENT[_size];
	_count = 0;
	del_count = 0;
	for (llint i = 0; i < _size; i++) {
		marks[i] = BAG_MARK_NULL;
		data[i] = (BAG_ELEMENT) 37;
	}
	llint tmp_count = 0;
	// copy the stuff
	if (old_data != nullptr) {
		for (llint i = 0; i < old_size; i++) {
			if (old_marks[i] == BAG_MARK_PRESENT) {
				tmp_count++;
				add(old_data[i]);
			}
		}
	}
	// cleanup
	delete[] old_data;
	delete[] old_marks;
}

template<class BAG_ELEMENT>
void Bag<BAG_ELEMENT>::extract_int(llint e, llint &a, llint &b, llint &c) {
	a = e;
	b = c = 0;
}

template<class BAG_ELEMENT>
llint Bag<BAG_ELEMENT>::hash(llint a, llint b, llint c) const {
	llint _a = (a < 0) ? 1 - 2 * a : 2 * a;
	llint _b = (b < 0) ? 1 - 2 * b : 2 * b;
	llint _c = (c < 0) ? 1 - 2 * c : 2 * c;
	llint tmp =
	LARGE_PRIME_A * _a +
	LARGE_PRIME_B * _b +
	LARGE_PRIME_C * _c;
	// note: abs of the largest negative number is undefined...
	tmp = tmp % _size;
	tmp = (tmp < 0) ? -tmp : tmp;
	tmp = tmp % _size;
	assert(tmp >= 0);
	assert(tmp < _size);
	return tmp;
}

template<class BAG_ELEMENT>
typename Bag<BAG_ELEMENT>::Iterator Bag<BAG_ELEMENT>::begin() {
	llint i = 0;

	while (i < _size) {
		if (marks[i] == BAG_MARK_PRESENT) {
			break;
		}
		i++;
	}

	return Iterator(this, i);
}

template<class BAG_ELEMENT>
typename Bag<BAG_ELEMENT>::Iterator Bag<BAG_ELEMENT>::end() {
	return Iterator(this, _size);
}

// skip function
template<class BAG_ELEMENT>
llint Bag<BAG_ELEMENT>::skip(llint orig, llint current) const {
	assert(current >= 0 && current < _size);
	auto tmp = (current + 1) % _size;
	assert(current >= 0 && current < _size);
	return tmp;
}

// ======================================================================

// CONSTRUCTOR & DESTRUCTOR
template<class BAG_ELEMENT>
Bag<BAG_ELEMENT>::Iterator::Iterator(Bag<BAG_ELEMENT> *b, llint _i) :
		bag(b), i(_i) {
}

template<class BAG_ELEMENT>
bool Bag<BAG_ELEMENT>::Iterator::operator ==(const Iterator& it2) {
	return (bag == it2.bag) && (i == it2.i);
}

template<class BAG_ELEMENT>
bool Bag<BAG_ELEMENT>::Iterator::operator !=(const Iterator& it2) {
	return (bag != it2.bag) || (i != it2.i);
}

template<class BAG_ELEMENT>
typename Bag<BAG_ELEMENT>::Iterator& Bag<BAG_ELEMENT>::Iterator::operator ++() {
	while (i < bag->_size) {
		i++;
		if (bag->marks[i] == BAG_MARK_PRESENT) {
			break;
		}
	}
	return *this;
}

template<class BAG_ELEMENT>
BAG_ELEMENT& Bag<BAG_ELEMENT>::Iterator::operator *() {
	return bag->data[i];
}

#endif
