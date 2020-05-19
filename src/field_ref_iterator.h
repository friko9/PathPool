#ifndef FIELD_REF_ITERATOR_H
#define FIELD_REF_ITERATOR_H

template<typename It_t,typename FieldRefT>
struct field_ref_iterator : It_t
{
  field_ref_iterator(It_t it,FieldRefT ref):
    It_t{it},m_ref{ref}
  {}

  auto
  operator *() const
  {
    return (&It_t::operator*())->*m_ref;
  }
private:
  FieldRefT m_ref;
};

template<typename It_t,typename FieldRefT>
field_ref_iterator<It_t,FieldRefT> field_ref(It_t it, FieldRefT ref)
{
  return field_ref_iterator<It_t,FieldRefT>{it,ref};
}

template <template<typename... X> class T,typename... tail>
inline
T<tail...> make(tail... args)
{
  return T<tail...>{args...};
}

// remove if boost::flyweight hashes without help
// template<>
// struct std::hash<boost::flyweight<std::string>>
// {
//   size_t operator()(boost::flyweight<std::string> arg) const noexcept
//   {
//     using hashed_t = std::decay<decltype(arg.get_key())>::type;
//     return std::hash<hashed_t>{}(arg.get_key());
//   }
// };

#endif /* FIELD_REF_ITERATOR_H */
