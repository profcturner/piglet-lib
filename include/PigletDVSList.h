/**
** Piglet Productions
**
** FileName       : PigletDVSList.h
**
** Defines        : PigletDoubleVSList <- PigletDoubleVList
** Implements     : As above
**
** Description
**
** Based on the vector doubly linked list class, this object also allows  
** items to be sorted by insertion, provided that the < operator has been 
** sensibly overridden. It is of course to add items unsorted, either by  
** calling AddObjectAtStart, or using a base class pointer to call the
** base AddObject function.
**
** This is a template class and so it implemented and defined in this file.
**
**
** Initial Coding : Colin Turner
**
** Date           : 8th Novemeber 1998
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

#ifndef Piglet_DVSLIST
  #define Piglet_DVSLIST

template <class DataT> class PigletDoubleVSList;

template <class DataT> class PigletDoubleVSList : public PigletDoubleVList<DataT>
{

public:
  // Services
  
  PigletDoubleVSList() : PigletDoubleVList() {};
  PigletDoubleVSList(unsigned short int setflags) : PigletDoubleVList(setflags) {};

  int    AddObject(DataT * newobject);

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
template <class DataT> int PigletDoubleVSList<DataT>::AddObject(DataT * newobject)
{
  // This function adds an element to the list, by default this is done at
  // the top, but if the list is sortable it will sort it as ordered by <

  // Returns 0 on failure, 1 at other times

  PigletDVListElement<DataT> * pointer;
  int placed=0;

  pointer = new PigletDVListElement<DataT>;
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

    PigletDVListElement<DataT> * iterator = start;

    while (iterator!=NULL && !placed) {
      if (*(pointer->object) < *(iterator->object)) {
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


#endif // (Piglet_DVSLIST)
