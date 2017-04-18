/*
 * MTypes.h
 *
 *  Created on: Sep 25, 2014
 *      Author: alexandru
 */

#ifndef MUSTEEN_H_
    #define MUSTEEN_H_

    #include <map>
    #include <iostream>
    #include <vector>
    #include "astox.h"
    #include "Macros.h"

namespace astox {

    enum MST_STR_STAT {
        MST_DIRECT_KEY = 2,
        MST_FALSE_KEY,
        MST_TRUE_KEY,
        MST_END_KEY,
        MST_LIST_INDEX,
        MST_LIST_NUM,
        MST_UNESCAPE,
        MST_PARTIAL,
        MST_DEFAULT,
        MST_TEXT_ONLY
    };

    class MusTag {
    private:
        int tagType;

        void parse(std::string input, bool asBracket);

    public:
        MusTag(std::string input, bool asBracket);
        int getType();

        std::string toString();
        std::vector<MusTag> childNodes;
        std::string key;
        std::string original;

    };

    class Musteen {
    private:
        std::vector<MusTag> _tagList;
        Object * _context;
        Array * _result;
        Object * _currentInstance;
        std::string _strResult;

    public:
        Musteen();
        virtual ~Musteen();
        std::string parse(std::string input, Object * context);
        bool escapeDefaultToHTML;
    protected:
        std::string iterateList(std::vector<MusTag> _tagList, Object * context, Object * _globalContext, std::string debugName = "root", bool trackAsArray = false, int numArrayIndeces = 0, int nextNumChilds = 0);
        std::string returnDefaultKey(MusTag & _tag, Object * context, Object * global);
        std::string htmlEscape(std::string input);

    };



}//end namespace

#endif
