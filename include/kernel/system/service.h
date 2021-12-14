#pragma once
#include <lib/types.h>

typedef enum
{
    SERVICETYPE_UNSPECIFIED,
    SERVICETYPE_DRIVER,
    SERVICETYPE_INTERFACE,
    SERVICETYPE_BACKGROUND,
} SERVICETYPE;

typedef void (*service_method_t)(struct service_t*);

typedef struct
{
    const char*      name;
    SERVICETYPE      type;
    bool_t           initialized;
    bool_t           started;
    service_method_t m_start;
    service_method_t m_stop;
} service_t;

bool_t service_initmgr();
void   service_printlist();

service_t* service_create(const char* name, SERVICETYPE type, service_method_t start, service_method_t stop);
bool_t     service_dispose(service_t* service);
bool_t     service_register(service_t* service);
bool_t     service_start(service_t* service);
bool_t     service_start_byname(const char* service);
bool_t     service_start_byindex(int service);
bool_t     service_stop(service_t* service);
bool_t     service_stop_byname(const char* service);
bool_t     service_stop_byindex(int service);
bool_t     service_isrunning(service_t* service);
bool_t     service_isrunning_byname(const char* service);
bool_t     service_isrunning_byindex(int service);
bool_t     service_isregistered(service_t* service);
bool_t     service_isregistered_byname(const char* service);
bool_t     service_isregistered_byindex(int service);
bool_t     service_validate(service_t* service);
bool_t     service_validate_byname(const char* service);
bool_t     service_validate_byindex(int service);

