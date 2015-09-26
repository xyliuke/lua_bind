//
// Created by liuke on 15/9/21.
//

#ifndef LUA_BIND_LUA_BIND_H
#define LUA_BIND_LUA_BIND_H

#include <lua.hpp>
#include <iosfwd>
#include <string>

#import "3rd/rapidjson/include/rapidjson/document.h"

void lua_bind_init();

void lua_bind_call(std::string name, std::shared_ptr<rapidjson::Document> param, std::function<void(std::string result)> callback);

void lua_bind();

void json2table(std::shared_ptr<rapidjson::Document> doc);

#endif //LUA_BIND_LUA_BIND_H
