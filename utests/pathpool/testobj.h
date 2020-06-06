#ifndef TESTOBJ_H
#define TESTOBJ_H

#include <gtest/gtest.h>
#include <boost/flyweight.hpp>

template<typename T> //just to make it header-only
struct TestObjBase
{
  using pathid_t = size_t;
  using tag_t = T;
public:
  virtual ~TestObjBase() = default;
  virtual TestObjBase* clone() const = 0;
  virtual pathid_t get_subnode(pathid_t path,tag_t subnode) =0;
  virtual std::vector<pathid_t> get_subnodes(pathid_t path) const =0;
  virtual pathid_t get_parent(pathid_t path) const =0;
  virtual tag_t get_tag(pathid_t path) const =0;
  virtual pathid_t get_root() const noexcept =0;
public:
  TestObjBase(std::string name):
  m_name { name }
  {}
  std::string toString() const
  {
    return m_name;
  }
private:
  std::string m_name;
};

template<typename ObjT>
struct TestObjImpl : public TestObjBase<typename ObjT::tag_t>
{
  using TestObjBaseT = TestObjBase<typename ObjT::tag_t>;
  using pathid_t = typename TestObjBaseT::pathid_t;
  using tag_t = typename TestObjBaseT::tag_t;
  using TestObjBaseT::TestObjBaseT;
public:
  template<typename... T>
  TestObjImpl(std::string name, T... args):
    TestObjBaseT {name},
    m_test_obj {args...}
  {}
  ~TestObjImpl() override = default;
  TestObjImpl<ObjT>* clone() const override
  {
    return new TestObjImpl<ObjT>(*this);
  }
  pathid_t get_subnode(pathid_t path,tag_t subnode) override
  {
    return m_test_obj.get_subnode(path,subnode);
  }
  std::vector<pathid_t> get_subnodes(pathid_t path) const override
  {
    return m_test_obj.get_subnodes(path);
  }
  pathid_t get_parent(pathid_t path) const override
  {
    return m_test_obj.get_parent(path);
  }
  tag_t get_tag(pathid_t path) const override
  {
    return m_test_obj.get_tag(path);
  }
  virtual pathid_t get_root() const noexcept override
  {
    return m_test_obj.get_root();
  }
private:
  TestObjImpl(const TestObjImpl<ObjT>& arg):
    TestObjBaseT(arg),
    m_test_obj(arg.m_test_obj)
  {}
private:
  ObjT m_test_obj;
};

template <typename T>
class testing::internal::UniversalTersePrinter<const TestObjBase<T>*> {
public:
  static void Print(const TestObjBase<T>* obj, ::std::ostream* os) {
    *os << obj->toString();
  }
};

template <typename T>
class testing::internal::UniversalTersePrinter<const TestObjBase<T>> {
public:
  static void Print(const TestObjBase<T>& obj, ::std::ostream* os) {
    *os << obj.toString();
  }
};

template<typename TestT, typename... ArgsT>
TestObjBase<typename TestT::tag_t>* make_test_object(std::string name, ArgsT... args)
{
  return new TestObjImpl<TestT>(name,args...);
}

#define NEW_TEST_OBJ(X, ...) (make_test_object<X>(STR(X), __VA_ARGS__ ))

#endif /* TESTOBJ_H */
