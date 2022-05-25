#include "sgs/qoi.hpp"
#include "testdata.hpp"

#include <array>
#include <deque>
#include <ostream>

bool operator==(const sgs::qoi::Header& sgsHeader, const impl::ImageDescription& testHeader) noexcept
{
	return sgsHeader.width == testHeader.width && sgsHeader.height == testHeader.height &&
	       static_cast<uint8_t>(sgsHeader.channels) == testHeader.channels &&
	       static_cast<uint8_t>(sgsHeader.colorspace) == testHeader.colorspace;
}
std::ostream& operator<<(std::ostream& oStrm, sgs::qoi::Header const& value)
{
	oStrm << '{' << value.width << ", " << value.height << ", " << static_cast<uint32_t>(value.channels) << ", "
	      << static_cast<uint32_t>(value.colorspace) << '}';
	return oStrm;
}
std::ostream& operator<<(std::ostream& oStrm, impl::ImageDescription const& value)
{
	oStrm << '{' << value.width << ", " << value.height << ", " << static_cast<uint32_t>(value.channels) << ", "
	      << static_cast<uint32_t>(value.colorspace) << '}';
	return oStrm;
}

namespace std
{
template<>
struct hash<std::vector<uint8_t>>
{
	using argument_type = std::vector<uint8_t>;
	using result_type = std::size_t;
	result_type operator()(argument_type const& data) const noexcept
	{
		return std::hash<std::string_view>{}(std::string_view{reinterpret_cast<const char*>(data.data()), data.size()});
	}
};
} // namespace std

std::ostream& operator<<(std::ostream& oStrm, const std::vector<uint8_t>& value)
{
	oStrm << "Hash: " << std::hex << std::hash<std::vector<uint8_t>>{}(value);
	return oStrm;
}

sgs::qoi::Header toHeader(const impl::ImageDescription& desc)
{
	return {desc.width, desc.height, static_cast<sgs::qoi::Channels>(desc.channels),
	    static_cast<sgs::qoi::Colorspace>(desc.colorspace)};
}

#include <catch2/catch.hpp>

TEST_CASE("helpers")
{
	SECTION("pushByte")
	{
		using sgs::qoi::helpers::pushByte;

		uint32_t value = 0;
		value = pushByte(value, 0x01);
		REQUIRE(value == 0x01);

		value = pushByte(value, 0x10);
		REQUIRE(value == 0x0110);

		value = pushByte(value, 0xff);
		REQUIRE(value == 0x0110ff);
	}

	SECTION("read32BE")
	{
		using sgs::qoi::helpers::read32BE;
		std::array<uint8_t, 4> bytes{0xAA, 0xBB, 0xCC, 0xDD};

		REQUIRE(read32BE(bytes.begin()) == 0xAABBCCDD);
	}

	SECTION("writeBE")
	{
		using sgs::qoi::helpers::writeBE;
		const std::array<uint8_t, 4> target{0xAA, 0xBB, 0xCC, 0xDD};
		std::array<uint8_t, 4> bytes{0, 0, 0, 0};

		writeBE(bytes.begin(), 0xAABBCCDD);

		REQUIRE(bytes == target);
	}
}

TEST_CASE("readHeader")
{
	using sgs::qoi::Header;
	using sgs::qoi::readHeader;
	const TestData& testData = TestData::getInstance();

	CHECK(readHeader(testData.wikipedia.qoi) == testData.wikipedia.desc);
	CHECK(readHeader(testData.testcardalpha.qoi) == testData.testcardalpha.desc);
	CHECK(readHeader(testData.testcard.qoi) == testData.testcard.desc);
	CHECK(readHeader(testData.qoilogo.qoi) == testData.qoilogo.desc);
	CHECK(readHeader(testData.kodim23.qoi) == testData.kodim23.desc);
	CHECK(readHeader(testData.kodim10.qoi) == testData.kodim10.desc);
	CHECK(readHeader(testData.dice.qoi) == testData.dice.desc);
}

