/****************************************************************************/
/*                                                                          */
/* Class PigletMsgId  (c) 1996, Colin Turner                                    */
/*                                                                          */
/* Related Classes (1st Generation)                                         */
/*   None                                                                   */
/*                                                                          */
/* Purpose                                                                  */
/*   Generation of Unique MSGID kludge values in FTS messages               */
/*                                                                          */
/* Revision 1.00                                                            */
/*                                                                          */
/****************************************************************************/

#ifndef PigletMSGID_H
#define PigletMSGID_H

#include <time.h>
#include <stdlib.h>

class PigletMsgId
{
  private:
    unsigned short int counter, pid;

  public:
    unsigned long GetId()
    {
      unsigned long work;
      work = counter++;
      work = ((work << 16) + pid) ^ (unsigned long) time(NULL);
      return(work);
    }
	 PigletMsgId() { counter=pid=0; }
    PigletMsgId(int p) { counter=0; pid=p; }
};

#endif // (PigletMSGID_H)