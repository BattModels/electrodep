#ifndef ELECDRIVINGFORCE_H
#define ELECDRIVINGFORCE_H

#include "AuxKernel.h"

// Forward Declarations
class ElecDrivingForce;

template <>
InputParameters validParams<ElecDrivingForce>();

class ElecDrivingForce : public AuxKernel
{
 public:
  ElecDrivingForce(const InputParameters & parameters);
  
 protected:
  virtual Real computeValue() override;
  const VectorVariableGradient & _grad_n;
  Real _constfactor;
  const MaterialProperty<Real> & _Gox;
  const MaterialProperty<Real> & _Gred;
};

#endif // ELECDRIVINGFORCE_H
