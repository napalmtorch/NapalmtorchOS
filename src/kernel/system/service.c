#include <kernel/system/service.h>
#include <kernel/core/kernel.h>

service_t** services_list;
uint32_t    services_count;
uint32_t    services_count_max;

bool_t service_initmgr()
{
    services_count_max = 512;
    services_count     = 0;
    services_list      = tcalloc(sizeof(service_t*) * services_count_max, MEMSTATE_PTRARRAY);
    debug_ok("Initialized service manager");
}

void service_printlist()
{
    term_write_fg("------ ", COL32_DARKGRAY);
    term_write_fg("SERVICES", COL32_YELLOW);
    term_writeln_fg(" ------------------------------------------", COL32_DARKGRAY);
    term_writeln_fg("INDEX       TYPE   STATE   START       STOP        NAME ", COL32_DARKGRAY);

    for (uint32_t i = 0; i < services_count_max; i++)
    {
        if (services_list[i] == NULL) { continue; }
        term_printf("0x%8x  0x%2x   0x%2x    0x%8x  0x%8x  %s\n", 
                    i, services_list[i]->type, services_list[i]->started, (uint32_t)services_list[i]->m_start, (uint32_t)services_list[i]->m_stop, services_list[i]->name);
    }
}

service_t* service_create(const char* name, SERVICETYPE type, service_method_t start, service_method_t stop)
{
    service_t* service = tcalloc(sizeof(service_t), MEMSTATE_SERVICE);
    service->name        = name;
    service->type        = type;
    service->m_start     = start;
    service->m_stop      = stop;
    service->initialized = FALSE;
    service->started     = FALSE;
    return service;
}

bool_t service_dispose(service_t* service)
{
    if (service == NULL) { return FALSE; }
    free(service);
}

int service_freeindex()
{
    for (uint32_t i = 0; i < services_count_max; i++)
    {
        if (services_list[i] == NULL) { return (int)i; }
    }
    return -1;
}

bool_t service_register(service_t* service)
{
    int i = service_freeindex();
    if (i < 0 || i >= services_count_max) { panicf(EXCEPTION_INDEX_OUTOFRANGE, NULL, "service_register"); return FALSE; }
    services_list[i] = service;
    services_count++;
    debug_info("Registered service: TYPE = 0x%2x, NAME = %s", service->type, service->name);
    return TRUE;
}

bool_t service_start(service_t* service)
{
    if (!service_validate(service)) { return FALSE; }
    if (service->started) { return FALSE; }
    service->started = TRUE;
    service->m_start(service);
    debug_info("Started service: %s", service->name);
    return TRUE;
}

bool_t service_start_byname(const char* service)
{
    if (!service_validate_byname(service)) { return FALSE; }
    for (uint32_t i = 0; i < services_count_max; i++)
    {
        if (services_list[i] == NULL) { continue; }
        if (streql(services_list[i]->name, service)) { return service_start(services_list[i]); }
    }

    panicf(EXCEPTION_ARG, NULL, "service_start_byname", service);
    return FALSE;
}

bool_t service_start_byindex(int service)
{
    if (!service_validate_byindex(service)) { return FALSE; }
    for (uint32_t i = 0; i < services_count_max; i++)
    {
        if (i == service && services_list[i] != NULL) { return service_start(services_list[i]); }
    }

    panicf(EXCEPTION_ARG, NULL, "service_start_byindex", service);
    return FALSE;
}

bool_t service_stop(service_t* service)
{

}

bool_t service_stop_byname(const char* service)
{

}

bool_t service_stop_byindex(int service)
{

}

bool_t service_isrunning(service_t* service)
{
    if (service == NULL) { panicf(EXCEPTION_ARG_NULL, NULL, "service_isrunning"); return FALSE; }
    for (uint32_t i = 0; i < services_count_max; i++)
    {
        if (services_list[i] == NULL) { continue; }
        if (services_list[i] == service)
        {
            if (services_list[i]->started) { return TRUE; }
            return FALSE;
        }
    }
    return FALSE;
}

bool_t service_isrunning_byname(const char* service)
{
    if (service == NULL) { panicf(EXCEPTION_ARG_NULL, NULL, "service_isrunning_byname"); return FALSE; }
    for (uint32_t i = 0; i < services_count_max; i++)
    {
        if (services_list[i] == NULL) { continue; }
        if (streql(services_list[i]->name, service))
        {
            if (services_list[i]->started) { return TRUE; }
            return FALSE;
        }
    }
    return FALSE;
}

bool_t service_isrunning_byindex(int service)
{
    if (service < 0 || service >= services_count_max) {  panicf(EXCEPTION_ARG_OUTOFRANGE, NULL, "service_isrunning_byindex"); return FALSE; }
    if (services_list[service] == NULL) { return FALSE; }
    if (!services_list[service]->started) { return FALSE; }
    return TRUE;
}

bool_t service_isregistered(service_t* service)
{
    if (service == NULL) {  panicf(EXCEPTION_ARG_NULL, NULL, "service_isregistered"); return FALSE; }
    for (uint32_t i = 0; i < services_count_max; i++)
    {
        if (services_list[i] == NULL) { continue; }
        if (services_list[i] == service) { return TRUE; }
    }
    return FALSE;
}

bool_t service_isregistered_byname(const char* service)
{
    if (service == NULL) {  panicf(EXCEPTION_ARG_NULL, NULL, "service_isregistered_byname"); return FALSE; }
    for (uint32_t i = 0; i < services_count_max; i++)
    {
        if (services_list[i] == NULL) { continue; }
        if (streql(services_list[i]->name, service)) { return TRUE; }
    }
    return FALSE;
}

bool_t service_isregistered_byindex(int service)
{
    if (service < 0 || service >= services_count_max) {  panicf(EXCEPTION_ARG_OUTOFRANGE, NULL, "service_isregistered_byindex"); return FALSE; }
    if (services_list[service] == NULL) { return FALSE; }
    return TRUE;
}

bool_t service_validate(service_t* service)
{
    if (service == NULL) { panicf(EXCEPTION_ARG_NULL, NULL, "service_validate");; return FALSE; }
    if (service_isregistered(service) && !service->initialized) { panicf(EXCEPTION_ARG, NULL, "service_validate"); return FALSE; }
    if (service_isregistered(service) && service->initialized) { return TRUE; }
    panicf(EXCEPTION_ARG, NULL, "service_validate");
    return FALSE;
}

bool_t service_validate_byname(const char* service)
{
    if (service == NULL) { panicf(EXCEPTION_ARG_NULL, NULL, "service_validate_byname"); return FALSE; }
    for (uint32_t i = 0; i < services_count_max; i++)
    {
        if (services_list[i] == NULL) { continue; }
        if (streql(services_list[i]->name, service))
        {
            if (!services_list[i]->initialized) { panicf(EXCEPTION_UNKKNOWN, NULL, "service_validate_byname"); return FALSE; }
            return TRUE;
        }
    }

    panicf(EXCEPTION_ARG, NULL, "service_validate_byname");
    return FALSE;
}

bool_t service_validate_byindex(int service)
{
    if (service < 0 || service >= services_count_max) { panicf(EXCEPTION_ARG_OUTOFRANGE, NULL, "service_validate_byindex"); return FALSE; }
    if (services_list[service] == NULL) { panicf(EXCEPTION_NULLPTR, NULL, "service_validate_byindex"); return FALSE; }
    if (!services_list[service]->initialized) { panicf(EXCEPTION_UNKKNOWN, NULL, "service_validate_byindex"); return FALSE; }
    return TRUE;
}