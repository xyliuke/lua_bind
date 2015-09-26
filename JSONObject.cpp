//
// Created by liuke on 15/9/24.
//

#include "JSONObject.h"

using namespace rapidjson;

rapidjson::Document JSONObject::doc;





Value& JSONObject::createObject() {
    Value ret(kObjectType);
    return ret;
}


Value& JSONObject::createObject(rapidjson::Value& pnode, std::string key) {
    Value node(kObjectType);
    JSONObject::add(pnode, key, node);
    return JSONObject::get(pnode, key);
}



//Value& JSONObject::createArray() {
//    Value ret(kArrayType);
//    return ret;
//}

void JSONObject::add(Value& obj, std::string key, int value) {
    Value val(value);
    obj.AddMember(key.c_str(), val, JSONObject::doc.GetAllocator());
}

void JSONObject::add(Value& obj, std::string key, double value) {
    Value val(value);
    obj.AddMember(key.c_str(), val, JSONObject::doc.GetAllocator());
}

void JSONObject::add(Value& obj, std::string key, std::string value) {
    Value val(value.c_str());
    obj.AddMember(key.c_str(), val, JSONObject::doc.GetAllocator());
}

Value& JSONObject::add(Value& obj, std::string key, rapidjson::Value& value) {
    obj.AddMember(key.c_str(), value, JSONObject::doc.GetAllocator());
    return JSONObject::get(obj, key);
}

int JSONObject::getInt(rapidjson::Value& obj, std::string key) {
    if (obj.HasMember(key.c_str()) && obj[key.c_str()].IsInt()) {
        return obj[key.c_str()].GetInt();
    }
    return 0;
}

double JSONObject::getDouble(rapidjson::Value& obj, std::string key) {
    if (obj.HasMember(key.c_str()) && obj[key.c_str()].IsDouble()) {
        return obj[key.c_str()].GetDouble();
    }
    return 0.0;
}

std::string JSONObject::getString(rapidjson::Value& obj, std::string key) {
    if (obj.HasMember(key.c_str()) && obj[key.c_str()].IsString()) {
        return obj[key.c_str()].GetString();
    }
    return nullptr;
}

Value& JSONObject::get(Value& obj, std::string key) {
    if (obj.HasMember(key.c_str())) {
        return obj[key.c_str()];
    }
    return obj;
}

//void JSONObject::generateValue(JSON_TYPE type) {
//    switch(type) {
//        case JSON_INT:
//            value_ptr.reset(new rapidjson::Value(rapidjson::kNumberType));
//            break;
//        case JSON_DOUBLE:
//            value_ptr.reset(new rapidjson::Value(rapidjson::kNumberType));
//            break;
//        case JSON_STRING:
//            value_ptr.reset(new rapidjson::Value(rapidjson::kStringType));
//            break;
//        case JSON_BOOL:
//            value_ptr.reset(new rapidjson::Value(rapidjson::kFalseType));
//            break;
//        case JSON_OBJECT:
//            value_ptr.reset(new rapidjson::Value(rapidjson::kObjectType));
//            break;
//        case JSON_ARRAY:
//            value_ptr.reset(new rapidjson::Value(rapidjson::kArrayType));
//            break;
//        default:
//            value_ptr.reset(new rapidjson::Value(rapidjson::kNullType));
//            break;
//    }
//}