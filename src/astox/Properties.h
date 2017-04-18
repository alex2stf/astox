/*
 * Properties.h
 *
 *  Created on: Jul 17, 2015
 *      Author: alex
 */

#ifndef PROPERTIES_H_
    #define PROPERTIES_H_

    #include "Macros.h"
    #include "Util.h"

namespace astox {

    class Properties {
    private:
        std::map<std::string, std::string> _properties;
        std::string s;
    public:
        Properties();
        virtual ~Properties();
        std::string getKey(std::string keyName);
        std::string getValue(std::string valueName);
        bool hasKey(std::string keyName);
        void add(std::string key, std::string value);
        void loadFile(std::string filePath);
        void saveToFile(std::string filePath);
        void appendToKey(std::string keyName, std::string value);
    };

} /* namespace astox */

#endif /* PROPERTIES_H_ */
