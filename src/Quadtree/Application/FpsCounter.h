/*******************************************************************************
* File:           FpsCounter.h
* Author:         Christian Alfons
* Date created:   2009-07-21
* Date modified:  2009-07-22
* Description:    A frame rate counter class.
*******************************************************************************/

#ifndef FPSCOUNTER_H_INCLUDED
#define FPSCOUNTER_H_INCLUDED

// Represents an FPS counter
template<typename Real>
class FpsCounter
{
  private:
    // Instance variables
    Real         frameRate;
    Real         timeSinceLastUpdate;
    unsigned int framesSinceLastUpdate;

  public:
    // Constructors
    FpsCounter();

    // Instance methods
    Real GetFps() const;
    void Update(Real frameTime);
};

// Default constructor
template<typename Real>
FpsCounter<Real>::FpsCounter()
{
  frameRate             = Real(0.0);
  timeSinceLastUpdate   = Real(0.0);
  framesSinceLastUpdate = 0;
}

// Returns the frame rate
template<typename Real>
Real FpsCounter<Real>::GetFps() const
{
  return frameRate;
}

// Updates the frame rate (once every frame)
template<typename Real>
void FpsCounter<Real>::Update(Real frameTime)
{
  // Increase the frame count
  framesSinceLastUpdate++;

  // Add to the time since the last update
  timeSinceLastUpdate += frameTime;

  // If the last frame rate update was a second or more ago
  if (timeSinceLastUpdate >= Real(1.0))
  {
    // Update the frame rate
    frameRate             = framesSinceLastUpdate / timeSinceLastUpdate;
    timeSinceLastUpdate   = Real(0.0);
    framesSinceLastUpdate = 0;
  }
}

#endif
