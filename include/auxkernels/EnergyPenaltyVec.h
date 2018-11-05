#ifndef ENERGYPENALTYVEC_H
#define ENERGYPENALTYVEC_H

#include "AuxKernel.h"

// Forward Declarations
class EnergyPenaltyVec;

template <>
InputParameters validParams<EnergyPenaltyVec>();

class EnergyPenaltyVec : public AuxKernel
{
 public:
  EnergyPenaltyVec(const InputParameters & parameters);
  
 protected:
  virtual Real computeValue() override;
  const VectorVariableGradient & _grad_n;
  Real _constfactor;
};

#endif // ENERGYPENALTYVEC_H
