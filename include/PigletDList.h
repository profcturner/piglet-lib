/**
** Piglet Productions
**
** FileName       : PigletDList.h
**
** Defines        : PigletDoubleList
**                  PigletDListElement
**                  PigletDoubleListIterator
** Implements     : As above
**
** Description
**
** A doubly linked list of objects.
**
** This class depends upon a sensible definition of the == operator for
** the data to be contained.
**
** This is a template class and so it implemented and defined in this file.
**
**
** Initial Coding : Colin Turner
**
** Date           : 4th May 1999
**
**
** Copyright applies on this file, and distribution may be limited.
** Copyright Colin Turner 1999, All Rights Reserved.
*/


/*
** Revision 1.10
**
** Substantial overhaul to be Watcom C++ friendly
**
** Colin Turner 10th April 1999
**
**
** Revision 1.00 
**
** Colin Turner 8th November 1998
*/


#ifndef Piglet_DLIST
  #define Piglet_DLIST
  #include <mem.h>

template <class DataT> class PigletDoubleList;
template <class DataT> class PigletDoubleSList;
template <class DataT> class PigletDListElement;
template <class DataT> class PigletDoubleListIterator;

#define NULLDATA (*(DataT::ZERO))

template <class DataT> class PigletDoubleList
{
  
  // Data

protected:

  PigletDListElement<DataT> * start, * end;        // The start and end of the linked list, obviously

protected :

  unsigned short int flags;               // Internal data on how to manage certain operations
  unsigned long int  itemsInContainer;

  // Services

public:

  PigletDoubleList()
  {
    start = end = NULL;
    flags = 0;
    itemsInContainer = 0;
  }

  // Overloaded constructor, flags have given values
  PigletDoubleList(unsigned short int setflags)
  {
    start=end=NULL;
    flags=setflags;
    itemsInContainer=0;
  }

  // Destructor
  virtual ~PigletDoubleList(){
    Empty();
  }
  
  int    AddObject(DataT & newobject);
  int    AddObjectAtStart(DataT & newobject);
  int    RemoveObject(DataT & delobject);
  DataT & FindObject(DataT & test);
  int    AlterObject(DataT & toreplace, DataT & newvalue);
  int    AlterObject(long item, DataT & newvalue);
  void   Empty();       // Emptys container

  unsigned long int GetItemsInContainer() { return(itemsInContainer);}
  int    IsEmpty();


  // Implementation
  private:

  friend class PigletDoubleListIterator<DataT>;

};


template <class DataT> class PigletDListElement
{

public  :
  DataT object;               // This is the object being stored

private :
  PigletDListElement *next;   // A pointer to the next object in the list
  PigletDListElement *prev;   // A pointer to the previous object in the list

  // Functions

public :
  PigletDListElement()  // Constructor function
  {
    //object = NULL;      // Clean the object pointer
    next   = NULL;      // Clear the list pointers
    prev   = NULL;
  }

  //void Getobject(DataT & get) {get = object;}  // Extracts the current object pointer for examination
  //void Setobject(DataT & set) {object = set;}  // Sets the current object pointer to a given value

  friend class PigletDoubleList<DataT>;
  friend class PigletDoubleSList<DataT>;
  friend class PigletDoubleListIterator<DataT>;

};


template <class DataT> class PigletDoubleListIterator
{

  // Data

private :

  int offStart;
  int offEnd;
  int StartingElement;                          // 0 indicates start, 1 indicates end
  unsigned long int currentNumber;
  PigletDoubleList<DataT> *ListInUse;
  PigletDListElement<DataT> * currentElement;

  // Services

public :

  PigletDoubleListIterator(PigletDoubleList<DataT> * list){
    ListInUse = list;
    offStart = offEnd = 0;
    currentElement = list->start;
    StartingElement = 0;
    if (currentElement) currentNumber = 1;
    else currentNumber = 0;
  }
  PigletDoubleListIterator(PigletDoubleList<DataT> & list){
    ListInUse = &list;
    offStart = offEnd = 0;
    currentElement = list.start;
    StartingElement = 0;
    if (currentElement) currentNumber = 1;
    else currentNumber = 0;
  }
  PigletDoubleListIterator(PigletDoubleList<DataT> & list, int)
  {
    ListInUse = &list;
    offStart = offEnd = 0;
    currentElement = list.end;
    StartingElement = 1;
    if (currentElement) currentNumber = list.itemsInContainer;
    else currentNumber = 0;
  }

  virtual DataT & Current();
  DataT & GoToStart();
  DataT & GoToEnd();
  int    AtStart() { return currentElement->prev==NULL;}
  int    AtEnd()   { return currentElement->next==NULL;}
  DataT & GoToItem(DataT & findobject);
  DataT & GoToItemNumber(unsigned long int itemNumber);
  virtual void Restart();
  unsigned long int RefreshNumber();
  unsigned long int GetCurrentNumber() { return currentNumber;}

  // Operator Overloads
  virtual operator int();
  virtual DataT & operator ++ (int);
  virtual DataT & operator ++ ();
  DataT & operator -- (int);
  DataT & operator -- ();

};

