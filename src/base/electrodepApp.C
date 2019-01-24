#include "electrodepApp.h"
#include "Moose.h"
#include "AppFactory.h"
#include "ModulesApp.h"
#include "MooseSyntax.h"

template <>
InputParameters
validParams<electrodepApp>()
{
  InputParameters params = validParams<MooseApp>();
  return params;
}

electrodepApp::electrodepApp(InputParameters parameters) : MooseApp(parameters)
{
  electrodepApp::registerAll(_factory, _action_factory, _syntax);

}

electrodepApp::~electrodepApp() {}

void
electrodepApp::registerAll(Factory & f, ActionFactory & af, Syntax & s)
{
  ModulesApp::registerAll(f, af, s);
  Registry::registerObjectsTo(f, {"electrodepApp"});
  Registry::registerActionsTo(af, {"electrodepApp"});

  /* register custom execute flags, action syntax, etc. here */
}

void
electrodepApp::registerApps()
{
  registerApp(electrodepApp);
}

/***************************************************************************************************                                   \
                                                                                                                                        
 *********************** Dynamic Library Entry Points - DO NOT MODIFY ******************************                                   \
                                                                                                                                        
 **************************************************************************************************/
extern "C" void
electrodepApp__registerAll(Factory & f, ActionFactory & af, Syntax & s)
{
  electrodepApp::registerAll(f, af, s);
}
extern "C" void
electrodepApp__registerApps()
{
  electrodepApp::registerApps();
}
