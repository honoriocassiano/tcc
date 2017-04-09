/*
 * MultiLevelArray.h
 *
 *  Created on: 16 de mar de 2017
 *      Author: cassiano
 */

#ifndef SRC_MULTILEVELARRAY_H_
#define SRC_MULTILEVELARRAY_H_

#include <cstddef>
#include <utility>
#include <vector>

template<class T>
class MultiLevelArray {

public:

	class ReverseIterator;
	class Iterator;

	MultiLevelArray(std::size_t sizeByLevel = 100, std::size_t numLevels = 4);
	virtual ~MultiLevelArray();

	void fitAllLevels(bool exactFit = false);
	void fit(std::size_t level, bool exactFit = false);
	void fit(std::initializer_list<bool> fits);

	void remove(const std::pair<std::size_t, std::size_t>& position)
			throw (std::overflow_error);

	T& operator[](const std::pair<std::size_t, std::size_t>& position)
			throw (std::overflow_error);
	const T& operator[](
			const std::pair<std::size_t, std::size_t>& position) const
					throw (std::overflow_error);

	std::size_t size() const;
	std::size_t size(std::size_t level) const;

	class Iterator {
	public:
		Iterator(MultiLevelArray<T> * array, std::size_t level,
				std::size_t position = 0);
		Iterator(MultiLevelArray<T> * array,
				std::pair<std::size_t, std::size_t> position = { 0, 0 });

		bool operator==(const Iterator& it2);
		bool operator!=(const Iterator& it2);
		Iterator& operator++();

		inline const std::pair<std::size_t, std::size_t>& getPosition() const {
			return position;
		}

		T& operator*();

		ReverseIterator reverse();

	private:
		MultiLevelArray<T> *array;

		std::pair<std::size_t, std::size_t> position;
	};

	class ReverseIterator {
	public:
		ReverseIterator(MultiLevelArray<T> * array, std::size_t level,
				std::size_t position = 0);
		ReverseIterator(MultiLevelArray<T> * array,
				std::pair<std::size_t, std::size_t> position = { 0, 0 });

		Iterator reverse();

		bool operator==(const ReverseIterator& it2);
		bool operator!=(const ReverseIterator& it2);
		ReverseIterator& operator++();

		inline const std::pair<std::size_t, std::size_t>& getPosition() const {
			return position;
		}

		T& operator*();

	private:
		MultiLevelArray<T> *array;

		std::pair<std::size_t, std::size_t> position;
	};

	Iterator add(const T& element, std::size_t level);

	void remove(const Iterator& iterator) throw (std::overflow_error);

	Iterator find(const T& element);
	Iterator find(const T& element, std::size_t level)
			throw (std::overflow_error);

	Iterator begin();
	Iterator end();

	Iterator begin(std::size_t level);
	Iterator end(std::size_t level);

	ReverseIterator rbegin();
	ReverseIterator rend();

	ReverseIterator rbegin(std::size_t level);
	ReverseIterator rend(std::size_t level);

private:

	bool checkBounds(const std::pair<std::size_t, std::size_t>& position);

	std::vector<std::pair<std::size_t, std::size_t>> levelsInfo;
	T** data;
	std::size_t defaultSize;
};

template<class T>
MultiLevelArray<T>::MultiLevelArray(std::size_t sizeByLevel,
		std::size_t numLevels) :
		levelsInfo(numLevels, { 0, sizeByLevel }), data(new T*[numLevels]), defaultSize(
				sizeByLevel) {

	for (auto i = 0; i < numLevels; ++i) {
		data[i] = new T[sizeByLevel];
	}
}

template<class T>
MultiLevelArray<T>::~MultiLevelArray() {

	for (auto i = 0; i < levelsInfo.size(); ++i) {
		delete data[i];
	}

	delete data;
}

template<class T>
T& MultiLevelArray<T>::operator [](
		const std::pair<std::size_t, std::size_t>& position)
				throw (std::overflow_error) {

	if ((position.first < levelsInfo.size())
			&& (position.second < levelsInfo[position.first].second)) {
		return data[position.first][position.second];
	} else {
		throw std::overflow_error(
				"Trying to access position " + std::to_string(position.first)
						+ ", " + std::to_string(position.second));
	}
}

template<class T>
typename MultiLevelArray<T>::Iterator MultiLevelArray<T>::add(const T& element,
		std::size_t level) {

	std::size_t s = levelsInfo[level].second;

	if (level >= levelsInfo.size()) {
		auto newData = new T*[level + 1] { nullptr };

		for (auto i = 0; i < levelsInfo.size(); ++i) {
			newData[i] = data[i];
		}

		for (auto i = levelsInfo.size(); i <= level; ++i) {
			newData[i] = new T[defaultSize];
			levelsInfo.push_back( { 0, defaultSize });
		}

		delete data;

		data = newData;

		s = defaultSize;
	} else if (levelsInfo[level].first >= levelsInfo[level].second) {
		auto newData = new T[levelsInfo[level].second << 1];

		for (auto i = 0; i < levelsInfo[level].second; ++i) {
			newData[i] = data[level][i];
		}

		delete data[level];

		data[level] = newData;

		s = (levelsInfo[level].second << 1);
	}

	data[level][levelsInfo[level].first] = element;

	++levelsInfo[level].first;
	levelsInfo[level].second = s;

	return Iterator(this, { level, levelsInfo[level].first - 1 });
}

