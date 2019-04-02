/*
 * serializers.cpp
 *
 *  Created on: 7 Jul 2017
 *      Author: alex
 */

#include "astox.h"


namespace astox {
	namespace serializers {

		stxtr to_composed(Value * v,
				stxtr &tl,
				stxtr &tr,
				bool wk, bool wn, bool wb,
				char al, char ar, char q, char c, char il, char ir, char td
				){


			if (v->isBoolean() && !wb) {
				return v->str();
			}

			if (v->isNumber() && !wn) {
				return v->str();
			}
			stxtr r =  "";
			int i = 0; size_t sz = 0;
			stxtr ntl = tl;
			if(!tl.empty()){
				ntl = tl.append(tl);
			}

			if (v->isArray()) {
				Array * a = Array::cast(v);
				sz = a->length();
				r+=al;//{
				if(!tr.empty()){ r.append(tr); } //\n

				for(i = 0; i < sz; i++){
					if(!tl.empty()){ r.append(tl); } //-
					stxtr tmp = (to_composed(
							Value::duplicate(a->at(i)),
							ntl, tr, wk, wn , wb, al, ar, q, c, il, ir, td
					));
					r.append(tmp);
					if(i < sz - 1 ) { r+=c; }
					if(!tr.empty()){ r.append(tr); } //\n
				}
				if(!tl.empty()){ r.append(tl); } //-
				r+=ar;

			}

			else if (v->isFunction()){
				r = "\"[function]\"";
			}

			else if (v->isObject()) {
				i = 0;
				Object * a = Object::cast(v);
				r+=il; //{
				if(!tr.empty()){ r.append(tr); } //\n
				object_map * om = a->getObjectList();
				sz = om->size();

				for(object_iterator oi = om->begin(); oi != om->end(); ++oi){

					if(!oi->second->isFunction()){
						if(i > 0 ){ r+=c; } // ,
						i++;
						if(!tl.empty()){ r.append(tl); } //-
						if(wk){ r+=q; }
						r.append(oi->first);
						if(wk){ r+=q; }
						r+=td;
						r.append(to_composed(
							oi->second,
							ntl, tr, wk, wn , wb, al, ar, q, c, il, ir, td));
						if(!tr.empty()){ r.append(tr); } //\n
					}
				}
				if(!tl.empty()){ r.append(tl); } //-
				r+=ir; //}

			}


			else {
				r += q;
				r.append(v->str());
				r += q;
			}
			std::cout << r << std::endl;
			return r;
		};

		stxtr to_json(Value * v){
			stxtr s1 = "", s2 = "";
			return to_composed(v, s1, s2);
		};


		stxtr to_js(Value * v){
			stxtr s1 = "", s2 = "";
			return to_composed(v,s1, s2, false, false, false, '[', ']', '\'');
		};


		stxtr to_csv(Value * v){
			stxtr r;
			if(v->isArray()){
				Array * a = Array::cast(v);
				for(size_t i = 0; i < a->length(); i++){
					if(i > 0){ r += ','; }
					if(a->at(i) != NULL && a->at(i) != Value::npos){
						r.append(to_csv(a->at(i)));
					}

				}
				return r;
			}

			else if(v->isObject() || v->isFunction()){
				r.append("[").append(v->strtype()).append("]");
			}
			else {
				r = v->str();
			}

			return r;
		};

		Value * from_args(int argc, char* argv[],  const lexic &l){

			if(argc % 2 == 0){
				Array * a = new Array(l);
				for(int i = 0; i < argc ; i++){
					a->push(String::New(argv[i], l));
				}
				return a;
			}

			Object * r = Object::New(l);
			if(argc == 0){
				return r;
			}
			r->addKey("exe", argv[0]);

			for(int i = 1; i < argc -1 ; i+=2){
				r->addKey(argv[i], argv[i+1]);
			}
			return  r;
		};
	}
}
