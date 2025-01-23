#include "model/MainModel.hpp"

namespace elcad::model
{
	MainModel::MainModel(SPtr<fw::Executor> executor)
		: m_executor{ executor }
		, m_mwModel{ makeShared<MwModel>(executor) }
	{

	}

	auto MainModel::init() -> void
	{

	}

	auto MainModel::getMwModel() -> SPtr<MwModel>
	{
		return m_mwModel;
	}
}