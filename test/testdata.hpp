#pragma once

#include "qoiimpls.hpp"

struct TestData
{
private:
	TestData();

public:
	static const TestData& getInstance();
	struct Triplet
	{
		std::vector<uint8_t> qoi;
		std::vector<uint8_t> raw;
		impl::ImageDescription desc;
	} dice, kodim10, kodim23, qoilogo, testcard, testcardalpha, wikipedia, diff, luma, rgb, rgba, rgbidx, run, rng01,
	    rng02;
};
