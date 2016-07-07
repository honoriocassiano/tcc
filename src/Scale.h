/*
 * Scale.h
 *
 *  Created on: 6 de jul de 2016
 *      Author: cassiano
 */

#ifndef SRC_SCALE_H_
#define SRC_SCALE_H_

class Scale {
public:
	virtual ~Scale();

	static float getLength();
	static void setLength(float value);

	static float getTime();
	static void setTime(float value);

private:
	/**
	 * Default value is 1.000.000 meters
	 */
	float m_length;
	float m_time;

	static Scale* m_scale;

	Scale();
};

#endif /* SRC_SCALE_H_ */
