#pragma once

#include "testdata.hpp"

#include <filesystem>
#include <fstream>

namespace
{
void load(const std::filesystem::path& path, std::vector<uint8_t>& qoi, std::vector<uint8_t>& raw, impl::ImageDescription& desc)
{
	if(!std::filesystem::exists(path))
	{
		throw std::exception{"file not found"};
	}

	std::basic_ifstream<uint8_t> file(path, std::ios_base::in | std::ios_base::binary);

	const size_t fileSize = file_size(path);
	qoi.resize(fileSize);

	file.read(qoi.data(), static_cast<std::streamsize>(fileSize));

	std::unique_ptr<impl::IImageData> decoded = impl::phoboslab::qoi::decode(qoi);
	raw.assign(decoded->data(), decoded->data() + decoded->size());
	desc.width = decoded->width();
	desc.height = decoded->height();
	desc.channels = decoded->channels();
	desc.colorspace = decoded->colorspace();
}
} // namespace

TestData::TestData()
{
	load("dice.qoi", qoi.dice, raw.dice, desc.dice);
	load("kodim10.qoi", qoi.kodim10, raw.kodim10, desc.kodim10);
	load("kodim23.qoi", qoi.kodim23, raw.kodim23, desc.kodim23);
	load("qoi_logo.qoi", qoi.qoilogo, raw.qoilogo, desc.qoilogo);
	load("testcard.qoi", qoi.testcard, raw.testcard, desc.testcard);
	load("testcard_rgba.qoi", qoi.testcardalpha, raw.testcardalpha, desc.testcardalpha);
	load("wikipedia_008.qoi", qoi.wikipedia, raw.wikipedia, desc.wikipedia);
}

const TestData& TestData::getInstance()
{
	static TestData testData;
	return testData;
}
