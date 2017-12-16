#ifndef INTERPOLATOR_H

#define INTERPOLATOR_H

#include <map>

template<typename T>
class Interpolator {
public:
	typedef std::map<float, T> Map;
	typedef typename Map::iterator Iterator;
	typedef typename Map::const_iterator ConstIterator;

	Interpolator(const T& _defaultValue = T()) :
			defaultValue(_defaultValue) {
	}

	void addValue(float time, const T& value);

	T getValue(float time) const;

protected:

private:
	Map values;
	T defaultValue;
};

template<typename T>
void Interpolator<T>::addValue(float time, const T& value) {
	values[time] = value;
}

//Procura dois valores mais pr�ximos de 'time' que o cercam e faz uma interpola��o linear entre esses pontos
template<typename T>
T Interpolator<T>::getValue(float time) const {
	if (values.size() < 1)
		return defaultValue;

	if (values.size() == 1)
		return values.begin()->second;

	ConstIterator iter0 = values.begin();
	ConstIterator iter1 = iter0;

	while (iter1 != values.end()) {
		if (iter1->first > time) {
			break;
		}
		iter0 = iter1;
		++iter1;
	}

	if (iter1 == values.end()) {
		return iter0->second;
	}

	float t0 = iter0->first;
	float t1 = iter1->first;

	float ratio = (time - t0) / (t1 - t0);

	const T& value0 = iter0->second;
	const T& value1 = iter1->second;

	return value0 * (1.0f - ratio) + (value1 * ratio);
}
#endif // INTERPOLATOR_H
