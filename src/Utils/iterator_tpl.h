// MIT License
//
// Copyright (c) 2017 Vinícius Garcia (vingarcia00@gmail.com)
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#ifndef _iterator_tpl_h_
#define _iterator_tpl_h_

namespace iterator_tpl {

// Use this define to declare both:
// - `iterator`
// - `const_iterator`:
// As members of your class
#define VGSI_SETUP_ITERATORS(C, T, S)  \
  VGSI_SETUP_MUTABLE_ITERATOR(C, T, S) \
  VGSI_SETUP_CONST_ITERATOR(C, T, S)

// Use this define to declare only `iterator`
#define VGSI_SETUP_MUTABLE_ITERATOR(C, T, S)         \
  typedef iterator_tpl::iterator<C, T, S> iterator;  \
  iterator begin() { return iterator::begin(this); } \
  iterator end()   { return iterator::end(this);   }

// Use this define to declare only `const_iterator`
#define VGSI_SETUP_CONST_ITERATOR(C, T, S)                              \
  typedef iterator_tpl::const_iterator<C, T, S> const_iterator;         \
  const_iterator begin() const { return const_iterator::begin(this); }  \
  const_iterator end()   const { return const_iterator::end(this);   }  \
  const_iterator cbegin() const { return const_iterator::begin(this); } \
  const_iterator cend()   const { return const_iterator::end(this);   }

// S should be the state struct used to forward iteration:
#define VGSI_SETUP_REVERSE_ITERATORS(C, T, S)                       \
  struct S##_reversed : public S {                                  \
    inline void next (const C* ref) { S::prev(ref); }               \
    inline void prev (const C* ref) { S::next(ref); }               \
    inline void begin(const C* ref) { S::end(ref); S::prev(ref);}   \
    inline void end  (const C* ref) { S::begin(ref); S::prev(ref);} \
  };                                                                \
  VGSI_SETUP_MUTABLE_RITERATOR(C, T, S)                             \
  VGSI_SETUP_CONST_RITERATOR(C, T, S)

#define VGSI_SETUP_MUTABLE_RITERATOR(C, T, S)                           \
  typedef iterator_tpl::iterator<C, T, S##_reversed > reverse_iterator; \
  reverse_iterator rbegin() { return reverse_iterator::begin(this); }   \
  reverse_iterator rend()   { return reverse_iterator::end(this); }     \

#define VGSI_SETUP_CONST_RITERATOR(C, T, S)                                         \
  typedef iterator_tpl::const_iterator<C, T, S##_reversed > const_reverse_iterator; \
  const_reverse_iterator rbegin() const {                                           \
    return const_reverse_iterator::begin(this);                                     \
  }                                                                                 \
  const_reverse_iterator rend() const {                                             \
    return const_reverse_iterator::end(this);                                       \
  }                                                                                 \
  const_reverse_iterator crbegin() const {                                          \
    return const_reverse_iterator::begin(this);                                     \
  }                                                                                 \
  const_reverse_iterator crend() const {                                            \
    return const_reverse_iterator::end(this);                                       \
  }

#define VGSI_STL_TYPEDEFS(T)               \
  typedef std::ptrdiff_t difference_type;  \
  typedef size_t size_type;                \
  typedef T value_type;                    \
  typedef T* pointer;                      \
  typedef const T* const_pointer;          \
  typedef T& reference;                    \
  typedef const T& const_reference

// Unsafe, and slighly cleaner, version of the macros above:
// (Note: its unsafe only in the sense of macro collisions)
#ifndef SETUP_ITERATORS
#define SETUP_ITERATORS(C, T, S) VGSI_SETUP_ITERATORS(C, T, S)
#endif

#ifndef SETUP_MUTABLE_ITERATOR
#define SETUP_MUTABLE_ITERATOR(C, T, S) VGSI_SETUP_MUTABLE_ITERATOR(C, T, S)
#endif

#ifndef SETUP_CONST_ITERATOR
#define SETUP_CONST_ITERATOR(C, T, S) VGSI_SETUP_CONST_ITERATOR(C, T, S)
#endif

#ifndef SETUP_REVERSE_ITERATORS
#define SETUP_REVERSE_ITERATORS(C, T, S) VGSI_SETUP_REVERSE_ITERATORS(C, T, S)
#endif

#ifndef SETUP_MUTABLE_RITERATOR
#define SETUP_MUTABLE_RITERATOR(C, T, S) VGSI_SETUP_MUTABLE_RITERATOR(C, T, S)
#endif

#ifndef SETUP_CONST_RITERATOR
#define SETUP_CONST_RITERATOR(C, T, S) VGSI_SETUP_CONST_RITERATOR(C, T, S)
#endif

