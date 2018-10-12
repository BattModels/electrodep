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
  virtual Real computeQpOffDiagJacobian(unsigned int jvar) override;

  const VariableValue & _lambda;
  unsigned int _lambda_var;
};

#endif /* NMAGNITUDEVECCONSTRAINTN_H */

/*  LocalWords:  NMAGNITUDEVECCONSTRAINTN
 */
