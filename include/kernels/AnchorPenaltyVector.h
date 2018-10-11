#ifndef ANCHORPENALTYVECTOR_H
#define ANCHORPENALTYVECTOR_H

#include "VectorKernel.h"

class AnchorPenaltyVector;

template <>
InputParameters validParams<AnchorPenaltyVector>();

class AnchorPenaltyVector : public VectorKernel
{
 public:
  AnchorPenaltyVector(const InputParameters & parameters);

 protected:
  virtual Real computeQpResidual() override;
  virtual Real computeQpJacobian() override;
  virtual Real computeQpOffDiagJacobian(unsigned int jvar) override;

  const Real _penalty;
  const VariableGradient & _grad_eta;
  unsigned int _eta_var;
};

#endif /* ANCHORPENALTYVECTOR_H */
