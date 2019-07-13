# implementation of the grand-potential phase-field model based on M.Plapp PRE 84,031601(2011)+D.Cogswell PRE92, 011301(R) (2015)
#1;95;0c w is the chemical potential, eta is the phase-field, pot is the electric potential, op is the overpotential
# add Bulter-Volmer equation with non-linear kinetics by ZJH Oct 2nd 2017
# major modifs and liquid electrolyte with orientation order by Zeeshan Ahmad
[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 200
  xmax = 200
  ny = 200
  ymax = 200
#  elem_type = QUAD9
[]

[GlobalParams]
  enable_jit = true
  penalty = 1.00E+01
  constfactor = 1.00E+00
  penaltyconstfactor = 5.09E-01
[]

[Variables]
  active = 'w eta pot nlc lambda gradeta' 
  [./w]
# order=SECOND
# family= LAGRANGE
  scaling = 1e+3
  [../]
  [./eta]
#  order=SECOND
#  family= HERMITE
  scaling = 1e+4
  [../]
  [./pot]
# order=SECOND
# family= LAGRANGE
  [../]
  [./nlc]
  family = LAGRANGE_VEC
  order = FIRST
  [../]
  [./gradeta]
  family = LAGRANGE_VEC
  order = FIRST
  scaling = 1e+2
  [../]
  [./lambda]
  [../]
[]

[AuxVariables]
  active = 'edf epen surfpen lcovp'
  [./edf]
  order = FIRST
  family = MONOMIAL
  [../]
  [./epen]
  order = FIRST
  family = MONOMIAL
  [../]
  [./surfpen]
  order = FIRST
  family = MONOMIAL
  [../]
  [./lcovp]
  order = FIRST
  family = MONOMIAL
  [../]
[]

[Functions]
  [./ic_func_eta]
  type = ParsedFunction
  value = 'r:=sqrt((x-20.0)^2+(y-100)^2);if(x<20.0, 0.5*(1.0-1.0*tanh((x-20)/2)), 0.5*(1.0-1.0*tanh((r-5)/2)) )'
  [../]
  [./ic_func_c]
  type = ParsedFunction
  value = 0
  [../]
  [./ic_func_pot]
  type = ParsedFunction
  value= 'r:=sqrt((x-20.0)^2+(y-100)^2);if(x<20.0, -0.1*(1.0-tanh((x-20)/2)), -0.1*(1.0-tanh((r-5)/2)) )'
  [../]
#  [./ic_func_op]
#  type = ParsedFunction
#  value = 0
#  [../]
[]

[ICs]
  [./ic_nlc]
  type = VectorConstantIC
  variable = nlc
  x_value = 0
  y_value = 1
  z_value = 0
  [../]
  [./ic_eta]
  variable = eta
  type = FunctionIC
  function = ic_func_eta
  [../]
  [./ic_w]
  variable = w
  type = FunctionIC
  function = ic_func_c
  [../]
  [./ic_pot]
  variable = pot
  type = FunctionIC
  function = ic_func_pot
  [../]
  [./lambda_ic]
  variable = lambda
  type = ConstantIC
  value = 0.0
  [../]
[]

[BCs]
  active = 'bottom_eta top_eta left_eta right_eta bottom_w top_w left_w right_w left_pot right_pot left_director right_director'
  [./bottom_eta]
  type = NeumannBC
  variable = 'eta'
  boundary = 'bottom'
  value = 0
  [../]
  [./top_eta]
  type = NeumannBC
  variable = 'eta'
  boundary = 'top'
  value = 0
  [../]
  [./left_eta]
  type = DirichletBC
  variable = 'eta'
  boundary = 'left'
  value = 1
  [../]
  [./right_eta]
  type = DirichletBC
  variable = 'eta'
  boundary = 'right'
  value = 0
  [../]
  [./bottom_w]
  type = NeumannBC
  variable = 'w'
  boundary = 'bottom'
  value = 0
  [../]
  [./top_w]
  type = NeumannBC
  variable = 'w'
  boundary = 'top'
  value = 0.0
  [../]
  [./left_w]
  type = NeumannBC
  variable = 'w'
  boundary = 'left'
  value = 0
  [../]
  [./right_w]
  type = DirichletBC
  variable = 'w'
  boundary = 'right'
  value = 0.0
  [../]
  [./left_pot]
  type = DirichletBC
  variable = 'pot'
  boundary = 'left'
  value = -0.2
  [../]
  [./right_pot]
  type = DirichletBC
  variable = 'pot'
  boundary = 'right'
  value = 0
  [../]
  [./left_director]
  type = LagrangeVecDirichletBC
  variable = nlc
  boundary = 'left'
  values = '0 1 0'
  [../]
  [./right_director]
  type = LagrangeVecDirichletBC
  variable = nlc
  boundary = 'right'
  values = '0 1 0'
  [../]
[]

[Kernels]
  active = 'gradeta gradeta_setval nlc_diff nlc_surf nlc_mag lagrange w_dot Diffusion1 Diffusion2 poteqn coupled_pot BVnlc BV coupled_etadot AC_bulk AC_int AC_bulk_lc_penalty e_dot'
  [./gradeta]
  type = VectorCoeffReaction
  variable = gradeta
  coefficient = 1.0
  [../]
  [./gradeta_setval]
  type = VectorGradScalar
  variable = gradeta
  scalar = eta
  coef = 1.0
  [../]
  [./nlc_diff]
  type = VectorCoefDiffusion #Take care of the minus sign when adding any other terms
  variable = nlc
  coef = 3.93E+01
#  h_name = h
#  eta = eta
  [../]
  [./nlc_surf]
  type = NAnchorPenaltyVec
  variable = nlc
  eta = eta
#  h_name = h
  [../]
  [./nlc_mag]
  type = NMagnitudeVecConstraintN
  variable = nlc
  lambda = lambda
  [../]
  [./lagrange]
  type = NMagnitudeVecConstraintLagrange
  variable = lambda
  vec = nlc
  epsilon = 1.0e-4
  [../]
  [./w_dot]
  type = SusceptibilityTimeDerivative
  variable = w
  f_name = chi
  args = 'w eta' # in this case chi (the susceptibility) is simply a constant
  [../]
  [./Diffusion1]
  type = MatDiffusion
  variable = w
  D_name = D
  args = 'eta w'
  [../]
  [./Diff2]
  type = MatDiffusion
  variable = w
  args = 'eta w'
  D_name = DN
  [../]
  [./Diffusion2]
  type = Diff
  variable = w
  cv = eta
  Q_name = zz
  QM_name = DN
  cp = pot
#args = 'w'
  [../]
  [./Noisew]
  type = LangevinNoise
  variable = w
  amplitude = 0.0
  [../]
  [./coupled_etadot]
  type = CoupledSusceptibilityTimeDerivative
  variable = w
  v = eta
  f_name = ft
  args = 'eta'
  [../]
  [./elecN]
  type = Electroneutrality
  variable = pot
  eta = eta
  sigma = Le2    #before phi
#  QM_name = 0.
  [../]
  [./poteqn]
  type = MatDiffusion
  variable = pot
  D_name = Le2
  args = 'eta'
  [../]
  [./coupled_pot]
  type = CoupledSusceptibilityTimeDerivative
  variable = pot
  v = eta
  f_name = ft2
  args = 'eta'
  [../]
  [./BVnlc] # with vector nlc
  type = KineticsLiqCrysVar
  variable = eta
  f_name = Gox
  args = 'pot w'
  n = 'nlc'
  h_name = h
  gradeta = gradeta
  [../]
  [./BV]
  type = Kinetics
  variable = eta
  f_name = Gred
  args = 'pot w'
  [../]
  [./AC_bulk]
  type = AllenCahn
  variable = 'eta'
  f_name = FF
  args = 'w'
  [../]
  [./AC_bulk_lc_penalty]
  type = AnchorPenaltyVec
  variable = eta
  n = 'nlc'
#  h_name = h
  [../]
  [./AC_int]
  type = ACInterface
  variable = eta
  [../]
  [./Noiseeta]
  type = LangevinNoise
  variable = eta
  amplitude = 0.04
  [../]
  [./e_dot]
  type = TimeDerivative
  variable = eta
  [../]
[]

[AuxKernels]
  active = 'edf  epen surfpenalty lctotovp'
  [./edf]
  type = ElecDrivingForce
  variable = edf
  n = nlc
  [../]
  [./epen]
  type = EnergyPenaltyVec
  variable = epen
  n = nlc
  [../]
  [./surfpenalty]
  type = SurfacePenaltyVec
  variable = surfpen
  n = nlc
  eta = eta
  [../]
  [./lctotovp]
  type = LiqCrysOverpotential
  variable = lcovp
  eta = eta
  n = 'nlc'
  h_name = h
  gradeta = gradeta
  [../]
[]

[Materials]
  [./constants]
  type = GenericConstantMaterial
#ul is the free energy density in the liquid phase
  prop_names  = 'kappa_op  M0    S1   S2          CL     L        Ls        B      es      el     zz    ul         us   dv   RT    Fconst      K     Liplusm3permol ft2  stdconc'
  prop_values = '0.33    319.7  3.953e12  4.704e5  14.4   6.318   1.347e-1  2.64  -13.64   2.598   0   6.854e-2   13.64  5.342  0.987  38.14   3.957e-7  0.05022 2.934e3  0.067083438'
# Ls value changed after ZJH suggested 100 times the value for limiting current i0
#  M0 M1 Normalized diffusion coefficient for liquid and solid B normalized constant for nF
#  M0=10^12*D    B=zF/(1000*R*T)   length 1 um, time 1s, energy is normalized by RT
# t transference number D+=D/(n*(1-t)) D-=D/(n*t)
  [../]
#  [./constfactor_lc]
#  type = ParsedMaterial
#  f_name = constfactor
#  function = '1/2 * K * Liplusm3permol / RT'
#  material_property_names = 'K Liplusm3permol RT'
#  [../]
  [./liquid_GrandPotential]
  type = DerivativeParsedMaterial
  function = 'ul - RT * log(1 + exp((w - el)/RT))'
  args = 'w'
  f_name = f1
  material_property_names = 'ul el RT'
  derivative_order = 2
    #outputs = exodus
  [../]
  [./solid_GrandPotential]
  type = DerivativeParsedMaterial
  function = 'us - RT * log(1 + exp((w - es) / RT))'
  args = 'w'
  f_name = f2
  material_property_names = 'us es RT'
  derivative_order = 2
   # outputs = exodus
  [../]
  [./switching_function]
  type = SwitchingFunctionMaterial
  eta ='eta'
  h_order = HIGH
  function_name = h
  # outputs = exodus
  [../]
  [./barrier_function]
  type = BarrierFunctionMaterial
  eta = eta
  [../]
  [./total_GrandPotential]
  type = DerivativeTwoPhaseMaterial
  args = 'w'
  eta = eta
  fa_name = f1
  fb_name = f2
  derivative_order = 2
  W = 2.64
   # outputs = exodus
  [../]
  [./coupled_eta_function]
  type = DerivativeParsedMaterial
  function = '-(cs * dv - cl) * dh'  # in this code cs=-cs h=eta dh=1
  args = 'w eta'
  f_name = ft
  material_property_names = 'dh:=D[h(eta),eta] h(eta) dv cs:=D[f2(w),w] cl:=D[f1(w),w]'
  derivative_order = 1
  [../]
  [./susceptibility]
  type = DerivativeParsedMaterial
  function = '-d2F1 * (1-h) - d2F2 * h * dv'
  args = 'w eta'
  f_name = chi
  derivative_order = 1
#outputs = exodus
  material_property_names = 'h(eta) dv d2F1:=D[f1(w),w,w] d2F2:=D[f2(w),w,w]'
  [../]
  [./Diffusion_coefficient]
  type = DerivativeParsedMaterial     #cn=h*(1-cs)+(1-h)*(1-2*cl)
  function = '-M0 * cl * (1 - h)'  #c is -c, removed 1-h extra factor which was there for convergence
  f_name = D
  args = 'eta w'
  derivative_order = 2
  material_property_names = 'M0 cl:=D[f1(w),w] h(eta)'
  #  outputs = exodus
  [../]
  [./Free]
  type = DerivativeParsedMaterial
  f_name = FF
  material_property_names = 'B'
  args = 'eta'
  function = 'B*eta*eta*(1-eta)*(1-eta)'
  derivative_order = 2
 #outputs = exodus
  [../]
  [./Convection_coefficient]
  type = DerivativeParsedMaterial     #cn=h*(1-cs)+(1-h)*(1-2*cl)
  function = '-cl * (1-h) * Fconst / RT * M0 * (1-h)'  #c is -c
  args = 'eta w'
  f_name = DN
  derivative_order = 1
  material_property_names = 'M0 cl:=D[f1(w),w] h(eta) RT Fconst'
  #   outputs = exodus
  [../]
#  [./ButlerVolmer]
#  type = DerivativeParsedMaterial
#  function = 'Ls * if(cl<0, if(h<1, sqrt(-cl * (1.0-h) / stdconc), 0), 0) * (exp(pot * Fconst/RT /2.0) + cl * (1-h) / stdconc * exp(-pot * Fconst/RT /2.)) * dh'
#  function = 'Ls * (exp(pot * Fconst/RT /2.0) + cl * (1-h) / stdconc * exp(-pot * Fconst/RT /2.)) * dh'
#  args = 'pot w eta'
#  f_name = G
#  derivative_order = 1
#  material_property_names = 'Ls dh:=D[h(eta),eta] h(eta) cl:=D[f1(w),w] RT Fconst stdconc'
#  outputs = other
#  [../]
  [./BVredn]
  type = DerivativeParsedMaterial
  function = 'Ls * cl * (1-h) / stdconc * exp(-(-0.4-pot) * Fconst / RT / 2.) * dh'
  args = 'pot w eta'
  f_name = Gred
  derivative_order = 1
  material_property_names = 'Ls dh:=D[h(eta),eta] h(eta) cl:=D[f1(w),w] RT Fconst stdconc'
  outputs = other
  [../]
  [./BVox]
  type = DerivativeParsedMaterial
  function = 'Ls * exp((-0.4-pot) * Fconst / RT / 2.) * dh'
  args = 'pot w eta'
  f_name = Gox
  derivative_order = 1
  material_property_names = 'Ls dh:=D[h(eta),eta] h(eta) cl:=D[f1(w),w] RT Fconst'
  outputs = other
  [../]
  [./eta]
  type = ParsedMaterial
  f_name = etao
  args='eta'
  function = 'eta'
  outputs = other
  [../]
  [./concentration]
  type = ParsedMaterial
  f_name = c
  args='eta w'
  material_property_names = 'h(eta) dFl:=D[f1(w),w]'
  function = '-dFl * (1-h)'
  outputs = other
  [../]
  [./Le2_name]
  type = DerivativeTwoPhaseMaterial
  fa_name = S2
  fb_name = S1
  f_name = Le2
#  args = 'eta'
  eta = eta
  derivative_order = 2
  [../]
  [./Le1]
  type = DerivativeParsedMaterial
  f_name = Le1
  function = 'S1*h + S2*(1-h)'
  material_property_names = 'S1 S2 h(eta)'
  #outputs = exodus
  derivative_order = 2
  [../]
  [./currdensity]
  type = ParsedMaterial
  f_name = currdens
  args = 'lcovp'
  function = 'Gox*exp(lcovp)+Gred'
  material_property_names = 'Gox Gred'
  outputs = other
  [../]
  [./lcarg]
  type = ParsedMaterial
  f_name = argexplc
  args = 'lcovp'
  function = 'exp(lcovp)'
  outputs = other
  [../]
#  [./mask_material]
#  type = ParsedMaterial
#  prop_names = 'mask_prop'
#  prop_values = 'mask_func'
#  [../]
#  [./mask_func]
#  type = ParsedMaterial
#  f_name = mask_func
#  args = eta
#  material_property_names = ''
#  function = 'if( eta<0.8, if(eta>0.2, 1.0, 0.0), 0.0 )'
#  outputs = other
#  [../]
[]

[Postprocessors]
  [./ETA]
  type = ElementIntegralMaterialProperty
  mat_prop = etao
  execute_on = 'initial timestep_end'
  [../]
  [./CONC]
  type = ElementIntegralMaterialProperty
  mat_prop = c
  execute_on = 'initial timestep_end'
  [../]
[]

[Preconditioning]
  [./SMP]
  type = SMP
  full = true
  petsc_options_iname = '-pc_type -ksp_grmres_restart -sub_ksp_type -sub_pc_type -pc_asm_overlap'
  petsc_options_value = 'asm      121                  preonly       lu           4'
  [../]
[]

[Executioner]
  type = Transient
  scheme = bdf2
  verbose = True
  solve_type = 'Newton'
#  dtmin = 1e-8
  l_max_its = 50
  l_tol = 1e-5
  nl_max_its = 50
  nl_rel_tol = 1e-6
  nl_abs_tol = 1e-10
  petsc_options_iname = '-pc_type -ksp_gmres_restart -sub_ksp_type -sub_pc_type -pc_asm_overlap'
  petsc_options_value = 'asm      31                  preonly      ilu          4'
#  petsc_options =  "-snes_view -snes_converged_reason -ksp_converged_reason -snes_error_if_not_converged -ksp_error_if_not_converged"
#  [./TimeStepper]
#    type = FunctionDT
#    time_t = '0        0.001    1    20'
#    time_dt ='1e-5    1.e-3   1.e-2 2.e-2 '
#     [../]
#  type = IterationAdaptiveDT
  dt=0.01
#  growth_factor = 1.5
#  optimal_iterations = 7
#  cutback_factor = 0.8
  end_time = 800
#  num_steps = 2
[]

[Outputs]
  exodus = false
  csv = true
  execute_on = 'TIMESTEP_END'
  [./other]        # creates input_other.e
  type = Exodus
  interval = 60
  [../]
  [./checkpt]
  type = Checkpoint
  num_files = 2
  interval = 500
  [../]
[]

[Debug]
  show_var_residual_norms = true
[]
