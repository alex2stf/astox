/*
 * AstoxBenc.h
 *
 *  Created on: Jul 7, 2014
 *      Author: alexandru
 */

#ifndef ASTOXBENC_H_
    #define ASTOXBENC_H_
    #include <iostream>
    #include <string>
    #include "astox.h"


namespace astox {

    namespace benchmark {

        using namespace std;
        using namespace astox;
        using namespace astox::xhtml;

        inline static void TEST(Boolean * in, Boolean expect) {
            cout << "TEST COMPLETE: (" << in << "==" << expect << ") SUCCESS: " << (in == expect) << endl;
        };

        inline static void testBam() {
            using namespace bam;
            //	cout << "sum(4) = " << bam::sum(4) << endl;
            //	cout << "sum(20) = " << bam::sum(20) << endl;
            //	cout << "gcd(0, 0) = " << gcd(0, 0) << endl;
            //	cout << "gcd(25, 10) = " << gcd(25, 10) << endl;
            cout << "gcd(10, 25) = " << gcd(10, 25) << endl;
            //	cout << "gcd(6, 18) = " << gcd(6, 18) << endl;
            //	cout << "gcd(234, 312) = " << gcd(234, 312) << endl;
            //	cout << "gcd(122, 4576) = " << gcd(122, 4576) << endl;
            //	cout << "gcd(-234, 312) = " << gcd(-234, 312) << endl;
        }

        inline static void testSScript() {
            SScript script;
            script.setContext(new Object());

            Object * astx_context = new Object();
            astx_context->addKey("a", new String("string"));
            astx_context->addKey("b", new Number(1));

            //	astx_context->registerFunction("_print", new ContextFunction(astx_context));
            //	astx_context->executeFunction("_print", "a, b,\"c\", d");

            script.getCommand("_print(a, b, \"c\", 1, false, dbs)", astx_context);


            return;

            Value * context = script.fromStrValue("{a:2, b:bla-bla-content, c:[1, \"str\", false, {obj:1}], d:{a:1, b:2}}");

            Value * result = script.getCommand("a", context);
            cout << astox::lang::toJSON(result) << endl;

            Value * result2 = script.getCommand("c._at(3)", context);
            cout << astox::lang::toJSON(result2) << endl;

            Value * result3 = script.getCommand("c._at(3).obj", context);
            cout << astox::lang::toJSON(result3) << endl;


            //	while(command!="exit"){
            //		cin >> command;
            //		context = script.getCommand(command, context);
            //		cout << astox::lang::toJSON(context) << endl;
            //
            //	}





        }

        inline static void testQuery() {
            Object scannable = "{a:1, b:2, c:3, d:4, e:5}";
            Query query(&scannable);
            query.run("SELECT * ANY VAL > 3 AND KEY = d JOIN REST");



            return;
            TEST(new Boolean(Query::compare(new Object(), ">", "20")), false);
            TEST(new Boolean(Query::compare(new Object(), "<", "0")), false);
            TEST(new Boolean(Query::compare(new Object(), "<=", "0")), true);
            TEST(new Boolean(Query::compare(new Object(), ">=", "0")), true);
            TEST(new Boolean(Query::compare(new Object(), "==", "0")), true);
            TEST(new Boolean(Query::compare(new Object(), "~", "{*")), true);
            TEST(new Boolean(Query::compare(new Object(), "~", "{}")), true);
            TEST(new Boolean(Query::compare(new Object(), "~", "{*}")), false);
            TEST(new Boolean(Query::compare(new Object(), "==", "{}")), true);
            TEST(new Boolean(Query::compare(new Number(10), ">", "20")), false);

            TEST(new Boolean(Query::compare(new String("alpha"), ">", "beta")), false);
            TEST(new Boolean(Query::compare(new String("alpha"), "<", "beta")), true);
            TEST(new Boolean(Query::compare(new String("alpha"), "<=", "beta")), true);
            TEST(new Boolean(Query::compare(new String("alpha"), ">=", "beta")), false);
            TEST(new Boolean(Query::compare(new String("alpha"), "==", "beta")), false);
            TEST(new Boolean(Query::compare(new String("alpha"), "~", "beta")), false);




            bool answ2 = Query::compare(new Number(30), "<", "20");
            cout << new Boolean(answ2) << endl;

            bool answ3 = Query::compare(new Number(30), ">", "20");
            cout << new Boolean(answ3) << endl;

            bool answ4 = Query::compare(new Number(40), "==", "40");
            cout << new Boolean(answ4) << endl;

            bool answ5 = Query::compare(new Number(-23455), "==", "40");
            cout << new Boolean(answ5) << endl;


            bool answ6 = Query::compare(new Number(-23455), "<=", "40");
            cout << new Boolean(answ6) << endl;

            bool answ7 = Query::compare(new Number(34), ">=", "35");
            cout << new Boolean(answ7) << endl;

            bool answ8 = Query::compare(new Number(99), ">=", "99");
            cout << new Boolean(answ8) << endl;


            TEST(new Boolean(Query::compare(new String("data"), "~", "*ata")), true);
            TEST(new Boolean(Query::compare(new String("dat another"), "~", "\"dat a\"*")), true);
            TEST(new Boolean(Query::compare(new String("data"), "~", "data")), true);
            TEST(new Boolean(Query::compare(new String("comps"), "~", "*omps")), true);
            TEST(new Boolean(Query::compare(new String("fff"), "~", "*")), true);
            TEST(new Boolean(Query::compare(new String("data"), "~", "*dat*")), false);
            TEST(new Boolean(Query::compare(new String("data"), "~", "sss*")), false);
            TEST(new Boolean(Query::compare(new String("comps"), "~", "*112")), false);


            //Query::describe();
        }

