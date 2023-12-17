#ifndef CRISP_CORE_COMMON_H
#define CRISP_CORE_COMMON_H

#include <cstdint>
#include <string>

enum GlobalUInt16VarIndex
{
    gCrispScreenWidth = 0,
    gCrispScreenHeight,
};

static uint16_t global_uint16_vals[] = {
    240,
    240,
};

static std::string_view global_uint16_keys[] = {
    "ScreeWidth",
    "ScreenHeight",
};

#endif // CRISP_CORE_COMMON_H