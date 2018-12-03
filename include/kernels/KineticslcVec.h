#ifndef KINETICSLCVEC_H
#define KINETICSLCVEC_H

#include "Kernel.h"
#include "DerivativeMaterialInterface.h"
#include "JvarMapInterface.h"

class KineticslcVec;

template <>
InputParameters validParams<KineticslcVec>();

class KineticslcVec: public DerivativeMaterialInterface<JvarMapKernelInterface<Kernel>>
{
public:
  KineticslcVec(const InputParameters & parameters);

protected:
  virtual Real computeQpResidual() override;
  virtual Real computeQpJacobian() override;
  virtual Real computeQpOffDiagJacobian(unsigned int jvar) override;
  unsigned int _n_id;

  const MaterialProperty<Real> & _Fbv;
  const MaterialProperty<Real> & _dFbvdu;
  std::vector<const MaterialProperty<Real> *> _dFbvdarg;
  const VectorVariableGradient & _grad_n;
  VectorMooseVariable & _n_var;
  const VectorVariablePhiGradient & _vec_grad_phi;
  const Real _constfactor;
};

#endif // KINETICSLCVEC_H