template<class T>
const T& MultiLevelArray<T>::operator [](
		const std::pair<std::size_t, std::size_t>& position) const
				throw (std::overflow_error) {

	if ((position.first < levelsInfo.size())
			&& (position.second < levelsInfo[position.first].second)) {
		return data[position.first][position.second];
	} else {
		throw std::overflow_error(
				"Trying to access position " + std::to_string(position.first)
						+ ", " + std::to_string(position.second));
	}
}

template<class T>
inline MultiLevelArray<T>::ReverseIterator::ReverseIterator(
		MultiLevelArray<T>* array, std::size_t level, std::size_t position) :
		array(array), position { level, position } {
}

template<class T>
inline MultiLevelArray<T>::ReverseIterator::ReverseIterator(
		MultiLevelArray<T>* array, std::pair<std::size_t, std::size_t> position) :
		array(array), position(position) {

}

template<class T>
inline bool MultiLevelArray<T>::ReverseIterator::operator ==(
		const ReverseIterator& it2) {

	return (this->array == it2.array) && (this->position == it2.position);
}

template<class T>
inline bool MultiLevelArray<T>::ReverseIterator::operator !=(
		const ReverseIterator& it2) {

	return !(*this == it2);
}

template<class T>
typename MultiLevelArray<T>::ReverseIterator& MultiLevelArray<T>::ReverseIterator::operator++() {

	if (this->position.first != -1) {

		while ((this->position.second) == 0 && (this->position.first != -1)) {
			--this->position.first;
			this->position.second =
					this->array->levelsInfo[this->position.first].first;
		}

		if (this->position.first != -1) {
			--this->position.second;
		} else {
			this->position.second = 0;
		}
	}
}

template<class T>
inline MultiLevelArray<T>::Iterator::Iterator(MultiLevelArray<T>* array,
		std::size_t level, std::size_t position) :
		array(array), position { level, position } {
}

template<class T>
inline MultiLevelArray<T>::Iterator::Iterator(MultiLevelArray<T>* array,
		std::pair<std::size_t, std::size_t> position) :
		array(array), position(position) {

}

template<class T>
inline bool MultiLevelArray<T>::Iterator::operator ==(const Iterator& it2) {

	return (this->array == it2.array) && (this->position == it2.position);
}

template<class T>
inline bool MultiLevelArray<T>::Iterator::operator !=(const Iterator& it2) {

	return !(*this == it2);
}

template<class T>
typename MultiLevelArray<T>::Iterator& MultiLevelArray<T>::Iterator::operator++() {

	if (this->position.first < array->levelsInfo.size()) {
		++this->position.second;

		while ((this->position.second
				>= array->levelsInfo[this->position.first].first)
				&& (this->position.first < array->levelsInfo.size())) {
			++this->position.first;
			this->position.second = 0;
		}
	}
}

template<class T>
typename MultiLevelArray<T>::Iterator MultiLevelArray<T>::begin() {
	std::size_t i = 0;

	while ((i < levelsInfo.size()) && (levelsInfo[i].first == 0)) {
		++i;
	}

	return Iterator(this, { i, 0 });
}

template<class T>
typename MultiLevelArray<T>::Iterator MultiLevelArray<T>::end() {

	std::size_t i = levelsInfo.size();

	while ((i >= begin().getPosition().first) && (levelsInfo[i - 1].first > 0)) {
		--i;
	}

	return Iterator(this, { i, 0 });
}

template<class T>
typename MultiLevelArray<T>::Iterator MultiLevelArray<T>::begin(
		std::size_t level) {

	std::size_t i = level;

	while ((i < levelsInfo.size()) && levelsInfo[i].first == 0) {
		++i;
	}

	return Iterator(this, i);
}

template<class T>
typename MultiLevelArray<T>::Iterator MultiLevelArray<T>::end(
		std::size_t level) {

	std::size_t i = level + 1;

	while ((i < levelsInfo.size()) && (levelsInfo[i].first == 0)) {
		++i;
	}

	return Iterator(this, { i, 0 });
}

template<class T>
void MultiLevelArray<T>::fitAllLevels(bool exactFit) {

	for (auto i = 0; i < levelsInfo.size(); ++i) {

		fit(i, exactFit);
	}
}

