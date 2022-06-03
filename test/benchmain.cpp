#define CATCH_CONFIG_ENABLE_BENCHMARKING
#include <catch2/catch.hpp>

#include "qoiimpls.hpp"
#include "testdata.hpp"

#include <array>

#define QOI_BENCH_DECODE(FUNCTION, DATA) \
	BENCHMARK(#FUNCTION) \
	{ \
		return std::array<std::unique_ptr<impl::IImageData>, 8>{FUNCTION(DATA.dice.qoi), FUNCTION(DATA.kodim23.qoi), \
		    FUNCTION(DATA.kodim10.qoi), FUNCTION(DATA.qoilogo.qoi), FUNCTION(DATA.testcard.qoi), \
		    FUNCTION(DATA.testcardalpha.qoi), FUNCTION(DATA.wikipedia.qoi)}; \
	}
#define QOI_BENCH_ENCODE(FUNCTION, DATA) \
	BENCHMARK(#FUNCTION) \
	{ \
		return std::array<std::unique_ptr<impl::IImageData>, 7>{FUNCTION(DATA.dice.raw, DATA.dice.desc), \
		    FUNCTION(DATA.kodim23.raw, DATA.kodim23.desc), FUNCTION(DATA.kodim10.raw, DATA.kodim10.desc), \
		    FUNCTION(DATA.qoilogo.raw, DATA.qoilogo.desc), FUNCTION(DATA.testcard.raw, DATA.testcard.desc), \
		    FUNCTION(DATA.testcardalpha.raw, DATA.testcardalpha.desc), \
		    FUNCTION(DATA.wikipedia.raw, DATA.wikipedia.desc)}; \
	}
#define QOI_BENCHMARK_B(TAGS, PATH) \
	TEST_CASE(#PATH, TAGS)\
	{\
		using namespace impl; /*NOLINT(google-build-using-namespace)*/\
		const TestData& testData = TestData::getInstance();\
		QOI_BENCH_DECODE(PATH::decode, testData);\
		QOI_BENCH_ENCODE(PATH::encode, testData);\
	}
#define QOI_BENCHMARK_D(TAGS, PATH) \
	TEST_CASE(#PATH, TAGS)\
	{\
		using namespace impl; /*NOLINT(google-build-using-namespace)*/\
		const TestData& testData = TestData::getInstance();\
		QOI_BENCH_DECODE(PATH::decode, testData);\
	}

QOI_BENCHMARK_B("[all][sgs]", stillgreensan::qoi)

QOI_BENCHMARK_B("[all][ext]", phoboslab::qoi)

QOI_BENCHMARK_D("[all][ext]", pfusik::qoici::c)

QOI_BENCHMARK_D("[all][ext]", pfusik::qoici::cpp)

QOI_BENCHMARK_B("[all][ext]", shadowmitia::libqoi)

QOI_BENCHMARK_D("[all][ext]", shraiwi::miniqoi::c)

QOI_BENCHMARK_D("[all][ext]", shraiwi::miniqoi::cpp)

QOI_BENCHMARK_B("[all][ext]", wx257osn2::qoixx)
