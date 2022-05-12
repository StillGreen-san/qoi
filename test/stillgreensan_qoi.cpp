#include "qoiimpls.hpp"

#include "sgs/qoi.hpp"

namespace impl::stillgreensan::qoi
{
struct Decoder : public IImageData
{
	Decoder() = delete;
	Decoder(const std::vector<uint8_t>& qoi) :
	    dataPair{sgs::qoi::decode(qoi)}
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

std::unique_ptr<IImageData> decode(const std::vector<uint8_t>& qoi)
{
	return std::make_unique<Decoder>(qoi);
}
} // namespace impl::stillgreensan::qoi
