#define CATCH_CONFIG_ENABLE_BENCHMARKING
#include <catch2/catch.hpp>

#include "qoiimpls.hpp"
#include "testdata.hpp"

#define QOI_BENCH_DECODE_FULL(FUNCTION) \
BENCHMARK_ADVANCED(#FUNCTION)(Catch::Benchmark::Chronometer meter) {\
	meter.measure([&] { return FUNCTION(testData.dice.qoi); });\
	meter.measure([&] { return FUNCTION(testData.kodim23.qoi); });\
	meter.measure([&] { return FUNCTION(testData.kodim10.qoi); });\
	meter.measure([&] { return FUNCTION(testData.qoilogo.qoi); });\
	meter.measure([&] { return FUNCTION(testData.testcard.qoi); });\
	meter.measure([&] { return FUNCTION(testData.testcardalpha.qoi); });\
	meter.measure([&] { return FUNCTION(testData.wikipedia.qoi); });\
	}
#define QOI_BENCH_DECODE_QUICK(FUNCTION) \
BENCHMARK_ADVANCED(#FUNCTION)(Catch::Benchmark::Chronometer meter) {\
	meter.measure([&] { return FUNCTION(testData.diff.qoi); });\
	meter.measure([&] { return FUNCTION(testData.luma.qoi); });\
	meter.measure([&] { return FUNCTION(testData.rgb.qoi); });\
	meter.measure([&] { return FUNCTION(testData.rgba.qoi); });\
	meter.measure([&] { return FUNCTION(testData.rgbidx.qoi); });\
	meter.measure([&] { return FUNCTION(testData.run.qoi); });\
	meter.measure([&] { return FUNCTION(testData.rng01.qoi); });\
	meter.measure([&] { return FUNCTION(testData.rng02.qoi); });\
	}
#define QOI_BENCH_ENCODE_FULL(FUNCTION) \
BENCHMARK_ADVANCED(#FUNCTION)(Catch::Benchmark::Chronometer meter) {\
	meter.measure([&] { return FUNCTION(testData.dice.raw, testData.dice.desc); });\
	meter.measure([&] { return FUNCTION(testData.kodim23.raw, testData.kodim23.desc); });\
	meter.measure([&] { return FUNCTION(testData.kodim10.raw, testData.kodim10.desc); });\
	meter.measure([&] { return FUNCTION(testData.qoilogo.raw, testData.qoilogo.desc); });\
	meter.measure([&] { return FUNCTION(testData.testcard.raw, testData.testcard.desc); });\
	meter.measure([&] { return FUNCTION(testData.testcardalpha.raw, testData.testcardalpha.desc); });\
	meter.measure([&] { return FUNCTION(testData.wikipedia.raw, testData.wikipedia.desc); });\
	}
#define QOI_BENCH_ENCODE_QUICK(FUNCTION) \
BENCHMARK_ADVANCED(#FUNCTION)(Catch::Benchmark::Chronometer meter) {\
	meter.measure([&] { return FUNCTION(testData.diff.raw, testData.diff.desc); });\
	meter.measure([&] { return FUNCTION(testData.luma.raw, testData.luma.desc); });\
	meter.measure([&] { return FUNCTION(testData.rgb.raw, testData.rgb.desc); });\
	meter.measure([&] { return FUNCTION(testData.rgba.raw, testData.rgba.desc); });\
	meter.measure([&] { return FUNCTION(testData.rgbidx.raw, testData.rgbidx.desc); });\
	meter.measure([&] { return FUNCTION(testData.run.raw, testData.run.desc); });\
	meter.measure([&] { return FUNCTION(testData.rng01.raw, testData.rng01.desc); });\
	}

TEST_CASE("decode quick", "[quick][decode]")
{
	using namespace impl;
	const TestData& testData = TestData::getInstance();

	QOI_BENCH_DECODE_QUICK(stillgreensan::qoi::decode);
	QOI_BENCH_DECODE_QUICK(phoboslab::qoi::decode);
	QOI_BENCH_DECODE_QUICK(pfusik::qoici::c::decode);
	QOI_BENCH_DECODE_QUICK(pfusik::qoici::cpp::decode);
	QOI_BENCH_DECODE_QUICK(shadowmitia::libqoi::decode);
//	QOI_BENCH_DECODE_QUICK(shraiwi::miniqoi::c::decode);
//	QOI_BENCH_DECODE_QUICK(shraiwi::miniqoi::cpp::decode);
	QOI_BENCH_DECODE_QUICK(wx257osn2::qoixx::decode);
}
TEST_CASE("decode full", "[full][decode]")
{
	using namespace impl;
	const TestData& testData = TestData::getInstance();

	QOI_BENCH_DECODE_FULL(stillgreensan::qoi::decode);
	QOI_BENCH_DECODE_FULL(phoboslab::qoi::decode);
	QOI_BENCH_DECODE_FULL(pfusik::qoici::c::decode);
	QOI_BENCH_DECODE_FULL(pfusik::qoici::cpp::decode);
	QOI_BENCH_DECODE_FULL(shadowmitia::libqoi::decode);
//	QOI_BENCH_DECODE(shraiwi::miniqoi::c::decode);
//	QOI_BENCH_DECODE(shraiwi::miniqoi::cpp::decode);
	QOI_BENCH_DECODE_FULL(wx257osn2::qoixx::decode);
}

TEST_CASE("encode quick", "[quick][encode]")
{
	using namespace impl;
	const TestData& testData = TestData::getInstance();

	QOI_BENCH_ENCODE_QUICK(stillgreensan::qoi::encode);
	QOI_BENCH_ENCODE_QUICK(phoboslab::qoi::encode);
//	QOI_BENCH_ENCODE_QUICK(pfusik::qoici::c::encode); // TODO assumes rgba fails on rgb
//	QOI_BENCH_ENCODE_QUICK(pfusik::qoici::cpp::encode);
	QOI_BENCH_ENCODE_QUICK(shadowmitia::libqoi::encode);
//	QOI_BENCH_ENCODE_QUICK(shraiwi::miniqoi::c::decode);
//	QOI_BENCH_ENCODE_QUICK(shraiwi::miniqoi::cpp::decode);
	QOI_BENCH_ENCODE_QUICK(wx257osn2::qoixx::encode);
}
TEST_CASE("encode full", "[full][encode]")
{
	using namespace impl;
	const TestData& testData = TestData::getInstance();

	QOI_BENCH_ENCODE_FULL(stillgreensan::qoi::encode);
	QOI_BENCH_ENCODE_FULL(phoboslab::qoi::encode);
//	QOI_BENCH_ENCODE_FULL(pfusik::qoici::c::encode); // TODO assumes rgba fails on rgb
//	QOI_BENCH_ENCODE_FULL(pfusik::qoici::cpp::encode);
	QOI_BENCH_ENCODE_FULL(shadowmitia::libqoi::encode);
//	QOI_BENCH_ENCODE_FULL(shraiwi::miniqoi::c::decode);
//	QOI_BENCH_ENCODE_FULL(shraiwi::miniqoi::cpp::decode);
	QOI_BENCH_ENCODE_FULL(wx257osn2::qoixx::encode);
}
