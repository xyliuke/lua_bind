//
// Created by liuke on 15/9/21.
//

#include <iostream>
#include <map>
#include <sys/time.h>
#include <sstream>
#include <vector>

#include "lua_bind.h"

static lua_State* L;

static std::map<std::string, std::function<void(std::string)>> callbacks;

static bool isLoad = false;

static std::shared_ptr<std::vector<std::string>> getCallLevel(std::string name) {
    std::shared_ptr<std::vector<std::string>> ret(new std::vector<std::string>());
    unsigned long pos = name.find(".");
    if (pos == std::string::npos) {
        ret->push_back(name);
    } else {
        std::string tmp = name.substr(0, pos);
        ret->push_back(tmp);

        unsigned long index = pos;
        pos = name.find(".", pos + 1);
        while (pos != std::string::npos) {
            std::string tmp = name.substr(index + 1, pos - index - 1);
            ret->push_back(tmp);
            index = pos;

            pos = name.find(".", pos + 1);
        }

        if (index < name.size()) {
            std::string tmp = name.substr(index + 1, name.size() - index - 1);
            ret->push_back(tmp);
        }
    }
    return ret;
}

void json2table(std::shared_ptr<rapidjson::Document> doc) {
    using namespace rapidjson;
    lua_newtable(L);
    Value::MemberIterator it = doc->MemberBegin();
    while (it != doc->MemberEnd()) {
        std::cout << "key:" << it->name.GetString() << std::endl;

        if (it->value.IsObject()) {

        } else if (it->value.IsInt()) {
            lua_pushinteger(L, it->value.GetInt());
            lua_setfield(L, -2, it->name.GetString());
//            std::cout << "value:" << it->value.GetInt() << std::endl;
        } else if (it->value.IsInt64()) {
            lua_pushinteger(L, it->value.GetInt64());
            lua_setfield(L, -2, it->name.GetString());
//            std::cout << "value:" << it->value.GetInt64() << std::endl;
        } else if (it->value.IsDouble()) {
            lua_pushnumber(L, it->value.GetDouble());
            lua_setfield(L, -2, it->name.GetString());
//            std::cout << "value:" << it->value.GetDouble() << std::endl;
        } else if (it->value.IsString()) {
            lua_pushstring(L, it->value.GetString());
            lua_setfield(L, -2, it->name.GetString());
//            std::cout << "value:" << it->value.GetString() << std::endl;
        } else if (it->value.IsTrue()) {
            lua_pushboolean(L, 1);
            lua_setfield(L, -2, it->name.GetString());
//            std::cout << "value:" << it->value.GetBool() << std::endl;
        } else if (it->value.IsFalse()) {
            lua_pushboolean(L, 0);
            lua_setfield(L, -2, it->name.GetString());
//            std::cout << "value:" << it->value.GetBool() << std::endl;
        } else if (it->value.IsArray()) {

        }

        it ++;
    }
}

static void wrap(std::shared_ptr<rapidjson::Document> doc, std::string& id) {
    doc->AddMember("id", rapidjson::Value().SetString(id.c_str()), doc->GetAllocator());
}

static int callback(lua_State* L) {

    luaL_checktype(L, -1, LUA_TTABLE);

    lua_getfield(L, -1, "id");
    const char* time = lua_tostring(L, -1);
    lua_pop(L, 1);

    lua_getfield(L, -1, "param");
    const char* param = lua_tostring(L, -1);
    lua_pop(L, 1);

    std::function<void(std::string)> cb = callbacks[time];
    if (cb != NULL) {
        cb(param);
        callbacks.erase(time);
    }
    return 0;
}


void lua_bind_init() {
    L = luaL_newstate();
    luaopen_base(L);
    luaopen_io(L);
    luaopen_string(L);
    luaopen_math(L);
    if (luaL_loadfile(L, "./../../test.lua") || lua_pcall(L, 0, 0, 0)) {
        isLoad = false;
        std::cout << "加载lua失败"<< std::endl;
        return ;
    }
    isLoad = true;
}


void lua_bind_call(std::string name, std::shared_ptr<rapidjson::Document> param, std::function<void(std::string)> callback) {
    if (!isLoad) {
        std::cout << "加载lua失败"<< std::endl;
        return;
    }

    struct timeval t_val;
    gettimeofday(&t_val, NULL);
    std::stringstream ss;
    ss << t_val.tv_sec << "." << t_val.tv_usec;
    std::string time = (std::string)ss.str();

    unsigned long pos = name.find(".");
    if (pos == std::string::npos) {
        //不包含 . 运算
        int type = lua_getglobal(L, name.c_str());//返回6表示函数,5表示table

        if (type == LUA_TFUNCTION) {
            wrap(param, time);
            json2table(param);

        } else if (type == LUA_TNIL) {
            std::cout << "没有在lua找到函数:"<< name << std::endl;
            return;
        }
    } else {
        std::shared_ptr<std::vector<std::string>> calls = getCallLevel(name);
        typedef std::vector<std::string>::iterator VStringIterator;
        VStringIterator end = calls->end();
        for( VStringIterator i = calls->begin(); i != end; ++i) {
            std::string call = (*i);
            int type = 0;
            if (i == calls->begin()) {
                type = lua_getglobal(L, call.c_str());
            } else {
                type = lua_getfield(L, -1, call.c_str());
            }

            if (type == LUA_TFUNCTION) {
                wrap(param, time);
                json2table(param);
                break;
            } else if (type == LUA_TNIL) {
                std::cout << "没有在lua找到函数:"<< name << std::endl;
                return;
            }
        }
    }

    callbacks[time] = callback;

    if (lua_pcall(L, 1, 0, 0) == 0) {
        std::cout << "run ok" << std::endl;
    }

    lua_pop(L, 1);
}

void lua_bind() {
    lua_register(L, "callback", callback);
}