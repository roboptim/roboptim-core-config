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

#include <stdexcept>

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

void testLoadedParameters (const ConfigLoader& loader)
{
  ConfigLoader::parameters_t::const_iterator it;
  typedef Function::vector_t vector_t;

  // max-iterations: 100
  it = loader.parameters ().find ("max-iterations");
  BOOST_CHECK (it != loader.parameters ().end ());
  BOOST_CHECK (boost::get<int> (it->second.value) == 100);

  // dummy-bool: true
  it = loader.parameters ().find ("dummy-bool");
  BOOST_CHECK (it != loader.parameters ().end ());
  BOOST_CHECK (boost::get<bool> (it->second.value) == true);

  // dummy-vector: [1., 2., 3.]
  it = loader.parameters ().find ("dummy-vector");
  vector_t dummy_v (3);
  dummy_v << 1., 2., 3.;
  BOOST_CHECK (it != loader.parameters ().end ());
  BOOST_CHECK (boost::get<vector_t> (it->second.value) == dummy_v);

  // ipopt.tol: 1.e-3
  it = loader.parameters ().find ("ipopt.tol");
  BOOST_CHECK (it != loader.parameters ().end ());
  BOOST_CHECK (boost::get<double> (it->second.value) == 1e-3);

  // ipopt.output_file: ipopt.log
  it = loader.parameters ().find ("ipopt.output_file");
  BOOST_CHECK (it != loader.parameters ().end ());
  BOOST_CHECK (boost::get<std::string> (it->second.value) == "ipopt.log");

}

BOOST_AUTO_TEST_CASE_TEMPLATE (loader, T, functionTypes_t)
{
  output = retrievePattern (std::string ("loader") + suffix<T> ());
  const std::string yaml_file = std::string (TESTS_DATA_DIR) + "/loader.yaml";
  const std::string json_file = std::string (TESTS_DATA_DIR) + "/loader.json";
  const std::string invalid_yaml_file =
    std::string (TESTS_DATA_DIR) + "/loader_invalid.yaml";

  // Test yaml file
  ConfigLoader loader;
  loader.load (yaml_file);
  testLoadedParameters (loader);

  // Test equivalent JSON file
  loader.clear ();
  loader.load (json_file);
  testLoadedParameters (loader);

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
  (*output) << solver << std::endl;

  // Apply loaded parameters
  loader.apply (solver);
  (*output) << solver << std::endl;

  // Overwrite all parameters
  loader.apply (solver, true);
  (*output) << solver << std::endl;

  // Test clear
  solver.parameters ().clear ();
  loader.clear ();
  loader.apply (solver);
  (*output) << solver << std::endl;

  // Load only ipopt-related parameters
  ConfigLoader ipopt_loader ("ipopt");
  ipopt_loader.load (yaml_file);
  solver.parameters ().clear ();
  ipopt_loader.apply (solver);
  (*output) << solver << std::endl;

  // Try to load a configuration file that does not exist
  BOOST_CHECK_THROW (ipopt_loader.load ("dummy_file.yaml"), std::runtime_error);

  // Try to load an invalid yaml file
  BOOST_CHECK_THROW (ipopt_loader.load (invalid_yaml_file), std::runtime_error);

  std::cout << output->str () << std::endl;
  BOOST_CHECK (output->match_pattern ());
}

BOOST_AUTO_TEST_SUITE_END ()
