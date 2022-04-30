#include "qoiimpls.hpp"

#include <MiniQOI.h>

namespace impl::shraiwi::miniqoi::cpp
{
struct Decoder : public IImageData
{
	Decoder() = delete;
	Decoder(const std::vector<uint8_t>& qoi)
	{
		const uint8_t* qoiIt = qoi.data();
		while(!description.done())
		{
			description.push(*qoiIt);
			++qoiIt;
		}

		uint32_t w, h;
		description.verify(&w, &h);

		mQOI::Decoder decoder(w * h);
		pixels = new uint8_t[w * h * description.channels()];
		uint8_t* rawIt = pixels;
		while(!decoder.done())
		{
			qoiIt += decoder.take(qoiIt);
			while(mQOI::RGBA* pix = decoder.pop())
			{
				memcpy(rawIt, pix->value, description.channels());
				rawIt += description.channels();
			}
		}
	}
	const uint8_t* data() override
	{
		return pixels;
	}
	size_t size() override
	{
		uint32_t w, h;
		description.verify(&w, &h);
		return w * h * channels();
	}
	uint32_t width() override
	{
		uint32_t w, h;
		description.verify(&w, &h);
		return w;
	}
	uint32_t height() override
	{
		uint32_t w, h;
		description.verify(&w, &h);
		return h;
	}
	uint8_t channels() override
	{
		return description.channels();
	}
	uint8_t colorspace() override
	{
		return description.colorspace();
	}
	~Decoder() override
	{
		delete[] pixels;
	}
	mQOI::Descriptor description;
	uint8_t* pixels;
};

std::unique_ptr<IImageData> decode(const std::vector<uint8_t>& qoi)
{
	return std::make_unique<Decoder>(qoi);
}
} // namespace impl::shraiwi::miniqoi::cpp
