#ifndef NMAGNITUDEVECCONSTRAINTN_H
#define NMAGNITUDEVECCONSTRAINTN_H

#include "VectorKernel.h"

class NMagnitudeVecConstraintN;

template <>
InputParameters validParams<NMagnitudeVecConstraintN>();

class NMagnitudeVecConstraintN : public VectorKernel
{
 public:
  NMagnitudeVecConstraintN(const InputParameters & parameters);

 protected:
  virtual Real computeQpResidual() override;
  virtual Real computeQpJacobian() override;
  virtual Real computeQpOffDiagJacobian(unsigned jvar) override;

  const VariableValue & _lambda;
  const unsigned _lambda_id;
  MooseVariable & _lambda_var;
  const VariablePhiValue & _standard_phi;
};

#endif /* NMAGNITUDEVECCONSTRAINTN_H */

/*  LocalWords:  NMAGNITUDEVECCONSTRAINTN
 */
