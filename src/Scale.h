/*
 * Scale.h
 *
 *  Created on: 6 de jul de 2016
 *      Author: cassiano
 */

#ifndef SRC_SCALE_H_
#define SRC_SCALE_H_

/**
 * Singleton class to manage scales (time, length...).
 */
class Scale {
public:
	virtual ~Scale();

	/**
	 * Get length scale;
	 */
	static float getLength();

	/**
	 * Set length scale;
	 */
	static void setLength(float value);

	/**
	 * Get time scale;
	 */
	static float getTime();

	/**
	 * Set time scale;
	 */
	static void setTime(float value);

private:
	/**
	 * Length scale. Default value is 1.000.000 meters.
	 */
	float length;

	/**
	 * Time scale. Default value is 20.000.000 seconds.
	 */
	float time;

	/**
	 * Unique reference.
	 */
	static Scale instance;

	/**
	 * Default constructor.
	 */
	Scale();
};

#endif /* SRC_SCALE_H_ */
