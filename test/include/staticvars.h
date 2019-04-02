#ifndef ASTOX_STATIC_VARS_H
#define ASTOX_STATIC_VARS_H

#include <astox.h>

using namespace astox;
using namespace astox::scriptengine;
using namespace astox::testing;
using namespace astox::ecma::methods;
using namespace std;

static lexic lxc = Syntax::JS;
static Function * ctx = Function::buildStandardContext(lxc);

#endif
