#ifndef VECTORGRADSCALAR_H
#define VECTORGRADSCALAR_H

#include "VectorKernel.h"

class VectorGradScalar;

template <>
InputParameters validParams<VectorGradScalar>();

class VectorGradScalar : public VectorKernel
{
 public:
  VectorGradScalar(const InputParameters & parameters);

 protected:
  virtual Real computeQpResidual() override;
  virtual Real computeQpJacobian() override;
  virtual Real computeQpOffDiagJacobian(unsigned jvar) override;

  const Real & _coef;
  const VariableGradient & _grad_eta;
  const unsigned _scalar_id;
  MooseVariable & _scalar_var;
  const VariablePhiGradient & _grad_standard_phi;
};

#endif /* VECTORGRADSCALAR_H */
