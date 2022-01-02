/*
 * serializers.h
 *
 *  Created on: 7 Jul 2017
 *      Author: alex
 */

#ifndef ASTOX_SERIALIZERS_H_
#define ASTOX_SERIALIZERS_H_

#include "types.h"

namespace astox {
	namespace serializers {
	stxtr to_composed(Value * v,
			stxtr &tableft,
			stxtr &tabrig,
				bool wk = true, bool wn = false, bool wb = false,
				char al = '[', char ar = ']',
				char q = '"', char c = ',', char il = '{', char ir = '}', char td = ':');
		stxtr to_json(Value * v);
		stxtr to_js(Value * v);
		stxtr to_csv(Value * v);

		Value * from_args(int argc, char* argv[],  const lexic &l);

	}
}


#endif /* SERIALIZERS_H_ */