#ifndef STL_TYPEDEFS
#define STL_TYPEDEFS(T) VGSI_STL_TYPEDEFS(T)
#endif

// Forward declaration of const_iterator:
template <class C, typename T, class S>
struct const_iterator;

/* * * * * MUTABLE ITERATOR TEMPLATE: * * * * */

// C - The container type
// T - The content type
// S - The state keeping structure
template <class C, typename T, class S>
// The non-specialized version is used for T=rvalue:
struct iterator {
  // Keeps a reference to the container:
  C* ref;

  // User defined struct to describe the iterator state:
  // This struct should provide the functions listed below,
  // however, note that some of them are optional
  S state;

  // Set iterator to next() state:
  void next()  { state.next(ref);  }
  // Initialize iterator to first state:
  void begin() { state.begin(ref); }
  // Initialize iterator to end state:
  void end()   { state.end(ref);   }
  // Returns current `value`
  T get() { return state.get(ref); }
  // Return true if `state != s`:
  bool equals(const S& s) const { return state.equals(s); }

  // Optional function for reverse iteration:
  void prev() { state.prev(ref); }

 public:
  static iterator begin(C* ref) {
    iterator it(ref);
    it.begin();
    return it;
  }
  static iterator end(C* ref) {
    iterator it(ref);
    it.end();
    return it;
  }

 protected:
  iterator(C* ref) : ref(ref) {}

 public:
  // Note: Instances build with this constructor should
  // be used only after copy-assigning from other iterator!
  iterator() {}

 public:
  T operator*() { return get(); }
  iterator& operator++() { next(); return *this; }
  iterator operator++(int) { iterator temp(*this); next(); return temp; }
  iterator& operator--() { prev(); return *this; }
  iterator operator--(int) { iterator temp(*this); prev(); return temp; }
  bool operator!=(const iterator& other) const {
    return ref != other.ref || !equals(other.state);
  }
  bool operator==(const iterator& other) const {
    return !operator!=(other);
  }

  friend struct iterator_tpl::const_iterator<C,T,S>;

  // Comparisons between const and normal iterators:
  bool operator!=(const const_iterator<C,T,S>& other) const {
    return ref != other.ref || !equals(other.state);
  }
  bool operator==(const const_iterator<C,T,S>& other) const {
    return !operator!=(other);
  }
};

template <class C, typename T, class S>
// This specialization is used for iterators to reference types:
struct iterator<C,T&,S> {
  // Keeps a reference to the container:
  C* ref;

  // User defined struct to describe the iterator state:
  // This struct should provide the functions listed below,
  // however, note that some of them are optional
  S state;

  // Set iterator to next() state:
  void next()  { state.next(ref);  }
  // Initialize iterator to first state:
  void begin() { state.begin(ref); }
  // Initialize iterator to end state:
  void end()   { state.end(ref);   }
  // Returns current `value`
  T& get() { return state.get(ref); }
  // Return true if `state != s`:
  bool equals(const S& s) const { return state.equals(s); }

  // Optional function for reverse iteration:
  void prev() { state.prev(ref); }

 public:
  static iterator begin(C* ref) {
    iterator it(ref);
    it.begin();
    return it;
  }
  static iterator end(C* ref) {
    iterator it(ref);
    it.end();
    return it;
  }

 protected:
  iterator(C* ref) : ref(ref) {}

 public:
  // Note: Instances build with this constructor should
  // be used only after copy-assigning from other iterator!
  iterator() {}

 public:
  T& operator*()  { return  get(); }
  T* operator->() { return &get(); }
  iterator& operator++() { next(); return *this; }
  iterator operator++(int) { iterator temp(*this); next(); return temp; }
  iterator& operator--() { prev(); return *this; }
  iterator operator--(int) { iterator temp(*this); prev(); return temp; }
  bool operator!=(const iterator& other) const {
    return ref != other.ref || !equals(other.state);
  }
  bool operator==(const iterator& other) const {
    return !operator!=(other);
  }

  friend struct iterator_tpl::const_iterator<C,T&,S>;

  // Comparisons between const and normal iterators:
  bool operator!=(const const_iterator<C,T&,S>& other) const {
    return ref != other.ref || !equals(other.state);
  }
  bool operator==(const const_iterator<C,T&,S>& other) const {
    return !operator!=(other);
  }
};

/* * * * * CONST ITERATOR TEMPLATE: * * * * */

// C - The container type
// T - The content type
// S - The state keeping structure
template <class C, typename T, class S>
// The non-specialized version is used for T=rvalue:
struct const_iterator {
  // Keeps a reference to the container:
  const C* ref;

