#include "pch.h"
#include "../CrowdSimulation/GraphAdapter.h"
#include "common/HiveCommonMicro.h"

class TestGraphAdapter : public testing::Test
{
protected:
	virtual void SetUp() override
	{
		pAdapter = new CGraphAdapter();
		pAdapter->constructGraph("Config.xml");
	}
	virtual void TearDown() override
	{
		_SAFE_DELETE(pAdapter);
	}

	CGraphAdapter* pAdapter = nullptr;
};

TEST_F(TestGraphAdapter, Initialization)
{
	
}