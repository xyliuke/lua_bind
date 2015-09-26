//
// Created by liuke on 15/9/24.
//

#ifndef LUA_BIND_JSONOBJECT_H
#define LUA_BIND_JSONOBJECT_H

#include <memory>
#include <string>
#import "3rd/rapidjson/include/rapidjson/document.h"

//enum JSON_TYPE {
//    JSON_INT, JSON_DOUBLE, JSON_BOOL, JSON_OBJECT, JSON_ARRAY, JSON_STRING
//};

class JSONObject{

public:

    static rapidjson::Value& createObject();
    static rapidjson::Value& createObject(rapidjson::Value& pnode, std::string key);
//    static rapidjson::Value& createArray();

    static void add(rapidjson::Value& obj, std::string key, std::string value);
    static void add(rapidjson::Value& obj, std::string key, int value);
    static void add(rapidjson::Value& obj, std::string key, double value);
    static rapidjson::Value& add(rapidjson::Value& obj, std::string key, rapidjson::Value& value);

//    static void add(int value);
//    static void add(double value);
//    static void add(JSONObject value);

    static int getInt(rapidjson::Value& obj, std::string key);
    static double getDouble(rapidjson::Value& obj, std::string key);
    static std::string getString(rapidjson::Value& obj, std::string key);
    static rapidjson::Value& get(rapidjson::Value& obj, std::string key);

private:
    static rapidjson::Document doc;
};


#endif //LUA_BIND_JSONOBJECT_H
