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

struct double_duo {
	double left;
	double right;
};

struct double_trio {
	double left;
	double middle;
	double right;
};

MU_TEST_EXT(test_assert_double_eq, param) {
	struct double_duo* duo = (struct double_duo*)param;
	mu_assert_double_eq(duo->left, duo->right);
}

MU_TEST_EXT(test_assert_double_eq_fail, param) {
	struct double_duo* duo = (struct double_duo*)param;
	mu_assert_double_eq(duo->left, duo->right);
}

MU_TEST_EXT(test_assert_statsmodels, param) {
	struct double_trio* trio = (struct double_trio*)param;
	mu_assert_double_tol(trio->left, trio->middle, trio->right);
}

MU_TEST_EXT(test_assert_forecast, param) {
	struct double_trio* trio = (struct double_trio*)param;
	mu_assert_double_tol(trio->left, trio->middle, trio->right);
}

MU_TEST_EXT(test_assert_mostfamous, param) {
	struct double_trio* trio = (struct double_trio*)param;
	mu_assert_double_tol(trio->left, trio->middle, trio->right);
}


MU_TEST_SUITE(test_suite_whole) {
	struct double_duo duo = {0.0, 0.0};
	struct double_trio trio = {0.0, 0.0, 0.0};
	
	MU_SUITE_CONFIGURE_EXT(&test_setup_ext, NULL, &test_teardown_ext, NULL);

	duo.left  = 0.1; duo.right = dbar; 
	MU_RUN_TEST_EXT(test_assert_double_eq, &duo);
	
	duo.left  = 0.2; duo.right = dbar; 
	MU_RUN_TEST_EXT(test_assert_double_eq_fail, &duo);
	
	mu_assert_double_tol(statsmodels, sarima, tolerance);
	
	trio.left = statsmodels; trio.middle = sarima; trio.right = tolerance;
	MU_RUN_TEST_EXT(test_assert_statsmodels, &trio);
		
	trio.left = forecast; trio.middle = sarima; trio.right = tolerance;
	MU_RUN_TEST_EXT(test_assert_forecast,  &trio);
		
	trio.left = forecast; trio.middle = statsmodels; trio.right = tolerance;
	MU_RUN_TEST_EXT(test_assert_mostfamous, &trio);
}

int main() {
	MU_RUN_SUITE(test_suite_whole);
	MU_REPORT();
	return MU_EXIT_CODE;
}

