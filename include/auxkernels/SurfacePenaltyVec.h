#ifndef SURFACEPENALTYVEC_H
#define SURFACEPENALTYVEC_H

#include "AuxKernel.h"

// Forward Declarations
class SurfacePenaltyVec;

template <>
InputParameters validParams<SurfacePenaltyVec>();

class SurfacePenaltyVec : public AuxKernel
{
 public:
  SurfacePenaltyVec(const InputParameters & parameters);
  
 protected:
  virtual Real computeValue() override;
  const VectorVariableValue & _n;
  const VariableGradient & _grad_eta;
  Real _penalty;
};

#endif // SURFACEPENALTYVEC_H
