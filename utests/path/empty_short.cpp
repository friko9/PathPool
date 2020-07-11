#include "path.h"
#include "path_pool.h"
#include "utest.h"

#include <gtest/gtest.h>

using namespace std;
using namespace testing;

namespace {
  template<typename T>
  class DefaultEmpty : public Test
  {
  public:
    using TestObjT = T;
    void SetUp() override
    {}
  public:
  };
  
  using types = Types<Path<ListPathPool<int>>,Path<HashPathPool<int>>>;
  TYPED_TEST_CASE(DefaultEmpty, types);

  TYPED_TEST(DefaultEmpty, DefaultPathsAreEqual)
  {
    ASSERT_EQ( TypeParam{}, TypeParam{});
  }
  TYPED_TEST(DefaultEmpty, DefaultPathsAreNotLess)
  {
    TypeParam p1,p2;
    ASSERT_FALSE(p1 < p2);
    ASSERT_FALSE(p2 < p1);
  }
  TYPED_TEST(DefaultEmpty, RootParentIsRoot)
  {
    TypeParam root;
    ASSERT_EQ(root,root.get_parent());
  }  
  TYPED_TEST(DefaultEmpty, RootHasNoSiblings)
  {
    TypeParam root;
    auto subnodes = root.template get_subpaths<std::vector>();
    ASSERT_EQ(std::distance(subnodes.begin(),subnodes.end()), 0);
  }
  TYPED_TEST(DefaultEmpty, ProperRootBeginIsEnd)
  {
    TypeParam root;
    ASSERT_EQ(root.begin(), root.end());
  }
}
