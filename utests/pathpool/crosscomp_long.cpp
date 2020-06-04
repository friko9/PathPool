#include "testobj.h"
#include "path_pool.h"
#include "utest.h"

#include <gtest/gtest.h>
#include <limits>
#include <string>

using namespace std::string_literals;

using namespace std;
using namespace testing;

class LongTest : public TestWithParam<std::tuple<const TestObjBase*,Pretty<int,vector>>>
{
public:
  void SetUp() override
  {
    m_test_obj.reset(get<0>(this->GetParam())->clone());
  }
public:
  std::unique_ptr<TestObjBase> m_test_obj;
};

namespace EmptyHashsetTest {
  using tag_t = const boost::flyweight<std::string>;
  using pathid_t = size_t;
  tag_t root_tag {"root"};
  
  
  //TEST SUITE
  //SUBJECT empty <PathPool>
  //ACTION Insert 1 subnode for all consecutive nodes
  //RESULT <PathPool> consistent
  TEST_P(LongTest, LongestDepth)
  {
    vector<pathid_t> paths;
    auto path = m_test_obj->get_root();
    for( auto& x : get<1>(GetParam()))
      paths.push_back(path = m_test_obj->get_subnode(path, boost::flyweight<std::string>(std::to_string(x))));

    path = m_test_obj->get_root();
    int i=0;
    for( auto& x : get<1>(GetParam()))
      {
	auto subnode = m_test_obj->get_subnode(path, boost::flyweight<std::string>(std::to_string(x)));
	ASSERT_EQ( subnode, paths[i++]);
	ASSERT_EQ( m_test_obj->get_parent(subnode), path);
	ASSERT_EQ( m_test_obj->get_subnodes(path).size(), 1);
	ASSERT_EQ( m_test_obj->get_subnodes(path)[0], subnode);
	path = subnode;
      }
  }

  //TEST SUITE
  //SUBJECT empty <PathPool>
  //ACTION Insert all as root subnodes depth = 1
  //RESULT <PathPool> consistent
  TEST_P(LongTest, ShortestDepth)
  {
    vector<pathid_t> paths;
    auto root = m_test_obj->get_root();
    for( auto& x : get<1>(GetParam()))
      paths.push_back(m_test_obj->get_subnode(root, boost::flyweight<std::string>(std::to_string(x))));

    auto root_subnodes = m_test_obj->get_subnodes(m_test_obj->get_root());
    ASSERT_EQ( root_subnodes.size() , get<1>(GetParam()).size() );
    for(auto x : paths)
      ASSERT_EQ( count(root_subnodes.begin(),root_subnodes.end(),x), 1u );
    
    for( auto& x : get<1>(GetParam()))
      {
	auto subnode = m_test_obj->get_subnode(root, boost::flyweight<std::string>(std::to_string(x)));
	ASSERT_EQ( m_test_obj->get_subnodes(subnode).size(), 0 );
      }
  }
  
  auto test_objects = Values( NEW_TEST_OBJ(HashPathPool,root_tag),
			      NEW_TEST_OBJ(ListPathPool,root_tag));

  //TEST DATASET
  INSTANTIATE_TEST_CASE_P(PathPools, LongTest,
			  Combine( test_objects,
				   Values ( make_Pretty( make_InclusiveRange(0, 10000) ))
				   ));
}
