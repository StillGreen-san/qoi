#include "qoiimpls.hpp"

#include <mini_qoi.h>

namespace impl::shraiwi::miniqoi::c
{
struct Decoder : public IImageData
{
	Decoder() = delete;
	Decoder(const std::vector<uint8_t>& qoi)
	{
		::mqoi_desc_init(&description);
		const uint8_t* qoiIt = qoi.data();
		while(!::mqoi_desc_done(&description))
		{
			::mqoi_desc_push(&description, *qoiIt);
			++qoiIt;
		}

		uint32_t w, h;
		::mqoi_desc_verify(&description, &w, &h);

		::mqoi_dec_init(&decoder, w * h);
		pixels = new uint8_t[w * h * description.channels];
		uint8_t* rawIt = pixels;
		while(!::mqoi_dec_done(&decoder))
		{
			qoiIt += ::mqoi_dec_take(&decoder, qoiIt);
			while(mqoi_rgba_t* pix = ::mqoi_dec_pop(&decoder))
			{
				memcpy(rawIt, pix->value, description.channels);
				rawIt += description.channels;
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
		::mqoi_desc_verify(&description, &w, &h);
		return w * h * channels();
	}
	uint32_t width() override
	{
		uint32_t w, h;
		::mqoi_desc_verify(&description, &w, &h);
		return w;
	}
	uint32_t height() override
	{
		uint32_t w, h;
		::mqoi_desc_verify(&description, &w, &h);
		return h;
	}
	uint8_t channels() override
	{
		return description.channels;
	}
	uint8_t colorspace() override
	{
		return description.colorspace;
	}
	~Decoder() override
	{
		delete[] pixels;
	}
	::mqoi_desc_t description;
	::mqoi_dec_t decoder;
	uint8_t* pixels;
};

std::unique_ptr<IImageData> decode(const std::vector<uint8_t>& qoi)
{
	return std::make_unique<Decoder>(qoi);
}
} // namespace impl::shraiwi::miniqoi::c