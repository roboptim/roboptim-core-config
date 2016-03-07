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

#ifndef ROBOPTIM_CORE_CONFIG_LOADER_HH
# define ROBOPTIM_CORE_CONFIG_LOADER_HH

# include <string>

# include <roboptim/core/function.hh>
# include <roboptim/core/solver.hh>
# include <roboptim/core/config/config.hh>

// Forward declaration
namespace YAML
{
  class Node;
} // end of namespace YAML

namespace roboptim
{
  /// \brief Solver configuration loader.
  class ROBOPTIM_CORE_CONFIG_DLLAPI ConfigLoader
  {
  public:
    /// \brief Map of parameters.
    /// Note: this map is the same in dense and sparse.
    typedef Solver<EigenMatrixDense>::parameters_t parameters_t;
    typedef Parameter parameter_t;

    /// \brief YAML node type.
    typedef YAML::Node node_t;

    /// \brief Constructor.
    /// \param solverName optional solver name to filter parameters.
    ConfigLoader (const std::string& solverName = "");
    virtual ~ConfigLoader ();

    /// \brief Load parameters from a given YAML file.
    /// \param path path to a YAML file.
    /// \throws TODO
    void load (const std::string& path);

    /// \brief Load parameters from a given YAML node.
    /// \param node node of a YAML dictionary.
    /// \param prefix prefix for the node's keys.
    void load (const node_t& node, const std::string& prefix = "");

    /// \brief Apply stored parameters to the given solver.
    /// \tparam S solver type.
    /// \param solver solver to parametrize.
    template <typename S>
    void apply (S& solver) const
    {
      // FIXME
      solver.parameters () = parameters_;
    }

    /// \brief Reset stored parameters.
    void reset ();

    /// \brief Get the loaded parameters.
    const parameters_t& parameters () const;

  private:
    typedef Function::vector_t vector_t;

    /// \brief Load a solver parameter from a given scalar or sequence node.
    /// \param node node to convert.
    /// \return solver parameter.
    parameter_t loadParameter (const node_t& node) const;

  private:
    /// \brief Loaded parameters.
    parameters_t parameters_;

    /// \brief Solver name (to filter parameters).
    std::string solverName_;
  };
} // end of namespace roboptim

#endif //! ROBOPTIM_CORE_CONFIG_LOADER_HH
