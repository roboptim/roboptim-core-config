// Copyright (C) 2016 by Benjamin Chrétien, CNRS-AIST JRL.
//
// This file is part of roboptim.
//
// roboptim is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// roboptim is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with roboptim.  If not, see <http://www.gnu.org/licenses/>.

#include "shared-tests/fixture.hh"

#include <boost/make_shared.hpp>

#include <roboptim/core/function.hh>
#include <roboptim/core/config/loader.hh>
#include <roboptim/core/function/constant.hh>

using namespace roboptim;

typedef boost::mpl::list< ::roboptim::EigenMatrixDense,
			  ::roboptim::EigenMatrixSparse> functionTypes_t;

boost::shared_ptr<boost::test_tools::output_test_stream> output;

BOOST_FIXTURE_TEST_SUITE (core_config, TestSuiteConfiguration)

template <typename T>
std::string suffix ();

template <>
std::string suffix<EigenMatrixDense> ()
{
  return "";
}

template <>
std::string suffix<EigenMatrixSparse> ()
{
  return "-sparse";
}

BOOST_AUTO_TEST_CASE_TEMPLATE (loader, T, functionTypes_t)
{
  output = retrievePattern (std::string ("loader") + suffix<T> ());

  ConfigLoader loader;
  loader.load (std::string (TESTS_DATA_DIR) + "/loader.yaml");

  typedef GenericConstantFunction<T> constantF_t;
  typename constantF_t::vector_t v (1);
  v[0] = 12.;
  boost::shared_ptr<constantF_t> f = boost::make_shared<constantF_t> (v);

  // Create problem.
  typedef Solver<T> solver_t;
  typename solver_t::problem_t pb (f);

  // Initialize solver.
  SolverFactory<solver_t> factory (std::string ("dummy") + suffix<T> (), pb);
  solver_t& solver = factory ();
  solver.parameters ().clear ();
  (*output) << solver << std::endl;

  // Apply loaded parameters
  loader.apply (solver);
  (*output) << solver << std::endl;

  std::cout << output->str () << std::endl;
  BOOST_CHECK (output->match_pattern ());
}

BOOST_AUTO_TEST_SUITE_END ()
