#pragma once

#include <util/Types.hpp>

namespace elcad::model
{
	class TextureMap
	{
	public:
		enum Type : u32
		{
			RGBA8 = 0,
			R8
		};

		TextureMap(StringView name);

		//set
		auto setData(const Vec<u8> data, u32 channel) -> void;

		auto setExtent(u32 width, u32 height) -> void;

		//get
		[[nodiscard]] auto getData() const -> Span<const u8>;

		[[nodiscard]] auto getWidth() const -> u32;

		[[nodiscard]] auto getHeight() const -> u32;

		[[nodiscard]] auto getSize() const -> u32;

		[[nodiscard]] auto getChannel() const -> u32;

	private:
		String	m_name{};

		Vec<u8>	m_data{};

		u32		m_width, m_height{};

		u32		m_channel{};
	};

	class TextureMapSet
	{
	public:
		TextureMapSet(StringView name);

		//set
		auto setColorMap(SPtr<TextureMap> map) -> void;

		auto setSpecularMap(SPtr<TextureMap> map) -> void;

		//get
		[[nodiscard]] auto getName() const -> StringView;

		[[nodiscard]] auto getColorMap() const -> SPtr<const TextureMap>;

		[[nodiscard]] auto getSpecularMap() const -> SPtr<const TextureMap>;

	private:
		String				m_name{};

		SPtr<TextureMap>	m_colorMap{};

		SPtr<TextureMap>	m_specularMap{};
	};
}