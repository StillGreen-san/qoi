#define CATCH_CONFIG_ENABLE_BENCHMARKING
#include <catch2/catch.hpp>

#include "qoiimpls.hpp"
#include "testdata.hpp"

#include <array>

#define QOI_BENCH_DECODE_FULL(FUNCTION, DATA) \
	BENCHMARK(#FUNCTION) \
	{ \
		return std::array<std::unique_ptr<impl::IImageData>, 8>{FUNCTION(DATA.dice.qoi), FUNCTION(DATA.kodim23.qoi), \
		    FUNCTION(DATA.kodim10.qoi), FUNCTION(DATA.qoilogo.qoi), FUNCTION(DATA.testcard.qoi), \
		    FUNCTION(DATA.testcardalpha.qoi), FUNCTION(DATA.wikipedia.qoi)}; \
	}
#define QOI_BENCH_DECODE_QUICK(FUNCTION, DATA) \
	BENCHMARK(#FUNCTION) \
	{ \
		return std::array<std::unique_ptr<impl::IImageData>, 8>{FUNCTION(DATA.diff.qoi), FUNCTION(DATA.luma.qoi), \
		    FUNCTION(DATA.rgb.qoi), FUNCTION(DATA.rgba.qoi), FUNCTION(DATA.rgbidx.qoi), FUNCTION(DATA.run.qoi), \
		    FUNCTION(DATA.rng01.qoi), FUNCTION(DATA.rng02.qoi)}; \
	}
#define QOI_BENCH_ENCODE_FULL(FUNCTION, DATA) \
	BENCHMARK(#FUNCTION) \
	{ \
		return std::array<std::unique_ptr<impl::IImageData>, 7>{FUNCTION(DATA.dice.raw, DATA.dice.desc), \
		    FUNCTION(DATA.kodim23.raw, DATA.kodim23.desc), FUNCTION(DATA.kodim10.raw, DATA.kodim10.desc), \
		    FUNCTION(DATA.qoilogo.raw, DATA.qoilogo.desc), FUNCTION(DATA.testcard.raw, DATA.testcard.desc), \
		    FUNCTION(DATA.testcardalpha.raw, DATA.testcardalpha.desc), \
		    FUNCTION(DATA.wikipedia.raw, DATA.wikipedia.desc)}; \
	}
#define QOI_BENCH_ENCODE_QUICK(FUNCTION, DATA) \
	BENCHMARK(#FUNCTION) \
	{ \
		return std::array<std::unique_ptr<impl::IImageData>, 7>{FUNCTION(DATA.diff.raw, DATA.diff.desc), \
		    FUNCTION(DATA.luma.raw, DATA.luma.desc), FUNCTION(DATA.rgb.raw, DATA.rgb.desc), \
		    FUNCTION(DATA.rgba.raw, DATA.rgba.desc), FUNCTION(DATA.rgbidx.raw, DATA.rgbidx.desc), \
		    FUNCTION(DATA.run.raw, DATA.run.desc), FUNCTION(DATA.rng01.raw, DATA.rng01.desc)}; \
	}

TEST_CASE("decode quick", "[quick][decode]")
{
	using namespace impl;
	const TestData& testData = TestData::getInstance();

	QOI_BENCH_DECODE_QUICK(stillgreensan::qoi::decode, testData);
	QOI_BENCH_DECODE_QUICK(phoboslab::qoi::decode, testData);
	QOI_BENCH_DECODE_QUICK(pfusik::qoici::c::decode, testData);
	QOI_BENCH_DECODE_QUICK(pfusik::qoici::cpp::decode, testData);
	QOI_BENCH_DECODE_QUICK(shadowmitia::libqoi::decode, testData);
	//	QOI_BENCH_DECODE_QUICK(shraiwi::miniqoi::c::decode, testData);
	//	QOI_BENCH_DECODE_QUICK(shraiwi::miniqoi::cpp::decode, testData);
	QOI_BENCH_DECODE_QUICK(wx257osn2::qoixx::decode, testData);
}
TEST_CASE("decode full", "[full][decode]")
{
	using namespace impl;
	const TestData& testData = TestData::getInstance();

	QOI_BENCH_DECODE_FULL(stillgreensan::qoi::decode, testData);
	QOI_BENCH_DECODE_FULL(phoboslab::qoi::decode, testData);
	QOI_BENCH_DECODE_FULL(pfusik::qoici::c::decode, testData);
	QOI_BENCH_DECODE_FULL(pfusik::qoici::cpp::decode, testData);
	QOI_BENCH_DECODE_FULL(shadowmitia::libqoi::decode, testData);
	//	QOI_BENCH_DECODE(shraiwi::miniqoi::c::decode, testData);
	//	QOI_BENCH_DECODE(shraiwi::miniqoi::cpp::decode, testData);
	QOI_BENCH_DECODE_FULL(wx257osn2::qoixx::decode, testData);
}

TEST_CASE("encode quick", "[quick][encode]")
{
	using namespace impl;
	const TestData& testData = TestData::getInstance();

	QOI_BENCH_ENCODE_QUICK(stillgreensan::qoi::encode, testData);
	QOI_BENCH_ENCODE_QUICK(phoboslab::qoi::encode, testData);
	//	QOI_BENCH_ENCODE_QUICK(pfusik::qoici::c::encode, testData); // TODO assumes rgba fails on rgb
	//	QOI_BENCH_ENCODE_QUICK(pfusik::qoici::cpp::encode, testData);
	QOI_BENCH_ENCODE_QUICK(shadowmitia::libqoi::encode, testData);
	//	QOI_BENCH_ENCODE_QUICK(shraiwi::miniqoi::c::encode, testData);
	//	QOI_BENCH_ENCODE_QUICK(shraiwi::miniqoi::cpp::encode, testData);
	QOI_BENCH_ENCODE_QUICK(wx257osn2::qoixx::encode, testData);
}
TEST_CASE("encode full", "[full][encode]")
{
	using namespace impl;
	const TestData& testData = TestData::getInstance();

	QOI_BENCH_ENCODE_FULL(stillgreensan::qoi::encode, testData);
	QOI_BENCH_ENCODE_FULL(phoboslab::qoi::encode, testData);
	//	QOI_BENCH_ENCODE_FULL(pfusik::qoici::c::encode, testData); // TODO assumes rgba fails on rgb
	//	QOI_BENCH_ENCODE_FULL(pfusik::qoici::cpp::encode, testData);
	QOI_BENCH_ENCODE_FULL(shadowmitia::libqoi::encode, testData);
	//	QOI_BENCH_ENCODE_FULL(shraiwi::miniqoi::c::encode, testData);
	//	QOI_BENCH_ENCODE_FULL(shraiwi::miniqoi::cpp::encode, testData);
	QOI_BENCH_ENCODE_FULL(wx257osn2::qoixx::encode, testData);
}
