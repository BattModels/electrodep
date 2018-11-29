#ifndef NANCHORPENALTYVEC_H
#define NANCHORPENALTYVEC_H

#include "VectorKernel.h"

class NAnchorPenaltyVec;

template <>
InputParameters validParams<NAnchorPenaltyVec>();

class NAnchorPenaltyVec : public VectorKernel
{
 public:
  NAnchorPenaltyVec(const InputParameters & parameters);

 protected:
  virtual Real computeQpResidual() override;
  virtual Real computeQpJacobian() override;
  virtual Real computeQpOffDiagJacobian(unsigned jvar) override;

  const Real _penalty;
  const VariableGradient & _grad_eta;
  const unsigned _eta_id;
  MooseVariable & _eta_var;
  const VariablePhiGradient & _grad_standard_phi;
};

#endif /* NANCHORPENALTYVEC_H */
