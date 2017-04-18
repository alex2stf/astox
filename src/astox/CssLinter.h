/*
 * Tracer.h
 *
 *  Created on: Jan 27, 2014
 *      Author: alexandru
 *      //TODO work on C
 */

#ifndef ASTOX_BAM
    #define ASTOX_BAM

    #include <iostream>

    #include "astox.h"

using namespace std;

enum CSS_STATUS_CODES {
    CSS_DEFINITION,
    CSS_LEFT_DATA,
    CSS_RIGHT_DATA,
    CSS_END_BRACKET,
    CSS_COMMENT_BLOCK,
    CSS_INHERITABLE,
    CSS_IMPORT_DIRECTIVE
};

static const char* CSS_STATUS[] = {
    "definition started--->",
    "left data -    -  --->",
    "right data  -  -- --->",
    "definition finished ->",
    "comment block       ->",
    "inheritable object  ->",
    "import directive--  ->"
};

enum CSS_SELECTOR_TYPES {
    CSS_SELECTOR_TAG,
    CSS_SELECTOR_ID,
    CSS_SELECTOR_CLASS,
    CSS_SELECTOR_MEDIA_QUERY

};

class CssLintErrorThrower {
public:
    CssLintErrorThrower();

    void duplicatedRuleInLastDef(std::string def, int line, std::string rest) {
        fprintf(stderr, "WARNING: duplicatedRuleInLastDef %s at line %i \n      %s\n", def.c_str(), line, rest.c_str());
    }

    void commaNotAllowedInDef(int line, std::string def) {
        fprintf(stderr, "ERROR: comma (,) not allowed in Def %s at line %i \n", def.c_str(), line);
    }

    void emptyDefContext(int line) {
        fprintf(stderr, "ERROR: empty CssContext at line %i\n", line);
    }

    void parsingError(int line, std::string rest) {
        fprintf(stderr, "ERROR: parsing error at line %i\n     << %s >> \n", line, rest.c_str());
    }

    void parsingWarning(int line, std::string rest) {
        cout << "PARSING WARNING at line " << line << "       \nin " << rest << "\n\n\n\n\n";
        //		fprintf( stderr, "ERROR: parsing error at line %i\n     << %s >> \n", line, rest.c_str());
    }

    void endLineRequired(int line) {
        fprintf(stderr, "ERROR: \";\" required for end of line %i\n", line);
    }

    void definitionDuplicated(std::string definition, int line) {
        using namespace std;
        //cout << "definitionDuplicated WARNING: " << definition << " found " << " times " << " at line " << line << endl;

        fprintf(stderr, "WARNING: duplicated  definition \"%s\"\n        at line %i\n",
                definition.c_str(),
                line
                );
    }

    void ruleLeftDuplicated(std::string definition, std::string leftData, std::string rightData, int line) {

        //		perror("ruleDuplicated WARNING: in definition%s", definition.c_str());

        fprintf(stderr, "WARNING: ovveriden left rule at definition \"%s\"\n leftData: %s\n rightData: %s\n line %i\n",
                definition.c_str(),
                leftData.c_str(),
                rightData.c_str(),
                line
                );

        //		cout << "ruleDuplicated WARNING: in definition " << definition << "\n rule:" << leftData << rightData << " ))) at line "<< line << endl;
    }

    void ruleDuplicated(std::string definition, std::string summary, int times, int line) {
        cout << "ruleDuplicated WARNING: in definition " << definition << "\n rule:" << summary << " found " << times << " times at line " << line << endl;
    }
};


//un singur selector css

class CssSelector {
private:
    int _type; //type: 0- tag, 1-id, 2-class

public:
    std::string name;

    std::string getSelectorName() {
        switch (_type) {
            case CSS_SELECTOR_TAG:
                return name;

            case CSS_SELECTOR_ID:
                return "#" + name;

            case CSS_SELECTOR_CLASS:
                return "." + name;
        }

        return "?" + name;
    }

    CssSelector(int type, std::string data) {
        _type = type;
        name = data;
    };
};




//o singura linie de css

struct CssLine {
    int status;
    std::string buffer;
    int line;

    CssLine(int _status, std::string _buff, int _line) {
        status = _status;
        buffer = _buff;
        line = _line;
    };
};

enum CSS_CONTEXT_TYPES {
    CSS_CONTEXT_RULE,
    CSS_CONTEXT_MEDIA_QUERY,
    CSS_CONTEXT_KEYFRAMES,
    CSS_CONTEXT_FONT_FACE,
    CSS_CONTEXT_UNDEFINED,
    CSS_CONTEXT_SUBTYPE_RULE,
    CSS_CONTEXT_SUBTYPE_INHERITABLE
};

//un context intre {}

class CssContext {
private:
    std::vector<CssSelector> _cssSelectorList; // vectorul de selectori fara virgula
    std::string _original; // inputul original de selectie
    int _contextType;
    int _contextSubType;

    int _line;
    CssLintErrorThrower _err;

public:
    std::vector<std::string> _comments; // background: red;
    std::vector<std::string> _imports; // background: red;
    std::map<std::string, std::string> definitions; // background: red;
    std::map<std::string, CssContext> childs;


    std::string _lastRuleId;

    CssContext();
    CssContext(std::string in, int contextType);

    std::string getOriginalDef() {
        return _original;
    }

    void merge(CssContext& _other) {
        std::map<std::string, std::string>::iterator defs;
        for (defs = _other.definitions.begin(); defs != _other.definitions.end(); ++defs) {
            definitions[defs->first] = defs->second;
        }

        std::map<std::string, CssContext>::iterator chlds;
        for (chlds = _other.childs.begin(); chlds != _other.childs.end(); ++chlds) {
            childs[chlds->first] = chlds->second;
        }
    }

    std::string toString();
    std::string toJson(bool asRoot = true);

    std::string selfCheck();

    void setDefinition(std::string input, int line = 0);

    int getLine() {
        return _line;
    }

    int getSubType() {
        return _contextSubType;
    }

    CssContext digestLineList(std::vector<CssLine> _lines);
};

class CssLinter {
private:
    CssLintErrorThrower _err;
    std::string _lastSelectorInDef;
    std::string _lastLeftData;
    std::string _lastDefinition;
    std::map<std::string, int> _lastSelectorRules;
    std::map<std::string, int> _definitionList;

    std::vector<std::string> _lastDefinitionLS;
    std::vector<CssLine> _lines;

    int numDefs;
    int numQueries;
    int numRight;
    int numEndRule;



    bool queryDefStarted;

    //used at string parsing:
    int numq; // " si '
    int numcomm; // /* si */
    int numrbrac; // ( si )
    int lastUsedStat;
    std::string _buffer;
    bool waitForQueryFinish;

    CssContext _rootContext;

public:
    CssLinter();
    void onIterate(int status, std::string _currentBuffer, int line);
    void parse(std::string input, void (_onIterate) (int status, std::string _currentBuffer, int line) = NULL, int lineNo = 0);
    CssContext digestLines();
};






#endif /* TRACER_H_ */
