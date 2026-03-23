#pragma once

// Forward declarations for dependency injection
class IEspLittleFs;
class IEspHttpServer;
class IEspNvs;
class IEspLogger;

class IApiStatus;
class IApiNodes;
class IApiSystem;
class IApiFirmware;
class IApiSecurity;
class IApiLogging;

/**
 * Groups all ESP hardware/platform abstraction layer dependencies.
 * Passed through the application via dependency injection.
 */
struct EspContexts
{
    IEspLittleFs   &espLittleFs;
    IEspHttpServer &espHttpServer;
    IEspNvs        &espNvs;
    IEspLogger     &espLogger;
};

/**
 * Groups all common (framework-level) API handler dependencies.
 * Passed through the application via dependency injection.
 */
struct CommonApiContexts
{
    IApiStatus   &apiStatus;
    IApiNodes    &apiNodes;
    IApiSystem   &apiSystem;
    IApiFirmware &apiFirmware;
    IApiSecurity &apiSecurity;
    IApiLogging  &apiLogging;
};

/**
 * Top-level common context container used by ApiServer and WebserverTask.
 */
struct Contexts
{
    EspContexts       &espContexts;
    CommonApiContexts &commonApiContexts;
};
