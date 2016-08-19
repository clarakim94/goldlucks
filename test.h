#ifndef __test_H__
#define __test_H__

#include <app.h>
#include <Elementary.h>
#include <system_settings.h>
#include <efl_extension.h>
#include <dlog.h>

#ifdef  LOG_TAG
#undef  LOG_TAG
#endif
#define LOG_TAG "test"

#if !defined(PACKAGE)
#define PACKAGE "org.example.test"
#endif

#endif /* __test_H__ */