/*
**    Empty
**
** Destroys the list by removing all items and deallocating storage for them.
**
**/
template <class DataT> void PigletDoubleList<DataT>::Empty()
{
  PigletDListElement<DataT> * iterator = start;

  while (start!=NULL) {
    iterator = start->next;
    delete start;
    start = iterator;
  }
  start=end=NULL;
  itemsInContainer=0;
}


/*
**    AddObject
**
** Adds an item to the linked list. By default this is added at the end of
** the list. The object is copied into the list, with new space allocated for
** the copy by the container.
**
**    Parameters
**
**    newObject     The object to be added
**
**    Returns
**
**    1 on success, 0 on failure (memalloc failure)
**
**/
template <class DataT> int PigletDoubleList<DataT>::AddObject(DataT & newobject)
{
  PigletDListElement<DataT> * pointer;

  pointer = new PigletDListElement<DataT>;
  if (!pointer) {
    // Memory allocation problem
    return(0);
  }
  pointer->object = newobject;

  if (start==NULL) {
    start = end = pointer;
  } 
  else {
    pointer->prev = end;
    end->next     = pointer;
    end           = pointer;
  }
  itemsInContainer++;
  return(1);
}


/*
**    AddObjectAtStart
**
** Adds an item to the linked list. This function adds the new item at the
** start of the list. The object is copied into the list, with new space
** allocated for the copy by the container.
**
**    Parameters
**
**    newObject     The object to be added
**
**    Returns
**
**    1 on success, 0 on failure (memalloc failure)
**
**/
template <class DataT> int PigletDoubleList<DataT>::AddObjectAtStart(DataT & newobject)
{
  PigletDListElement<DataT> *pointer;

  pointer = new PigletDListElement<DataT>;
  if (!pointer) {
    // Memory allocation problem
    return(0);
  }
  pointer->object = newobject;

  if (start==NULL) {
    start = end = pointer;
  } else {
    start->prev=pointer;
    pointer->next=start;
    start=pointer;

  }
  itemsInContainer++;
  return(1);
}


/**
**    RemoveObject
**
**
**    Parameters
**
**    delObject     The object to be removed (or equivalent)
**
**    Returns
**
**    1 on success, 0 on failure (not found)
**/
template <class DataT> int PigletDoubleList<DataT>::RemoveObject(DataT & delObject)
{
  int found=0;
  PigletDListElement<DataT> * iterator=start;

  while (iterator!=NULL && !found) {
    if (iterator->object == delObject) found = 1;
    else iterator=iterator->next;
  }
  if (!found) return 0;
  else {
    if (iterator==start) {  // First element
      if (iterator==end) {  // Only Element
        start=end=NULL;
      } else {
        iterator->next->prev=NULL;
        start=iterator->next;
      }
    } else {
      if (iterator==end) {  // End element
        iterator->prev->next=NULL;
        end=iterator->prev;
      } else {     // Middle element
        iterator->prev->next = iterator->next;
        iterator->next->prev = iterator->prev;
      }
    }

    // Ok, that concludes the unlinkage.
    delete iterator;
    itemsInContainer--;
    return 1;
  }
}


/*
**    FindObject
**
** This function simply attempts to find a matching object in the list and
** returns it.
**
**    Parameters
**
**    Returns
**
**/
template <class DataT> DataT & PigletDoubleList<DataT>::FindObject(DataT & toFind)
{
  PigletDListElement<DataT> * iterator = start;

  while (iterator!=NULL) {
    if (iterator->object == toFind) {
      return(iterator->object);
    }
    iterator=iterator->next;
  }
  return NULLDATA;
}


/*
**    AlterObject
**
**
**
**    Parameters
**
**    Returns
**
**/
template <class DataT> int PigletDoubleList<DataT>::AlterObject(DataT & toReplace, DataT & newObject)
{
  int found = 0;
  PigletDListElement<DataT> * iterator = start;

  while (iterator!=NULL && !found) {
    if (iterator->object == toReplace) found = 1;
    else iterator=iterator->next;
  }
  if (found) {
    iterator->object = newObject;
    return 1;
  }
  else return 0;
}


/*
**    AlterObject
**
** This function replaces a specified item number in the container with a new
** object.
**
**    Parameters
**
**    item      The number of the item to replace, starting count from 1
**    newObject The item to put in place of the old object
**
**    Returns
**
**    1 on success, 0 on failure (not found)
**
**/
template <class DataT> int PigletDoubleList<DataT>::AlterObject(long item, DataT & newObject)
{
  int found = 0;
  long count = 1;
  PigletDListElement<DataT> * iterator = start;

  while (iterator!=NULL && !found) {
    if (item == count) found = 1;
    else {
      iterator=iterator->next;
      count++;
    }
  }
  if (found) {
    iterator->object = newObject;
    return 1;
  } else return 0;
}


// PigletDoubleListIterator


template <class DataT> PigletDoubleListIterator<DataT>::operator int()
{
  return(currentElement!=NULL && !offEnd && !offStart);
}



