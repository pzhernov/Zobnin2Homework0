//! -----Declaration-----

//! -----ContainerPointerInterface declaration-----
class ContainerPointerInterface {
 public:
  ContainerPointerInterface();
  virtual ~ContainerPointerInterface();

  template <typename ContainerType>
  virtual const ContainerType* const GetPointer() const = 0;
  virtual size_t GetSize() const = 0;
};

//! -----ContainerPointer declaration-----
template <typename ContainerType>
class ContainerPointer<ContainerType> : public ContainerPointerInterface {
 public:
  ContainerPointer(const ContainerType& container);
  virtual ~ContainerPointer();
  virtual const ContainerType* const GetPointer() const;
  virtual size_t GetSize() const;

 private:
  ContainerType* pointer_;
  size_t size_;
};

template <typename ContainerType>
class ContainerPointer<ContainerType*> : public ContainerPointerInterface {
 public:
  ContainerPointer(const ContainerType* container, size_t size);
  virtual ~ContainerPointer();
  virtual const ContainerType* const GetPointer() const;
  virtual size_t GetSize() const;

 private:
  ContainerType* pointer_;
  size_t size_;
};

//! -----StandardContainerIteratorInterface declaration-----
class StandardContainerIteratorInterface {
 public:
  StandardContainerIteratorInterface();
  virtual ~StandardContainerIteratorInterface();

  template <typename ContainerIteratorType>
  virtual ContainerIteratorType GetIterator() const = 0;
};

//! -----StandardContainerIterator declaration-----
template <typename ContainerType>
class StandardContainerIterator<ContainerType> : public StandardContainerIteratorInterface {
 public:
  StandardContainerIterator(const typename ContainerType::iterator& container_iterator);
  virtual ~StandardContainerIterator();
  virtual typename ContainerType::iterator& GetIterator() const;

 private:
  typename ContainerType::iterator container_iterator_;
};

template <typename ContainerType>
class StandardContainerIterator<ContainerType*> : public StandardContainerIteratorInterface {
 public:
  StandardContainerIterator(const ContainerType* container_iterator);
  virtual ~StandardContainerIterator();
  virtual ContainerType*& const GetIterator() const;

 private:
  ContainerType* container_iterator_;
};

//! -----IntegratedContainer declaration-----
class IntegratedContainer {
 public:
  IntegratedContainer();
  ~IntegratedContainer();

  class ContainerIterator {
   public:
    typedef ptrdiff_t difference_type;
    typedef std::bidirectional_iterator_tag iterator_category;

    ContainerIterator();

    template <typename ContainerType>
    ContainerIterator(const ContainerType& container);

    ContainerIterator& operator++();
    ContainerIterator& operator--();
    ContainerIterator operator++(int);
    ContainerIterator operator--(int);
    bool operator==(const ContainerIterator& container_iterator);
    bool operator!=(const ContainerIterator& container_iterator);
    ptrdiff_t operator-(const ContainerIterator& container_iterator);

    template <typename ContainerType>
    const ContainerType& operator*();

   private:
    typename std::list<ContainerPointerInterface*>::iterator container_iterator_;
  };

  class ElementIterator {
   public:
    typedef ptrdiff_t difference_type;
    typedef std::bidirectional_iterator_tag iterator_category;

    ElementIterator();

    template <typename ElementType>
    ElementIterator(const ElementType& element);

    //ElementIterator(const ElementIterator& element_iterator);

    ElementIterator& operator++();
    ElementIterator& operator--();
    ElementIterator operator++(int);
    ElementIterator operator--(int);
    bool operator==(const ElementIterator& element_iterator);
    bool operator!=(const ElementIterator& element_iterator);
    ptrdiff_t operator-(const ElementIterator& element_iterator);

    template <typename ElementType>
    const ElementType& operator*();

   private:
    typename std::list<ContainerPointerInterface*>::iterator container_iterator_;
    StandardContainerIteratorInterface* element_iterator_;
  };

 private:
  std::list<ContainerPointerInterface*> integrated_container_;

  IntegratedContainer(const IntegratedContainer&);
  IntegratedContainer& operator=(const IntegratedContainer&);

  template <typename ContainerType>
  ContainerPointerInterface* CreateCell(const ContainerType& container);

  void DeleteCell(ContainerPointerInterface* cell);
};

//! -----Implementation-----

//! -----ContainerPointerInterface implementation-----
inline
ContainerPointerInterface::ContainerPointerInterface() {
}

inline
ContainerPointerInterface::~ContainerPointerInterface() {
}

//! -----ContainerPointer implementation-----
template <typename ContainerType>
inline
ContainerPointer<ContainerType>::ContainerPointer(
    const ContainerType& container)
  : ContainerPointerInterface()
  , pointer_(&container)
  , size_(container->size()) {
}

template <typename ContainerType>
inline
ContainerPointer<ContainerType>::~ContainerPointer() {
}

template <typename ContainerType>
inline
const ContainerType* const ContainerPointer<ContainerType>::GetPointer() const {
  return pointer_;
}

template <typename ContainerType>
inline
size_t ContainerPointer<ContainerType>::GetSize() const {
  return size_;
}

template <typename ContainerType*>
inline
ContainerPointer<ContainerType*>::ContainerPointer(
    const ContainerType& container, size_t size)
  : ContainerPointerInterface()
  , pointer_(&container)
  , size_(size) {
}

template <typename ContainerType*>
inline
ContainerPointer<ContainerType*>::~ContainerPointer() {
}

template <typename ContainerType>
inline
const ContainerType* const ContainerPointer<ContainerType*>::GetPointer() const {
  return pointer_;
}

template <typename ContainerType>
inline
size_t ContainerPointer<ContainerType*>::GetSize() const {
  return size_;
}

//! -----IntegratedContainer implementation-----
IntegratedContainer::IntegratedContainer() {
}

IntegratedContainer::~IntegratedContainer() {
  for (typename std::list<ContainerPointerInterface*>::iterator
       cell = integrated_container_.begin();
       cell != integrated_container_.end();
       ++cell) {
    DeleteCell(*cell);
  }
}

template <typename ContainerType>
ContainerPointerInterface*
IntegratedContainer::CreateCell(const ContainerType& container) {
  return new ContainerPointer<ContainerType>(container);
}

void IntegratedContainer::DeleteCell(ContainerPointerInterface* cell) {
  delete cell;
}
