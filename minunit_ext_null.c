/**
 * Sample code for *ext() macros
 * Created on Wed Jan 15 12:31:27 2025
 * 
 * @author: hilton
 **/

#include "minunit.h"

/*  Test setup and teardown function pointers */
static void (*minunit_setup_ext)(void* param) = NULL;
static void (*minunit_teardown_ext)(void* param) = NULL;

/*  Test setup and teardown parameter pointers */
static void *minunit_setup_parm = NULL;
static void *minunit_teardown_parm = NULL;

static double dbar = 0.1;
static double sarima = -11530.2;
static double statsmodels = -15458.077;
static double forecast = -11252.24;
static double tolerance = 0.15;

#define MU_TEST_EXT(method_name, param) static void method_name(void* param)
#define MU_TEST_SUITE_EXT(suite_name, param) static void suite_name(void*, param)

#define UNUSED(x) (void)(x)

void test_setup_ext(void* param) {
	UNUSED(param);
}

void test_teardown_ext(void* param) {
	UNUSED(param);
}

struct double_comp {
	double left;
	double right;
};

/*  Configure setup and teardown functions, extended */
#define MU_SUITE_CONFIGURE_EXT(setup_fun, setup_parm, teardown_fun, teardown_parm) MU__SAFE_BLOCK(\
	minunit_setup_ext = setup_fun;\
	minunit_teardown_ext = teardown_fun;\
	minunit_setup_parm = setup_parm;\
	minunit_teardown_parm = teardown_parm;\
)

/*  Test runner */
#define MU_RUN_TEST_EXT(test, parm) MU__SAFE_BLOCK(\
	if (minunit_real_timer==0 && minunit_proc_timer==0) {\
		minunit_real_timer = mu_timer_real();\
		minunit_proc_timer = mu_timer_cpu();\
	}\
	if (minunit_setup_ext) (*minunit_setup_ext)(minunit_setup_parm);\
	minunit_status = 0;\
	test(parm);\
	minunit_run++;\
	if (minunit_status) {\
		minunit_fail++;\
		printf("F");\
		printf("\n%s\n", minunit_last_message);\
	}\
	(void)fflush(stdout);\
	if (minunit_teardown_ext) (*minunit_teardown_ext)(minunit_teardown_parm);\
)


MU_TEST_EXT(test_assert_double_eq, param) {
	struct double_comp parm = {0, 0};
	
	UNUSED(param);
	
	parm.left = 0.1;
	parm.right = dbar;
	mu_assert_double_eq(parm.left, parm.right);
}

MU_TEST_EXT(test_assert_double_eq_fail, param) {
	struct double_comp parm = {0, 0};
	
	UNUSED(param);
	
	parm.left = 0.2;
	parm.right = dbar;
	mu_assert_double_eq(parm.left, parm.right);
}

MU_TEST_EXT(test_assert_statsmodels, param) {
	UNUSED(param);
	
	mu_assert_double_tol(statsmodels, sarima, tolerance);
}

MU_TEST_EXT(test_assert_forecast, param) {
	UNUSED(param);
	
	mu_assert_double_tol(forecast, sarima, tolerance);
}

MU_TEST_EXT(test_assert_mostfamous, param) {
	UNUSED(param);
	
	mu_assert_double_tol(forecast, statsmodels, tolerance);
}


MU_TEST_SUITE(test_suite_null) {
	MU_SUITE_CONFIGURE_EXT(&test_setup_ext, NULL, &test_teardown_ext, NULL);

	MU_RUN_TEST_EXT(test_assert_double_eq, NULL);
	MU_RUN_TEST_EXT(test_assert_double_eq_fail, NULL);
	
	MU_RUN_TEST_EXT(test_assert_statsmodels, NULL);
	MU_RUN_TEST_EXT(test_assert_forecast, NULL);
	MU_RUN_TEST_EXT(test_assert_mostfamous, NULL	);
}

int main() {
	MU_RUN_SUITE(test_suite_null);
	MU_REPORT();
	return MU_EXIT_CODE;
}

