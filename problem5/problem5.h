//! -----Declaration-----

//! -----ContainerPointerInterface declaration-----
class ContainerPointerInterface {
 public:
  ContainerPointerInterface();
  virtual ~ContainerPointerInterface();

  template <typename ContainerType>
  virtual const ContainerType* const GetPointer() const = 0;
};

//! -----ContainerPointer declaration-----
template <typename ContainerType>
class ContainerPointer : public ContainerPointerInterface {
 public:
  ContainerPointer(const ContainerType& container);
  virtual ~ContainerPointer();
  virtual const ContainerType* const GetPointer() const;

 private:
  ContainerType* pointer_;
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

    ContainerIterator(const ContainerIterator& container_iterator);

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

    ElementIterator(const ElementIterator& element_iterator);

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
    typename //TODO
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
ContainerPointerInterface<ContainerType>::ContainerPointerInterface() {
}

inline
ContainerPointerInterface<ContainerType>::~ContainerPointerInterface() {
}

//! -----ContainerPointer implementation-----
template <typename ContainerType>
inline
ContainerPointer<ContainerType>::ContainerPointer(
    const ContainerType& container)
  : ContainerPointerInterface()
  , pointer_(&container) {
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
