#include <CLI/App.hpp>
#include <CLI/CLI.hpp>
#include <CLI/Config.hpp>

#include "GaussSeidel.hpp"
#include "Graph.hpp"

int main(int argc, char **argv) {
  CLI::App App;
  double Omega;
  // Omega = 1 is Gauss-Seidel solver
  App.add_option("--omega", Omega, "Omega coefficient for SOR solver")
      ->default_val(1);
  
}