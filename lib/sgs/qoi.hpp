#pragma once

#include <cstdint>

#include <vector>

namespace sgs::qoi
{
enum class Channels : uint8_t
{
	RGB = 3,
	RGBA = 4
};
enum class Colorspace : uint8_t
{
	sRGB = 0,
	Linear = 1
};
struct Header
{
	static constexpr char magic[4]{'q', 'o', 'i', 'f'};
	uint32_t width{0};
	uint32_t height{0};
	Channels channels{Channels::RGBA};
	Colorspace colorspace{Colorspace::Linear};
};
template<typename TData>
struct DataPair
{
	Header header;
	TData data;
};

using DataVector = std::vector<uint8_t>; // TODO make templates and remove direct vector dependency

Header readHeader([[maybe_unused]] const DataVector& qoiData)
{
	// check size : min 14bytes
	// check for magic bytes
	// read dimensions accounting for endianness
	// read channels & colorspace
	return {};
}

//DataPair<DataVector> decode(const DataVector& qoiData);

// Header readHeader(const TData& qoiData);
// TDataPair<TData> decode(const TData& qoiData);
// DataPair<TContainer> decode(const TData& qoiData);
// Header decode(const TData& qoiData, TData& rawData);
// TData encode(const TDataPair<TData>& rawData);
// TData encode(const Header& header, const TData& rawData);
// TContainer encode(const DataPair<TData>& rawData);
// TContainer encode(const Header& header, const TData& rawData);
} // namespace sgs::qoi
