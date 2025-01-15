#include "minunit.h"

static double dbar = 0.1;
static double sarima = -11530.2;
static double statsmodels = -15458.077;
static double forecast = -11252.24;
static double tolerance = 0.15;

void test_setup(void) {
}

void test_teardown(void) {
}


// TODO handle tolerance negative or zero

#define alt_assert_double_tol(expected, result, tolerance) {\
	double minunit_tmp_e;\
	double minunit_tmp_r;\
	double minunit_tmp_tol;\
	double alt_ratio;\
	minunit_assert++;\
	minunit_tmp_e = (expected);\
	minunit_tmp_r = (result);\
	minunit_tmp_tol = (tolerance);\
	alt_ratio = minunit_tmp_r;\
	if (minunit_tmp_e != 0.0) {\
		alt_ratio = minunit_tmp_r / minunit_tmp_e;\
	}\
	if (fabs(1.0 - alt_ratio) > minunit_tmp_tol) {\
		int minunit_significant_figures = 7;\
		(void)snprintf(minunit_last_message, MINUNIT_MESSAGE_LEN, \
		"%s failed:\n\t%s:%d: result %.*g exceeds tolerance %.*g against standard %.*g", \
		__func__, __FILE__, __LINE__, \
		minunit_significant_figures, minunit_tmp_r, \
		minunit_significant_figures, minunit_tmp_tol,\
		minunit_significant_figures, minunit_tmp_e);\
		minunit_status = 1;\
		return;\
	} else {\
		printf(".");\
	}\
}

MU_TEST(test_assert_double_eq) {
	mu_assert_double_eq(0.1, dbar);
}

MU_TEST(test_assert_double_eq_fail) {
	mu_assert_double_eq(0.2, dbar);
}

MU_TEST(test_assert_statsmodels) {
	alt_assert_double_tol(statsmodels, sarima, tolerance);
}

MU_TEST(test_assert_forecast) {
	alt_assert_double_tol(forecast, sarima, tolerance);
}

MU_TEST(test_assert_mostfamous) {
	alt_assert_double_tol(forecast, statsmodels, tolerance);
}

MU_TEST(test_fail) {
	mu_fail("Fail now!");
}


MU_TEST_SUITE(test_suite) {
	MU_SUITE_CONFIGURE(&test_setup, &test_teardown);

	MU_RUN_TEST(test_assert_double_eq);
	MU_RUN_TEST(test_assert_double_eq_fail);
	
	MU_RUN_TEST(test_assert_statsmodels);
	MU_RUN_TEST(test_assert_forecast);
	MU_RUN_TEST(test_assert_mostfamous);
}

int main(int argc, char *argv[]) {
	MU_RUN_SUITE(test_suite);
	MU_REPORT();
	return MU_EXIT_CODE;
}

