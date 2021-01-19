# MPI parallelization of the Wang-Landau method

- [How to use](#how-to-use)
  - [How to build `sample.cpp`](#how-to-build-samplecpp)
  - [Example](#example)
  - [Available functions and classes](#available-functions-and-classes)
- [Requirements](#requirements)
  - [Modules](#modules)
  - [Member functions you must prepare](#member-functions-you-must-prepare)
- [License](#license)
- [Thanks](#thanks)

## How to use
The single file `inlcude/wl_mpi.hpp` is required to use this package.
When you use this package you need to create directory named `log` on your running
directory.

~~~shell-session
$ mkdir log
~~~

### How to build `sample.cpp`
You can build `sample.cpp` with

~~~shell-session
$ mkdir build
$ cd build
$ cmake ..
$ make
~~~

### Example
TODO: Add sentenses.
```c++
// Generate the MPIV type object as "mpiv".
int numprocs, myid;
MPI_Status status;
MPI_Init(&argc, &argv);
MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
MPI_Comm_rank(MPI_COMM_WORLD, &myid);
int num_wakkers_window = 2;
MPIV mpiv(numprocs, myid, num_walkers_window);
// Generate the object of your model which must satisfy requirements
// given below.
YourModel model;
// Generate the HistoEnvManager type object as "histo_env".
double histo_min = 0.0;
double histo_max = 9.0;
size_t num_bins = 10;
HistoEnvManager histo_env(histo_min, histo_max, num_bins, true);
// Generate the WLParams type object as "wl_params".
int check_flatness_every = 500;
double lnf = 1.0;
double lnfmin = 1e-8;
double flatness = 0.95;
double overlap = 0.75; // 0<= overlap <= 1.
int swap_every = 100;
WLParams wl_params(check_flatness_every, lnf, lnfmin, flatness, overlap,
    swap_every);
// Generate random engine according to the process rank.
std::mt19937 engine(myid);
// Set timelimit of this program as 60[sec].
double timelimit_secs = 60;
// Specify that this program run from the top with parameters given above.
bool from_the_top = true;

// Result will be written in "ln_dos" with the logarithmic scale.
std::vector<double> ln_dos;
RunningState running_state = rewl<YourModel>(&ln_dos, &model, histo_env,
    &wl_params, &mpiv, engine, timelimit_secs, from_the_top);
```

Note: When the whole REWL routine finishes, result of each devided window has been jonited and all-range result has been constructed. However the rank 0 process in MPI_COMM_WORLD is the only process that has the all-range results. Therefore if you would like to share this all-range result with the other (all) process(es), you have to send (broadcast) "ln_dos" to the other (all) process(es).

## Requirements
### Modules
This package needs OpenMPI and Json for Modern C++.
The latter is embedded as the git submodule.
Code for sample model depends on Eigen/Dense.
### Member functions you must prepare
If you would like to apply this package to your model, you need to prepare the following member functions in your model class (`YourModel`).
```c++
/**
 * @brief Just propose not update.
 * @param[in] engine the random engine.
 * @return double the proposed Wang-Landau variable (e.g. energy).
 */
double YourModel::Propose(std::mt19937 &engine);

//! @brief Accept propose and update the state.
void YourModel::Update();

/**
 * @brief Exchange informations with the given partner in the given communicator.
 *        e.g. the energy and spin configuration.
 * @param[in] partner the rank of the exchange partner in the communicator (local_comm).
 * @param[in] local_comm the communicator in which the exchange is executed.
 */
void YourModel::Exchange(int partner, MPI_Comm local_comm);

/**
 * @brief Store the intermediate state in the log file,
 *        which is necessary to restart the execution.
 * @param[out] ofs_ptr the pointer of the output file stream.
 */
void YourModel::StoreLog(std::ofstream *ofs_ptr);

/**
 * @brief Load the intermediate state from the log file.
 * @param[in] ifs the reference to the input file stream.
 */
void YourModel::SetFromLog(std::ifstream &ifs);

/**
 * @brief Gettor.
 * @return double the current value of the Wang-Landau variable (e.g. the energy).
 */
double YourModel::val();

/**
 * @brief Gettor.
 * @return size_t the definition of 1MCS on your model.
 */
size_t YourModel::sweeps();
```
Implementation examples are in `model_sample/ferro_ising.hpp`.

## License
The code is licensed under the [MIT License](https://opensource.org/licenses/MIT):

Copyright &copy; 2020-2021 Chihiro Kondo

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

## Thanks
- [Replica-Exchange Wang-Landau sampling tutorial lectures at IX Brazilian Meeting on Simulational Physics, 2017](https://github.com/yingwaili/bmsp2017)
- [JSON for Modern C++](https://github.com/nlohmann/json#)
