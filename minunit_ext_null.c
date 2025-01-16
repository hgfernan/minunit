/**
 * Sample code for *ext() macros
 * Created on Wed Jan 15 12:31:27 2025
 * 
 * @author: hilton
 **/

#include "minunit.h"

static double dbar = 0.1;
static double sarima = -11530.2;
static double statsmodels = -15458.077;
static double forecast = -11252.24;
static double tolerance = 0.15;

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