template<class T>
void MultiLevelArray<T>::fit(std::size_t level, bool exactFit) {
	auto& size = levelsInfo[level];

	auto newData = data[level];

	if (exactFit) {
		if (size.first < size.second) {
			newData = new T[size.first];
		}
	} else {
		if (size.second > (size.first << 1)) {
			newData = new T[size.first << 1];
		}
	}

	if (newData != data[level]) {
		for (auto j = 0; j < size.first; ++j) {
			newData[j] = data[level][j];
		}
	}

	delete data[level];

	data[level] = newData;
}

template<class T>
void MultiLevelArray<T>::fit(std::initializer_list<bool> fits) {

	auto size =
			(fits.size() > levelsInfo.size()) ? levelsInfo.size() : fits.size();
	auto it = std::begin(fits);

	for (auto level = 0; level < size; ++level) {
		fit(level, *(it++));
	}
}

template<class T>
void MultiLevelArray<T>::remove(
		const std::pair<std::size_t, std::size_t>& position)
				throw (std::overflow_error) {

	if (checkBounds(position)) {

		auto& size = levelsInfo[position.first];

		// Remove if element exists
//		if (position.second < size.first) {
		data[position.first][position.second] =
				data[position.first][--size.first];
//		}
		// Else do nothing

	} else {
		throw std::overflow_error(
				"Trying to access position " + std::to_string(position.first)
						+ ", " + std::to_string(position.second));
	}
}

template<class T>
inline void MultiLevelArray<T>::remove(
		const MultiLevelArray<T>::Iterator& iterator)
				throw (std::overflow_error) {

	remove(iterator.getPosition());
}

template<class T>
std::size_t MultiLevelArray<T>::size() const {
	std::size_t sum = 0;

	for (const auto& s : levelsInfo) {
		sum += s.first;
	}

	return sum;
}

template<class T>
std::size_t MultiLevelArray<T>::size(std::size_t level) const {
	if (level < levelsInfo.size()) {

		return levelsInfo[level].first;

	} else {

		return 0;

	}
}

template<class T>
typename MultiLevelArray<T>::Iterator MultiLevelArray<T>::find(
		const T& element) {

	auto it = begin();

	while (it != end()) {
		++it;
	}

	return it;
}

template<class T>
typename MultiLevelArray<T>::Iterator MultiLevelArray<T>::find(const T& element,
		std::size_t level) throw (std::overflow_error) {
	if (checkBounds( { level, 0 })) {
		auto it = begin(level);

		while (it.getPosition().first == level) {
			++it;
		}

		return it;
	} else {
		throw std::overflow_error(
				"Level " + std::to_string(level) + "not exists");
	}
}

template<class T>
typename MultiLevelArray<T>::ReverseIterator MultiLevelArray<T>::rbegin() {

	std::size_t i = levelsInfo.size() - 1;

	while ((i != -1) && (levelsInfo[i].first == 0)) {
		--i;
	}

	return ReverseIterator(this, { i, (i != -1) ? levelsInfo[i].first - 1 : 0 });
}

template<class T>
typename MultiLevelArray<T>::ReverseIterator MultiLevelArray<T>::rend() {

	std::size_t i = -1;

	while ((i < rbegin().getPosition().first) && (levelsInfo[i].first > 0)) {
		++i;
	}

	return ReverseIterator(this, { i, (i != -1) ? levelsInfo[i].first : 0 });
}

template<class T>
typename MultiLevelArray<T>::ReverseIterator MultiLevelArray<T>::rbegin(
		std::size_t level) {

	std::size_t i = level;

	while ((i != -1) && levelsInfo[i].first == 0) {
		--i;
	}

	return ReverseIterator(this, { i, (i != -1) ? levelsInfo[i].first - 1 : 0 });
}

template<class T>
typename MultiLevelArray<T>::ReverseIterator MultiLevelArray<T>::rend(
		std::size_t level) {

	std::size_t i = level - 1;

	while ((i != -1) && (levelsInfo[i].first == 0)) {
		--i;
	}

	return ReverseIterator(this, { i, (i != -1) ? levelsInfo[i].first : 0 });
}

template<class T>
inline bool MultiLevelArray<T>::checkBounds(
		const std::pair<std::size_t, std::size_t>& position) {

	return (position.first < levelsInfo.size())
			&& (position.second < levelsInfo[position.first].second);
}

template<class T>
inline T& MultiLevelArray<T>::Iterator::operator *() {
	return (*array)[position];
}

template<class T>
typename MultiLevelArray<T>::Iterator MultiLevelArray<T>::ReverseIterator::reverse() {
	return Iterator(this->array, getPosition());
}

template<class T>
inline T& MultiLevelArray<T>::ReverseIterator::operator *() {
	return (*array)[position];
}

template<class T>
typename MultiLevelArray<T>::ReverseIterator MultiLevelArray<T>::Iterator::reverse() {
	return ReverseIterator(this->array, getPosition());
}

#endif /* SRC_MULTILEVELARRAY_H_ */
