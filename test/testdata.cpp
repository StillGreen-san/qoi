#pragma once

#include "testdata.hpp"

#include <filesystem>
#include <fstream>

namespace
{
void load(const std::filesystem::path& path, TestData::Triplet& data)
{
	if(!std::filesystem::exists(path))
	{
		throw std::runtime_error{"file not found"};
	}

	std::basic_ifstream<uint8_t> file(path, std::ios_base::in | std::ios_base::binary);

	const size_t fileSize = file_size(path);
	data.qoi.resize(fileSize);

	file.read(data.qoi.data(), static_cast<std::streamsize>(fileSize));

	std::unique_ptr<impl::IImageData> decoded = impl::phoboslab::qoi::decode(data.qoi);
	data.raw.assign(decoded->data(),
	    decoded->data() + decoded->size()); // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
	data.desc.width = decoded->width();
	data.desc.height = decoded->height();
	data.desc.channels = decoded->channels();
	data.desc.colorspace = decoded->colorspace();
}
} // namespace

TestData::TestData()
{
	load("dice.qoi", dice);
	load("kodim10.qoi", kodim10);
	load("kodim23.qoi", kodim23);
	load("qoi_logo.qoi", qoilogo);
	load("testcard.qoi", testcard);
	load("testcard_rgba.qoi", testcardalpha);
	load("wikipedia_008.qoi", wikipedia);

	load("diff.qoi", diff);
	load("luma.qoi", luma);
	load("rgb.qoi", rgb);
	load("rgba.qoi", rgba);
	load("rgbidx.qoi", rgbidx);
	load("run.qoi", run);
	load("rng01.qoi", rng01);
	load("rng02.qoi", rng02);
}

const TestData& TestData::getInstance()
{
	static TestData testData;
	return testData;
}
