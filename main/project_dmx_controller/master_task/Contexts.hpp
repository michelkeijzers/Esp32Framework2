#pragma once

#include "../../common/context/Contexts.hpp"

class IApiConfig;
class IApiPresets;
class IApiPresetValues;
class IApiConfiguration;

/**
 * Groups all DMX Controller project-specific API handler dependencies.
 */
struct DmxApiContexts
{
    IApiConfig        &apiConfig;
    IApiPresets       &apiPresets;
    IApiPresetValues  &apiPresetValues;
    IApiConfiguration &apiConfiguration;
};

/**
 * Top-level DMX Controller project context, extending the common Contexts.
 */
struct DmxContexts
{
    Contexts       &contexts;
    DmxApiContexts &dmxApiContexts;
};
