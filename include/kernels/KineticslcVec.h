#ifndef KINETICSLCVEC_H
#define KINETICSLCVEC_H

#include "Kernel.h"
#include "DerivativeMaterialInterface.h"

class KineticslcVec;

template <>
InputParameters validParams<KineticslcVec>();

class KineticslcVec: public DerivativeMaterialInterface<Kernel>
{
public:
  KineticslcVec(const InputParameters & parameters);

protected:
  virtual Real computeQpResidual() override;
  virtual Real computeQpJacobian() override;
  virtual Real computeQpOffDiagJacobian(unsigned int jvar) override;
  unsigned int _cp_var, _cv_var, _n_id;
  const VariableValue & _cp;
  const VariableValue & _cv;
  const VectorVariableGradient & _grad_n;
  VectorMooseVariable & _n_var;
  const VectorVariablePhiGradient & _vec_grad_phi;
  const MaterialProperty<Real> & _F;
  const MaterialProperty<Real> & _dFe;
  const MaterialProperty<Real> & _dFv;
  const MaterialProperty<Real> & _dF;
  const Real _constfactor;
};

#endif // KINETICSLCVEC_H
