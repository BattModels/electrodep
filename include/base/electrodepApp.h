//* This file is part of the MOOSE framework                                                                                            
//* https://www.mooseframework.org                                                                                                      
//*                                                                                                                                     
//* All rights reserved, see COPYRIGHT for full restrictions                                                                            
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT                                                                             
//*                                                                                                                                     
//* Licensed under LGPL 2.1, please see LICENSE for details                                                                             
//* https://www.gnu.org/licenses/lgpl-2.1.html                                                                                          
#ifndef ELECTRODEPAPP_H
#define ELECTRODEPAPP_H

#include "MooseApp.h"

class electrodepApp;

template <>
InputParameters validParams<electrodepApp>();

class electrodepApp : public MooseApp
{
 public:
  electrodepApp(InputParameters parameters);
  virtual ~electrodepApp();

  static void registerApps();
  static void registerAll(Factory & factory, ActionFactory & action_factory, Syntax & syntax);
  static void associateSyntax(Syntax & /*syntax*/, ActionFactory & /*action_factory*/) {}

};

#endif /* ELECTRODEPAPP_H */
