#pragma once

#include "qoiimpls.hpp"

struct TestData
{
private:
	TestData();
public:
	static const TestData& getInstance();
	struct
	{
		std::vector<uint8_t> dice;
		std::vector<uint8_t> kodim10;
		std::vector<uint8_t> kodim23;
		std::vector<uint8_t> qoilogo;
		std::vector<uint8_t> testcard;
		std::vector<uint8_t> testcardalpha;
		std::vector<uint8_t> wikipedia;

		std::vector<uint8_t> diff;
		std::vector<uint8_t> luma;
		std::vector<uint8_t> rgb;
		std::vector<uint8_t> rgba;
		std::vector<uint8_t> rgbidx;
		std::vector<uint8_t> run;
		std::vector<uint8_t> rng01;
		std::vector<uint8_t> rng02;
	} qoi, raw;
	struct
	{
		impl::ImageDescription dice;
		impl::ImageDescription kodim10;
		impl::ImageDescription kodim23;
		impl::ImageDescription qoilogo;
		impl::ImageDescription testcard;
		impl::ImageDescription testcardalpha;
		impl::ImageDescription wikipedia;

		impl::ImageDescription diff;
		impl::ImageDescription luma;
		impl::ImageDescription rgb;
		impl::ImageDescription rgba;
		impl::ImageDescription rgbidx;
		impl::ImageDescription run;
		impl::ImageDescription rng01;
		impl::ImageDescription rng02;
	} desc;
};
