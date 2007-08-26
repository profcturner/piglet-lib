#ifndef Piglet_Netmail_H
#define Piglet_Netmail_H

#include <PigletMSGId.h>
#include <stdio.h>
#include <share.h>
#include <string.h>


const long int PigletNMNoAutoLoad   = 0x0001; // Contructor does not load header
const long int PigletNMAutoSave     = 0x0002; // Automatically save changes on destruction (Not implemented)
const long int PigletNMLoadText     = 0x0004; // Automatically load text to memory (Not implemented)
const long int PigletNMKeepClosed   = 0x0008; // Keep file closed whenever possible

//static char *PigletNMMonthName[]={"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

#define PigletNMMessageHeader 190

class PigletNetmail
{
  private:

   // Basic FD Header
   char                FromUser[36], ToUser[36];
   char                Subject[72];
   char                Date[20];
   unsigned short int  TimesRead, DestNode, OrigNode, Cost, OrigNet, DestNet;
   unsigned long int   DateWritten, DateArrived;
   unsigned short int  ReplyToN, Attr, ReplyNext;

   // Extended Header

   unsigned short int DestZone, OrigZone, DestPoint, OrigPoint;
   char *Msgid, *Reply, *ReplyTo, *ReplyAddr, *IntTo, *IntFrom, *Flags, *PID;

   // File Offsets

   long TextStart; // offset to start of text
   long ViaStart;  // offset to start of via (not set yet)

   // Text Info

   char *Text;                 // to message text (NULL if not loaded).
   char FileName[72];          // filename, zero indicates inaccuracy

   PigletMsgId *IdObject;
   long ClassFlags;

   FILE *fp;

  // Services

  public:

   int Load(); // Loads all data
   int Save(); // Saves all data

   int LoadHeader(); // Loads header information only
   int SaveHeader(); // Saves header information only
   int SaveHeader(time_t old);

   int  OpenMsg(); // Opens a non existant message for write
   long OpenMsg(char *path, long highest);

   void Close();
    void CloseNull();

   int  Receive(); // Marks message as received, and save
   int  Remove();  // Delete message

   // Altering or fetching data

   FILE *GetMessP()                 { return(fp); }

   void SetFilename(char *filename) { strcpy(FileName, filename); }
   char *GetFilename()              { return(FileName); }

   void SetIdObject(PigletMsgId * Id)   {IdObject = Id; }

    char *GetKludge(char * pointer, char * buffer);

   void SetAttr(unsigned short v)   {Attr=v; }
   void SetFromUser(char *user)     {strcpy(FromUser, user); }
   void SetToUser(char *user)       {strcpy(ToUser, user); }
   void SetSubject(char *subject)   {strcpy(Subject, subject); }
   void SetDate(char *date)         {strcpy(Date, date); }
   void SetMsgid(char *msgid)       {SetString(&Msgid, msgid); }
   void SetReply(char *reply)       {SetString(&Reply, reply); }
   void SetReplyTo(char *replyto)   {SetString(&ReplyTo, replyto); }
   void SetReplyAddr(char *addr)    {SetString(&ReplyAddr, addr); }
   void SetIntTo(char *to)          {SetString(&IntTo, to); }
   void SetIntFrom(char *from)      {SetString(&IntFrom, from); }
   void SetFlags(char *flags)       {SetString(&Flags, flags); }
   void SetPID(char *pid)           {SetString(&PID, pid); }

   void SetDestZone(unsigned short dzone)   {DestZone  = dzone; }
   void SetDestNet(unsigned short dnet)     {DestNet   = dnet;  }
   void SetDestNode(unsigned short dnode)   {DestNode  = dnode; }
   void SetDestPoint(unsigned short dpoint) {DestPoint = dpoint;}

   void SetOrigZone(unsigned short ozone)   {OrigZone  = ozone; }
   void SetOrigNet(unsigned short onet)     {OrigNet   = onet;  }
   void SetOrigNode(unsigned short onode)   {OrigNode  = onode; }
   void SetOrigPoint(unsigned short opoint) {OrigPoint = opoint;}

   unsigned short GetAttr(void)             {return(Attr); }
   unsigned short GetDestZone(void)         {return(DestZone);  }
   unsigned short GetDestNet(void)          {return(DestNet);   }
   unsigned short GetDestNode(void)         {return(DestNode);  }
   unsigned short GetDestPoint(void)        {return(DestPoint); }

   unsigned short GetOrigZone(void)         {return(OrigZone);  }
   unsigned short GetOrigNet(void)          {return(OrigNet);   }
   unsigned short GetOrigNode(void)         {return(OrigNode);  }
   unsigned short GetOrigPoint(void)        {return(OrigPoint); }

   char *GetFromUser()     { return(GetString(FromUser)); }
   char *GetToUser()       { return(GetString(ToUser)); }
   char *GetSubject()      { return(GetString(Subject)); }
   char *GetDate()         { return(GetString(Date)); }
   char *GetMsgid()        { return(GetString(Msgid)); }
   char *GetReply()        { return(GetString(Reply)); }
   char *GetReplyTo()      { return(GetString(ReplyTo)); }
   char *GetReplyAddr()    { return(GetString(ReplyAddr)); }
   char *GetIntTo()        { return(GetString(IntTo)); }
   char *GetIntFrom()      { return(GetString(IntFrom)); }
   char *GetFlags()        { return(GetString(Flags)); }
   char *GetPID()          { return(GetString(PID)); }

   void AddFlag(char *flag);
   void RemoveFlag(char *flag);

  private:

   void SetString(char **toset, char *value);
   char *GetString(char *toget);

  // Implementation

  public:

   PigletNetmail();
    PigletNetmail(char *filename, PigletMsgId * Id);
    PigletNetmail(char *filename, PigletMsgId * Id, long flags);
   ~PigletNetmail();

  private:

   void Constructor(char *filename, PigletMsgId * Id, long flags);
    int ReadToCR(FILE *file, char *buffer, size_t maxlength);

};

#endif // (Piglet_Netmail_H)
