#include "cpp11.hpp"
#include "../subprojects/potlib/CppCore/src/base_types.hpp"
#include "../subprojects/potlib/CppCore/src/pot_types.hpp"
#include "../subprojects/potlib/CppCore/src/CuH2/CuH2Pot.hpp"
using namespace cpp11;
namespace writable = cpp11::writable;

//' @export
[[cpp11::register]] cpp11::writable::doubles cuh2pot(cpp11::data_frame df) {
  // Extract data from dataframe
  std::vector<double> x = cpp11::as_cpp<std::vector<double>>(df["x"]);
  std::vector<double> y = cpp11::as_cpp<std::vector<double>>(df["y"]);
  std::vector<double> z = cpp11::as_cpp<std::vector<double>>(df["z"]);
  cpp11::doubles atmNum = df["atmNum"];
  Eigen::VectorXi atmtypes(atmNum.size());
  std::copy(atmNum.begin(), atmNum.end(), atmtypes.data());

  rgpot::AtomMatrix positions;
  positions.resize(atmNum.size(), 3);
  for(int i = 0; i < atmNum.size(); ++i) {
    positions(i, 0) = x[i];
    positions(i, 1) = y[i];
    positions(i, 2) = z[i];
  }

  // Assuming box is constant for now.
  Eigen::Matrix3d box{{15.345599999999999, 0, 0}, //
                      {0, 21.702000000000002, 0}, //
                      {0, 0, 100.00000000000000}};

  auto cuh2pot = rgpot::CuH2Pot();
  auto [energy, forces] = cuh2pot(positions, atmtypes, box);

  // For now, let's just return the energy as a single element vector.
  return cpp11::writable::doubles({energy});
}
