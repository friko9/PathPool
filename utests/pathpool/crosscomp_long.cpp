#include "testobj.h"
#include "path_pool.h"
#include "utest.h"

#include <gtest/gtest.h>
#include <functional>
#include <random>

using namespace std::string_literals;

using namespace std;
using namespace testing;

class CrosscompLong : public TestWithParam<std::tuple<const TestObjBase<int>*,const TestObjBase<int>*>>
{
  void SetUp() override
  {
    m_test_obj1.reset(get<0>(this->GetParam())->clone());
    m_test_obj2.reset(get<1>(this->GetParam())->clone()); 
  }
public:
  std::unique_ptr<TestObjBase<int>> m_test_obj1;
  std::unique_ptr<TestObjBase<int>> m_test_obj2;
};

namespace {
  using tag_t = int;
  using pathid_t = size_t;
  tag_t root_tag {0};
  
  //TEST SUITE
  //SUBJECT empty <PathPool>
  //ACTION Insert 1 subnode for all consecutive nodes
  //RESULT <PathPool> consistent
  TEST_P(CrosscompLong, RandomInsertion)
  {
    auto rand = std::bind(std::uniform_int_distribution<size_t>{0}, std::default_random_engine{});
    vector<std::pair<pathid_t,pathid_t>> paths;
    for( size_t i =1; i< 1000000; ++i)
      {
	int index = rand() % i;
	auto first = m_test_obj1->get_subnode(index, i);
	auto second  = m_test_obj2->get_subnode(index, i);
	paths.push_back({first,second});
      }

    for( auto& x : paths )
      {
	auto first = x.first;
	auto second = x.second;
	ASSERT_EQ(m_test_obj1->get_tag(first),m_test_obj2->get_tag(second));
	ASSERT_EQ(m_test_obj1->get_parent(first),m_test_obj2->get_parent(second));
	auto subs1 = m_test_obj1->get_subnodes(first);
	auto subs2 = m_test_obj2->get_subnodes(second);
	ASSERT_EQ(subs1.size(),subs2.size());
	std::sort(subs1.begin(),subs1.end());
	std::sort(subs2.begin(),subs2.end());
	ASSERT_TRUE( std::equal(subs1.begin(), subs1.end(), subs2.begin(), subs2.end()));
      }
  };

  auto test_objects = Values( NEW_TEST_OBJ(HashPathPool<int>,root_tag),
			      NEW_TEST_OBJ(ListPathPool<int>,root_tag));

  INSTANTIATE_TEST_CASE_P(PathPools, CrosscompLong,
			  Combine( test_objects, test_objects));
}
