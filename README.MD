A C++ implementation for the [QOI (The Quite OK Image Format for Fast, Lossless Compression)](https://qoiformat.org/)
format

templated, header only, operating on containers, C++17(could probably be lower)

---

```cmake
target_link_libraries( ${Target} PRIVATE sgs::qoi )
```

```c++
#include <sgs/qoi.hpp>
```

decoding

```c++
 std::vector<uint8_t> data;
 // load qoi file data
 sgs::qoi::DataPair decoded = sgs::qoi::decode(data);
 // decoded.data (raw pixel data)
 // decoded.header (width, height, channels, colorspace)
 sgs::qoi::Header header = sgs::qoi::readHeader(data);
 // header (width, height, channels, colorspace)
```

encoding

```c++
 std::vector<uint8_t> data;
 // load raw pixel data
 sgs::qoi::Header header{
     300, 200, // width, height
     sgs::qoi::Channels::RGB,
     sgs::qoi::Colorspace::Linear
 };
 std::vector encoded = sgs::qoi::encode(header, data);
 // encoded (QOI file data (header, packets & endmarker))
```

specifying the output container type

```c++
 sgs::qoi::DataPair decoded = sgs::qoi::decode<std::deque<uint8_t>>(data);
 std::deque encoded = sgs::qoi::encode<std::deque<uint8_t>>(header, data);
```

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

to build benchmarks and tests enable the cmake options `ENABLE_BENCHMARK` & `ENABLE_TESTS`

for the purpose of benchmarking and testing all implementations are used through a common virtual interface,
the timings below may therefore not reflect the best performance one could achieve

all functions are benched against a set of example images, that you can find
[on the qoi website](https://qoiformat.org/qoi_test_images.zip), on an Intel i7-5820K (3.6GHz) Windows 10

| encoding                       | msvc       | clang-cl   | mingw      |
|--------------------------------|------------|------------|------------|
| **stillgreensan::qoi::encode** | 33.5666 ms | 41.5951 ms | 29.9616 ms |
| phoboslab::qoi::encode         | 28.4051 ms | 26.3849 ms | 23.9919 ms |
| shadowmitia::libqoi::encode    | 36.2259 ms | 32.4339 ms | 31.3426 ms |
| wx257osn2::qoixx::encode       | 32.1062 ms | 26.3908 ms | 22.0142 ms |

| decoding                       | msvc       | clang-cl   | mingw      |
|--------------------------------|------------|------------|------------|
| **stillgreensan::qoi::decode** | 29.8009 ms | 26.3818 ms | 24.2907 ms |
| phoboslab::qoi::decode         | 19.1761 ms | 21.6312 ms | 20.3470 ms |
| pfusik::qoici::c::decode       | 17.3896 ms | 17.5273 ms | 16.3815 ms |
| pfusik::qoici::cpp::decode     | 17.9252 ms | 18.5737 ms | 17.4916 ms |
| shadowmitia::libqoi::decode    | 21.0683 ms | 22.1937 ms | 20.0901 ms |
| shraiwi::miniqoi::c::decode    | 45.3839 ms | 47.1148 ms | 30.3102 ms |
| shraiwi::miniqoi::cpp::decode  | 46.2561 ms | 47.5463 ms | 30.5549 ms |
| wx257osn2::qoixx::decode       | 18.8729 ms | 15.9443 ms | 16.3083 ms |