TEST_CASE("decode")
{
	using sgs::qoi::decode;
	const TestData& testData = TestData::getInstance();

	SECTION("simple")
	{
		{
			auto dataPair = decode(testData.rgb.qoi);
			CHECK(dataPair.header == testData.rgb.desc);
			CHECK(dataPair.data == testData.rgb.raw);
		}
		{
			auto dataPair = decode(testData.rgba.qoi);
			CHECK(dataPair.header == testData.rgba.desc);
			CHECK(dataPair.data == testData.rgba.raw);
		}
		{
			auto dataPair = decode(testData.run.qoi);
			CHECK(dataPair.header == testData.run.desc);
			CHECK(dataPair.data == testData.run.raw);
		}
		{
			auto dataPair = decode(testData.diff.qoi);
			CHECK(dataPair.header == testData.diff.desc);
			CHECK(dataPair.data == testData.diff.raw);
		}
		{
			auto dataPair = decode(testData.luma.qoi);
			CHECK(dataPair.header == testData.luma.desc);
			CHECK(dataPair.data == testData.luma.raw);
		}
		{
			auto dataPair = decode(testData.rgbidx.qoi);
			CHECK(dataPair.header == testData.rgbidx.desc);
			CHECK(dataPair.data == testData.rgbidx.raw);
		}
		{
			auto dataPair = decode(testData.rng01.qoi);
			CHECK(dataPair.header == testData.rng01.desc);
			CHECK(dataPair.data == testData.rng01.raw);
		}
		{
			auto dataPair = decode(testData.rng02.qoi);
			CHECK(dataPair.header == testData.rng02.desc);
			CHECK(dataPair.data == testData.rng02.raw);
		}
	}

	SECTION("advanced")
	{
		{
			auto dataPair = decode(testData.kodim10.qoi);
			CHECK(dataPair.header == testData.kodim10.desc);
			CHECK(dataPair.data == testData.kodim10.raw);
		}
		{
			auto dataPair = decode(testData.dice.qoi);
			CHECK(dataPair.header == testData.dice.desc);
			CHECK(dataPair.data == testData.dice.raw);
		}
		{
			auto dataPair = decode(testData.testcardalpha.qoi);
			CHECK(dataPair.header == testData.testcardalpha.desc);
			CHECK(dataPair.data == testData.testcardalpha.raw);
		}
		{
			auto dataPair = decode(testData.testcard.qoi);
			CHECK(dataPair.header == testData.testcard.desc);
			CHECK(dataPair.data == testData.testcard.raw);
		}
		{
			auto dataPair = decode(testData.qoilogo.qoi);
			CHECK(dataPair.header == testData.qoilogo.desc);
			CHECK(dataPair.data == testData.qoilogo.raw);
		}
		{
			auto dataPair = decode(testData.wikipedia.qoi);
			CHECK(dataPair.header == testData.wikipedia.desc);
			CHECK(dataPair.data == testData.wikipedia.raw);
		}
		{
			auto dataPair = decode(testData.kodim23.qoi);
			CHECK(dataPair.header == testData.kodim23.desc);
			CHECK(dataPair.data == testData.kodim23.raw);
		}
	}

	SECTION("template")
	{
		{
			std::deque<uint8_t> container(testData.rng01.qoi.begin(), testData.rng01.qoi.end());
			auto dataPair = decode<std::vector<uint8_t>>(container);
			CHECK(dataPair.header == testData.rng01.desc);
			CHECK(dataPair.data == testData.rng01.raw);
		}
	}
}

TEST_CASE("encode")
{
	using sgs::qoi::encode;
	const TestData& testData = TestData::getInstance();

	SECTION("simple")
	{
		{
			auto data = encode(toHeader(testData.rgb.desc), testData.rgb.raw);
			CHECK(data == testData.rgb.qoi);
		}
		{
			auto data = encode(toHeader(testData.rgba.desc), testData.rgba.raw);
			CHECK(data == testData.rgba.qoi);
		}
		{
			auto data = encode(toHeader(testData.run.desc), testData.run.raw);
			CHECK(data == testData.run.qoi);
		}
		{
			auto data = encode(toHeader(testData.diff.desc), testData.diff.raw);
			CHECK(data == testData.diff.qoi);
		}
		{
			auto data = encode(toHeader(testData.luma.desc), testData.luma.raw);
			CHECK(data == testData.luma.qoi);
		}
		{
			auto data = encode(toHeader(testData.rgbidx.desc), testData.rgbidx.raw);
			CHECK(data == testData.rgbidx.qoi);
		}
		{
			auto data = encode(toHeader(testData.rng01.desc), testData.rng01.raw);
			CHECK(data == testData.rng01.qoi);
		}
		{
			auto data = encode(toHeader(testData.rng02.desc), testData.rng02.raw);
			CHECK(data == testData.rng02.qoi);
		}
	}

	SECTION("advanced")
	{
		{
			auto data = encode(toHeader(testData.kodim10.desc), testData.kodim10.raw);
			CHECK(data == testData.kodim10.qoi);
		}
		{
			auto data = encode(toHeader(testData.dice.desc), testData.dice.raw);
			CHECK(data == testData.dice.qoi);
		}
		{
			auto data = encode(toHeader(testData.testcardalpha.desc), testData.testcardalpha.raw);
			CHECK(data == testData.testcardalpha.qoi);
		}
		{
			auto data = encode(toHeader(testData.testcard.desc), testData.testcard.raw);
			CHECK(data == testData.testcard.qoi);
		}
		{
			auto data = encode(toHeader(testData.qoilogo.desc), testData.qoilogo.raw);
			CHECK(data == testData.qoilogo.qoi);
		}
		{
			auto data = encode(toHeader(testData.wikipedia.desc), testData.wikipedia.raw);
			CHECK(data == testData.wikipedia.qoi);
		}
		{
			auto data = encode(toHeader(testData.kodim23.desc), testData.kodim23.raw);
			CHECK(data == testData.kodim23.qoi);
		}
	}

	SECTION("template")
	{
		{
			std::deque<uint8_t> container(testData.rng01.raw.begin(), testData.rng01.raw.end());
			auto data = encode<std::vector<uint8_t>>(toHeader(testData.rng01.desc), container);
			CHECK(data == testData.rng01.qoi);
		}
	}
}
