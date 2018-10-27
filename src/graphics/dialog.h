#ifndef SKYVAULT_DIALOG_H
#define SKYVAULT_DIALOG_H 
#include <sol.hpp>

struct Dialog {
    Dialog(const sol::table& Data) : Data(std::move(Data)) {}
    sol::table Data;
    int CurrentIndex;
};

#endif//SKYVAULT_DIALOG_H
