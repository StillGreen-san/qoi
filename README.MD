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

_(there is also quite a bit of variance in results on my machine, even when averaging over multiple runs)_

all functions are benched against a set of example images, you can find
[on the qoi website](https://qoiformat.org/qoi_test_images.zip), on an Intel i7-5820K (3.6GHz) Windows 10

| encoding                       | msvc       | clang-cl   |
|--------------------------------|------------|------------|
| **stillgreensan::qoi::encode** | 36.6321 ms | 44.7250 ms |
| phoboslab::qoi::encode         | 30.7396 ms | 28.1780 ms |
| shadowmitia::libqoi::encode    | 39.0356 ms | 33.1281 ms |
| wx257osn2::qoixx::encode       | 35.0958 ms | 27.3886 ms |

| decoding                       | msvc       | clang-cl   |
|--------------------------------|------------|------------|
| **stillgreensan::qoi::decode** | 32.2759 ms | 28.7773 ms |
| phoboslab::qoi::decode         | 20.7712 ms | 23.1185 ms |
| pfusik::qoici::c::decode       | 18.7147 ms | 18.2227 ms |
| pfusik::qoici::cpp::decode     | 19.5511 ms | 19.8339 ms |
| shadowmitia::libqoi::decode    | 22.7735 ms | 23.4334 ms |
| shraiwi::miniqoi::c::decode    | 48.4286 ms | 49.3448 ms |
| shraiwi::miniqoi::cpp::decode  | 50.7878 ms | 48.7711 ms |
| wx257osn2::qoixx::decode       | 20.2520 ms | 16.4365 ms |
