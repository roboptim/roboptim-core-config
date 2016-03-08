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

#include <roboptim/core/config/loader.hh>

#include <Eigen/Core>
#include <yaml-cpp/yaml.h>

namespace YAML
{
  template <>
  struct convert<Eigen::VectorXd>
  {
    static bool decode (const Node& node, Eigen::VectorXd& rhs)
    {
      if (!node.IsSequence ())
        return false;

      typedef Eigen::VectorXd::Index size_type;

      rhs.resize (static_cast<size_type> (node.size ()));
      for (size_type i = 0; i < static_cast<size_type> (node.size ()); ++i)
        rhs (i) = node[i].as<double> ();

      return true;
    }
  };
} // end of namespace YAML

namespace roboptim
{
  ConfigLoader::ConfigLoader (const std::string& solverName)
    : parameters_ (),
      solverName_ (solverName)
  {}

  ConfigLoader::~ConfigLoader ()
  {}

  void ConfigLoader::load (const std::string& path)
  {
    YAML::Node root = YAML::LoadFile (path);
    load (root);
  }

  void ConfigLoader::load (const node_t& root, const std::string& prefix)
  {
    if (!root.IsMap ())
      throw std::runtime_error
        ("roboptim configuration should be a map");

    std::string key;

    typedef YAML::const_iterator citer_t;
    for (citer_t it = root.begin (); it != root.end (); ++it)
    {
      if (prefix.empty ())
        key = it->first.as<std::string> ();
      else key = prefix + "." + it->first.as<std::string> ();

      switch (it->second.Type ())
      {
        case YAML::NodeType::Scalar:
        case YAML::NodeType::Sequence:
        {
          parameters_[key] = loadParameter (it->second);
        }
        break;

        // recursive call
        case YAML::NodeType::Map:
        {
          load (it->second, key);
        }
        break;

        case YAML::NodeType::Null:
        case YAML::NodeType::Undefined:
        default:
          break;
      }
    }
  }

  const ConfigLoader::parameters_t&
  ConfigLoader::parameters () const
  {
    return parameters_;
  }

  void ConfigLoader::clear ()
  {
    parameters_.clear ();
  }

// TODO: find a cleaner method
#define TRY_TYPE(T) \
    try { \
      param.value = node.as<T> (); \
      return param; \
    } \
    catch (...) {}

  ConfigLoader::parameter_t
  ConfigLoader::loadParameter (const node_t& node) const
  {
    parameter_t param;
    TRY_TYPE(double);
    TRY_TYPE(vector_t);
    TRY_TYPE(int);
    TRY_TYPE(std::string);
    TRY_TYPE(bool);

    return param;
  }

#undef TRY_TYPE
} // end of namespace roboptim
