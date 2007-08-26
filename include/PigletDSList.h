/***************************************************************************
*                                                                          *
* Piglet Productions - Doubly Linked List Sorted Container Class           *
*                                                                          *
* (c) 1998 Colin Turner                                                    *
*                                                                          *
* FileName        : P_DSL.H                                                *
* Defined Classes : PigletDoubleSList <- PigletDoubleList                  *
*                                                                          *
* Based on the ordinary doubly linked list class, this object also allows  *
* items to be sorted by insertion, provided that the < operator has been   *
* sensibly overridden. It is of course to add items unsorted, either by    *
* calling AddObjectAtStart, or using a base class pointer to call the      *
* base AddObject function.                                                 *
*                                                                          *
***************************************************************************/

/*
** Revision 1.00 
**
** Colin Turner 8th November 1998
*/

#ifndef Piglet_DSLIST
  #define Piglet_DSLIST
  #include <PigletDList.H>

template <class DataT> class PigletDoubleSList;

template <class DataT> class PigletDoubleSList : public PigletDoubleList<DataT>
{

  // Services

public:

  virtual int    AddObject(DataT & newobject);
  
  // Implementation

public:
  
  PigletDoubleSList() : PigletDoubleList() {};
  PigletDoubleSList(unsigned short int setflags) : PigletDoubleList(setflags) {};

};


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
template <class DataT> int PigletDoubleSList<DataT>::AddObject(DataT & newobject)
{
  // This function adds an element to the list, by default this is done at
  // the top, but if the list is sortable it will sort it as ordered by <

  // Returns 0 on failure, 1 at other times

  PigletDListElement<DataT> * pointer;
  int placed=0;

  pointer = new PigletDListElement<DataT>;
  if (!pointer) {
    // Memory allocation problem
    return(0);
  }
  pointer->object = newobject;

  if (start==NULL) {
    // the list is currently empty, sort or no sort, we add at the top
    start = end = pointer;
    placed=1;
  }
  else {

    PigletDListElement<DataT> * iterator = start;

    while (iterator!=NULL && !placed) {
      if (pointer->object < iterator->object) {
        // place the element
        if (iterator->prev==NULL) {
          // At top of list
          pointer->next=start;
          start->prev  =pointer;
          start        =pointer;
          placed=1;
        } else {
          iterator->prev->next = pointer;
          pointer->prev  = iterator->prev;
          iterator->prev = pointer;
          pointer->next  = iterator;
          placed=1;
        }
      }
      iterator=iterator->next;
    }

    if (!placed) {
      // List is not sorted, or we're at the end
      pointer->prev = end;
      end->next     = pointer;
      end           = pointer;
    }
  }
  itemsInContainer++;
  return(1);
}


#endif // (Piglet_DSLIST)
