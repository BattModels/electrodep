#ifndef KINETICSLCVECHPOSEPEN_H
#define KINETICSLCVECHPOSEPEN_H

#include "Kernel.h"
#include "DerivativeMaterialInterface.h"
#include "JvarMapInterface.h"

class KineticslcVecHPosEpen;

template <>
InputParameters validParams<KineticslcVecHPosEpen>();

class KineticslcVecHPosEpen: public DerivativeMaterialInterface<JvarMapKernelInterface<Kernel>>
{
public:
  KineticslcVecHPosEpen(const InputParameters & parameters);

protected:
  virtual Real computeQpResidual() override;
  virtual Real computeQpJacobian() override;
  virtual Real computeQpOffDiagJacobian(unsigned int jvar) override;

  unsigned int _n_id;
  const MaterialProperty<Real> & _h;
  const MaterialProperty<Real> & _Fbv;
  const MaterialProperty<Real> & _dFbvdu;
  const MaterialProperty<Real> & _dhdu;
  std::vector<const MaterialProperty<Real> *> _dFbvdarg;
  const VectorVariableGradient & _grad_n;
  VectorMooseVariable & _n_var;
  const VectorVariablePhiGradient & _vec_grad_phi;
  const Real _constfactor;
};

#endif // KINETICSLCVECHPOSEPEN_H
