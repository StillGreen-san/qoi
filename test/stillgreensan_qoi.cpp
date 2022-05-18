#include "qoiimpls.hpp"

#include "sgs/qoi.hpp"

namespace impl::stillgreensan::qoi
{
sgs::qoi::Header toHeader(const ImageDescription& desc)
{
	return {desc.width, desc.height, static_cast<sgs::qoi::Channels>(desc.channels),
	    static_cast<sgs::qoi::Colorspace>(desc.colorspace)};
}

struct Decoder : public IImageData
{
	Decoder() = delete;
	Decoder(const std::vector<uint8_t>& qoi) : dataPair{sgs::qoi::decode(qoi)}
	{
	}
	const uint8_t* data() override
	{
		return dataPair.data.data();
	}
	size_t size() override
	{
		return dataPair.data.size();
	}
	uint32_t width() override
	{
		return dataPair.header.width;
	}
	uint32_t height() override
	{
		return dataPair.header.height;
	}
	uint8_t channels() override
	{
		return static_cast<uint8_t>(dataPair.header.channels);
	}
	uint8_t colorspace() override
	{
		return static_cast<uint8_t>(dataPair.header.colorspace);
	}
	sgs::qoi::DataPair<sgs::qoi::DataVector> dataPair;
};
struct Encoder : public IImageData
{
	Encoder() = delete;
	Encoder(const std::vector<uint8_t>& raw, const ImageDescription& desc) :
	    header{toHeader(desc)}, dataVec{sgs::qoi::encode(header, raw)}
	{
	}
	const uint8_t* data() override
	{
		return dataVec.data();
	}
	size_t size() override
	{
		return dataVec.size();
	}
	uint32_t width() override
	{
		return header.width;
	}
	uint32_t height() override
	{
		return header.height;
	}
	uint8_t channels() override
	{
		return static_cast<uint8_t>(header.channels);
	}
	uint8_t colorspace() override
	{
		return static_cast<uint8_t>(header.colorspace);
	}
	sgs::qoi::Header header;
	sgs::qoi::DataVector dataVec;
};

std::unique_ptr<IImageData> decode(const std::vector<uint8_t>& qoi)
{
	return std::make_unique<Decoder>(qoi);
}
std::unique_ptr<IImageData> encode(const std::vector<uint8_t>& raw, const ImageDescription& desc)
{
	return std::make_unique<Encoder>(raw, desc);
}
} // namespace impl::stillgreensan::qoi