        static inline void testCircularObject() {
            Object * obiect = new Object();
            obiect->addKey("s1_string", new String("name"));

            Object data = *(obiect);
            obiect->addKey("s1_number", new Number(1));
            obiect->addKey("s1_self_direct", obiect);
            obiect->addKey("s1_self_referenced", &data);


            Object * urlOne = new Object();
            Object * urlTwo = new Object();
            urlOne->addKey("url1_url2", urlTwo);
            urlOne->addKey("url1_self", urlOne);
            urlTwo->addKey("url2_number", new Number(2));
            urlTwo->addKey("url2_bool", new Boolean(false));
            urlOne->addKey("url1_bool", new Boolean(false));

            obiect->addKey("s1_url1", urlOne);
            obiect->addKey("s1_url2", urlTwo);
            urlTwo->addKey("url2_parent", urlOne);

            astox::lang::scannedObjects.clear();
            cout << astox::lang::toJsonLine(obiect) << std::endl;
            //		cout << astox::lang::toJsonLine(obiect->getKeyAsObject("urlOne")->getKeyAsObject("inherit")->getKeyAsObject("inherit")->getKeyAsObject("inherit")->getKey("data")) << endl;
            //		cout << astox::lang::toJsonLine(Object::cast(Object::cast(obiect->getKey("self"))->getKey("self"))->getKey("test")) << endl;
        }

        static inline void testXHTML() {

            string data = "<unu></unu \n><selfClosed attr/>";
            //	XHTMLNode * node = new XHTMLNode(data);

            string content;
            astox::stringAlg::appendStringFromFile("inputFiles/xhtml.html", content);

            cout << content << endl;

            xhtml::parse(content);

        }

        static inline void testBeautifiers() {
            Object * root = new Object();
            root->addKey("c1_one", new String("val1"));
            root->addKey("c1_two", new String("val2"));
            root->addKey("c1_three", new String("val3"));

            Object * child = new Object();
            child->addKey("one", new String("val4"));
            child->addKey("two", new String("val5"));
            child->addKey("three", new String("val6"));

            root->addKey("child1", child);

            Object * child2 = new Object();
            Object * child23 = new Object();
            Object * child231 = new Object();
            child231->addKey("c231_bool", new Boolean(false));
            child23->addKey("child231", child231);


            child2->addKey("child23", child23);



            root->addKey("child2", child2);



            Array * basicArray = new Array();
            basicArray->push(new String("test"));
            basicArray->push(new Number(1));
            basicArray->push(new Boolean(true));


            Object * arrObj = new Object();
            arrObj->addKey("value", new Number(23.67));
            arrObj->addKey("minVal", new Number(-23.6548888));
            basicArray->push(arrObj);

            //child2->addKey("basicArray", basicArray);
            root->addKey("basicArray", basicArray);
            //cout << lang::toJsonLine(root) << endl;
            cout << lang::toJSON(root) << endl;

            XMLKeyNames names;
            names.run_in_OBJ = true;
            cout << lang::toXMLLine(root, names) << endl;
        }

        static inline void testBasicObject2XML() {
            XMLKeyNames names;
            String * string = new String("testString");
            cout << astox::lang::toXMLLine(string, names) << endl;

            Number * number = new Number(23);
            cout << astox::lang::toXMLLine(number, names) << endl;


            Boolean * boolean = new Boolean(false);
            cout << astox::lang::toXMLLine(boolean, names) << endl;


            Value * value = new Value();
            cout << astox::lang::toXMLLine(value, names) << endl;

        }

        static inline void testXHTML2Object() {



            string content;
            astox::stringAlg::appendStringFromFile("inputFiles/xhtml.html", content);
            std::vector<XHTMLNode *> nodes = xhtml::parse(content);
            Object * res = new Object();
            xhtml::fromXHTMLNodes(nodes, res);
            cout << astox::lang::toJSON(res) << endl;
            XMLKeyNames names;
            //		names.run_in_OBJ = false;
            cout << astox::lang::toXMLLine(res, names, "data") << endl;
        }

        static inline void testHashParsers() {
            string in = "prop1 object=\"{one:1, two:2, three:3}\" sngprop2 val =\"key\" val2= \"this value can contain =  spaces\" booleanProp prop=2 val = \"final ovveride in here\" trei= data patru =cinci justaprop ";
            astox::lang::HashParser parser;
            Object * pobje = new Object();
            pobje->setKeys(parser.fromHash(in));

            parser.trace();
            cout << "decode input sample: |" << astox::lang::HashParser::toHash(pobje) << "|" << endl;

            Object * inobj = new Object();
            inobj->addKey("one", new Number(1));
            inobj->addKey("two", new String("doi"));
            inobj->addKey("bool", new Boolean(true));
            inobj->addKey("object", new Object());

            Array * array = new Array();
            array->push(new String("one"));
            array->push(new Number(54));
            array->push(new Boolean(false));
            array->push(inobj);
            array->push(array);

            inobj->addKey("array", array);

            cout << "array sample: |" << astox::lang::HashParser::toHash(array) << "|" << endl;
            cout << "object sample: |" << astox::lang::HashParser::toHash(inobj) << "|" << endl;

        }

    }

}

#endif /* ASTOXBENC_H_ */
