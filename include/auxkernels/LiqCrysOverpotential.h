#ifndef LIQCRYSOVERPOTENTIAL_H
#define LIQCRYSOVERPOTENTIAL_H

#include "AuxKernel.h"
#include "DerivativeMaterialInterface.h"

class LiqCrysOverpotential;

template <>
InputParameters validParams<LiqCrysOverpotential>();

class LiqCrysOverpotential : public DerivativeMaterialInterface<AuxKernel>
{
 public:
  LiqCrysOverpotential(const InputParameters & parameters);
  
 protected:
  virtual Real computeValue() override;
  
  const VariableValue & _eta;
  const unsigned int _eta_var;
  const MaterialProperty<Real> & _h;
  const MaterialProperty<Real> & _dhdu;
  const VectorVariableValue & _n;
  const VectorVariableGradient & _grad_n;
  const VectorVariableValue & _gradeta;
  const VectorVariableGradient & _grad_gradeta;
  const Real _constfactor;
  const Real _penaltyconstfactor;
};

#endif // LIQCRYSOVERPOTENTIAL_H
