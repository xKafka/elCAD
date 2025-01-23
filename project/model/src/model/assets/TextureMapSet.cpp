#include "model/assets/TextureMapSet.hpp"

namespace elcad::model
{
	TextureMap::TextureMap(StringView name)
		: m_name{ name }
	{

	}

	//set
	auto TextureMap::setData(const Vec<u8> data, u32 channel) -> void
	{
		m_data = data;

		m_channel = channel;
	}

	auto TextureMap::setExtent(u32 width, u32 height) -> void
	{
		m_width = width;

		m_height = height;
	}

	auto TextureMap::getData() const -> Span<const u8>
	{
		return m_data;
	}

	auto TextureMap::getWidth() const -> u32
	{
		return m_width;
	}

	auto TextureMap::getHeight() const -> u32
	{
		return m_height;
	}

	auto TextureMap::getSize() const -> u32
	{
		return static_cast<u32>(m_data.size());
	}

	auto TextureMap::getChannel() const -> u32
	{
		return m_channel;
	}

	TextureMapSet::TextureMapSet(StringView name)
		: m_name{ name }
	{

	}

	auto TextureMapSet::setColorMap(SPtr<TextureMap> map) -> void
	{
		m_colorMap = map;
	}

	auto TextureMapSet::setSpecularMap(SPtr<TextureMap> map) -> void
	{
		m_specularMap = map;
	}

	auto TextureMapSet::getName() const -> StringView
	{
		return m_name;
	}

	auto TextureMapSet::getColorMap() const -> SPtr<const TextureMap>
	{
		return m_colorMap;
	}

	auto TextureMapSet::getSpecularMap() const -> SPtr<const TextureMap>
	{
		return m_specularMap;
	}
}