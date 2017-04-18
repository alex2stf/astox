/*
 * Properties.cpp
 *
 *  Created on: Jul 17, 2015
 *      Author: alex
 */

#include "Properties.h"

namespace astox {

    Properties::Properties() {
        // TODO Auto-generated constructor stub
    }

    Properties::~Properties() {
        _properties.clear();
    }

    void Properties::appendToKey(std::string keyName, std::string value) {
        if (_properties.find(keyName) == _properties.end()) {
            _properties[keyName] = value;
        } else {
            _properties[keyName].append(" ");
            _properties[keyName].append(value);
        }
    };

    std::string Properties::getKey(std::string keyName) {
        if (_properties.find(keyName) != _properties.end()) {
            return _properties[keyName];
        }
        return s;
    };

    std::string Properties::getValue(std::string valueName) {
        std::map<std::string, std::string>::iterator it;
        for (it = _properties.begin(); it != _properties.end(); ++it) {
            if (it->second == valueName) {
                return it->first;
            }
        }
        return s;
    };

    bool Properties::hasKey(std::string keyName) {
        return (_properties.find(keyName) != _properties.end());
    };

    void Properties::add(std::string key, std::string name) {
        if (key.find("=") != std::string::npos) {
            printf(" prop key (%s) not allowed", key.c_str());
            return;
        }

        if (name.find("=") != std::string::npos) {
            printf(" prop name (%s) not allowed", name.c_str());
            return;
        }
        _properties[key] = name;
    };

    void Properties::loadFile(std::string filePath) {
        std::ifstream o;
        std::string line;
        std::string res;
        o.open(filePath.c_str(), std::ios::in);
        if (o.is_open()) {
            while (getline(o, line)) {
                astox::stringAlg::trim(line);
                if (!line.empty() && line.at(0) != '#') {
                    //line = astox::stringAlg::trim(line);
                    size_t pos = line.find("=");

                    if (pos != std::string::npos) {
                        std::string key = line.substr(0, pos);
                        std::string value = line.substr(pos + 1, line.size());
                        //key = astox::stringAlg::trim(key);

                        if (!key.empty()) {
                            if (!value.empty()) {
                                _properties[key] = value;
                            }
                        }
                    }
                }
            }
        } else {
            printf("Unable to read props %s", filePath.c_str());
        }
        o.close();
    };

    void Properties::saveToFile(std::string filePath) {
        std::map<std::string, std::string>::iterator it;
        std::string response;
        for (it = _properties.begin(); it != _properties.end(); ++it) {
            response.append(it->first);
            response.append("=");
            response.append(it->second);
            response.append("\n");
        }
        astox::stringAlg::saveStringToFile(response, filePath);
    };

} /* namespace astox */
