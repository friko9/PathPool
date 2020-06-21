#ifndef TESTOBJ_H
#define TESTOBJ_H

#include <iterator>
#include <gtest/gtest.h>

template<typename T>
struct IteratorBase
{
  using iterator_category = std::forward_iterator_tag;
  using value_type = T;
  using difference_type = ptrdiff_t;
  using pointer = value_type*;
  using reference = value_type&;
public:
  virtual ~IteratorBase(){}
  virtual IteratorBase<T>* clone() =0;
  virtual value_type operator *() =0;
  virtual IteratorBase& operator ++() =0;
  virtual IteratorBase& operator ++(int) =0;
  virtual bool operator ==(const IteratorBase& arg)const =0;
  virtual bool operator !=(const IteratorBase& arg)const =0;
};

template<typename T>
struct IteratorImpl : public IteratorBase<typename T::value_type>
{
  using baseT = IteratorBase<typename T::value_type>;
  using iterator_category = typename baseT::iterator_category;
  using value_type = typename baseT::value_type;
  using differance_type = typename baseT::difference_type;
  using pointer = typename baseT::pointer;
  using reference = typename baseT::reference;
public:
  explicit
  IteratorImpl(T it):
    m_it {it}
  {}
  ~IteratorImpl() override
  {};
  IteratorImpl<T>* clone() override
  {
    return new IteratorImpl<T>{m_it};
  }
  value_type operator *() override
  {
    return *m_it;
  }
  IteratorImpl<T>& operator ++() override
  {
    ++m_it;
    return *this;
  };
  IteratorImpl<T>& operator ++(int) override
  {
    const IteratorImpl<T>& result = IteratorImpl<T>{*this};
    m_it++;
    return const_cast<IteratorImpl<T>&>(result);
  }
  bool operator ==(const baseT& arg)const override
  {
    auto arg_it = dynamic_cast<decltype(*this)>(arg).m_it;
    return m_it == arg_it;
  }
  bool operator !=(const baseT& arg)const override
  {
    auto arg_it = dynamic_cast<decltype(*this)>(arg).m_it;
    return m_it != arg_it;
  }
private:
  T m_it;
};

template<typename T>
struct Iterator
{
  using iterator_category = std::forward_iterator_tag;
  using value_type = T;
  using difference_type = ptrdiff_t;
  using pointer = value_type*;
  using reference = value_type&;
public:
  explicit
  Iterator( IteratorBase<T>* arg):
    m_it{arg}
  {}
  Iterator(const Iterator<T>& arg):
    m_it { arg.m_it->clone() }
  {}
  value_type operator *()
  {
    return **m_it;
  }
  bool operator ==(const Iterator<T>& arg) const
  {
    return *m_it == *arg.m_it;
  }
  bool operator !=(const Iterator<T>& arg) const
  {
    return *m_it != *arg.m_it;
  }
  Iterator<T>& operator ++(){ ++*m_it; return *this;}
  Iterator<T> operator ++(int)
  {
    auto result = m_it->clone();
    (*m_it)++;
    return Iterator<T>{result};
  }
private:
  std::unique_ptr<IteratorBase<T>> m_it;
};

template<typename T> //just to make it header-only
struct TestObjBase
{
  using pathid_t = size_t;
  using tag_t = T;
  using iterator_t = Iterator<pathid_t>;
public:
  virtual ~TestObjBase() = default;
  virtual TestObjBase* clone() const = 0;
  virtual pathid_t get_subnode(pathid_t path,tag_t subnode) =0;
  virtual std::pair<iterator_t,iterator_t> get_subnodes(pathid_t path) const = 0;
  virtual std::vector<pathid_t> get_subnodes2(pathid_t path) const = 0;
  virtual pathid_t get_parent(pathid_t path) const =0;
  virtual tag_t get_tag(pathid_t path) const =0;
  virtual pathid_t get_root() const noexcept =0;
public:
  explicit
  TestObjBase(const std::string& name):
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
  using iterator_t = typename TestObjBaseT::iterator_t;
  using TestObjBaseT::TestObjBaseT;
public:
  template<typename... T>
  TestObjImpl(const std::string& name, T... args):
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
  std::pair<iterator_t,iterator_t> get_subnodes(pathid_t path) const override
  {
    using it_t = typename ObjT::iterator_t;
    std::pair<it_t,it_t> p = m_test_obj.get_subnodes(path);
    Iterator<pathid_t> beg { new IteratorImpl<it_t>{p.first} };
    Iterator<pathid_t> end { new IteratorImpl<it_t>{p.second} };
    return std::pair<Iterator<pathid_t>,Iterator<pathid_t>> {beg,end};
  }
  std::vector<pathid_t> get_subnodes2(pathid_t path) const override
  {
    return m_test_obj.template get_subnodes<std::vector<pathid_t>>(path);
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
TestObjBase<typename TestT::tag_t>*
make_test_object(std::string name, ArgsT... args)
{
  return new TestObjImpl<TestT>(name,args...);
}

#define NEW_TEST_OBJ(X, ...) (make_test_object<X>(STR(X), __VA_ARGS__ ))
#define NEW_TEST_OBJ_DEFAULT(X) (make_test_object<X>(STR(X)))

#endif /* TESTOBJ_H */