template <class DataT> DataT & PigletDoubleListIterator<DataT>::Current()
{
  if (currentElement == NULL) return NULLDATA;
  return(currentElement->object);
}


template <class DataT> DataT & PigletDoubleListIterator<DataT>::operator ++ (int)
{
  // Postfix increment
  PigletDListElement<DataT> * old = currentElement;
  
  offStart = 0;
  if (!(ListInUse->itemsInContainer)) return NULLDATA; // No elements
  if (currentElement->next == NULL) offEnd = 1;
  if (!offEnd) {
    currentElement = currentElement->next;
    currentNumber++;
  }
  return(old->object);
}


template <class DataT> DataT & PigletDoubleListIterator<DataT>::operator ++ ()
{
  // Prefix increment
  if (!(ListInUse->itemsInContainer)) return NULLDATA; // No elements
  
  offStart = 0;
  offEnd = (currentElement->next == NULL);
  if (!offEnd) {
    currentElement=currentElement->next;
    currentNumber++;
  }
  return(currentElement->object);
}


template <class DataT> DataT & PigletDoubleListIterator<DataT>::operator -- (int)
{
  PigletDListElement<DataT> * old = currentElement;
  
  offEnd = 0;
  if (!(ListInUse->itemsInContainer)) return NULLDATA; // No elements
  if (currentElement->prev==NULL) offStart=1;
  if (!offStart) {
    currentElement = currentElement->prev;
    currentNumber--;
  }
  return(old->object);
}


template <class DataT> DataT & PigletDoubleListIterator<DataT>::operator -- ()
{
  if (!(ListInUse->itemsInContainer)) return NULLDATA; // No elements

  offEnd = 0;
  offStart = (currentElement->prev==NULL);
  if (!offStart) {
    currentElement = currentElement->prev;
    currentNumber--;
  }
  return(currentElement->object);
}


template <class DataT> void PigletDoubleListIterator<DataT>::Restart()
{
  offStart = offEnd = 0;
  if(StartingElement){
    currentElement = ListInUse->start;
  }
  else{
    currentElement = ListInUse->end;
  }
  RefreshNumber();
}


template <class DataT> DataT & PigletDoubleListIterator<DataT>::GoToStart()
{
  offStart = offEnd = 0;
  currentElement = ListInUse->start;
  if (currentElement == NULL) {
    currentNumber = 0;
    return NULLDATA;
  } else {
    currentNumber = 1;
    return(currentElement->object);
  }
}


template <class DataT> DataT & PigletDoubleListIterator<DataT>::GoToEnd()
{
  offStart = offEnd = 0;
  currentElement = ListInUse->end;
  if (currentElement == NULL) {
    currentNumber = 0;
    return NULLDATA;
  } else {
    currentNumber = ListInUse->itemsInContainer;
    return(currentElement->object);
  }
}


template <class DataT> DataT & PigletDoubleListIterator<DataT>::GoToItem(DataT & findobject)
{
  unsigned long int count = 1;
  int found = 0;
  PigletDListElement<DataT> * pointer = ListInUse->start;

  if (pointer == NULL) return NULLDATA; // No list
  while (!found && count <= ListInUse->itemsInContainer) {
    found=(pointer->object == findobject);
    if (!found) {
      pointer=pointer->next;
      count++;
    }
  }
  if (!found) return NULLDATA;
  else {
    offStart = offEnd = 0;
    currentElement = pointer;
    currentNumber = count;
    return(currentElement->object);
  }
}


template <class DataT> DataT & PigletDoubleListIterator<DataT>::GoToItemNumber(unsigned long int itemNumber)
{
  // This function goes to the nth data item from the start in the list
  // Returns the data element found there and sets currentElement to it's carrier
  // On error, it returns NULLDATA (when the request could not be honoured).
  unsigned long int count = 1;
  PigletDListElement<DataT> * pointer = ListInUse->start;

  if (ListInUse->itemsInContainer == 0 || ListInUse->itemsInContainer < itemNumber)
    // Some error has occured, no list elements, or too few.
    return NULLDATA;
  else {
    offStart = offEnd = 0;
    while (count++ < itemNumber) pointer = pointer->next;
    // Now we are at the right element
    currentElement = pointer;
    currentNumber = itemNumber;
    return(currentElement->object);
  }
}


template <class DataT> unsigned long int PigletDoubleListIterator<DataT>::RefreshNumber()
{
  unsigned long int count=1;
  PigletDListElement<DataT> * pointer = ListInUse->start;

  while (pointer != currentElement && pointer!= ListInUse->end) {
    count++;
    pointer=pointer->next;
  }
  currentNumber=count;

  if(pointer == ListInUse->end){
    // Current Element seems to have gone, better attach it to something
    if(StartingElement){
      currentElement = ListInUse->start;
      count = 1;
    }
    currentElement = pointer;
  }
  
  // Is the list now empty?
  if(!currentElement) count = 0;
  return(count);
}

#endif // (Piglet_DLIST)
