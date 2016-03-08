roboptim-core-config
====================

[![License LGPL 3][badge-license]](http://www.gnu.org/licenses/lgpl-3.0.txt)
[![Build Status](https://travis-ci.org/roboptim/roboptim-core-config.png?branch=master)](https://travis-ci.org/roboptim/roboptim-core-config)
[![codecov.io](https://codecov.io/github/roboptim/roboptim-core-config/coverage.svg?branch=master)](https://codecov.io/github/roboptim/roboptim-core-config?branch=master)
[![Join the chat at https://gitter.im/roboptim/development](https://badges.gitter.im/Join%20Chat.svg)](https://gitter.im/roboptim/development?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge)

Helper library used to load RobOptim solver configurations from YAML files.

For now, this library is outside of roboptim-core as it adds an extra
dependency, and yaml-cpp may require C++11 for its next release.

### Installing dependencies

This package uses the following extra external tools compared to roboptim-core:

 * [yaml-cpp][] a YAML parser and emitter in C++

### YAML configuration example

Configuration files are organized as follows:

```yaml
# Shared RobOptim parameters
max-iterations: 10

# Parameters specific to Ipopt
ipopt:
  output_file: "ipopt.out"
  linear_solver: "ma57"
  tol: 1.e-3
  constr_viol_tol: 1.e-4
  acceptable_iter: 1

# Parameters specific to KNITRO
knitro:
  xtol: 1.e-5
  feastol: 1.e-3
```

Note that since RobOptim's solver parameters are stored as `boost::variant`
objects, great care needs to be taken when writing the configuration files. For
instance:

```yaml
# Parameters specific to Ipopt
ipopt:
  # This is treated as an integer (as expected):
  acceptable_iter: 10

  # This is treated as a double (which is wrong!):
  acceptable_iter: 10.
```

[badge-license]: https://img.shields.io/badge/license-LGPL_3-green.svg
[yaml-cpp]: https://github.com/jbeder/yaml-cpp
