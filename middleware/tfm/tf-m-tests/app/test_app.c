/*
 * Copyright (c) 2017-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "test_app.h"
#include "tfm_log.h"

#ifdef TEST_FRAMEWORK_NS
#include "test_framework_integ_test.h"
#endif

#if defined(TEST_FRAMEWORK_S) && defined(TFM_LIB_MODEL)
/* Function tfm_secure_client_run_tests() which is declared in
 * tfm_secure_client_service_api.h is only required in NS for Library mode. */
#include "tfm_secure_client_service_api.h"
#endif

#if PSA_API_TEST_NS
/**
 * \brief This symbol is the entry point provided by the PSA API compliance
 *        test libraries
 */
extern void val_entry(void);
#endif

/**
 * \brief Services test thread
 *
 */
__attribute__((noreturn))
void test_app(void *argument)
{
    UNUSED_VARIABLE(argument);

#if defined(TEST_FRAMEWORK_S) && defined(TFM_LIB_MODEL)
    /* FIXME: The non-secure audit log test currently relies on the fact that
     * the audit log secure test is run first. However the Non-secure tests
     * represent simpler and more common test cases which would make more sense
     * to be run first. Therefore if this dependency is removed the execution
     * order of these test classes should be reversed. */
    tfm_secure_client_run_tests();
#endif

#ifdef TEST_FRAMEWORK_NS
    tfm_non_secure_client_run_tests();
#endif

#ifdef PSA_API_TEST_NS
    val_entry();
#endif

    /* Output EOT char for test environments like FVP. */
    LOG_MSG("\x04");

    /* End of test */
    for (;;) {
    }
}
