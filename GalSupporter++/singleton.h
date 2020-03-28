#pragma once

/* ----------------------------------------------------------/
 * singleton.h
 * ----------------------------------------------------------/
 * Macro-style singleton declaration to avoid copying codes.
 * ---------------------------------------------------------*/
#include <memory>

#define DEFINE_SINGLETON(TYPE_)\
private:\
    using self_type = TYPE_;\
    using unique_ptr = std::unique_ptr<self_type>;\
public:\
    using singleton = const unique_ptr &;\
    static singleton instance() {\
        return ref();\
    }\
    static void create() {\
        if (!ref()) ref() = make_unique();\
    }\
    static void destroy() {\
        ref().reset();\
    }\
private:\
    template <typename... Args>\
    static unique_ptr make_unique(Args&&... args) {\
        struct temp : self_type { temp() : self_type() {} };\
        return std::move(unique_ptr(new temp(std::forward<Args>(args)...)));\
    }\
    static unique_ptr &ref() {\
        static unique_ptr p = make_unique();\
        return p;\
    }