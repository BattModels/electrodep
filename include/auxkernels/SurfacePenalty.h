#ifndef SURFACEPENALTY_H
#define SURFACEPENALTY_H

#include "AuxKernel.h"

// Forward Declarations
class SurfacePenalty;

template <>
InputParameters validParams<SurfacePenalty>();

/**
 * Coupled auxiliary value
 */
class SurfacePenalty : public AuxKernel
{
 public:
  /**
   * Factory constructor, takes parameters so that all derived classes can be built using the same
   * constructor.
   */
  SurfacePenalty(const InputParameters & parameters);

 protected:
  virtual Real computeValue() override;

  const VariableValue & _nx;
  const VariableValue & _ny;
  const VariableGradient & _grad_eta;
  Real _penalty;
};

#endif // SURFACEPENALTY_H
