/* wrapper.hpp -- header file for wrapper around diptest implementation
 * Copyright 2022 R. Urlus
 */
#ifndef INCLUDE_DIPTEST_WRAPPER_HPP_
#define INCLUDE_DIPTEST_WRAPPER_HPP_
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <pybind11/stl.h>

extern "C" {
#include <diptest/diptest.h>
}

#include <algorithm>         // sort
#include <cmath>             // NAN
#include <numeric>           // accumulate
#include <memory>            // unique_ptr
#include <random>            // mt19937_64, uniform_real_distribution
#include <stdexcept>         // runtime_error

#if defined(DIPTEST_HAS_OPENMP_SUPPORT)
#include <omp.h>
#endif

#if defined(_WIN32) || defined(_WIN64) || defined(WIN32) || defined(__CYGWIN__) || defined(__MINGW32__) || defined(__BORLANDC__)
#define OS_WIN
#endif

// handle error C2059: syntax error: ';'  on windows for this Macro
#if not defined(OS_WIN)
  #define STRINGIFY(x) #x
  #define MACRO_STRINGIFY(x) STRINGIFY(x)
#endif

// Fix for lack of ssize_t on Windows for CPython3.10
#if defined(_MSC_VER)
#  pragma warning(push)
#  pragma warning(disable: 4127) // warning C4127: Conditional expression is constant
#include <BaseTsd.h>
typedef SSIZE_T ssize_t;
#endif

namespace py = pybind11;

namespace diptest {

namespace details {
void draw_sorted_std_uniform(double* dest, const size_t size, const size_t seed = 0);
double* std_uniform(const int64_t n_boot, const int64_t n, const int64_t seed);
double diptest(const double* x_ptr, int N, int allow_zero = 1, int debug = 0);
}  // namespace details

double diptest(const py::array_t<double>&x, int allow_zero, int debug);
py::dict diptest_full(const py::array_t<double>&x, int allow_zero, int debug);
double diptest_pval(
    const double dipstat,
    const int64_t n,
    const int64_t n_boot,
    int allow_zero,
    int debug,
    int64_t seed
);

double diptest_pval_mt(
    const double dipstat,
    const int64_t n,
    const int64_t n_boot,
    int allow_zero,
    int debug,
    int64_t seed,
    size_t n_threads

);

namespace bindings {

void bind_diptest(py::module &m);
void bind_diptest_full(py::module &m);
void bind_diptest_pval(py::module &m);
void bind_diptest_pval_mt(py::module &m);


}  // namespace bindings
}  // namespace diptest

#endif  // INCLUDE_DIPTEST_WRAPPER_HPP_