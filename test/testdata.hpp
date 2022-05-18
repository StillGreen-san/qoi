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
	} desc;
};
