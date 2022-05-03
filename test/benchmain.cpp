#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_ENABLE_BENCHMARKING
#include <catch2/catch.hpp>

#include "testdata.hpp"
#include "qoiimpls.hpp"

#define QOI_BENCH_DECODE(FUNCTION) \
BENCHMARK_ADVANCED(#FUNCTION)(Catch::Benchmark::Chronometer meter) {\
	meter.measure([&] { return FUNCTION(testData.qoi.dice); });\
	meter.measure([&] { return FUNCTION(testData.qoi.kodim23); });\
	meter.measure([&] { return FUNCTION(testData.qoi.kodim10); });\
	meter.measure([&] { return FUNCTION(testData.qoi.qoilogo); });\
	meter.measure([&] { return FUNCTION(testData.qoi.testcard); });\
	meter.measure([&] { return FUNCTION(testData.qoi.testcardalpha); });\
	meter.measure([&] { return FUNCTION(testData.qoi.wikipedia); });\
	}
#define QOI_BENCH_ENCODE(FUNCTION) \
BENCHMARK_ADVANCED(#FUNCTION)(Catch::Benchmark::Chronometer meter) {\
	meter.measure([&] { return FUNCTION(testData.raw.dice, testData.desc.dice); });\
	meter.measure([&] { return FUNCTION(testData.raw.kodim23, testData.desc.kodim23); });\
	meter.measure([&] { return FUNCTION(testData.raw.kodim10, testData.desc.kodim10); });\
	meter.measure([&] { return FUNCTION(testData.raw.qoilogo, testData.desc.qoilogo); });\
	meter.measure([&] { return FUNCTION(testData.raw.testcard, testData.desc.testcard); });\
	meter.measure([&] { return FUNCTION(testData.raw.testcardalpha, testData.desc.testcardalpha); });\
	meter.measure([&] { return FUNCTION(testData.raw.wikipedia, testData.desc.wikipedia); });\
	}

TEST_CASE("decode bench")
{
	using namespace impl;
	TestData testData;

	QOI_BENCH_DECODE(phoboslab::qoi::decode);
	QOI_BENCH_DECODE(pfusik::qoici::c::decode);
	QOI_BENCH_DECODE(pfusik::qoici::cpp::decode);
	QOI_BENCH_DECODE(shadowmitia::libqoi::decode);
//	QOI_BENCH_DECODE(shraiwi::miniqoi::c::decode);
//	QOI_BENCH_DECODE(shraiwi::miniqoi::cpp::decode);
	QOI_BENCH_DECODE(wx257osn2::qoixx::decode); // TODO insufficient input data
}

TEST_CASE("encode bench")
{
	using namespace impl;
	TestData testData;

	QOI_BENCH_ENCODE(phoboslab::qoi::encode);
//	QOI_BENCH_ENCODE(pfusik::qoici::c::encode); // TODO assumes rgba fails on rgb
//	QOI_BENCH_ENCODE(pfusik::qoici::cpp::encode);
	QOI_BENCH_ENCODE(shadowmitia::libqoi::encode);
//	QOI_BENCH_ENCODE(shraiwi::miniqoi::c::decode);
//	QOI_BENCH_ENCODE(shraiwi::miniqoi::cpp::decode);
	QOI_BENCH_ENCODE(wx257osn2::qoixx::encode);
}
