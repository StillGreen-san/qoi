#pragma once

#include <cstdint>

#include <algorithm>
#include <exception> // TODO do without?
#include <iterator>
#include <vector>

// TODO constrain templates
// TODO optimizations for continues storage

namespace sgs::qoi
{
namespace constants
{
constexpr size_t headerSize = 14;
constexpr size_t magicBytes = 4;
} // namespace constants
namespace helpers
{
#ifdef CMAKE_CXX_BIG_ENDIAN
constexpr bool isLittleEndian = false;
#else
constexpr bool isLittleEndian = true;
#endif

template<typename TResult, typename TInt>
inline TResult pushByte(TResult value, TInt byte)
{
	value <<= 8U;
	return value | byte;
}

template<typename TIterator>
uint32_t read32BE(TIterator it)
{
	if constexpr(isLittleEndian)
	{
		uint32_t value = 0;
		value = pushByte(value, *it++);
		value = pushByte(value, *it++);
		value = pushByte(value, *it++);
		value = pushByte(value, *it);
		return value;
	}
	else
	{
		return *reinterpret_cast<const uint32_t*>(&*it);
	}
}
} // namespace helpers

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
	static constexpr uint8_t magic[constants::magicBytes]{'q', 'o', 'i', 'f'};
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

Header readHeader(const DataVector& qoiData)
{
	if(qoiData.size() < constants::headerSize)
	{
		throw std::exception{"insufficient data"};
	}

	auto qoiIt = next(cbegin(qoiData), constants::magicBytes);

	if(!std::equal(cbegin(qoiData), qoiIt, std::cbegin(Header::magic)))
	{
		throw std::exception{"magic bytes mismatch"};
	}

	Header header;

	header.width = helpers::read32BE(qoiIt);
	std::advance(qoiIt, 4);

	header.height = helpers::read32BE(qoiIt);
	std::advance(qoiIt, 4);

	header.channels = static_cast<Channels>(*qoiIt);
	std::advance(qoiIt, 1);

	header.colorspace = static_cast<Colorspace>(*qoiIt);

	return header;
}

// DataPair<DataVector> decode(const DataVector& qoiData);

// Header readHeader(const TData& qoiData);
// TDataPair<TData> decode(const TData& qoiData);
// DataPair<TContainer> decode(const TData& qoiData);
// Header decode(const TData& qoiData, TData& rawData);
// TData encode(const TDataPair<TData>& rawData);
// TData encode(const Header& header, const TData& rawData);
// TContainer encode(const DataPair<TData>& rawData);
// TContainer encode(const Header& header, const TData& rawData);
} // namespace sgs::qoi
