#include "minunit.h"

/*  Test setup and teardown function pointers */
static void (*minunit_setup_ext)(void*) = NULL;
static void (*minunit_teardown_ext)(void*) = NULL;

/*  Test setup and teardown parameter pointers */
static void *minunit_setup_parm) = NULL;
static void *minunit_teardown_parm) = NULL;

static double dbar = 0.1;
static double sarima = -11530.2;
static double statsmodels = -15458.077;
static double forecast = -11252.24;
static double tolerance = 0.15;

void test_setup(void) {
}

void test_teardown(void) {
}

void test_setup_ext(void*) {
}

void test_teardown_ext(void*) {
}

/*  Configure setup and teardown functions, extended */
#define MU_SUITE_CONFIGURE_EXT(setup_fun, setup_parm, teardown_fun, teardown_parm) MU__SAFE_BLOCK(\
	minunit_setup_ext = setup_fun;\
	minunit_teardown_ext = teardown_fun;\
	minunit_setup_parm = setup_fun_parm;\
	minunit_teardown_parm = teardown_fun_parm;\
)

/*  Test runner */
#define MU_RUN_TEST_ext(test, parm) MU__SAFE_BLOCK(\
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


MU_TEST(test_assert_double_eq) {
	mu_assert_double_eq(0.1, dbar);
}

MU_TEST(test_assert_double_eq_fail) {
	mu_assert_double_eq(0.2, dbar);
}

MU_TEST(test_assert_statsmodels) {
	mu_assert_double_tol(statsmodels, sarima, tolerance);
}

MU_TEST(test_assert_forecast) {
	mu_assert_double_tol(forecast, sarima, tolerance);
}

MU_TEST(test_assert_mostfamous) {
	mu_assert_double_tol(forecast, statsmodels, tolerance);
}


MU_TEST_SUITE(test_suite) {
	MU_SUITE_CONFIGURE(&test_setup, &test_teardown);

	MU_RUN_TEST(test_assert_double_eq);
	MU_RUN_TEST(test_assert_double_eq_fail);
	
	MU_RUN_TEST(test_assert_statsmodels);
	MU_RUN_TEST(test_assert_forecast);
	MU_RUN_TEST(test_assert_mostfamous);
}

int main() {
	MU_RUN_SUITE(test_suite);
	MU_REPORT();
	return MU_EXIT_CODE;
}

