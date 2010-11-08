#include <cstdlib>
#include <unordered_map>  // for std::hash

#include "MurmurHash2.h"
#include "stringpiece.h"
#include "cmph_types.h"

namespace cxxmph {

template <class HashFcn>
struct seeded_hash_function {
  template <class Key>
  cmph_uint32 operator()(const Key& k, cmph_uint32 seed) const {
    return HashFcn()(k) ^ seed;
  }
};

struct Murmur2 {
  template<class Key>
  cmph_uint32 operator()(const Key& k) const {
    return MurmurHash2(k, sizeof(Key), 1 /* seed */);
  }
};
struct Murmur2StringPiece {
  template <class Key>
  cmph_uint32 operator()(const Key& k) const {
    StringPiece s(k);
    return MurmurHash2(s.data(), s.length(), 1 /* seed */);
  }
};

template <>
struct seeded_hash_function<Murmur2> {
  template <class Key>
  cmph_uint32 operator()(const Key& k, cmph_uint32 seed) const {
    return MurmurHash2(reinterpret_cast<const void*>(&k), sizeof(Key), seed);
  }
};

template <>
struct seeded_hash_function<Murmur2StringPiece> {
  template <class Key>
  cmph_uint32 operator()(const Key& k, cmph_uint32 seed) const {
    StringPiece s(k);
    return MurmurHash2(s.data(), s.length(), seed);
  }
};

template <class HashFcn> struct OptimizedSeededHashFunction
{ typedef seeded_hash_function<HashFcn> hash_function; };
// Use Murmur2 instead for all types defined in std::hash, plus
// std::string which is commonly extended.
template <> struct OptimizedSeededHashFunction<std::hash<char*> >
{ typedef seeded_hash_function<Murmur2StringPiece> hash_function; };
template <> struct OptimizedSeededHashFunction<std::hash<const char*> >
{ typedef seeded_hash_function<Murmur2StringPiece> hash_function; };
template <> struct OptimizedSeededHashFunction<std::hash<std::string> >
{ typedef seeded_hash_function<Murmur2StringPiece> hash_function; };

template <> struct OptimizedSeededHashFunction<std::hash<char> >
{ typedef seeded_hash_function<Murmur2> hash_function; };
template <> struct OptimizedSeededHashFunction<std::hash<unsigned char> >
{ typedef seeded_hash_function<Murmur2> hash_function; };
template <> struct OptimizedSeededHashFunction<std::hash<short> >
{ typedef seeded_hash_function<Murmur2> hash_function; };
template <> struct OptimizedSeededHashFunction<std::hash<unsigned short> >
{ typedef seeded_hash_function<Murmur2> hash_function; };
template <> struct OptimizedSeededHashFunction<std::hash<int> >
{ typedef seeded_hash_function<Murmur2> hash_function; };
template <> struct OptimizedSeededHashFunction<std::hash<unsigned int> >
{ typedef seeded_hash_function<Murmur2> hash_function; };
template <> struct OptimizedSeededHashFunction<std::hash<long> >
{ typedef seeded_hash_function<Murmur2> hash_function; };
template <> struct OptimizedSeededHashFunction<std::hash<unsigned long> >
{ typedef seeded_hash_function<Murmur2> hash_function; };
template <> struct OptimizedSeededHashFunction<std::hash<long long> >
{ typedef seeded_hash_function<Murmur2> hash_function; };
template <> struct OptimizedSeededHashFunction<std::hash<unsigned long long> >
{ typedef seeded_hash_function<Murmur2> hash_function; };

}  // namespace cxxmph
