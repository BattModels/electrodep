//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#ifndef ANCHORPENALTYVECH_H
#define ANCHORPENALTYVECH_H

#include "Kernel.h"
#include "DerivativeMaterialInterface.h"

// Forward Declarations
class AnchorPenaltyVecH;

template <>
InputParameters validParams<AnchorPenaltyVecH>();

/**
 * AnchorPenalty is a constraint kernel adds a penalty
 * to each order parameter to
 * enforce \f$ \sum_n h_i(\eta_i) \equiv 1 \f$.
 */
class AnchorPenaltyVecH : public DerivativeMaterialInterface<Kernel>
{
public:
  AnchorPenaltyVecH(const InputParameters & parameters);

protected:
  virtual Real computeQpResidual() override;
  virtual Real computeQpJacobian() override;
  virtual Real computeQpOffDiagJacobian(unsigned int jvar) override;

  const VectorVariableValue & _n;
  unsigned int _n_id;
  VectorMooseVariable & _n_var;
  const VectorVariablePhiValue & _vec_phi;
  const Real _penalty;
  const MaterialProperty<Real> & _h;
  const MaterialProperty<Real> & _dhdu;
  //  const MaterialProperty<Real> & _L;
  //  const bool _variable_L;
};

#endif // ANCHORPENALTYVECH_H
