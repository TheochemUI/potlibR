#include "cpp11.hpp"
#include "../subprojects/potlib/CppCore/src/base_types.hpp"
#include "../subprojects/potlib/CppCore/src/pot_types.hpp"
#include "../subprojects/potlib/CppCore/src/CuH2/CuH2Pot.hpp"
using namespace cpp11::literals;
using namespace cpp11;

//' @export
[[cpp11::register]]
writable::list cuh2pot_list(writable::data_frame df) {
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
  rgpot::AtomMatrix positions = Eigen::Map<rgpot::AtomMatrix>(coords.data(), x.size(), 3);

  // Define box (assuming it's constant for now)
  Eigen::Matrix3d box{{15.345599999999999, 0, 0}, {0, 21.702000000000002, 0}, {0, 0, 100.00000000000000}};

  // Compute the energy and forces
  auto cuh2pot = rgpot::CuH2Pot();
  auto [energy, forces] = cuh2pot(positions, atmtypes, box);

  // Prepare forces output matrix
  cpp11::writable::doubles_matrix<cpp11::by_row> forces_matrix(forces.rows(), 3);
  for(int i = 0; i < forces.rows(); ++i) {
    forces_matrix(i, 0) = forces(i, 0);
    forces_matrix(i, 1) = forces(i, 1);
    forces_matrix(i, 2) = forces(i, 2);
  }

  // Return a named List with the energy and forces Matrix
  cpp11::writable::list result;
  result.push_back("energy"_nm = cpp11::writable::doubles({energy}));
  result.push_back("forces"_nm = forces_matrix);

  return result;}
