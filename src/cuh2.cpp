#include "../subprojects/potlib/CppCore/src/CuH2/CuH2Pot.hpp"
#include "../subprojects/potlib/CppCore/src/base_types.hpp"
#include "../subprojects/potlib/CppCore/src/pot_types.hpp"
#include "cpp11.hpp"
using namespace cpp11::literals;
using namespace cpp11;

// Constant for now
const Eigen::Matrix3d BOX{{15.345599999999999, 0, 0},
                          {0, 21.702000000000002, 0},
                          {0, 0, 100.00000000000000}};

//' @export
[[cpp11::register]] writable::list cuh2pot_list(writable::data_frame df,
                                                bool a_isFixed = false) {
  // Extract data from dataframe
  std::vector<double> x = cpp11::as_cpp<std::vector<double>>(df["x"]);
  std::vector<double> y = cpp11::as_cpp<std::vector<double>>(df["y"]);
  std::vector<double> z = cpp11::as_cpp<std::vector<double>>(df["z"]);

  cpp11::doubles atmNum = df["atmNum"];
  Eigen::VectorXi atmtypes(atmNum.size());
  std::copy(atmNum.begin(), atmNum.end(), atmtypes.data());

  // Populate atom positions matrix
  std::vector<double> coords;
  coords.reserve(3 * x.size());
  for (size_t i = 0; i < x.size(); ++i) {
    coords.insert(coords.end(), {x[i], y[i], z[i]});
  }
  rgpot::AtomMatrix positions =
      Eigen::Map<rgpot::AtomMatrix>(coords.data(), x.size(), 3);

  // Compute the energy and forces
  auto cuh2pot = rgpot::CuH2Pot();
  auto [energy, forces] = cuh2pot(positions, atmtypes, BOX);

  // Prepare forces output matrix
  cpp11::writable::doubles_matrix<cpp11::by_row> forces_matrix(forces.rows(),
                                                               3);
  if (a_isFixed) {
    cpp11::writable::logicals isfixed = df["is_fixed"];
    for (size_t idx{0}; idx < forces.rows(); ++idx) {
      if (!(isfixed[idx] == true)) {
        forces_matrix(idx, 0) = forces(idx, 0);
        forces_matrix(idx, 1) = forces(idx, 1);
        forces_matrix(idx, 2) = forces(idx, 2);
      } else {
        forces_matrix(idx, 0) = 0;
        forces_matrix(idx, 1) = 0;
        forces_matrix(idx, 2) = 0;
      }
    }
  } else {
    for (size_t idx{0}; idx < forces.rows(); ++idx) {
      forces_matrix(idx, 0) = forces(idx, 0);
      forces_matrix(idx, 1) = forces(idx, 1);
      forces_matrix(idx, 2) = forces(idx, 2);
    }
  }

  // Return a named List with the energy and forces Matrix
  cpp11::writable::list result;
  result.push_back("energy"_nm = cpp11::writable::doubles({energy}));
  result.push_back("forces"_nm = forces_matrix);

  return result;
}
