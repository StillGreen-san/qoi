A C++ implementation for the [QOI (The Quite OK Image Format for Fast, Lossless Compression)](https://qoiformat.org/)
format

templated, header only, operating on containers, C++17(could probably be lower)

---

using the library itself has no dependencies beyond the standard library

tests and benchmark do however make use of other libraries:

| repo                                                        | license                                                          | language    | used in      |
|-------------------------------------------------------------|------------------------------------------------------------------|-------------|--------------|
| [Catch2](https://github.com/catchorg/Catch2)                | [BSL](https://github.com/catchorg/Catch2/blob/devel/LICENSE.txt) | C++         | bench & test |
| [phoboslab/qoi](https://github.com/phoboslab/qoi)           | [MIT](https://github.com/phoboslab/qoi/blob/master/README.md)    | C           | bench & test |
| [pfusik/qoi-ci](https://github.com/pfusik/qoi-ci)           | [MIT](https://github.com/pfusik/qoi-ci/blob/master/README.md)    | Ć, (C, C++) | bench        |
| [ShadowMitia/libqoi](https://github.com/ShadowMitia/libqoi) | [MIT](https://github.com/ShadowMitia/libqoi/blob/main/LICENSE)   | C++         | bench        |
| [wx257osn2/qoixx](https://github.com/wx257osn2/qoixx)       | [MIT](https://github.com/wx257osn2/qoixx/blob/master/LICENSE)    | C++20       | bench        |
| [shraiwi/mini-qoi](https://github.com/shraiwi/mini-qoi)     | [MIT](https://github.com/shraiwi/mini-qoi/blob/master/LICENSE)   | C, (C++)    | bench        |

for the purpose of benchmarking and testing all implementations are used through a common virtual interface,
the timings below may therefore not reflect the best performance one could achieve

all functions are benched against a set of example images, you can find
[on the qoi website](https://qoiformat.org/qoi_test_images.zip), on an Intel i7-5820K (3.6GHz) Windows 10

| encoding                       | msvc       |
|--------------------------------|------------|
| **stillgreensan::qoi::encode** | 39.8410 ms |
| phoboslab::qoi::encode         | 34.1664 ms |
| shadowmitia::libqoi::encode    | 42.9022 ms |
| wx257osn2::qoixx::encode       | 38.1814 ms |

| decoding                       | msvc       |
|--------------------------------|------------|
| **stillgreensan::qoi::decode** | 34.5848 ms |
| phoboslab::qoi::decode         | 22.3543 ms |
| pfusik::qoici::c::decode       | 20.1871 ms |
| pfusik::qoici::cpp::decode     | 21.6659 ms |
| shadowmitia::libqoi::decode    | 24.8392 ms |
| shraiwi::miniqoi::c::decode    | 52.9142 ms |
| shraiwi::miniqoi::cpp::decode  | 54.5783 ms |
| wx257osn2::qoixx::decode       | 22.6510 ms |
