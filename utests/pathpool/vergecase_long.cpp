#include "testobj.h"
#include "path_pool.h"
#include "utest.h"

#include <gtest/gtest.h>

using namespace std::string_literals;

using namespace std;
using namespace testing;

class VergecaseLong : public TestWithParam<std::tuple<const TestObjBase<int>*,Pretty<int,vector>>>
{
public:
  void SetUp() override
  {
    m_test_obj.reset(get<0>(this->GetParam())->clone());
  }
public:
  std::unique_ptr<TestObjBase<int>> m_test_obj;
};

namespace {
  using tag_t = int;
  using pathid_t = size_t;
  tag_t root_tag {0};
  
  
  //TEST SUITE
  //SUBJECT empty <PathPool>
  //ACTION Insert 1 subnode for all consecutive nodes
  //RESULT <PathPool> consistent
  TEST_P(VergecaseLong, LongestDepth)
  {
    vector<pathid_t> paths;
    auto path = m_test_obj->get_root();
    for( auto& x : get<1>(GetParam()))
      paths.push_back(path = m_test_obj->get_subnode(path, x));

    path = m_test_obj->get_root();
    int i=0;
    for( auto& x : get<1>(GetParam()))
      {
	auto subnode = m_test_obj->get_subnode(path, x);
	ASSERT_EQ( subnode, paths[i++]);
	ASSERT_EQ( m_test_obj->get_parent(subnode), path);
	auto subs = m_test_obj->get_subnodes(path);
	ASSERT_EQ( std::distance(subs.first,subs.second), 1);
	ASSERT_EQ( *subs.first, subnode);
	path = subnode;
      }
  }

  //TEST SUITE
  //SUBJECT empty <PathPool>
  //ACTION Insert all as root subnodes depth = 1
  //RESULT <PathPool> consistent
  TEST_P(VergecaseLong, ShortestDepth)
  {
    vector<pathid_t> paths;
    auto root = m_test_obj->get_root();
    for( auto& x : get<1>(GetParam()))
      paths.push_back(m_test_obj->get_subnode(root, x));

    auto root_subnodes = m_test_obj->get_subnodes2(m_test_obj->get_root());
    ASSERT_EQ( root_subnodes.size(), get<1>(GetParam()).size() );
    for(auto x : paths)
      ASSERT_EQ( count(root_subnodes.begin(),root_subnodes.end(),x), 1u );
    
    for( auto& x : get<1>(GetParam()))
      {
	auto subnode = m_test_obj->get_subnode(root, x);
	auto subs = m_test_obj->get_subnodes(subnode);
	ASSERT_EQ( std::distance(subs.first,subs.second), 0 );
      }
  }
  
  auto test_objects = Values( NEW_TEST_OBJ(HashPathPool<int>,root_tag),
			      NEW_TEST_OBJ(ListPathPool<int>,root_tag));

  //TEST DATASET
  INSTANTIATE_TEST_CASE_P(PathPools, VergecaseLong,
			  Combine( test_objects,
				   Values ( make_pretty( make_InclusiveRange(0, 10000) ))
				   ));
}
