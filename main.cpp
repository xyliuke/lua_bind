#include <iostream>
#include "lua_bind.h"

#import "3rd/rapidjson/include/rapidjson/document.h"

using namespace std;

int main() {
    lua_bind_init();
    lua_bind();

    std::string str = "test";
    std::string param = "param";

    std::shared_ptr<rapidjson::Document> doc(new rapidjson::Document());
    doc->SetObject();
    rapidjson::Value i("test test parameter");
    doc->AddMember("param", i, doc->GetAllocator());

    lua_bind_call(str, doc, [=](std::string result) {
        std::cout << "lambda " << result << std::endl;
    });

    return 0;
}