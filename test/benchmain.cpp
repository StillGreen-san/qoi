#define CATCH_CONFIG_ENABLE_BENCHMARKING
#include <catch2/catch.hpp>

#include "testdata.hpp"
#include "qoiimpls.hpp"

// TODO add bench for simple tests?

#define QOI_BENCH_DECODE(FUNCTION) \
BENCHMARK_ADVANCED(#FUNCTION)(Catch::Benchmark::Chronometer meter) {\
	meter.measure([&] { return FUNCTION(testData.dice.qoi); });\
	meter.measure([&] { return FUNCTION(testData.kodim23.qoi); });\
	meter.measure([&] { return FUNCTION(testData.kodim10.qoi); });\
	meter.measure([&] { return FUNCTION(testData.qoilogo.qoi); });\
	meter.measure([&] { return FUNCTION(testData.testcard.qoi); });\
	meter.measure([&] { return FUNCTION(testData.testcardalpha.qoi); });\
	meter.measure([&] { return FUNCTION(testData.wikipedia.qoi); });\
	}
#define QOI_BENCH_ENCODE(FUNCTION) \
BENCHMARK_ADVANCED(#FUNCTION)(Catch::Benchmark::Chronometer meter) {\
	meter.measure([&] { return FUNCTION(testData.dice.raw, testData.dice.desc); });\
	meter.measure([&] { return FUNCTION(testData.kodim23.raw, testData.kodim23.desc); });\
	meter.measure([&] { return FUNCTION(testData.kodim10.raw, testData.kodim10.desc); });\
	meter.measure([&] { return FUNCTION(testData.qoilogo.raw, testData.qoilogo.desc); });\
	meter.measure([&] { return FUNCTION(testData.testcard.raw, testData.testcard.desc); });\
	meter.measure([&] { return FUNCTION(testData.testcardalpha.raw, testData.testcardalpha.desc); });\
	meter.measure([&] { return FUNCTION(testData.wikipedia.raw, testData.wikipedia.desc); });\
	}

TEST_CASE("decode bench")
{
	using namespace impl;
	const TestData& testData = TestData::getInstance();

	QOI_BENCH_DECODE(stillgreensan::qoi::decode);
	QOI_BENCH_DECODE(phoboslab::qoi::decode);
	QOI_BENCH_DECODE(pfusik::qoici::c::decode);
	QOI_BENCH_DECODE(pfusik::qoici::cpp::decode);
	QOI_BENCH_DECODE(shadowmitia::libqoi::decode);
//	QOI_BENCH_DECODE(shraiwi::miniqoi::c::decode);
//	QOI_BENCH_DECODE(shraiwi::miniqoi::cpp::decode);
	QOI_BENCH_DECODE(wx257osn2::qoixx::decode);
}

TEST_CASE("encode bench")
{
	using namespace impl;
	const TestData& testData = TestData::getInstance();

	QOI_BENCH_ENCODE(stillgreensan::qoi::encode);
	QOI_BENCH_ENCODE(phoboslab::qoi::encode);
//	QOI_BENCH_ENCODE(pfusik::qoici::c::encode); // TODO assumes rgba fails on rgb
//	QOI_BENCH_ENCODE(pfusik::qoici::cpp::encode);
	QOI_BENCH_ENCODE(shadowmitia::libqoi::encode);
//	QOI_BENCH_ENCODE(shraiwi::miniqoi::c::decode);
//	QOI_BENCH_ENCODE(shraiwi::miniqoi::cpp::decode);
	QOI_BENCH_ENCODE(wx257osn2::qoixx::encode);
}