  // User defined struct to describe the iterator state:
  // This struct should provide the functions listed below,
  // however, note that some of them are optional
  S state;

  // Set iterator to next() state:
  void next()  { state.next(ref);  }
  // Initialize iterator to first state:
  void begin() { state.begin(ref); }
  // Initialize iterator to end state:
  void end()   { state.end(ref);   }
  // Returns current `value`
  const T get() { return state.get(ref); }
  // Return true if `state != s`:
  bool equals(const S& s) const { return state.equals(s); }

  // Optional function for reverse iteration:
  void prev() { state.prev(ref); }

 public:
  static const_iterator begin(const C* ref) {
    const_iterator it(ref);
    it.begin();
    return it;
  }
  static const_iterator end(const C* ref) {
    const_iterator it(ref);
    it.end();
    return it;
  }

 protected:
  const_iterator(const C* ref) : ref(ref) {}

 public:
  // Note: Instances build with this constructor should
  // be used only after copy-assigning from other iterator!
  const_iterator() {}

  // To make possible copy-construct non-const iterators:
  const_iterator(const iterator<C,T,S>& other) : ref(other.ref) {
    state = other.state;
  }

 public:
  const T operator*() { return get(); }
  const_iterator& operator++() { next(); return *this; }
  const_iterator operator++(int) { const_iterator temp(*this); next(); return temp;  }
  const_iterator& operator--() { prev(); return *this; }
  const_iterator operator--(int) { const_iterator temp(*this); prev(); return temp;  }
  bool operator!=(const const_iterator& other) const {
    return ref != other.ref || !equals(other.state);
  }
  bool operator==(const const_iterator& other) const {
    return !operator!=(other);
  }
  const_iterator& operator=(const iterator<C,T,S>& other) {
    ref = other.ref;
    state = other.state;
    return *this;
  }

  friend struct iterator_tpl::iterator<C,T,S>;

  // Comparisons between const and normal iterators:
  bool operator!=(const iterator<C,T,S>& other) const {
    return ref != other.ref || !equals(other.state);
  }
  bool operator==(const iterator<C,T,S>& other) const {
    return !operator!=(other);
  }
};

// This specialization is used for iterators to reference types:
template <class C, typename T, class S>
struct const_iterator<C,T&,S> {
  // Keeps a reference to the container:
  const C* ref;

  // User defined struct to describe the iterator state:
  // This struct should provide the functions listed below,
  // however, note that some of them are optional
  S state;

  // Set iterator to next() state:
  void next()  { state.next(ref);  }
  // Initialize iterator to first state:
  void begin() { state.begin(ref); }
  // Initialize iterator to end state:
  void end()   { state.end(ref);   }
  // Returns current `value`
  const T& get() { return state.get(ref); }
  // Return true if `state != s`:
  bool equals(const S& s) const { return state.equals(s); }

  // Optional function for reverse iteration:
  void prev() { state.prev(ref); }

 public:
  static const_iterator begin(const C* ref) {
    const_iterator it(ref);
    it.begin();
    return it;
  }
  static const_iterator end(const C* ref) {
    const_iterator it(ref);
    it.end();
    return it;
  }

 protected:
  const_iterator(const C* ref) : ref(ref) {}

 public:
  // Note: Instances build with this constructor should
  // be used only after copy-assigning from other iterator!
  const_iterator() {}

  // To make possible copy-construct non-const iterators:
  const_iterator(const iterator<C,T&,S>& other) : ref(other.ref) {
    state = other.state;
  }

 public:
  const T& operator*()  { return  get(); }
  const T* operator->() { return &get(); }
  const_iterator& operator++() { next(); return *this; }
  const_iterator operator++(int) { const_iterator temp(*this); next(); return temp; }
  const_iterator& operator--() { prev(); return *this; }
  const_iterator operator--(int) { const_iterator temp(*this); prev(); return temp; }
  bool operator!=(const const_iterator& other) const {
    return ref != other.ref || !equals(other.state);
  }
  bool operator==(const const_iterator& other) const {
    return !operator!=(other);
  }
  const_iterator& operator=(const iterator<C,T&,S>& other) {
    ref = other.ref;
    state = other.state;
    return *this;
  }

  friend struct iterator_tpl::iterator<C,T&,S>;

  // Comparisons between const and normal iterators:
  bool operator!=(const iterator<C,T&,S>& other) const {
    return ref != other.ref || !equals(other.state);
  }
  bool operator==(const iterator<C,T&,S>& other) const {
    return !operator!=(other);
  }
};

}  // namespace iterator_tpl

#endif
