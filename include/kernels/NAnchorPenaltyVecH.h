#ifndef NANCHORPENALTYVECH_H
#define NANCHORPENALTYVECH_H

#include "VectorKernel.h"
#include "DerivativeMaterialInterface.h"

class NAnchorPenaltyVecH;

template <>
InputParameters validParams<NAnchorPenaltyVecH>();

class NAnchorPenaltyVecH : public DerivativeMaterialInterface<VectorKernel>
{
 public:
  NAnchorPenaltyVecH(const InputParameters & parameters);

 protected:
  virtual Real computeQpResidual() override;
  virtual Real computeQpJacobian() override;
  virtual Real computeQpOffDiagJacobian(unsigned jvar) override;

  const Real _penalty;
  const MaterialProperty<Real> & _h;
  const VariableValue & _eta;
  const VariableGradient & _grad_eta;
  const unsigned _eta_id;
  MooseVariable & _eta_var;
  const MaterialProperty<Real> & _dhdeta;
  const VariablePhiGradient & _grad_standard_phi;
};

#endif /* NANCHORPENALTYVECH_H */
